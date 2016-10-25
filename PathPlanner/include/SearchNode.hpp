#ifndef SEARCHNODE_HPP
#define SEARCHNODE_HPP

#include <atomic>

#include <PolySyncNode.hpp>

#include "Planner.hpp"


using namespace cv;
using namespace std;


class SearchNode : public polysync::Node {

public:

    SearchNode( );

    ~SearchNode( );


private:

    virtual void initStateEvent( ) override;

    virtual void okStateEvent( ) override;

    virtual void messageEvent( std::shared_ptr< polysync::Message > ) override;

    virtual void sendGoalToRobot( );

    virtual void sendNextWaypoint( int waypointIndex, int waypointID );


private:

    std::unique_ptr< Planner > _searcher;

    std::atomic_int _golLocX;

    std::atomic_int _golLocY;

    std::atomic_int _robLocX;

    std::atomic_int _robLocY;

    std::atomic_int _newRobLocX;

    std::atomic_int _newRobLocY;

    std::atomic_int _numWaypoints;

    std::atomic_int _waypointCounter;

};

#endif //SEARCHNODE_HPP
