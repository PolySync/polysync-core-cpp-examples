#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>
#include <PolySyncVideo.hpp>
#include <PolySyncDTCException.hpp>

#include <QObject>
#include <QImage>
#include <QPixmap>

using namespace std;

/**
 * @brief The VideoProcessor class
 * This class is responsible for interaction with PolySync and the incoming image data
 * messages.
 * This class connects to the PolySync bus, and subscribes to the ImageDataMessage
 * (ps_image_data_msg)
 * For each image received, a signal is emitted to the Pixmap.
 */
class VideoProcessor : public QObject, public polysync::Node
{
    // Qt Macro allowing for signals/slots
    Q_OBJECT

public:


    ~VideoProcessor();

    void initStateEvent() override
        {
            auto messageType = getMessageTypeByName( "ps_image_data_msg" );

            // Register as a listener for the message type that the publisher
            // is going to send.  Message types are defined in later tutorials.
            registerListener( messageType );
        }

        /**
         * @brief messageEvent
         *
         * Extract the information from the provided message
         *
         * @param std::shared_ptr< Message > - variable containing the message
         */
        virtual void messageEvent( shared_ptr< polysync::Message > message )
        {
            using namespace polysync::datamodel;
            if( shared_ptr< ImageDataMessage > incomingMessage = getSubclass< ImageDataMessage >( message ) )
            {

                if( incomingMessage->getPixelFormat() != PIXEL_FORMAT_MJPEG )
                {
                    cout << "Warning: unsupported pixel format type; this image data is not viewable" << endl;
                }

                auto buffer = incomingMessage->getDataBuffer();

                emit signalPixmap( QPixmap::fromImage(
                                       QImage::fromData(
                                           buffer.data(), // get the C-style pointer
                                           buffer.size() ) ) );
            }
        }


public slots:

    /**
     * @brief slotRun
     * Contains the busy loop which polls for image data, packages
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
