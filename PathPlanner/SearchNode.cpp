//PathPlanner
#include <cmath>
#include <iostream>
#include <PolySyncDataModel.hpp>

#include "SearchNode.hpp"
#include "GridMap.hpp"

using namespace cv;
using namespace std;
using namespace polysync::datamodel;

constexpr int INVALID_LOC = -1;

SearchNode::SearchNode( )
    :
    searcher( ),
    initLocation( false ),
    newLocation( false ),
    golLocX( INVALID_LOC ),
    golLocY( INVALID_LOC ),
    robLocX( INVALID_LOC ),
    robLocY( INVALID_LOC ),
    waypointCounter( 0 )
{
    setNodeName( "searchNode" );
    setNodeType( PSYNC_NODE_TYPE_SOFTWARE_ALGORITHM );

}

SearchNode::~SearchNode( ){

}

void SearchNode::initStateEvent( ) {

}

void SearchNode::okStateEvent( ) {
    if ( golLocX == INVALID_LOC && golLocY == INVALID_LOC ) {
        searcher = new Planner;
        golLocX = searcher->getGoalX( );
        golLocY = searcher->getGoalY( );

        PlatformMotionMsg msg;
        msg.setHeaderTimestamp( polysync::getTimestamp() );
        msg.setOrientation( {golLocX, golLocY, 0, 0} );
        msg.publish( );
        polysync::sleepMicro(1000);
    } else if ( initLocation && searcher ) {
        int robIndex = searcher->world.getIndexFromState(robLocX, robLocY);
        searcher->searchAStar( robIndex );
        initLocation = false;
    }
    if ( newLocation && searcher ) {
        int robIndex = searcher->world.getIndexFromState(robLocX, robLocY);
        if ( searcher->world.checkGoal( robIndex ) ) {
            disconnectPolySync( );
            return;
        }
        int newIndex = searcher->getNextWaypoint( waypointCounter++ );
        searcher->world.getStateFromIndex( newIndex );
        robLocX = searcher->world.checkMoveIndexX;
        robLocY = searcher->world.checkMoveIndexY;

        PlatformMotionMsg msg;
        msg.setHeaderTimestamp( polysync::getTimestamp() );
        msg.setOrientation( {robLocX, robLocY, 0, 0} );
        msg.publish();
        polysync::sleepMicro(1000);
        newLocation = false;
    }
}

void SearchNode::messageEvent( std::shared_ptr<polysync::Message> msg ) {
    if ( msg->getSourceGuid( ) == getGuid( ) ) {
        return;
    }
    if ( auto platformMotionMsg = getSubclass<PlatformMotionMsg>( msg ) ) {
        if ( msg->getOrientation()[0] != robLocX && msg->getOrientation()[1] != robLocY ) {
            robLocX = msg->getOrientation()[0];
            robLocY = msg->getOrientation()[1];
            newLocation = true;
            if ( waypointCounter == 0 ) {
              initLocation = true;
            }
          //do stuff with platformMotionMsg
          //platformMotionMsg.print();
        }
    }
}
