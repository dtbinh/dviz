#include <QApplication>

#include "GLWidget.h"
#include "../livemix/VideoThread.h"
#include "../livemix/CameraThread.h"

#include "GLVideoDrawable.h"
#include "StaticVideoSource.h"
#include "TextVideoSource.h"

#ifdef HAS_QT_VIDEO_SOURCE
#include "QtVideoSource.h"
#endif

GLDrawable * addCamera(GLWidget *glw, QString camera = "")
{
	#ifdef Q_OS_WIN
		QString defaultCamera = "vfwcap://0";
	#else
		QString defaultCamera = "/dev/video0";
	#endif

	CameraThread *source = CameraThread::threadForCamera(camera.isEmpty() ? defaultCamera : camera);
	if(source)
	{
		source->setFps(30);
		usleep(750 * 1000); // This causes a race condition to manifist itself reliably, which causes a crash every time instead of intermitently. 
		// With the crash reproducable, I can now work to fix it.
		source->enableRawFrames(true);
		//source->setDeinterlace(true);
		
		GLVideoDrawable *drawable = new GLVideoDrawable(glw);
		drawable->setVideoSource(source);
		drawable->setRect(glw->viewport());
		
		if(camera != "/dev/video1")
			drawable->setAlphaMask(QImage("alphamask2.png"));
		
		glw->addDrawable(drawable);
		drawable->addShowAnimation(GLDrawable::AnimFade);
		//drawable->addShowAnimation(GLDrawable::AnimZoom,2500).curve = QEasingCurve::OutElastic;
		
		//drawable->addHideAnimation(GLDrawable::AnimSlideBottom,1000);
		drawable->addHideAnimation(GLDrawable::AnimFade);
		//drawable->addHideAnimation(GLDrawable::AnimFade).startDelay = 500;
		//drawable->addHideAnimation(GLDrawable::AnimZoom,1000);
		drawable->show();
		drawable->setObjectName(qPrintable(camera.isEmpty() ? defaultCamera : camera));
		
// 		QTimer *timer = new QTimer;
// 		QObject::connect(timer, SIGNAL(timeout()), drawable, SLOT(hide()));
// 		timer->start(5000);
// 		timer->setSingleShot(true);
// 		


		VideoDisplayOptionWidget *opts = new VideoDisplayOptionWidget(drawable);
		opts->adjustSize();
		opts->show();
		return drawable;
	}

	return 0;
}

GLDrawable * addQtSource(GLWidget * glw)
{
	#ifdef HAS_QT_VIDEO_SOURCE
	//QString testFile = "/opt/qt-mobility-opensource-src-1.0.1/examples/player/dsc_7721.avi";
	QString testFile = "dsc_0259.avi";
	
	QtVideoSource *source = new QtVideoSource();
	source->setFile(testFile);
	source->start();
	
	GLVideoDrawable *drawable = new GLVideoDrawable(glw);
	drawable->setVideoSource(source);
	drawable->setRect(glw->viewport());
	glw->addDrawable(drawable);
	drawable->addShowAnimation(GLDrawable::AnimFade);
	//drawable->addShowAnimation(GLDrawable::AnimZoom);
	drawable->addShowAnimation(GLDrawable::AnimSlideTop,1000);
	
	drawable->addHideAnimation(GLDrawable::AnimZoom,1000);
	drawable->addHideAnimation(GLDrawable::AnimFade);
	drawable->show();
	drawable->setObjectName("QtVideoSource");
	
// 	QTimer *timer = new QTimer;
// 	QObject::connect(timer, SIGNAL(timeout()), drawable, SLOT(hide()));
// 	timer->start(5000);
// 	timer->setSingleShot(true);
// 	
	return drawable;

	#else
	
	Q_UNUSED(glw)
	
	return 0;
	
	#endif

}

