/*
 * Copyright (c) 2016 PolySync
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

/**
 * GroundPlaneDetection.cpp
 * 
 * Detect the ground plane from an incoming LiDAR point cloud.
 *
 * This application subscribes to the high level `ps_lidar_points_msg`. In the callback
 * the node filters all non-ground points and republishs the data to the PolySync bus
 * in another `ps_lidar_points_msg`.
 *
 */

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>


using namespace polysync::datamodel;


/**
 * Create a node that can publish/subscribe to the PolySync bus,
 * by subclassing the PolySync C++ Node.
 */
class GroundPlaneDetection : public polysync::Node
{

public:

    /**
     * Register this node to listen for the PolySync LidarPointsMessage
     */
    void initStateEvent() override
    {
        // Connect messageEvent() to PolySync for the given message topic string
        registerListener( getMessageTypeByName( "ps_lidar_points_msg" ) );
    }


    /**
     * First safe downcast polysync::Message to a LidarPointsMessage
     * Ignore all LidarPointsMessage instances that orignated from this node
     * Filter for ground points
     * Publish filtered points in a new LidarPointsMessage
     * 
     * param [in] std::shared_ptr< Message > - variable containing the incoming message
     */
    virtual void messageEvent( std::shared_ptr< polysync::Message > message )
    {
        if( std::shared_ptr <LidarPointsMessage > lidarPointsMessage =
                getSubclass< LidarPointsMessage >( message ) )
        {
            // Filter out messages from this node
            if( lidarPointsMessage->getHeaderSrcGuid() != getGuid() )
            {
                publishPoints(
                    buildGroundPoints( lidarPointsMessage->getPoints() ) );
            }

        }
    }

    /**
     * Take all 3D points from a LidarPointsMessage object and return only the
     * points that are near the ground plane.
     *
     * param [in] pointsToFilter - Full set of points from a LidarPointsMessage.
     *
     * return std::vector< LidarPoint > - Filtered ground plane points
     */
    const std::vector< LidarPoint > buildGroundPoints(
            const std::vector< LidarPoint > & pointsToFilter )
    {
        std::vector< LidarPoint > groundPoints;

        for( auto point : pointsToFilter )
        {
            if( pointIsNearTheGround( point.getPosition() ) )
            {
                auto pos = point.getPosition();

                // Comment the next line out to get the exact position
                // Having two LiDAR sources publishing points in the exact same postiion
                // causes a "flicker" in 3D rendering
                pos[ 2 ] += 0.05;

                point.setPosition( pos );

                groundPoints.push_back( point );
            }
        }

        return groundPoints;
    }

    /**
     * Used to filter out lidar points that are not near the ground plane.
     *
     * param [in] point in 3D space
     *
     * return true if the point is near the ground, relative to the platform vehicle
     */
    bool pointIsNearTheGround( const std::array< float, 3 > & point )
    {
        // The vehicle origin is at the center of the rear axle, on the ground
        // Incoming LiDAR point messages have been corrected for sensor mount position already

        return point[0] >= 2.5 and      // x is 2.5+ meters from the vehicle origin
               point[0] < 35 and        // x is less than 35 meters from the vehicle origin
               point[1] > -12 and       // y is greater than -12 meters from the vehicle origin (towards the passenger side)
               point[1] < 12 and        // y is less than 12 meters from the vehicle origin (towards the driver side)
               point[2] > -0.35 and     // z is greater than -0.35 meters from the vehicle origin (towards the ground),
                                        // this componsates for vehicle pitch as the vehicle drives
               point[2] < 0.25;         // z is less than 0.25 meters from the vehicle origin
    }


    /**
     * Take a set of 3D points and publish them as a PolySync LidarPointsMessage.
     *
     * param [in] points std::vector of 3D points
     */
    void publishPoints(
            const std::vector< polysync::datamodel::LidarPoint > & points )
    {
        using namespace polysync::datamodel;

        // Create a message, and set the message timestamp which represents when the data
        // in this message was created, or when it originated
        LidarPointsMessage groundPlaneMessage ( *this );

        // Set the message buffer
        groundPlaneMessage.setPoints( points );

        groundPlaneMessage.setHeaderTimestamp( polysync::getTimestamp() );

        // Publish this message instance with the ground plane points that were found
        groundPlaneMessage.publish();
    }

};

/**
 * Entry point for this node.
 *
 * The "connectPolySync" method starts the node's PolySync execution loop,
 * and will exit gracefully if CTRL-C is received.
 *
 * returns an integer exit code
 */
int main()
{
    // Create an instance of the node and connect it to the PolySync bus
    GroundPlaneDetection groundPlaneNode;

    // This places the node in the initStateEvent.
    groundPlaneNode.connectPolySync();

    return 0;
}
