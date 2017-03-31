#include "ImageDataProcessor.hpp"

#include <QImage>


ImageDataProcessor::ImageDataProcessor()
    :
    decoder( { PIXEL_FORMAT_MJPEG, IMAGE_WIDTH, IMAGE_HEIGHT, FRAME_RATE },
             { PIXEL_FORMAT_RGB24, IMAGE_WIDTH, IMAGE_HEIGHT, FRAME_RATE } )
{}


void ImageDataProcessor::slotFrameBuffer(
        const std::vector< uchar > & frameBuffer )
{
    auto decodedBuffer = decodeBuffer( frameBuffer );

    if( decodedBuffer.size() > 0 )
    {
        auto image = buildQImageFromFrameBuffer( decodedBuffer );

        if( not image.isNull() )
        {
            emit signalPixmap( image );
        }
    }
}


std::vector< uchar > ImageDataProcessor::decodeBuffer(
        const std::vector<uchar> & buffer )
{
    decoder.decode( buffer );

    return decoder.getCopyOfDecodedBuffer();
}


QImage ImageDataProcessor::buildQImageFromFrameBuffer(
        const std::vector< uchar > & buffer )
{
    return QImage{
        buffer.data(), IMAGE_WIDTH, IMAGE_HEIGHT, QImage::Format_RGB888 };
}