GLDrawable * addSecondSource(GLWidget * glw)
{
	// add secondary frame
	GLVideoDrawable *drawable = new GLVideoDrawable(glw);
	
	
	StaticVideoSource *source = new StaticVideoSource();
	source->setImage(QImage("me2.jpg"));
	//source->setImage(QImage("/opt/qtsdk-2010.02/qt/examples/opengl/pbuffers/cubelogo.png"));
	
	source->start();
	drawable->setVideoSource(source);
	drawable->setRect(glw->viewport());
	drawable->setZIndex(-1);
	drawable->setObjectName("SecondSource");
	//drawable->setOpacity(0.5);
	//drawable->show();
	
	
	#ifdef HAS_QT_VIDEO_SOURCE
	// just change enterance anim to match effects
	drawable->addShowAnimation(GLDrawable::AnimFade);
	drawable->addShowAnimation(GLDrawable::AnimZoom);
	#else
	drawable->addShowAnimation(GLDrawable::AnimSlideTop,1000);
	#endif
	
	drawable->addHideAnimation(GLDrawable::AnimFade);
	drawable->addHideAnimation(GLDrawable::AnimZoom);
	
// 	QTimer *timer = new QTimer;
// 	QObject::connect(timer, SIGNAL(timeout()), drawable, SLOT(show()));
// 	timer->start(5000);
// 	timer->setSingleShot(true);
// 	
// 	timer = new QTimer;
// 	QObject::connect(timer, SIGNAL(timeout()), drawable, SLOT(hide()));
// 	timer->start(8000);
// 	timer->setSingleShot(true);
	
	glw->addDrawable(drawable);
	
	return drawable;
}

GLDrawable * addVideoBug(GLWidget *glw)
{
	GLVideoDrawable *drawable = new GLVideoDrawable(glw);
	
	
	StaticVideoSource *source = new StaticVideoSource();
	//source->setImage(QImage("me2.jpg"));
	QImage image = QImage("/opt/qtsdk-2010.02/qt/examples/opengl/pbuffers/cubelogo.png");
	if(image.isNull())
		image = QImage("../data/icon-next-large.png");
		//image = QImage("/home/josiah/Documents/JDBryanPhotography/Logo.png");
	source->setImage(image);
	
	source->start();
	drawable->setVideoSource(source);
	drawable->setRect(QRectF(
		qMax(glw->viewport().right()  - image.width()  * 1.75, 50.0),
		qMax(glw->viewport().bottom() - image.height() * 1.25, 50.0),
		image.width(),
		image.height()));
	drawable->setZIndex(1);
	drawable->setOpacity(0.5);
	drawable->setObjectName("VideoBug");
	//drawable->show();
	
	drawable->addShowAnimation(GLDrawable::AnimSlideLeft,2500).curve = QEasingCurve::OutElastic;
	
	drawable->addHideAnimation(GLDrawable::AnimFade,300);
	drawable->addHideAnimation(GLDrawable::AnimZoom,300);
	
// 	QTimer * timer = new QTimer;
// 	QObject::connect(timer, SIGNAL(timeout()), drawable, SLOT(show()));
// 	timer->start(2000);
// 	timer->setSingleShot(true);
// 	
// 	timer = new QTimer;
// 	QObject::connect(timer, SIGNAL(timeout()), drawable, SLOT(hide()));
// 	timer->start(8000);
// 	timer->setSingleShot(true);
	
	glw->addDrawable(drawable);
	
	return drawable;
}

GLDrawable * addStaticSource(GLWidget * glw)
{
	// add secondary frame
	GLVideoDrawable *drawable = new GLVideoDrawable(glw);
	
	
	StaticVideoSource *source = new StaticVideoSource();
	//source->setImage(QImage("me2.jpg"));
	QImage img("dsc_6645-1.jpg");
	if(img.isNull())
		source->setImage(QImage("me2.jpg"));
	else
		source->setImage(img);
	//source->setImage(QImage("/opt/qtsdk-2010.02/qt/examples/opengl/pbuffers/cubelogo.png"));
	
	source->start();
	drawable->setVideoSource(source);
	drawable->setRect(glw->viewport());
	drawable->setZIndex(-1);
	drawable->setObjectName("Static");
	//drawable->setOpacity(0.5);
	//drawable->show();
	
	
// 	#ifdef HAS_QT_VIDEO_SOURCE
// 	// just change enterance anim to match effects
// 	drawable->addShowAnimation(GLDrawable::AnimFade);
 	//drawable->addShowAnimation(GLDrawable::AnimZoom);
// 	#else
	drawable->addShowAnimation(GLDrawable::AnimZoom,2500).curve = QEasingCurve::OutElastic;
// 	#endif
// 	
 	drawable->addHideAnimation(GLDrawable::AnimFade);
 	drawable->addHideAnimation(GLDrawable::AnimZoom,1000);
// 	
// 	QTimer *timer = new QTimer;
// 	QObject::connect(timer, SIGNAL(timeout()), drawable, SLOT(show()));
// 	timer->start(5000);
// 	timer->setSingleShot(true);
// 	
// 	timer = new QTimer;
// 	QObject::connect(timer, SIGNAL(timeout()), drawable, SLOT(hide()));
// 	timer->start(8000);
// 	timer->setSingleShot(true);
	
// 	VideoDisplayOptionWidget *opts = new VideoDisplayOptionWidget(drawable);
// 	opts->adjustSize();
// 	opts->show();

	glw->addDrawable(drawable);
	
	//drawable->show();
	
	return drawable;
}



