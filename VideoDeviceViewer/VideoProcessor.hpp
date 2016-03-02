#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <PolySyncVideo.hpp>
#include <PolySyncDTCException.hpp>

#include <QObject>

/**
 * @brief The VideoProcessor class
 * This class is responsible for interaction with a video device using the
 * PolySyncVideo API. It attempts to open the first available video device on
 * the system. In Linux, this is often /dev/video0. On a laptop, this will
 * likely be the webcam. On a desktop machine, a video device will need to be
 * connected, typically via USB.
 */
class VideoProcessor : public QObject
{
    // Qt Macro allowing for signals/slots
    Q_OBJECT

public slots:

    /**
     * @brief slotRun
     * Contains the busy loop which polls video device for frames, packages
     * the data into a QPixmap, and sends to the VideoViewer object for
     * rendering.
     */
    void slotRun();

signals:

    /**
     * @brief signalPixmap
     * Qt Signal emitted when new data is ready for rendering. This triggers
     * @ref VideoViewer::slotUpdatePixmap, passing the new QPixmap to draw.
     */
    void signalPixmap( const QPixmap & );
};
#endif // VIDEOPROCESSOR_H
