#include <iostream>
#include <PolySyncNode>
#include <PolySyncMessage>
#include <PolySyncDataModel>

using namespace std;

int main()
{
    // Creating a node initializes PolySync
    polysync::Node node;

    // Grab PolySync node reference for message creation and publishing.
    auto nodeRef = node.reference();

    // Initialize classifcation kind
    auto classifictionIndex = OBJECT_CLASSIFICATION_CAR;

    while( 1 )
    {
        // Create a message
        polysync::ObjectsMessage message( nodeRef );

        // Set length of the output buffer
        message.setObjectsOutputBufferLength( 1 );

        // Populate an object for the output buffer
        ps_object o;

        // Set x,y,z size of object
        o.size[ 0 ] = 5;
        o.size[ 1 ] = 5;
        o.size[ 2 ] = 5;

        // Alternate object classification kind
        if( classifictionIndex == OBJECT_CLASSIFICATION_CAR )
        {
            o.classification = index;
            o.position[0] = 5;
            o.position[1] = 5;
            o.position[2] = 1;
            o.id = 0;

            o.course_angle = 90.0;
            classifictionIndex = OBJECT_CLASSIFICATION_PEDESTRIAN;
        }
        else if( classifictionIndex == OBJECT_CLASSIFICATION_PEDESTRIAN )
        {
            o.classification = index;
            o.position[0] = 5;
            o.position[1] = -5;
            o.position[2] = 1;
            o.id = 0;

            o.course_angle = -90.0;
            classifictionIndex = OBJECT_CLASSIFICATION_BIKE;
        }
        else if( classifictionIndex == OBJECT_CLASSIFICATION_BIKE )
        {
            o.classification = index;
            o.position[0] = 5;
            o.position[1] = 0;
            o.position[2] = 1;
            o.id = 0;

            o.course_angle = 0;
            classifictionIndex = OBJECT_CLASSIFICATION_CAR;
        }

        // Place object to the object buffer in the message
        message.objectsBufferPushBack( o );

        // Publish message to the bus
        polysync::message::publish( nodeRef, message );

        // Slow the loop down by sleeping for one second
        sleep( 1 );
    }

    return 0;
}

