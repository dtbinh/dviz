
#include <QImage>
#include <QMutex>
#include <QMutexLocker>
#include <QStringList>
#include <QDebug>

extern "C" {
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
}

#include "VideoThread.h"

#define MAX_SKIPS_TILL_RESET 30

int Round(double value)
{
	return (int)(value + 0.5f);
}

VideoThread::VideoThread(QObject *parent)
	: QThread(parent)
	, m_inited(false)
	, m_videoFile()
{
	m_time_base_rational.num = 1;
	m_time_base_rational.den = AV_TIME_BASE;
	m_previous_pts_delay = 40e-3;
	m_skipFrameCount = 0;
	m_frameTimer = 0; //(double)av_gettime() / 1000000.0;
		
	m_total_runtime = 0;
	
	m_sws_context = NULL;
// 	m_frame = NULL;

}

void VideoThread::setVideo(const QString& name)
{
	m_videoFile = name;
}

int VideoThread::initVideo()
{
	avcodec_init();
	avcodec_register_all();
	avdevice_register_all();
	av_register_all();

	QString fileTmp = m_videoFile;
////	qDebug() << "[DEBUG] VideoThread::load(): input file:"<<fileTmp;

	
	AVInputFormat *inFmt = NULL;
	AVFormatParameters formatParams;
	memset(&formatParams, 0, sizeof(AVFormatParameters));

	//qDebug() << "[DEBUG] VideoThread::load(): starting with fileTmp:"<<fileTmp;
	
	// Open video file
	//
	int res = av_open_input_file(&m_av_format_context, qPrintable(fileTmp), inFmt, 0, &formatParams);
	if(res != 0)
	//if(av_open_input_file(&m_av_format_context, "1", inFmt, 0, NULL) != 0)
	{
		qDebug() << "[ERROR] VideoThread::load(): av_open_input_file() failed, fileTmp:"<<fileTmp<<", res:"<<res;
		return false;
	}

	if(av_find_stream_info(m_av_format_context) < 0)
	{
		qDebug() << "[ERROR] VideoThread::load(): av_find_stream_info() failed.";
		return false;
	}
	
	uint i;

	// Find the first video stream
	m_video_stream = -1;
	m_audio_stream = -1;
	for(i = 0; i < m_av_format_context->nb_streams; i++)
	{
		if(m_av_format_context->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO)
		{
			m_video_stream = i;
		}
		if(m_av_format_context->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO)
		{
			m_audio_stream = i;
		}
	}
	if(m_video_stream == -1)
	{
		qDebug() << "[ERROR] VideoThread::load(): Cannot find video stream.";
		return false;
	}

	// Get a pointer to the codec context for the video and audio streams
	m_video_codec_context = m_av_format_context->streams[m_video_stream]->codec;
// 	m_video_codec_context->get_buffer = our_get_buffer;
// 	m_video_codec_context->release_buffer = our_release_buffer;

	// Find the decoder for the video stream
	m_video_codec = avcodec_find_decoder(m_video_codec_context->codec_id);
	if(m_video_codec == NULL)
	{
		qDebug() << "[ERROR] VideoThread::load(): avcodec_find_decoder() failed for codec_id:" << m_video_codec_context->codec_id;
		return false;
	}

	// Open codec
	if(avcodec_open(m_video_codec_context, m_video_codec) < 0)
	{
		qDebug() << "[ERROR] VideoThread::load(): avcodec_open() failed.";
		return false;
	}

	// Allocate video frame
	m_av_frame = avcodec_alloc_frame();

	// Allocate an AVFrame structure
	m_av_rgb_frame =avcodec_alloc_frame();
	if(m_av_rgb_frame == NULL)
	{
		qDebug() << "[ERROR] VideoThread::load(): avcodec_alloc_frame() failed.";
		return false;
	}

	// Determine required buffer size and allocate buffer
	int num_bytes = avpicture_get_size(PIX_FMT_RGB565, m_video_codec_context->width, m_video_codec_context->height);

	m_buffer = (uint8_t *)av_malloc(num_bytes * sizeof(uint8_t));

	// Assign appropriate parts of buffer to image planes in pFrameRGB
	// Note that pFrameRGB is an AVFrame, but AVFrame is a superset of AVPicture
	avpicture_fill((AVPicture *)m_av_rgb_frame, m_buffer, PIX_FMT_RGB565,
					m_video_codec_context->width, m_video_codec_context->height);

	if(m_audio_stream != -1)
	{
		m_audio_codec_context = m_av_format_context->streams[m_audio_stream]->codec;

		m_audio_codec = avcodec_find_decoder(m_audio_codec_context->codec_id);
		if(!m_audio_codec)
		{
			//unsupported codec
			return false;
		}
		avcodec_open(m_audio_codec_context, m_audio_codec);
	}

	m_timebase = m_av_format_context->streams[m_video_stream]->time_base;

	m_readTimer = new QTimer();
	connect(m_readTimer, SIGNAL(timeout()), this, SLOT(readFrame()));
	int ts = 1000/30; //int(m_timebase.den);
	qDebug() << "VideoThread::initVideo: setting interval to "<<ts<<", den:"<<m_timebase.den<<", num:"<<m_timebase.num;
	m_readTimer->setInterval(ts);
	
	calculateVideoProperties();

	m_inited = true;
	return 0;
}

