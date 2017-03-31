#include "VideoProcessor.hpp"

#include <QImage>


VideoProcessor::VideoProcessor()
    :
    decoder( { PIXEL_FORMAT_MJPEG, 320, 240, 30 },
             { PIXEL_FORMAT_RGB24, 320, 240, 30 } )
{}


VideoProcessor::~VideoProcessor()
{
    disconnectPolySync();
}


void VideoProcessor::slotRun()
{
    connectPolySync();
}


void VideoProcessor::initStateEvent()
{
    // Register as a listener for the message type that the publisher
    // is going to send.  Message types are defined in later tutorials.
    registerListener( getMessageTypeByName( "ps_image_data_msg" ) );
}


void VideoProcessor::messageEvent(
        std::shared_ptr< polysync::Message > message )
{
    using namespace polysync::datamodel;
    if( auto incomingMessage = getSubclass< ImageDataMessage >( message ) )
    {
        if( incomingMessage->getPixelFormat() == PIXEL_FORMAT_MJPEG )
        {
            auto buffer = decodeBuffer( incomingMessage->getDataBuffer() );
            if( buffer.size() > 0 )
            {
                auto image = buildQImageFromFrameBuffer( buffer );
                if( not image.isNull() )
                {
                    emit signalPixmap( QPixmap::fromImage( image ) );
                }
            }
        }
        else
        {
            std::cout << "Warning: unsupported pixel format type; "
                         "this image data is not viewable\n";
        }

    }
}


std::vector< uchar > VideoProcessor::decodeBuffer(
        const std::vector<uchar> & buffer )
{
    decoder.decode( buffer );

    return decoder.getCopyOfDecodedBuffer();
}


QImage VideoProcessor::buildQImageFromFrameBuffer( const std::vector< uchar > & buffer )
{
    return QImage{ buffer.data(), 320, 240, QImage::Format_RGB888 };
}
