#ifndef ROBOTNODE_HPP
#define ROBOTNODE_HPP

#include <atomic>

#include <PolySyncNode.hpp>

#include "GridMap.hpp"


using namespace cv;
using namespace std;


class RobotNode : public polysync::Node {

public:

    RobotNode( );

    ~RobotNode( );


private:

    virtual void initStateEvent( ) override;

    virtual void okStateEvent( ) override;

    virtual void messageEvent( std::shared_ptr< polysync::Message > ) override;

    virtual void sendLocationToPlanner( );


private:

    std::unique_ptr< GridMap > _world;

    std::atomic_int _golLocX;

    std::atomic_int _golLocY;

    std::atomic_int _newRobLocX;

    std::atomic_int _newRobLocY;

    std::atomic_int _numWaypoints;

    std::atomic_int _waypointCounter;

};

#endif //ROBOTNODE_HPP
