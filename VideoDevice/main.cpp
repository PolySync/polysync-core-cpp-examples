#include <PolySyncVideo.hpp>
#include <PolySyncNode.hpp>
#include <PolySyncDTCException.hpp>

#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QLayout>
#include <QLabel>
#include <QTimer>

#include <iostream>

using namespace std;

class VideoViewer : public QWidget
{
public:
    VideoViewer(){}

signals:

public slots:
    void slotReceiveEncodedBuffer( std::vector< uchar > encodedBuffer )
    {

    }

private:
    QLabel _label;
    QHBoxLayout _layout;


    polysync::VideoFormat inFormat{ PIXEL_FORMAT_YUYV, 640, 480, 30 };
    polysync::VideoFormat outFormat{ PIXEL_FORMAT_H264, 640, 480, 30 };
};

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
                auto bufferLength = videoDevice.getBufferLength();
                encoder.encode( videoDevice.getBuffer() );
                sleep( 4 );

                auto eBuffer = encoder.getCopyOfEncodedBuffer( bufferLength );
                cout << "encoded buffer size(): " << buffer.size() << endl;

                decoder.decode( encoder.getCopyOfEncodedBuffer( bufferLength ) );
                sleep( 4 );

                auto dBuffer = decoder.getCopyOfDecodedBuffer( bufferLength );
                cout << "decoded buffer.size(): " << buffer.size() << endl;

                QImage img( dBuffer.data(),
                            640,
                            480,
                            QImage::Format::Format_RGB888 );

                label->setPixmap( QPixmap::fromImage( img ) );
            }
        }
    }
    catch( polysync::DTCException & e )
    {
        cout << e.what() << endl;
    }

    return app.exec();
}
