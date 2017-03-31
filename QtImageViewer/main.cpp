#include <QApplication>
#include <QTimer>

#include "ImageDataNode.hpp"
#include "ImageDataProcessor.hpp"
#include "ImageDataView.hpp"


int main( int argc, char *argv[] )
{
    // Object required to execute Qt GUI functionality
    QApplication app( argc, argv );

    ImageDataNode imageDataNode;

    ImageDataProcessor imageDataProcessor;

    ImageDataView imageDataView;

    // Qt Signal/Slot connections
    // Enables async message passing from node to processor
    QObject::connect( &imageDataNode, &ImageDataNode::signalFrameBuffer,
                      &imageDataProcessor, &ImageDataProcessor::slotFrameBuffer );

    // Enable async message passing between processor and view
    QObject::connect( &imageDataProcessor, &ImageDataProcessor::signalPixmap,
                      &imageDataView, &ImageDataView::slotRenderImage );

    // Start the PolySync event loop in 50 milliseconds. Allow the next
    // statement to spawn the Qt event loop.
    QTimer::singleShot( 50/*ms*/, &imageDataNode, SLOT( slotRun() ) );

    // Begin the blocking Qt event loop
    app.exec();
}
