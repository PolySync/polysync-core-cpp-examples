#include <PolySyncDataModel.hpp>

#include "ImageDataNode.hpp"


ImageDataNode::ImageDataNode()
{
    // Inform Qt event loop about std:: type passed from signal to slot
    qRegisterMetaType< std::vector< uchar > >( "std::vector< uchar >&" );

    moveToThread( &thread );

    thread.start();
}


ImageDataNode::~ImageDataNode()
{
    disconnectPolySync();

    thread.quit();

    thread.wait();
}


void ImageDataNode::slotRun()
{
    connectPolySync();
}


void ImageDataNode::initStateEvent()
{
    registerListener( getMessageTypeByName( "ps_image_data_msg" ) );
}


void ImageDataNode::messageEvent( std::shared_ptr< polysync::Message > message )
{
    using namespace polysync::datamodel;

    if( auto imageDataMsg = getSubclass< ImageDataMessage >( message ) )
    {
        if( imageDataMsg->getPixelFormat() == PIXEL_FORMAT_MJPEG )
        {
            signalFrameBuffer( imageDataMsg->getDataBuffer() );
        }
        else
        {
            std::cout << "Warning: unsupported pixel format type; "
                         "this image data is not viewable\n";
        }

    }
}
