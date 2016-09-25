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
 * LaneKeepAssit.cpp
 *
 *
 */

#include <iostream>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

using namespace std;


/**
 * PublisherSubscriberNode class
 * 
 */
class LaneKeepAssist : public polysync::Node
{

private:
    const string node_name = "polysync-lane-keep-assist-cpp";

    // Messages to subscribe to
    const string lane_model_msg_name = "ps_lane_model_msg";
    const string platform_motion_msg_name = "ps_platform_motion_msg";

    // Messages to publish
    const string waypoints_msg_name = "ps_waypoints_msg";
    const string events_msg_name = "ps_event_msg";

    ps_msg_type _laneModelMessageType;
    ps_msg_type _platformMotionMessageType;
    ps_msg_type _waypointsMessageType;
    ps_msg_type _eventsMessageType;


    double _platformHeading;
    std::array< double, 3 > _platformVelocity;

    polysync::datamodel::LaneModel _leftLane;
    polysync::datamodel::LaneModel _rightLane;


    
public:
    
    LaneKeepAssist()
    {
        setNodeType( PSYNC_NODE_TYPE_API_USER );
        setDomainID( PSYNC_DEFAULT_DOMAIN );
        setSDFID( PSYNC_SDF_ID_INVALID );
        setFlags( PSYNC_INIT_FLAG_STDOUT_LOGGING );
        setNodeName( node_name );
    }
    
    ~LaneKeepAssist()
    {
        
    }

    void initStateEvent() override
    {
        _laneModelMessageType = getMessageTypeByName( lane_model_msg_name );

        _platformMotionMessageType = getMessageTypeByName( platform_motion_msg_name );
        
        // Register as a listener for diagnostic messages that any nodes may 
        // send.
        registerListener( _laneModelMessageType );

        registerListener( _platformMotionMessageType );
    }
    
    void releaseStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro( 10000 );
    }
    
    void errorStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro( 10000 );
    }
    
    void fatalStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro( 10000 );
    }
    
    void warnStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro( 10000 );
    }
    
    /**
     * okStateEvent
     *
     */
    void okStateEvent() override
    {
        // Create a message
//        polysync::datamodel::EventMessage message( *this );
        
        // Set message data
//        message.setHeaderTimestamp( polysync::getTimestamp() );
//        message.setId(0);

        // Publish to the PolySync bus
//        message.publish();

        // The ok state is called periodically by the system so sleep to reduce
        // the number of messages sent.
        polysync::sleepMicro( 1000000 );
    }
    
    /**
     * messageEvent
     * 
     * Extract the information from the provided message
     * 
     * param [in] std::shared_ptr< Message > - variable containing the message
     */
    void messageEvent( std::shared_ptr< polysync::Message > message ) override
    {
        using namespace polysync::datamodel;

        if( std::shared_ptr< polysync::datamodel::LaneModelMessage > incomingMessage =
                getSubclass< LaneModelMessage >( message ) )
        {  
            // Get an instance of the incoming lanes
            std::vector< polysync::datamodel::LaneModel > incomingLanes = incomingMessage->getLanes();

             // Note: lanes may be invalid, check with getQuality()
             _leftLane = incomingLanes.at(1);
             _rightLane = incomingLanes.at(2);

        }

        else if( std::shared_ptr< polysync::datamodel::PlatformMotionMessage > incomingMessage =
                getSubclass< PlatformMotionMessage >( message ) )
        {
            // Copy the data to our local instance of platform heading and velocity
            _platformHeading = incomingMessage->getHeading();
            _platformVelocity = incomingMessage->getVelocity();
        }

    }
    
};

class LKANodeData
{
    // internal node struct i.e. internal_data_s in C

    // could hold all private data types

    int whyDidIMakeThis;
};


/**
 * main
 *
 * Entry point for this tutorial application
 * The "connectToPolySync" is a blocking call, must use Ctrl-C to exit
 * this function.
 *
 * int - exit code
 */
int main()
{
    // Create an instance of the PublisherSubscriberNode and connect it to 
    // PolySync.
    LaneKeepAssist LKANode;

    // When the node has been created, it will cause an initStateEvent to be
    // sent and then proceed into the okState.  connectToPolySync does not
    // return, use Ctrl-C to exit.
    LKANode.connectPolySync();

    return 0;
}
