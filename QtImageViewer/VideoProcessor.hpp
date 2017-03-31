#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>
#include <PolySyncDTCException.hpp>
#include <PolySyncVideo.hpp>

#include <QObject>
#include <QPixmap>
#include <QThread>

/**
 * @brief The VideoProcessor class
 * This class is responsible for interaction with PolySync and the incoming image data
 * messages.
 * This class connects to the PolySync bus, and subscribes to the ImageDataMessage
 * (ps_image_data_msg)
 * For each image received, a signal is emitted to the VideoViewer to render.
 */
class VideoProcessor : public QObject, public polysync::Node
{
    // Qt Macro allowing for signals/slots
    Q_OBJECT

public:

    VideoProcessor();

    ~VideoProcessor();

    /**
     * This method is called once after calling polysync::Node::connectPolySync.
     * It is used here to register as a listener for image data messaages.
     */
    void initStateEvent() override;

    /**
     * Extract the information from the provided message
     *
     * @param std::shared_ptr< Message > - variable containing the message
     */
    void messageEvent( std::shared_ptr< polysync::Message > message ) override;


public slots:

    /**
     * Contains the busy loop which polls for image data, packages
     * the data into a QPixmap, and sends to the VideoViewer object for
     * rendering.
     */
    void slotRun();


signals:

    /**
     * Qt Signal emitted when new data is ready for rendering. This triggers
     * @ref VideoViewer::slotUpdatePixmap, passing the new QPixmap to draw.
     */
    void signalPixmap( const QPixmap & );


private:

    QImage buildQImageFromFrameBuffer( const std::vector< uchar > & buffer );

    std::vector< uchar > decodeBuffer(
            const std::vector< uchar > & buffer );

    polysync::VideoDecoder decoder;

    QThread thread;

    static constexpr auto IMAGE_WIDTH = 320;

    static constexpr auto IMAGE_HEIGHT = 240;

    static constexpr auto FRAME_RATE = 30;
};

#endif // VIDEOPROCESSOR_H
