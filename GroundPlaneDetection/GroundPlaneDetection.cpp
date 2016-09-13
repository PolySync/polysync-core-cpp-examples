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

int MAX_POINTS = 1000;

/**
 * @brief
 */
class GroundPlaneDetection : public polysync::Node
{
private:
    ps_msg_type _messageType;
    
public:
    /**
     * @brief initStateEvent
     *
     * Subscribe to a message that the publisher node will send.
     *
     * @param void
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
     * @brief messageEvent
     * 
     * Extract the information from the provided message
     * 
     * @param std::shared_ptr< Message > - variable containing the message
     * @return void
     */
    virtual void messageEvent( std::shared_ptr< polysync::Message > message )
    {
        using namespace polysync::datamodel;
        if( std::shared_ptr <LidarPointsMessage > lidarPointsMessage = getSubclass< LidarPointsMessage >( message ) )
        {
            // don't want to process our own messages
            if( lidarPointsMessage->getHeaderSrcGuid() != this->getGUID() )
            {
                LidarPointsMessage groundPlaneMessage ( *this );

                groundPlaneMessage.setHeaderTimestamp( polysync::getTimestamp() );


                std::vector< polysync::datamodel::LidarPoint > lidarPoints = lidarPointsMessage->getPoints();

                std::vector< polysync::datamodel::LidarPoint > groundPlanePoints;

                std::array< float, 3 > position;

    //            groundPlanePoints.reserve( MAX_POINTS );

                for( polysync::datamodel::LidarPoint point : lidarPoints )
                {
                    position = point.getPosition();

                    if( position[0] >= 2.5 and
                            position[0] < 4 and
                            position[2] < 0.5 )
                    {
                        groundPlanePoints.push_back( point );
                    }
                }

                groundPlaneMessage.setPoints( groundPlanePoints );

                groundPlaneMessage.publish();

                groundPlanePoints.clear();
                lidarPoints.clear();

            }
        }
    }

};

/**
 * @brief main
 *
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
