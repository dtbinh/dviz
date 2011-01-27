#include "GLVideoFileDrawable.h"

#ifdef HAS_QT_VIDEO_SOURCE
#include "QtVideoSource.h"
#endif

#include <QPropertyAnimation>
#include "GLEditorGraphicsScene.h"

GLVideoFileDrawable::GLVideoFileDrawable(QString file, QObject *parent)
	: GLVideoDrawable(parent)
	, m_videoLength(-1)
	, m_qtSource(0)
{
	if(!file.isEmpty())
		setVideoFile(file);
	
	connect(this, SIGNAL(sourceDiscarded(VideoSource*)), this, SLOT(deleteSource(VideoSource*)));
}
	
void GLVideoFileDrawable::testXfade()
{
	qDebug() << "GLVideoFileDrawable::testXfade(): loading file #2";
	setVideoFile("/data/appcluster/dviz-old/dviz-r62-b2/src/data/Seasons_Loop_2_SD.mpg");
}
	
bool GLVideoFileDrawable::setVideoFile(const QString& file)
{
	qDebug() << "GLVideoFileDrawable::setVideoFile(): "<<(QObject*)this<<" file:"<<file;
	
	QFileInfo fileInfo(file);
	if(!fileInfo.exists())
	{
		qDebug() << "GLVideoFileDrawable::setVideoFile: "<<(QObject*)this<<" "<<file<<" does not exist!";
		return false;
	}
	
	m_videoFile = file;
	
#ifdef HAS_QT_VIDEO_SOURCE
	if(m_qtSource)
	{
		QPropertyAnimation *anim = new QPropertyAnimation(m_qtSource->player(), "volume");
		anim->setEndValue(0);
		anim->setDuration(xfadeLength());
		anim->start();
		
		disconnect(m_qtSource->player(), 0, this, 0);
	}
	
	m_qtSource = new QtVideoSource();
	m_qtSource->setFile(file);
	m_qtSource->start();
	//qDebug() << "GLVideoFileDrawable::setVideoFile(): file:"<<file<<", mark1, m_qtSource:"<<m_qtSource; 
	
	connect(m_qtSource->player(), SIGNAL(positionChanged(qint64)), this, SIGNAL(positionChanged(qint64))); 
	
	// Reset length for next query to videoLength(), below 
	m_videoLength = -1;
	
	setVideoSource(m_qtSource);
	//qDebug() << "GLVideoFileDrawable::setVideoFile(): file:"<<file<<", mark2, m_qtSource:"<<m_qtSource;
	
	m_qtSource->player()->setVolume(0);
	
	//GLEditorGraphicsScene *scenePtr = dynamic_cast<GLEditorGraphicsScene*>(scene());
	//if(!scenePtr && scene())
	if(1)
	{
		QPropertyAnimation *anim = new QPropertyAnimation(m_qtSource->player(), "volume");
		anim->setEndValue(100);
		anim->setDuration(xfadeLength());
		anim->start();
	}
	//qDebug() << "GLVideoFileDrawable::setVideoFile(): file:"<<file<<", mark3, m_qtSource:"<<m_qtSource;
	//qDebug() << "GLVideoFileDrawable::setVideoFile: "<<file<<": scenePtr:"<<scenePtr<<", scene():"<<scene();
	
#else

	qDebug() << "GLVideoFileDrawable::setVideoFile: "<<file<<": GLVidTex Graphics Engine not compiled with QtMobility support, therefore, unable to play back video files with sound. Use GLVideoLoopDrawable to play videos as loops without QtMobility.";

#endif
	
	emit videoFileChanged(file);
	//qDebug() << "GLVideoFileDrawable::setVideoFile(): file:"<<file<<", mark4, m_qtSource:"<<m_qtSource;
	return true;
	
}

