#include <VideoProcessor.hpp>

#include <PolySyncVideo.hpp>
#include <PolySyncDTCException.hpp>

#include <QImage>
#include <QPixmap>
#include <QThread>

#include <iostream>

using namespace std;

void VideoProcessor::slotRun()
{
    // Set the desired format for video device.
    polysync::VideoFormat deviceFormat{
        PIXEL_FORMAT_MJPEG,
        640, 480,
        PSYNC_VIDEO_DEFAULT_FRAMES_PER_SECOND };

    try
    {
        // Search system for possible devices
        auto videoDeviceList = polysync::getAvailableVideoDevices();

        if( videoDeviceList.size() > 0 )
        {
            // Grab the first available device, likely /dev/video0 in Linux.
            auto videoDevice = videoDeviceList[ 0 ];

            // Make sure the device supports our desired format
            if( ! videoDevice.formatIsSupported( deviceFormat ) )
            {
                return;
            }

            videoDevice.setFormat( deviceFormat );

            videoDevice.enableStreaming();

            // Pull data from device
            while( videoDevice.poll() )
            {
                auto frame = videoDevice.getFrame();

                emit signalPixmap( QPixmap::fromImage(
                                       QImage::fromData(
                                           frame.data(),
                                           frame.size() ) ) );
            }
        }
    }
    catch( polysync::DTCException & e )
    {
        cout << e.what() << endl;
    }
}
