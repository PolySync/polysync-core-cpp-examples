#include <iostream>
#include <PolySyncNode>
#include <PolySyncMessage>
#include <PolySyncDataModel>

using namespace std;

int main()
{
    polysync::Node node;
    auto nodeRef = node.reference();
    auto index = OBJECT_CLASSIFICATION_CAR;
    while( 1 )
    {
        // create a message
        polysync::ObjectsMessage message( node.reference() );

        // set length of the output buffer
        message.setObjectsOutputBufferLength( 1 );

        // populate an object for the output buffer
        ps_object o;

        // alternate data
        if( index == OBJECT_CLASSIFICATION_CAR )
        {
            o.classification = index;
            o.position[0] = 5;
            o.position[1] = 5;
            o.position[2] = 1;
            o.id = 0;

            o.course_angle = 90.0;
            index = OBJECT_CLASSIFICATION_PEDESTRIAN;
        }
        else if( index == OBJECT_CLASSIFICATION_PEDESTRIAN )
        {
            o.classification = index;
            o.position[0] = 5;
            o.position[1] = -5;
            o.position[2] = 1;
            o.id = 0;

            o.course_angle = -90.0;
            index = OBJECT_CLASSIFICATION_BIKE;
        }
        else if( index == OBJECT_CLASSIFICATION_BIKE )
        {
            o.classification = index;
            o.position[0] = 5;
            o.position[1] = 0;
            o.position[2] = 1;
            o.id = 0;

            o.course_angle = 0;
            index = OBJECT_CLASSIFICATION_CAR;
        }
        // place object to the object buffer in the message
        message.objectsBufferPushBack( o );

        // publish message to the bus
        polysync::message::publish( nodeRef, message );

        // sleep for one second
        sleep( 1 );
    }
    return 0;
}

