#include <QApplication>
#include <QThread>
#include <QTimer>

#include "VideoProcessor.hpp"
#include "VideoViewer.hpp"

#include <memory>

using namespace std;

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    auto videoProcessor =
            std::unique_ptr< VideoProcessor >( new VideoProcessor );

    QThread * processingThread = new QThread;
    videoProcessor->moveToThread( processingThread );
    processingThread->start();

    auto videoViewer =
            std::unique_ptr< VideoViewer >( new VideoViewer );

    QObject::connect( videoProcessor.get(), &VideoProcessor::signalPixmap,
                      videoViewer.get(), &VideoViewer::slotUpdatePixmap );

    QTimer::singleShot( 1, videoProcessor.get(), SLOT( slotRun() ) );

    int appReturn = app.exec();

    processingThread->requestInterruption();
    processingThread->deleteLater();

    return appReturn;
}
