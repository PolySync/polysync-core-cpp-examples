//PathPlanner
#include <cmath>
#include <iostream>
#include <PolySyncDataModel.hpp>

#include "RobotNode.hpp"
#include "GridMap.hpp"

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

}

void RobotNode::okStateEvent( ) {
    if ( golLocX != INVALID_LOC && golLocY != INVALID_LOC && !newLocation ) {
        world = new GridMap; //pass in goal location
        int goalLocation = world->getIndexFromState( golLocX, golLocY );
        world.generateMap( goalLocation );
        PlatformMotionMsg msg;
        msg.setHeaderTimestamp( polysync::getTimestamp() );
        msg.setOrientation( {world->robLoc[0][0], world->robLoc[0][1], 0, 0} );
        msg.publish();
        polysync::sleepMicro(1000);
    }
    else if ( newLocation ) {
        int newIndex = world->getIndexFromState(robLocX, robLocY);
        world->moveRobot(newRobLocX, newRobLocY);
        PlatformMotionMsg msg;
        msg.setHeaderTimestamp( polysync::getTimestamp() );
        msg.setOrientation( {world->robLoc[0][0],world->robLoc[0][1],0,0} );
        msg.publish();
        polysync::sleepMicro(1000);
        if ( world->checkGoal( newIndex ) ) {
            disconnectPolySync( );
            return;
        }
        newLocation = false;
    }
}

void RobotNode::messageEvent( std::shared_ptr<polysync::Message> msg ) {
    if ( msg->getSourceGuid( ) == getGuid( ) ) {
        return;
    }
        if ( auto platformMotionMsg = getSubclass<PlatformMotionMsg>( msg ) ) {
            if ( golLocX == INVALID_LOC && golLocY == INVALID_LOC ) {
                golLocX = msg->getOrientation()[0];
                golLocY = msg->getOrientation()[1];
                newLocation = false;
            }
            else if ( msg->getOrientation()[0] != newRobLocX &&
                      msg->getOrientation()[1] != newRobLocY ) {
              newRobLocX = msg->getOrientation()[0];
              newRobLocY = msg->getOrientation()[1];
              //do stuff with platformMotionMsg
              //platformMotionMsg.print();
              newLocation = true;
            }
      }
}
