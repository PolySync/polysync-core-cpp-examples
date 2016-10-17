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

    virtual void messageEvent( std::shared_ptr<polysync::Message> ) override;

    virtual void sendGoalToRobot( );

    virtual void sendNextWaypoint( int waypointIndex, int waypointID );

    std::unique_ptr<Planner> searcher;
    std::atomic_int golLocX;
    std::atomic_int golLocY;
    std::atomic_int robLocX;
    std::atomic_int robLocY;
    std::atomic_int newRobLocX;
    std::atomic_int newRobLocY;
    std::atomic_int waypointCounter;

};

#endif //SEARCHNODE_HPP
