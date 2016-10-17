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

    virtual void messageEvent( std::shared_ptr<polysync::Message> ) override;

    virtual int sendLocationToPlanner( );

    std::unique_ptr<GridMap> world;
    std::atomic_int golLocX;
    std::atomic_int golLocY;
    std::atomic_int newRobLocX;
    std::atomic_int newRobLocY;
    std::atomic_int waypointCounter;

};

#endif //ROBOTNODE_HPP
