#include <iostream>
#include <PolySyncNode>
#include <PolySyncMessage>
#include <PolySyncDataModel>

using namespace std;

int main()
{
    polysync::Node node;
    auto nodeRef = node.reference();
    while( 1 )
    {
        // create a message
        polysync::ObjectsMessage message( node.reference() );

        // set length of the output buffer
        message.setObjectsOutputBufferLength( 1 );

        // populate an object for the output buffer
        ps_object o;
        o.classification = OBJECT_CLASSIFICATION_CAR;
        o.classification_quality = QUALITY_9;
        o.course_angle = 90.0;
        o.id = 0;
        o.position[0] = 5;
        o.position[1] = 5;
        o.position[2] = 5;

        o.timestamp = 654645;
        o.velocity[0] = 5;
        o.velocity[1] = 5;
        o.velocity[2] = 5;

        // place object to the object buffer in the message
        message.objectsBufferPushBack( o );

        // publish message to the bus
        polysync::message::publish( nodeRef, message );

        // sleep for one second
        sleep( 1 );
    }
    return 0;
}