GLDrawable * addTextOverlay(GLWidget * glw)
{
	// add text overlay frame
	GLVideoDrawable *drawable = new GLVideoDrawable(glw);
	
	
	TextVideoSource *source = new TextVideoSource();
	source->start();
	//source->setHtml("<img src='me2.jpg'><b>TextVideoSource</b>");
	source->setHtml("<b>Welcome to LiveMix</b>");
	source->changeFontSize(40);
	QSize size = source->findNaturalSize();
	source->setTextWidth(size.width());
	//qDebug() << "New html: "<<source->html();
	//source->setImage(QImage("/opt/qtsdk-2010.02/qt/examples/opengl/pbuffers/cubelogo.png"));
	
	drawable->setVideoSource(source);
	//drawable->setRect(glw->viewport());
	//qDebug() << "Text Size: "<<size;
	
	drawable->setRect(QRectF(
		qMax(glw->viewport().right()  - size.width()  , 0.0),
		qMax(glw->viewport().bottom() - size.height() , 0.0),
		size.width(),
		size.height()));
	drawable->setZIndex(1);
	//drawable->setOpacity(0.5);
	drawable->setObjectName("Text");
	
	//drawable->addShowAnimation(GLDrawable::AnimFade);
	//drawable->addShowAnimation(GLDrawable::AnimSlideTop,2500).curve = QEasingCurve::OutElastic;
 	
 	drawable->addHideAnimation(GLDrawable::AnimFade);
 	drawable->addHideAnimation(GLDrawable::AnimZoom);
 	
// 	VideoDisplayOptionWidget *opts = new VideoDisplayOptionWidget(drawable);
// 	opts->adjustSize();
// 	opts->show();

	glw->addDrawable(drawable);
	
// 	QTimer *timer = new QTimer;
// 	QObject::connect(timer, SIGNAL(timeout()), drawable, SLOT(show()));
// 	timer->start(5000);
// 	timer->setSingleShot(true);
	
	drawable->show();
	
	return drawable;
}


QFormLayout * createToggleBox()
{
	QWidget *tb = new QWidget();
	tb->setWindowTitle("Toggle Box");
	QFormLayout *layout = new QFormLayout;
	tb->setLayout(layout);
	tb->show();
	return layout;
}

void addButtons(QFormLayout *layout, GLDrawable *glw)
{
	QHBoxLayout *box = new QHBoxLayout;
	
	QPushButton *show = new QPushButton("Show");
	QObject::connect(show, SIGNAL(clicked()), glw, SLOT(show()));
	QObject::connect(glw, SIGNAL(isVisible(bool)), show, SLOT(setDisabled(bool)));
	show->setDisabled(glw->isVisible());
	
	QPushButton *hide = new QPushButton("Hide");
	QObject::connect(hide, SIGNAL(clicked()), glw, SLOT(hide()));
	QObject::connect(glw, SIGNAL(isVisible(bool)), hide, SLOT(setEnabled(bool)));
	hide->setEnabled(glw->isVisible());
	
	box->addWidget(show);
	box->addWidget(hide);
	
	layout->addRow(glw->objectName(), box); 
}


int main(int argc, char *argv[])
{

	QApplication app(argc, argv);
	GLWidget *glw = new GLWidget();
		
	QFormLayout * tb = createToggleBox();
	#undef HAS_QT_VIDEO_SOURCE
	#ifdef HAS_QT_VIDEO_SOURCE
		addButtons(tb, addQtSource(glw));
	#else
		GLDrawable *d;
/*		d = addCamera(glw,"/dev/video0");
		if(d)
		{*/
			d = addCamera(glw,"/dev/video1");
			if(d)
				addButtons(tb,d); 
			
			d = addCamera(glw,"/dev/video0");
			if(d)
				addButtons(tb,d); 

/*		}
		else
		*/
			addButtons(tb,addStaticSource(glw));
	#endif
	
	addButtons(tb,addSecondSource(glw));	
	addButtons(tb,addVideoBug(glw));
	addButtons(tb,addTextOverlay(glw));
	
	glw->resize(glw->viewport().width(),glw->viewport().height());
	
	glw->show();
	
	int x = app.exec();
	delete glw;
	return x;
}



	
	
// 	if(!thread)
// 	{
// 		VideoThread * thread = new VideoThread();
// 		thread->setVideo("../data/Seasons_Loop_3_SD.mpg");
// 		thread->start();
// 		m_source = thread;
// 	}