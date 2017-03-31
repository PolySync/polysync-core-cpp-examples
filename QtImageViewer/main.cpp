#include <QApplication>
#include <QTimer>

#include "VideoProcessor.hpp"
#include "VideoViewer.hpp"


int main( int argc, char *argv[] )
{
    // Object required to execute Qt GUI functionality
    QApplication app( argc, argv );

    VideoProcessor videoProcessor;

    VideoViewer videoViewer;

    // Qt signal/slot connect for passing data between processor and viewer.
    QObject::connect( &videoProcessor, &VideoProcessor::signalPixmap,
                      &videoViewer, &VideoViewer::slotUpdatePixmap );

    // Start processing in fifty milliseconds, this allows for the next line of
    // code to be called, spawning the Qt application context.
    QTimer::singleShot( 50/*ms*/, &videoProcessor, SLOT( slotRun() ) );

    // Blocking loop until the Viewer widget is closed.
    app.exec();
}