double GLVideoFileDrawable::videoLength()
{
#ifdef HAS_QT_VIDEO_SOURCE
	if(m_videoLength < 0)
	{
		if(!m_qtSource)
		{
			qDebug() << "GLVideoFileDrawable::videoLength: No source set, unable to find length.";
		}
		else
		{ 
			// Duration is in milleseconds, we store length in seconds
			m_videoLength = m_qtSource->player()->duration() / 1000.;
			qDebug() << "GLVideoFileDrawable::videoLength: "<<m_qtSource->file()<<": Duration: "<<m_videoLength;
		}
	}
#endif	
	return m_videoLength;
}	
		

void GLVideoFileDrawable::deleteSource(VideoSource *source)
{
#ifdef HAS_QT_VIDEO_SOURCE
	QtVideoSource *vt = dynamic_cast<QtVideoSource*>(source);
	if(vt)
	{
		qDebug() << "GLVideoFileDrawable::deleteSource: Deleting video thread:" <<vt;
		delete vt;
		vt = 0;
		source = 0;
	}
	else
	{
		qDebug() << "GLVideoFileDrawable::deleteSource: Source not deleted because its not a 'QtVideoSource':" <<source;
	}
#endif
}

void GLVideoFileDrawable::setVolume(int v)
{ 
#ifdef HAS_QT_VIDEO_SOURCE
	//qDebug() << "GLVideoFileDrawable::setVolume(): "<<(QObject*)this<<" source:"<<m_qtSource;
	if(m_qtSource)
	{
		qDebug() << "GLVideoFileDrawable::setVolume(): "<<(QObject*)this<<" New volume:"<<v;
		m_qtSource->player()->setVolume(v);
	}
	else
	{
		//qDebug() << "GLVideoFileDrawable::setVolume(): "<<(QObject*)this<<" Cannot set volume, v:"<<v;
	}
#endif
}

int GLVideoFileDrawable::volume()
{ 
#ifdef HAS_QT_VIDEO_SOURCE
	if(m_qtSource)
		return m_qtSource->player()->volume();
	return 100;
#endif
}

void GLVideoFileDrawable::setMuted(bool flag)
{ 
#ifdef HAS_QT_VIDEO_SOURCE
	if(m_qtSource)
		m_qtSource->player()->setMuted(flag);
#endif
}

bool GLVideoFileDrawable::isMuted()
{
#ifdef HAS_QT_VIDEO_SOURCE
	if(m_qtSource)
		return m_qtSource->player()->isMuted();
	return false;
#endif

}

void GLVideoFileDrawable::setStatus(int status)
{ 
#ifdef HAS_QT_VIDEO_SOURCE
	if(m_qtSource)
	{
		switch(status)
		{
			case 0:
				m_qtSource->player()->stop();
				break;
			case 1:
				m_qtSource->player()->play();
				break;
			case 2:
				m_qtSource->player()->pause();
				break;
			default:
				break;
		}
	}
			
#endif
}

int GLVideoFileDrawable::status()
{
#ifdef HAS_QT_VIDEO_SOURCE
	if(m_qtSource)
		return (int)m_qtSource->player()->state();
	return 0;
#endif

}

void GLVideoFileDrawable::setPosition(qint64 pos)
{ 
#ifdef HAS_QT_VIDEO_SOURCE
	if(m_qtSource)
		m_qtSource->player()->setPosition(pos);
#endif
}

quint64 GLVideoFileDrawable::position()
{ 
#ifdef HAS_QT_VIDEO_SOURCE
	if(m_qtSource)
		return m_qtSource->player()->position();
	return 0;
#endif
}



QVariant GLVideoFileDrawable::itemChange(GraphicsItemChange change, const QVariant &value)
{
/*	if(change == ItemSceneChange)
		qDebug() << "GLVideoDrawable::itemChange: change:"<<change<<", value:"<<value;*/
	if (change == ItemSceneChange)
	{
// 		 QGraphicsScene *newScene = value.value<QGraphicsScene *>();
// 		 if(newScene)
// 		 	setVolume(0);
	}
	return GLDrawable::itemChange(change, value);
}