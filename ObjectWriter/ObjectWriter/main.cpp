/*
 * Copyright (c) 2015 HARBRICK TECHNOLOGIES, INC
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <iostream>
#include <PolySyncNode>
#include <PolySyncMessage>
#include <PolySyncDataModel>

using namespace std;

/*******************************************************************************
 * Convenience Method Prototypes
 ******************************************************************************/
void setObjectValues( ps_object &object, ps_object_classification_kind &kind );
void setRadarTargetValues( ps_radar_target &target, int index );
void setLidarPointValues( ps_lidar_point &point, int index );
/*******************************************************************************
 * END Convenience Method Prototypes
 ******************************************************************************/




/*******************************************************************************
 * Publish Method Prototypes
 ******************************************************************************/
void publishObjectsMessage( polysync::NodeReference nodeRef,
                            ps_object_classification_kind &kind );
void publishRadarTargetsMessage( polysync::NodeReference nodeRef );
void publishLidarPointsMessage( polysync::NodeReference nodeRef );
/*******************************************************************************
 * END Publish Method Prototypes
 ******************************************************************************/




/*******************************************************************************
 * Execution loop
 ******************************************************************************/
int main()
{
    // Creating a node initializes PolySync
    polysync::Node node;

    // Grab PolySync node reference for message creation and publishing.
    auto nodeRef = node.reference();

    // Initialize classifcation kind
    auto kind = OBJECT_CLASSIFICATION_CAR;

    while( 1 )
    {
        // Publish polysync::ObjectsMessage
        publishObjectsMessage( nodeRef, kind );

        // Publish polysync::RadarTargetsMessage
        publishRadarTargetsMessage( nodeRef );

        // Publish polysync::LidarPointsMessage
        publishLidarPointsMessage( nodeRef );

        // Slow the loop down by sleeping for one second
        sleep( 1 );
    }

    return 0;
}
/*******************************************************************************
 * END Execution Loop
 ******************************************************************************/




/*******************************************************************************
 * Publish Methods
 ******************************************************************************/
void publishObjectsMessage( polysync::NodeReference nodeRef,
                            ps_object_classification_kind &kind )
{
    // Create a message
    polysync::ObjectsMessage message( nodeRef );

    // Set length of the output buffer
    message.setObjectsOutputBufferLength( 1 );

    // Populate an object for the output buffer
    ps_object object;
    setObjectValues( object, kind );

    // Place object to the object buffer in the message
    message.objectsBufferPushBack( object );

    // Publish message to the bus
    polysync::message::publish( nodeRef, message );
}

void publishRadarTargetsMessage( polysync::NodeReference nodeRef )
{
    // Create a message
    polysync::RadarTargetsMessage message( nodeRef );

    auto bufferLength = 10;

    // Set length of the output buffer
    message.setTargetsOutputBufferLength( bufferLength );

    for( auto i = 0; i < bufferLength; ++i )
    {
        // Populate an object for the output buffer
        ps_radar_target target;
        setRadarTargetValues( target, i );

        // Place object to the object buffer in the message
        message.targetsBufferPushBack( target );
    }
    // Publish message to the bus
    polysync::message::publish( nodeRef, message );
}

void publishLidarPointsMessage( polysync::NodeReference nodeRef )
{
    // Create a message
    polysync::LidarPointsMessage message( nodeRef );

    // Set buffer length
    auto bufferLength = 100;

    // Set length of the output buffer
    message.setPointsOutputBufferLength( bufferLength );

    for( auto i = 0; i < bufferLength; ++i )
    {
        // Populate an object for the output buffer
        ps_lidar_point point;
        setLidarPointValues( point, i );

        // Place object to the object buffer in the message
        message.pointsBufferPushBack( point );
    }

    // Publish message to the bus
    polysync::message::publish( nodeRef, message );
}
/*******************************************************************************
 * END Publish Methods
 ******************************************************************************/




/*******************************************************************************
 * Convenience Methods
 ******************************************************************************/
void setObjectValues( ps_object &object, ps_object_classification_kind &kind )
{
    // Alternate object classification kind
    if( kind == OBJECT_CLASSIFICATION_CAR )
    {
        object.classification = kind;

        // World position relative to origin
        object.position[ 0 ] = 5;
        object.position[ 1 ] = 5;
        object.position[ 2 ] = 1;

        // Set x,y,z size of object
        object.size[ 0 ] = 3;
        object.size[ 1 ] = 2;
        object.size[ 2 ] = 1.5;

        object.id = 0;

        object.course_angle = 90.0;
        kind = OBJECT_CLASSIFICATION_PEDESTRIAN;
    }
    else if( kind == OBJECT_CLASSIFICATION_PEDESTRIAN )
    {
        object.classification = kind;

        // World position relative to origin
        object.position[ 0 ] = 5;
        object.position[ 1 ] = -5;
        object.position[ 2 ] = 1;

        // Set x,y,z size of object
        object.size[ 0 ] = 0.35;
        object.size[ 1 ] = 0.5;
        object.size[ 2 ] = 2;

        object.id = 0;

        object.course_angle = -90.0;
        kind = OBJECT_CLASSIFICATION_BIKE;
    }
    else if( kind == OBJECT_CLASSIFICATION_BIKE )
    {
        object.classification = kind;

        // World position relative to origin
        object.position[ 0 ] = 5;
        object.position[ 1 ] = 0;
        object.position[ 2 ] = 1;

        // Set x,y,z size of object
        object.size[ 0 ] = 2;
        object.size[ 1 ] = 0.5;
        object.size[ 2 ] = 1.5;

        object.id = 0;

        object.course_angle = 0;
        kind = OBJECT_CLASSIFICATION_CAR;
    }
}

void setRadarTargetValues( ps_radar_target &target, int index )
{
    target.id = index;

    // World position relative to origin
    target.position[ 0 ] = index;
    target.position[ 1 ] = -index;
    target.position[ 2 ] = index;

    target.amplitude = 4.0;

    // Will not draw if == TRACK_STATUS_INVALID
    target.track_status = TRACK_STATUS_ACTIVE;
}

void setLidarPointValues( ps_lidar_point &point, int index )
{
    // World position relative to origin
    point.position[ 0 ] = index;
    point.position[ 1 ] = index;
    point.position[ 2 ] = index;
}
/*******************************************************************************
 * END Convenience Methods
 ******************************************************************************/



