#ifndef VIDEOFILECONTENT_H
#define VIDEOFILECONTENT_H

#include "AbstractContent.h"
//#include "qvideo/QVideoBuffer.h"
#include "qvideo/QVideo.h"
#include "qvideo/QVideoProvider.h"
#include "ButtonItem.h"


/// \brief TODO
class VideoFileContent : public AbstractContent
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    
    public:
	VideoFileContent(QGraphicsScene * scene, QGraphicsItem * parent = 0);
	~VideoFileContent();
	
	QRectF boundingRect() const;

    public Q_SLOTS:
	QString filename() const { return m_filename; }
	void setFilename(const QString & text);

    private slots:
	void setPixmap(const QPixmap & pixmap);
	void slotTogglePlay();

    public:
        // ::AbstractContent
	QString contentName() const { return tr("VideoFile"); }
	QWidget * createPropertyWidget();
	
	QPixmap renderContent(const QSize & size, Qt::AspectRatioMode ratio) const;
	int contentHeightForWidth(int width) const;
	
	void syncFromModelItem(AbstractVisualItem*);
	AbstractVisualItem * syncToModelItem(AbstractVisualItem*);
	
	void applySceneContextHint(MyGraphicsScene::ContextHint);
	
	// ::QGraphicsItem
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	
    private:
//         void updateTextConstraints();
//         void updateCache();

        // text document, layouting & rendering
         QString m_filename;
//         QList<QRect> m_blockRects;
//         QRect m_textRect;
//         int m_textMargin;
         QPixmap m_pixmap;
         QSize m_imageSize;
         //QVideo * m_video;
         QVideoProvider * m_videoProvider;
         ButtonItem * m_bSwap;
         bool m_still;

};

#endif
