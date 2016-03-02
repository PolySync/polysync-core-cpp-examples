#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <PolySyncVideo.hpp>
#include <PolySyncDTCException.hpp>

#include <QObject>

class VideoProcessor : public QObject
{
    Q_OBJECT

public slots:
    void slotRun();

signals:
    void signalPixmap( const QPixmap & );
};
#endif // VIDEOPROCESSOR_H