void VideoThread::run()
{
	initVideo();
	//m_readTimer->start();
	play();
	exec();
}


VideoThread::~VideoThread()
{
	m_killed = true;
	quit();
	wait();

	freeResources();

	if(m_sws_context != NULL)
	{
		sws_freeContext(m_sws_context);
		m_sws_context = NULL;
	}

// 	if(m_frame != NULL)
// 	{
// 		delete m_frame;
// 		m_frame = 0;
// 	}
}


void VideoThread::calculateVideoProperties()
{
	//filename
	//m_video->m_filename = QString(m_av_format_context->filename);

	//frame rate
	m_frame_rate = Round(av_q2d(m_av_format_context->streams[m_video_stream]->r_frame_rate));
	m_fpms = (double)m_frame_rate / 1000.0f;
	//printf("m_fpms = %.02f, frame_rate=%d\n",m_fpms,m_video->m_frame_rate);

	//duration
	m_duration = (m_av_format_context->duration / AV_TIME_BASE);

	//framesize
	m_frame_size = QSize(m_video_codec_context->width, m_video_codec_context->height);
}


void VideoThread::freeResources()
{
	// Free the RGB image
	av_free(m_buffer);
	av_free(m_av_rgb_frame);

	// Free the YUV frame
	//av_free(m_av_frame);
	//mutex.unlock();

	// Close the codec
	avcodec_close(m_video_codec_context);

	// Close the video file
	av_close_input_file(m_av_format_context);
}

void VideoThread::seek(int ms, int flags)
{
// 	QMutexLocker locker(&mutex);
	m_total_runtime = ms;
	m_run_time.start();
	m_frameTimer = ms;
	

	double seconds = (double)ms / 1000.0f;

	int64_t seek_target = (int64_t)(seconds * AV_TIME_BASE);

	seek_target = av_rescale_q(seek_target, m_time_base_rational,
		m_av_format_context->streams[m_video_stream]->time_base);

	av_seek_frame(m_av_format_context, m_video_stream, seek_target, flags);

	avcodec_flush_buffers(m_video_codec_context);
}

void VideoThread::restart()
{
// if(!m_video->m_video_loaded)
// 		return;
	
	seek(0, AVSEEK_FLAG_BACKWARD);
}

void VideoThread::play()
{
	if(!m_inited)
		return;
		
	m_status = Running;
	
	if(m_readTimer->isActive())
 		m_total_runtime += m_run_time.restart();
	else
	{
		m_readTimer->start();
		m_run_time.start();
	}
	//m_readTimer->start();
	//m_video_decoder->decode(); // start decoding again
	emit movieStateChanged(QMovie::Running);
}

void VideoThread::pause()
{
	m_status = Paused;
	m_total_runtime += m_run_time.elapsed();
	//qDebug() << "QVideo::pause(): m_total_runtime:"<<m_total_runtime;
	
	//emit startDecode();
	m_readTimer->stop();
	emit movieStateChanged(QMovie::Paused);
}

void VideoThread::stop()
{
	seek(0, AVSEEK_FLAG_BACKWARD);
	
	m_status = NotRunning;
	m_readTimer->stop();
	
	// Since there is no stop() or pause(), we dont need to touch m_run_time right now - it will be reset in play()
	m_total_runtime = 0;

	emit movieStateChanged(QMovie::NotRunning);
	
}

void VideoThread::setStatus(Status s)
{
	m_status = s;
	if(s == NotRunning)
		stop();
	else
	if(s == Paused)
		pause();
	else
	if(s == Running)
		play();
}


