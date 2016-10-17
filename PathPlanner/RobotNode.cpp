//PathPlanner
#include <cmath>
#include <iostream>

#include <PolySyncDataModel.hpp>

#include "RobotNode.hpp"

using namespace cv;
using namespace std;
using namespace polysync::datamodel;

constexpr int INVALID_LOC = -1;

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

void RobotNode::initStateEvent( ) {
    registerListener( getMessageTypeByName( "ps_platform_motion_msg" ) );
    setSubscriberReliabilityQOS(
                                getMessageTypeByName( "ps_platform_motion_msg" ),
                                RELIABILITY_QOS_RELIABLE );

    //registerListener( getMessageTypeByName( "ps_diagnostic_trace_msg" ) );
}

void RobotNode::okStateEvent( ) {
    if ( golLocX == INVALID_LOC || golLocY == INVALID_LOC ) {
        polysync::sleepMicro(1000);
        return;
    } else if ( newRobLocX == INVALID_LOC || newRobLocY == INVALID_LOC ) {
        cout << "Goal location received by robot." << endl;
        cout << "Generating map - - - - - - > " << std::flush;
        world = std::unique_ptr<GridMap>{ new GridMap }; //pass in goal location
        world->generateMap( golLocX, golLocY );
        newRobLocX = world->robLoc[0][0];
        newRobLocY = world->robLoc[0][1];
        cout << "Map generated. " << endl;
        cout << "Sending robot start location to planner algorithm." << endl;
    } else if ( golLocX != INVALID_LOC && golLocY != INVALID_LOC ) {
        int newIndex = sendLocationToPlanner( );
        if ( world->checkGoal( newIndex ) ) {
            disconnectPolySync( );
            return;
        }
        polysync::sleepMicro(1000);
    } else {
        polysync::sleepMicro(1000);
        return;
    }
}

void RobotNode::messageEvent( std::shared_ptr<polysync::Message> newMsg ) {
    if ( newMsg->getSourceGuid( ) == getGuid( ) ) {
        //cout << "This is Robot's message." << endl;
        return;
    }
    if ( auto msg = getSubclass<PlatformMotionMessage>( newMsg ) ) {
        if ( golLocX == INVALID_LOC && golLocY == INVALID_LOC ) {
            golLocX = msg->getOrientation()[0];
            golLocY = msg->getOrientation()[1];
        } else if ( msg->getOrientation()[0] == golLocX ||
                  msg->getOrientation()[1] == golLocY) {
            //cout << "Still receiving goal state." << endl;
            return;
        } else if ( msg->getPosition()[0] > waypointCounter ) {
            waypointCounter = msg->getPosition()[0];
            newRobLocX = msg->getOrientation()[0];
            newRobLocY = msg->getOrientation()[1];
            //msg->print();
        }
    }
    //cout << "HELLO" << endl;
}

int RobotNode::sendLocationToPlanner( ) {
    int newIndex = world->getIndexFromState(newRobLocX, newRobLocY);
    world->moveRobot(newRobLocX, newRobLocY);
    double actualRobLocX = double( world->robLoc[0][0] );
    double actualRobLocY = double( world->robLoc[0][1] );

    PlatformMotionMessage msg( *this );
    msg.setHeaderTimestamp( polysync::getTimestamp() );
    msg.setPosition( {waypointCounter, 0, 0} );
    msg.setOrientation( {actualRobLocX, actualRobLocY, 0, 0} );
    msg.publish();
    //msg.print();

    return newIndex;
}

int main( ) {

    RobotNode robotNode;
    robotNode.connectPolySync( );

    return 0;
}
