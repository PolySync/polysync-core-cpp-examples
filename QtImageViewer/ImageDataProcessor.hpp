#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H


#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>
#include <PolySyncDTCException.hpp>
#include <PolySyncVideo.hpp>

#include <QObject>
#include <QThread>

/**
 * @brief The ImageDataProcessor class
 * This class is responsible for interaction with PolySync and the incoming image data
 * messages.
 * This class connects to the PolySync bus, and subscribes to the ImageDataMessage
 * (ps_image_data_msg)
 * For each image received, a signal is emitted to the ImageDataView to render.
 */
class ImageDataProcessor : public QObject, public polysync::Node
{
    // Qt Macro allowing for signals/slots
    Q_OBJECT

public:

    ImageDataProcessor();


public slots:

    void slotFrameBuffer( const std::vector< uchar > & frameBuffer );


signals:

    /**
     * Qt Signal emitted when new data is ready for rendering. This triggers
     * @ref VideoViewer::slotUpdatePixmap, passing the new QPixmap to draw.
     */
    void signalPixmap( const QImage & );


private:

    QImage buildQImageFromFrameBuffer( const std::vector< uchar > & buffer );

    std::vector< uchar > decodeBuffer(
            const std::vector< uchar > & buffer );

    polysync::VideoDecoder decoder;

    static constexpr auto IMAGE_WIDTH = 320;

    static constexpr auto IMAGE_HEIGHT = 240;

    static constexpr auto FRAME_RATE = 30;
};


#endif // VIDEOPROCESSOR_H