void VideoThread::readFrame()
{
	if(!m_inited)
	{
		emit newImage(QImage(),QTime::currentTime());
		return;
	}
	AVPacket pkt1, *packet = &pkt1;
	double pts;

	int frame_finished = 0;
	while(!frame_finished && !m_killed)
	{
		if(av_read_frame(m_av_format_context, packet) >= 0)
		{
			// Is this a packet from the video stream?
			if(packet->stream_index == m_video_stream)
			{
				//global_video_pkt_pts = packet->pts;

				avcodec_decode_video(m_video_codec_context, m_av_frame, &frame_finished, packet->data, packet->size);

				if(packet->dts == AV_NOPTS_VALUE &&
						  m_av_frame->opaque &&
				  *(uint64_t*)m_av_frame->opaque != AV_NOPTS_VALUE)
				{
					pts = *(uint64_t *)m_av_frame->opaque;
				}
				else if(packet->dts != AV_NOPTS_VALUE)
				{
					pts = packet->dts;
				}
				else
				{
					pts = 0;
				}

				pts *= av_q2d(m_timebase);

				// Did we get a video frame?
				if(frame_finished)
				{

					// Convert the image from its native format to RGB, then copy the image data to a QImage
					if(m_sws_context == NULL)
					{
						//mutex.lock();
						m_sws_context = sws_getContext(
							m_video_codec_context->width, m_video_codec_context->height,
							m_video_codec_context->pix_fmt,
							m_video_codec_context->width, m_video_codec_context->height,
							//PIX_FMT_RGB32,SWS_BICUBIC,
							PIX_FMT_RGB565, SWS_FAST_BILINEAR,
							NULL, NULL, NULL); //SWS_PRINT_INFO
						//mutex.unlock();
						//printf("decode(): created m_sws_context\n");
					}
					//printf("decode(): got frame\n");

					sws_scale(m_sws_context,
						  m_av_frame->data,
						  m_av_frame->linesize, 0,
						  m_video_codec_context->height,
						  m_av_rgb_frame->data,
						  m_av_rgb_frame->linesize);

					m_frame = QImage(m_av_rgb_frame->data[0],
								m_video_codec_context->width,
								m_video_codec_context->height,
								QImage::Format_RGB16);

					av_free_packet(packet);

					// This block from the synchronize_video(VideoState *is, AVFrame *src_frame, double pts) : double
					// function given at: http://dranger.com/ffmpeg/tutorial05.html
					{
						// update the frame pts
						double frame_delay;

						if(pts != 0)
						{
							/* if we have pts, set video clock to it */
							m_video_clock = pts;
						} else {
							/* if we aren't given a pts, set it to the clock */
							pts = m_video_clock;
						}
						/* update the video clock */
						frame_delay = av_q2d(m_timebase);
						/* if we are repeating a frame, adjust clock accordingly */
						frame_delay += m_av_frame->repeat_pict * (frame_delay * 0.5);
						m_video_clock += frame_delay;
						qDebug() << "Frame Dealy: "<<frame_delay<<", avq2d:"<<av_q2d(m_timebase)<<", repeat:"<<(m_av_frame->repeat_pict * (frame_delay * 0.5))<<", vidclock: "<<m_video_clock; 
					}


					//QFFMpegVideoFrame video_frame;
					//video_frame.frame = m_frame;
					//video_frame.pts = pts;
					//video_frame.previous_pts = m_previous_pts;

					//m_current_frame = video_frame;

					//emit newFrame(video_frame);
					//qDebug() << "emit newImage(), frameSize:"<<frame.size();
					//emit newImage(frame);
					
					
					//calculate the pts delay
					double pts_delay = pts - m_previous_pts;
					if((pts_delay < 0.0) || (pts_delay > 1.0))
						pts_delay = m_previous_pts_delay;
			
					m_previous_pts_delay = pts_delay;
					
					/* update delay to sync to audio */
			// 		double ref_clock = get_audio_clock(is);
			// 		double diff = vp->pts - ref_clock;
			
					
			
					//calculate the global delay
					//int global_delay = (m_current_frame.pts * 1000) - m_run_time.elapsed();
					//m_run_time.elapsed() / 1000; //
					//double curTime = (double)(av_gettime() / 1000000.0);
					double curTime = ((double)m_run_time.elapsed() + m_total_runtime) / 1000.0;
					m_frameTimer += pts_delay;
					//if(m_frameTimer > curTime)
						//m_frameTimer = curTime;
					double actual_delay = m_frameTimer - curTime;
					//qDebug() << "frame timer: "<<m_frameTimer<<", curTime:"<<curTime<<", \t actual_delay:"<<((int)(actual_delay*1000))<<", pts_delay:"<<((int)(pts_delay*1000))<<", m_run_time:"<<m_run_time.elapsed()<<", m_total_runtime:"<<m_total_runtime;
					if(actual_delay < 0.010)
					{
						// This should just skip this frame
			// 			qDebug() << "Skipping this frame, skips:"<<m_skipFrameCount;
			// 			if(status() == Running)
			// 				m_play_timer = startTimer(0);
			// 			return;
						
						actual_delay = 0.0;
						
						m_skipFrameCount ++;
						
						if(m_skipFrameCount > MAX_SKIPS_TILL_RESET)
						{
							m_skipFrameCount = 0;
							m_frameTimer = curTime - pts_delay;
			// 				qDebug() << "Reset frame timer";
						}
					}
					
					int frameDelay = (int)(actual_delay * 1000 + 0.5);
					if(frameDelay < 0)
						frameDelay = 5;
					//if(frameDelay > 123)
					//	frameDelay = 123;
					qDebug() << "VideoThread::readVideo: frameDelay:"<<frameDelay;
					
					m_time = QTime::currentTime(); 
					QTimer::singleShot(frameDelay, this, SLOT(releaseCurrentFrame()));
					
					m_previous_pts = pts;
				}

			}
			else if(packet->stream_index == m_audio_stream)
			{
// 				mutex.lock();
				//decode audio packet, store in queue
				av_free_packet(packet);
// 				mutex.unlock();

			}
			else
			{
// 				mutex.lock();
				av_free_packet(packet);
// 				mutex.unlock();

			}
		}
		else
		{
			//emit reachedEnd();
			//qDebug() << "reachedEnd()";
			restart();
			
		}
	}
}

void VideoThread::releaseCurrentFrame()
{
	emit newImage(m_frame,m_time);
}


