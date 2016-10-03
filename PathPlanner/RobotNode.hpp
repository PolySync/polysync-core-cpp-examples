#ifndef ROBOTNODE_HPP
#define ROBOTNODE_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <PolySyncNode.hpp>
#include <atomic>

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

    std::unique_ptr<GridMap> world;
    std::atomic_int golLocX;
    std::atomic_int golLocY;
    std::atomic_bool newLocation;
    std::atomic_int newRobLocX;
    std::atomic_int newRobLocY;
};

#endif //ROBOTNODE_HPP
