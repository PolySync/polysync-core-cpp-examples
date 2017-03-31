#ifndef IMAGE_DATA_NODE_H
#define IMAGE_DATA_NODE_H


#include <QObject>
#include <QThread>

#include <PolySyncNode.hpp>


/**
 * @brief The ImageDataNode class
 * This class is the interface to PolySync. It is responsible for connecting
 * to PolySync and registering to listen for image data. When a PolySync Video
 * Device publishes image data, this node receives it and passes the frame
 * buffer to the ImageDataProcessor.
 *
 * @note This object is contained in a separate QThread to allow both the
 * polysync::Node event loop and the QApplication event loop to run
 * asychronously.
 */
class ImageDataNode : public QObject, public polysync::Node
{
    // Qt Macro allowing for signals/slots
    Q_OBJECT

public:

    ImageDataNode();

    ~ImageDataNode();


public slots:

    /**
     * Calls polysync::Node::connectPolySync() which is a blocking event loop.
     */
    void slotRun();


signals:

    /**
     * Pass image frame to ImageDataProcessor
     * @param frameBuffer This represents a single image data frame
     */
    void signalFrameBuffer( const std::vector< uchar > & frameBuffer );


private:

    void initStateEvent() override;

    void messageEvent( std::shared_ptr< polysync::Message > message ) override;

    QThread thread;

};


#endif
