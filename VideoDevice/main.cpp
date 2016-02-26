#include <PolySyncVideo.hpp>
#include <PolySyncNode.hpp>
#include <PolySyncDTCException.hpp>

#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QLayout>
#include <QLabel>

#include <iostream>

using namespace std;

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    QWidget widget;
    widget.resize( 640, 480 );

    QHBoxLayout * layout = new QHBoxLayout;
    widget.setLayout( layout );

    QLabel * label = new QLabel;
    layout->addWidget( label );
    widget.show();

    polysync::VideoFormat inFormat( PIXEL_FORMAT_YUYV, 640, 480, 30 );
    polysync::VideoFormat outFormat( PIXEL_FORMAT_H264, 640, 480, 30 );

    polysync::VideoEncoder encoder( inFormat, outFormat );
    polysync::VideoDecoder decoder( outFormat, inFormat );
    try
    {
        auto videoDeviceList = polysync::getAvailableVideoDevices();

        if( videoDeviceList.size() > 0 )
        {
            auto videoDevice = videoDeviceList[ 0 ];

            polysync::VideoFormat format( PIXEL_FORMAT_YUYV, 640, 480, 30 );
            if( ! videoDevice.formatIsSupported( format ) )
            {
                return 0;
            }

            videoDevice.setFormat( format );

            videoDevice.enableStreaming();

            cout << "Device Name: " << videoDevice.getName() << endl;
            cout << "Device Handle: " << videoDevice.getType() << endl;

            if( videoDevice.poll() )
            {
                cout << "Polled frame size: " << videoDevice.getBufferLength() << endl;

                auto buffer = videoDevice.getBuffer();

                cout << "buffer.size(): " << buffer.size() << endl;

                std::vector< uchar > copyOfBuffer{ buffer };
                QImage img( copyOfBuffer.data(),
                            640,
                            480,
                            QImage::Format::Format_RGB888 );

                label->setPixmap( QPixmap::fromImage( img ) );
//                polysync::sleepMicro( 1000000 );
            }
        }
    }
    catch( polysync::DTCException & e )
    {
        cout << e.what() << endl;
    }

    return app.exec();
}
