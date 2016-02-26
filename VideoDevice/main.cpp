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
                QImage img( videoDevice.getBuffer().data(),
                            1000,
                            1000,
                            QImage::Format::Format_RGB888 );

                QPixmap map = QPixmap::fromImage( img.copy() );
                label->setPixmap( map );
            }
        }
    }
    catch( polysync::DTCException & e )
    {
        cout << e.what() << endl;
    }

    return app.exec();
}
