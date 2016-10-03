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
    newLocation(false),
    newRobLocX( ),
    newRobLocY( )
{
    setNodeName( "robotNode" );

}

RobotNode::~RobotNode( ){

}

void RobotNode::initStateEvent( ) {
    registerListener( getMessageTypeByName( "ps_platform_motion_msg" ) );
    //registerListener( getMessageTypeByName( "ps_diagnostic_trace_msg" ) );
}

void RobotNode::okStateEvent( ) {
    //cout << "This is okStateEvent( )" << endl;
    if ( golLocX != INVALID_LOC && golLocY != INVALID_LOC && !newLocation ) {
        cout << "Goal location set; generating map" << endl;
        world = std::unique_ptr<GridMap>{}; //pass in goal location
        world->generateMap( golLocX, golLocY );
        double actualRobLocX = double( world->robLoc[0][0] );
        double actualRobLocY = double( world->robLoc[0][1] );

        PlatformMotionMessage msg( *this );
        msg.setHeaderTimestamp( polysync::getTimestamp() );
        msg.setOrientation( {actualRobLocX, actualRobLocY, 0, 0} );
        msg.publish();
        polysync::sleepMicro(1000);
        cout << "Map generated. Sending robot start location to planner algorithm.";
        cout << endl;
    }
    else if ( newLocation ) {
        cout << "should not be in newLocation" << endl;
        int newIndex = world->getIndexFromState(newRobLocX, newRobLocY);
        world->moveRobot(newRobLocX, newRobLocY);
        double actualRobLocX = double( world->robLoc[0][0] );
        double actualRobLocY = double( world->robLoc[0][1] );

        PlatformMotionMessage msg( *this );
        msg.setHeaderTimestamp( polysync::getTimestamp() );
        msg.setOrientation( {actualRobLocX, actualRobLocY, 0, 0} );
        msg.publish();
        polysync::sleepMicro(1000);
        if ( world->checkGoal( newIndex ) ) {
            disconnectPolySync( );
            return;
        }
        newLocation = false;
    }
}

void RobotNode::messageEvent( std::shared_ptr<polysync::Message> newMsg ) {
    cout << "HELLO 1" << endl;
    if ( newMsg->getSourceGuid( ) == getGuid( ) ) {
        cout << "My Message" << endl;
        return;
    }
    cout << "HELLO 2" << endl;
        if ( auto msg = getSubclass<PlatformMotionMessage>( newMsg ) ) {
            cout << "HELLO 3" << endl;
            if ( golLocX == INVALID_LOC && golLocY == INVALID_LOC ) {
                cout << "HELLO 4" << endl;
                golLocX = msg->getOrientation()[0];
                golLocY = msg->getOrientation()[1];
                newLocation = false;
                cout << endl << "Goal location received by robot." << endl;
            }
            else if ( msg->getOrientation()[0] != newRobLocX &&
                      msg->getOrientation()[1] != newRobLocY ) {
                cout << "HELLO 5" << endl;
                newRobLocX = msg->getOrientation()[0];
                newRobLocY = msg->getOrientation()[1];
                //do stuff with PlatformMotionMessage
                //PlatformMotionMessage.print();
                newLocation = true;
            }
            cout << "HELLO 6" << endl;
      }
      cout << "HELLO 7" << endl;
}
