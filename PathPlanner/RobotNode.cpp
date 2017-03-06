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

#include <cmath>
#include <iostream>

#include <PolySyncDataModel.hpp>

#include "RobotNode.hpp"

using namespace cv;
using namespace std;
using namespace polysync::datamodel;

constexpr int INVALID_LOC = -1;

/**
 * \example RobotNode.cpp
 *
 * PolySync Path Planner Example.
 *
 * Shows how to subscribe a simulated robot node to a planning algorithm node.
 * Robot node listens for platform motion messages with desired waypoints from
 * planner algorithm.  Specifically, each platform motion message has a position
 * and an orientation.  The incoming position fields give the current waypoint
 * number and total number of waypoints.  The incoming orientation fields give
 * the x and y axis coordinate that the robot should move to.  Robot moves to
 * the new position and reports back its new location.
 *
 * @file RobotNode.cpp
 * @brief RobotNode Source
 *
 */
RobotNode::RobotNode( )
    :
    _world( ),
    _golLocX( INVALID_LOC ),
    _golLocY( INVALID_LOC ),
    _newRobLocX( INVALID_LOC ),
    _newRobLocY( INVALID_LOC ),
    _numWaypoints( INVALID_LOC ),
    _waypointCounter( 0 )
{
    setNodeName( "robotNode" );
}

RobotNode::~RobotNode( )
{
}

/**
 * @brief initStateEvent
 *
 * The initStateEvent is triggered once when this node is initialized in
 * PolySync. This is a good place to initialize variables dependant on a
 * polysync::Node reference.
 *
 */
void RobotNode::initStateEvent( )
{

    // register node as a subscriber to platform motion messages from ANY node.
    registerListener( getMessageTypeByName( "ps_platform_motion_msg" ) );

    setSubscriberReliabilityQOS(
            getMessageTypeByName( "ps_platform_motion_msg" ),
            RELIABILITY_QOS_RELIABLE );
}

/**
 * @brief okStateEvent
 *
 * Override the base class functionality to send messages when the node
 * reaches the "ok" state. This is the state where the node is in its
 * normal operating mode.
 *
 */
void RobotNode::okStateEvent( )
{

    // wait for a goal location message from searchNode to be received
    if ( _golLocX == INVALID_LOC || _golLocY == INVALID_LOC )
    {

        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro(10000);
        return;

    }

    // when goal received, generate a map with that goal state.
    else if ( _newRobLocX == INVALID_LOC || _newRobLocY == INVALID_LOC )
    {

        cout << endl << "Goal location received by robot.";
        cout << endl << "Generating map - - - - - - > " << std::flush;

        _world = std::unique_ptr< GridMap >{ new GridMap };

        // pass in goal location
        _world->generateMap( _golLocX, _golLocY );

        _newRobLocX = _world->robLoc[0][0];
        _newRobLocY = _world->robLoc[0][1];

        cout << "Map generated. " << endl;
        cout << "Sending robot start location to planner algorithm." << endl;
        cout << endl << "RobotNode waiting for waypoints." << endl;

    }

    // with map generated, begin sending location messages to searchNode
    else if ( _golLocX != INVALID_LOC && _golLocY != INVALID_LOC )
    {

        sendLocationToPlanner( );

        // have I recieved the final waypoint?  if so, ask the user to shut down
        if ( _waypointCounter == _numWaypoints - 2)
        {
            cout << endl << "Robot is at goal state after ";
            cout << _waypointCounter << " waypoints. " <<  endl << endl;
            cout << "Press return key or Ctrl-C to shut down RobotNode.";
            cout << endl;

            cin.get();
            cout << endl << "Shutting down RobotNode." << endl << endl;

            disconnectPolySync( );

            return;
        }

        // The ok state is called periodically by the system so sleep to reduce
        // the number of messages sent. do nothing, sleep for 1 millisecond.
        polysync::sleepMicro(1000);

    }

    else
    {

        // do nothing, sleep for 100 milliseconds
        polysync::sleepMicro(100000);
        return;

    }
}

/**
 * @brief messageEvent
 *
 * Extract the information from the provided message
 *
 * @param std::shared_ptr< Message > - variable containing the message
 */
void RobotNode::messageEvent( std::shared_ptr< polysync::Message > newMsg )
{

    // check whether new message is not your own. This check is only important
    // since robotNode and searchNode both publish and subscribe to messages.
    if ( newMsg->getHeaderSrcGuid( ) == getGuid( ) )
    {
        return;
    }

    // now check whether new message is a PlatformMotionMessage.
    if ( auto msg = getSubclass< PlatformMotionMessage >( newMsg ) )
    {

        // the first received message from searchNode will be the goal location.
        if ( _golLocX == INVALID_LOC && _golLocY == INVALID_LOC )
        {
            _golLocX = msg->getOrientation()[0];
            _golLocY = msg->getOrientation()[1];
        }

        // discard any received messages still containing the goal location
        else if ( msg->getOrientation()[0] == _golLocX &&
                  msg->getOrientation()[1] == _golLocY )
        {
            return;
        }

        // if a new waypoint, send it to the robot and move to the new location.
        else if ( msg->getPosition()[0] > _waypointCounter )
        {
            _waypointCounter = msg->getPosition()[0];
            _numWaypoints = msg->getPosition()[2];

            _newRobLocX = msg->getOrientation()[0];
            _newRobLocY = msg->getOrientation()[1];

            cout << "Received waypoint " << _waypointCounter;
            cout << " from Planner.  X = " << _newRobLocX << ", Y = ";
            cout << _newRobLocY << endl << std::flush;
        }
    }
}

/**
 * @brief sendLocationToPlanner
 *
 * Move robot to new desired location and report back current position.
 *
 * @return int - GridMap index of robot's new position.
 */
void RobotNode::sendLocationToPlanner( )
{

    _world->moveRobot( _newRobLocX, _newRobLocY );

    double actualRobLocX = double( _world->robLoc[0][0] );
    double actualRobLocY = double( _world->robLoc[0][1] );

    // Create a message
    PlatformMotionMessage msg( *this );

    // Set publish time
    msg.setHeaderTimestamp( polysync::getTimestamp() );

    // Populate buffer
    msg.setPosition( { double( _waypointCounter ), 0, 0 } );
    msg.setOrientation( { actualRobLocX, actualRobLocY, 0, 0 } );

    // Publish to the PolySync bus
    msg.publish();

}

/**
 * Entry point for the RobotNode (platform) side of this tutorial application.
 * The node will receive waypoints by the planner algorithm and move to the
 * desired position. The "connectPolySync" is a blocking call, users must
 * use Ctrl-C to exit this function.
 */

int main( )
{

    RobotNode robotNode;
    robotNode.connectPolySync( );

    return 0;
}
