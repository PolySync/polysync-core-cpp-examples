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

using namespace std;


/**
 * Create a node that can publish/subscribe to the PolySync bus,
 * by subclassing the PolySync C++ Node.
 */
class GroundPlaneDetection : public polysync::Node
{
private:
    ps_msg_type _messageType;

    std::vector< polysync::datamodel::LidarPoint > _groundPlanePoints;
    
public:
    /**
     * initStateEvent
     *
     * Subscribe to the LiDAR points message that we expect to be on the bus
     * when this node is running.
     */
    void initStateEvent() override
    {
        // Get the integer representation for this message type, which dynamically changes for
        // each instance of the PolySync runtime.
        _messageType = getMessageTypeByName( "ps_lidar_points_msg" );

        registerListener( _messageType );
    }
    
    /**
     * messageEvent
     * 
     * Extract the information from the incoming message by promoting it from the base class
     * to a LidarPointsMessage.
     * 
     * param [in] std::shared_ptr< Message > - variable containing the incoming message
     */
    virtual void messageEvent( std::shared_ptr< polysync::Message > message )
    {
        using namespace polysync::datamodel;
        if( std::shared_ptr <LidarPointsMessage > lidarPointsMessage = getSubclass< LidarPointsMessage >( message ) )
        {
            // Filter out this nodes own messages
            if( lidarPointsMessage->getHeaderSrcGuid() != getGUID() )
            {
                _groundPlanePoints.clear();

                // Create a message, and set the message timestamp which represents when the data
                // in this message was created, or when it originated
                LidarPointsMessage groundPlaneMessage ( *this );

                groundPlaneMessage.setHeaderTimestamp( polysync::getTimestamp() );

                // Get the entire LiDAR point cloud from the incoming message
                std::vector< polysync::datamodel::LidarPoint > lidarPoints = lidarPointsMessage->getPoints();

                // Create a container that will hold all ground plane points that are found in the nodes processing
                std::vector< polysync::datamodel::LidarPoint > groundPlanePoints;

                // Create a container to hold a single point as the node iterates over the full point cloud
                std::array< float, 3 > position;

                for( polysync::datamodel::LidarPoint point : lidarPoints )
                {
                    // Get the x/y/z position for this point in the point cloud
                    position = point.getPosition();

                    if( pointIsNearTheGround( position ) )
                    {
                        // This point is close the ground, place it in our point vector
                        _groundPlanePoints.push_back( point );
                    }
                }

                // Set the message buffer
                groundPlaneMessage.setPoints( _groundPlanePoints );

                // Publish this message instance with the ground plane points that were found
                groundPlaneMessage.publish();

                groundPlanePoints.clear();
                lidarPoints.clear();
            }
        }
    }

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
