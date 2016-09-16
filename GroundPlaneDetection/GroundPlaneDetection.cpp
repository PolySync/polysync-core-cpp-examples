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
 * \example GroundPlaneDetection.cpp
 * 
 * Detect the ground plane from a LiDAR point cloud.
 *
 * This application subscribes to the high level `ps_lidar_points_msg`. In the callback
 * we filter all non-ground points and republish the data to the PS bus.
 *
 */

#include <iostream>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

using namespace std;


/**
 * @brief
 */
class GroundPlaneDetection : public polysync::Node
{
    
public:

    /**
     * Subscribe to a message that the publisher node will send.
     *
     * @param void
     *
     * @return void
     */
    void initStateEvent() override
    {
        _messageType = getMessageTypeByName( "ps_lidar_points_msg" );

        // Register as a listener for the message type that the publisher
        // is going to send.  Message types are defined in later tutorials.
        registerListener( _messageType );
    }
    
    /**
     * Publish our current ground plane on each call to okStateEvent().
     *
     * @param void
     *
     * @return void
     */
    void okStateEvent() override
    {
        publishGroundPlane();

        polysync::sleepMicro( _50_MS );
    }

    /**
     * Extract the information from the provided message
     * 
     * @param std::shared_ptr< Message > - variable containing the message
     *
     * @return void
     */
    virtual void messageEvent( std::shared_ptr< polysync::Message > message )
    {
        using namespace polysync::datamodel;
        if( std::shared_ptr < LidarPointsMessage > lidarPointsMessage =
                getSubclass< LidarPointsMessage >( message ) )
        {
            // don't want to process our own messages
            if( lidarPointsMessage->getHeaderSrcGuid() != getGuid() )
            {
                _groundPlanePoints.clear();

                for( auto point : lidarPointsMessage->getPoints() )
                {
                    auto position = point.getPosition();

                    if( pointIsNearGround( position ) )
                    {
                        _groundPlanePoints.push_back( point );
                    }
                }
            }
        }
    }

    /**
     * Populate a LidarPointsMessage with our ground plane points, and publish
     * the data to the bus.
     *
     * @param void
     *
     * @return void
     */
    void publishGroundPlane()
    {
        polysync::datamodel::LidarPointsMessage groundPlaneMessage ( *this );

        groundPlaneMessage.setHeaderTimestamp(
                    polysync::getTimestamp() );

        groundPlaneMessage.setPoints( _groundPlanePoints );

        groundPlaneMessage.publish();
    }

    /**
     * Validate a 3d point, in this case, we're interested in points near the
     * ground plane, close to the origin.
     *
     * @param [in] point 3d point
     *
     * @return true if point is near the ground plane and near the origin.
     */
    bool pointIsNearGround( const std::array< float, 3 > & point )
    {
        return point[ 0 ] >= 2.5 and
               point[ 0 ] < 25 and
               point[ 1 ] > -12 and
               point[ 1 ] < 12 and
               point[ 2 ] < 0.25 and
               point[ 2 ] > -0.25;
    }

private:

    static constexpr ps_timestamp _50_MS = 10000;

    ps_msg_type _messageType;

    std::vector< polysync::datamodel::LidarPoint > _groundPlanePoints;

};

/**
 * Entry point for this tutorial application
 * The "connectPolySync" begins the node's PolySync execution loop.
 *
 * @return int - exit code
 */
int main()
{
    // Create an instance of the HelloWorldNode and connect it to PolySync
    GroundPlaneDetection groundPlaneNode;

    // When the node has been created, it will cause an initStateEvent to
    // to be sent.
    groundPlaneNode.connectPolySync();

    return 0;
}
