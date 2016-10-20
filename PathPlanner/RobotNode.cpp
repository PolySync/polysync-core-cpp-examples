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
 * planner algorithm.  Robot moves to the new position and reports back its
 * new location.
 *
 * @file RobotNode.cpp
 * @brief RobotNode Source
 *
 */
RobotNode::RobotNode( )
    :
    world( ),
    golLocX( INVALID_LOC ),
    golLocY( INVALID_LOC ),
    newRobLocX( INVALID_LOC ),
    newRobLocY( INVALID_LOC ),
    waypointCounter( 0 )
{
    setNodeName( "robotNode" );

}

RobotNode::~RobotNode( ){

}

/**
 * @brief initStateEvent
 *
 * The initStateEvent is triggered once when this node is initialized in
 * PolySync. This is a good place to initialize variables dependant on a
 * polysync::Node reference.
 *
 * @param void
 * @return void
 */
void RobotNode::initStateEvent( ) {
    // register node as a subscriber to platform motion messages from ANY node.
    registerListener( getMessageTypeByName( "ps_platform_motion_msg" ) );
    setSubscriberReliabilityQOS(
            getMessageTypeByName( "ps_platform_motion_msg" ),
            RELIABILITY_QOS_RELIABLE );
    //registerListener( getMessageTypeByName( "ps_diagnostic_trace_msg" ) );
}

/**
 * @brief okStateEvent
 *
 * Override the base class functionality to send messages when the node
 * reaches the "ok" state. This is the state where the node is in its
 * normal operating mode.
 *
 * @param void
 * @return void
 */
void RobotNode::okStateEvent( ) {
    // wait for a goal location message from searchNode to be received
    if ( golLocX == INVALID_LOC || golLocY == INVALID_LOC ) {
        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro(10000);
        return;
    // when goal received, generate a map with that goal state.
    } else if ( newRobLocX == INVALID_LOC || newRobLocY == INVALID_LOC ) {
        cout << "Goal location received by robot." << endl;
        cout << "Generating map - - - - - - > " << std::flush;
        world = std::unique_ptr<GridMap>{ new GridMap };
        // pass in goal location
        world->generateMap( golLocX, golLocY );
        newRobLocX = world->robLoc[0][0];
        newRobLocY = world->robLoc[0][1];
        cout << "Map generated. " << endl;
        cout << "Sending robot start location to planner algorithm." << endl;
    // with map generated, begin sending location messages to searchNode
    } else if ( golLocX != INVALID_LOC && golLocY != INVALID_LOC ) {
        int newIndex = sendLocationToPlanner( );
        if ( world->checkGoal( newIndex ) ) {
            disconnectPolySync( );
            return;
        }
        // The ok state is called periodically by the system so sleep to reduce
        // the number of messages sent. do nothing, sleep for 1 millisecond.
        polysync::sleepMicro(1000);
    } else {
        // do nothing, sleep for 100 milliseconds
        //cout << "I should never be here." << endl;
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
 * @return void
 */
void RobotNode::messageEvent( std::shared_ptr<polysync::Message> newMsg ) {
    // check whether new message is not your own. This check is only important
    // since robotNode and searchNode both publish and subscribe to messages.
    if ( newMsg->getSourceGuid( ) == getGuid( ) ) {
        //cout << "This is Robot's message." << endl;
        return;
    }
    // now check whether new message is a PlatformMotionMessage.
    if ( auto msg = getSubclass<PlatformMotionMessage>( newMsg ) ) {
        // the first received message from searchNode will be the goal location.
        if ( golLocX == INVALID_LOC && golLocY == INVALID_LOC ) {
            golLocX = msg->getOrientation()[0];
            golLocY = msg->getOrientation()[1];
        // discard any received messages still containing the goal location
        } else if ( msg->getOrientation()[0] == golLocX &&
                  msg->getOrientation()[1] == golLocY) {
            //cout << "Still receiving goal state." << endl;
            return;
        // if a new waypoint, send it to the robot and move to the new location.
        } else if ( msg->getPosition()[0] > waypointCounter ) {
            waypointCounter = msg->getPosition()[0];
            newRobLocX = msg->getOrientation()[0];
            newRobLocY = msg->getOrientation()[1];
            //msg->print();
        }
    }
    //cout << "HELLO" << endl;
}

/**
 * @brief sendLocationToPlanner
 *
 * Move robot to new desired location and report back current position.
 *
 * @param void
 * @return int - GridMap index of robot's new position.
 */
int RobotNode::sendLocationToPlanner( ) {
    int newIndex = world->getIndexFromState(newRobLocX, newRobLocY);
    world->moveRobot(newRobLocX, newRobLocY);
    double actualRobLocX = double( world->robLoc[0][0] );
    double actualRobLocY = double( world->robLoc[0][1] );

    // Create a message
    PlatformMotionMessage msg( *this );
    // Set publish time
    msg.setHeaderTimestamp( polysync::getTimestamp() );
    // Populate buffer
    msg.setPosition( {waypointCounter, 0, 0} );
    msg.setOrientation( {actualRobLocX, actualRobLocY, 0, 0} );
    // Publish to the PolySync bus
    msg.publish();
    //msg.print();

    return newIndex;
}

/**
 * Entry point for the RobotNode (platform) side of this tutorial application.
 * The node will receive waypoints by the planner algorithm and move to the
 * desired position. The "connectPolySync" is a blocking call, users must
 * use Ctrl-C to exit this function.
 */

int main( ) {

    RobotNode robotNode;
    robotNode.connectPolySync( );

    return 0;
}
