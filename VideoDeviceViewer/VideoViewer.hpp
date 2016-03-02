#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include <QWidget>

class QPixmap;
class QHBoxLayout;
class QLabel;

class VideoViewer : public QWidget
{
    Q_OBJECT

public:

    VideoViewer();

public slots:

    void slotUpdatePixmap( const QPixmap & pixmap );

private:

    QHBoxLayout * _layout{ nullptr };
    QLabel * _label{ nullptr };
};


#endif // VIDEOVIEWER_H
