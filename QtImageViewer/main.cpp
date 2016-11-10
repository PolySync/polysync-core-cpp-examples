#include <QApplication>
#include <QThread>
#include <QTimer>

#include "VideoProcessor.hpp"
#include "VideoViewer.hpp"

#warning "Qt natively supports the MJPEG pixel format; a decoder is required for other pixel formats"

#include <memory>

using namespace std;

int main( int argc, char *argv[] )
{
    cout << "Qt natively supports the MJPEG pixel format; a decoder is required" <<
            "for other incoming source pixel formats" << endl;

    // Object required to execute Qt GUI functionality
    QApplication app( argc, argv );

    auto videoProcessor =
            std::unique_ptr< VideoProcessor >( new VideoProcessor );

    // Thread video processing loop, this will continously pull data from
    // a video device and send a QPixmap to the VideoViewer object for rendering.
    QThread * processingThread = new QThread;
    videoProcessor->moveToThread( processingThread );
    processingThread->start();

    auto videoViewer =
            std::unique_ptr< VideoViewer >( new VideoViewer );

    // Qt signal/slot connect for passing data between processor and viewer.
    QObject::connect( videoProcessor.get(), &VideoProcessor::signalPixmap,
                      videoViewer.get(), &VideoViewer::slotUpdatePixmap );

    // Start processing in fifty milliseconds, this allows for the next line of
    // code to be called, spawning the Qt application context.
    QTimer::singleShot( 50/*ms*/, videoProcessor.get(), SLOT( slotRun() ) );

    // Blocking loop until the Viewer widget is closed.
    int appReturn = app.exec();

    // Halt thread execution and release dynamic memory
    processingThread->requestInterruption();
    processingThread->deleteLater();

    return appReturn;
}
