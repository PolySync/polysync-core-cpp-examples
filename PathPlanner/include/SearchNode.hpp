#ifndef SEARCHNODE_HPP
#define SEARCHNODE_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <PolySyncNode.hpp>
#include <atomic>

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

    std::unique_ptr<Planner> searcher;
    std::atomic_bool initLocation;
    std::atomic_bool newLocation;
    std::atomic_int golLocX;
    std::atomic_int golLocY;
    std::atomic_int robLocX;
    std::atomic_int robLocY;
    std::atomic_int waypointCounter;
};

#endif //SEARCHNODE_HPP
