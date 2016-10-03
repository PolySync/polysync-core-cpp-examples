//A* Searcher
#ifndef PLANNER_HPP
#define PLANNER_HPP

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>
#include <armadillo>

#include "GridMap.hpp"

using namespace arma;
using namespace std;


class Planner {

public:

    Planner( );

    ~Planner( );

    void searchAStar( int startLoc );

    void plotOptimalPath( );

    int getGoalX( );

    int getGoalY( );

    int getNextWaypoint( int index );

    GridMap world;
    int curLoc;
    std::vector< std::vector<int> > path;


private:

    void initializeSearchSpace( );

    int getSearchNode( );

    void getNeighbors( int index );

    arma::Mat<int> searchMap;
    arma::Mat<float> heuristic;
    arma::Mat<float> globalScore;
    arma::Mat<float> pathScore;
    float epsilon{2};
    bool endGame{false};
    int newLoc;
    arma::uword curLocU;
    arma::uword newLocU;
    std::vector< std::vector<int> > moves;
    std::array<int, 8> tempMoves;
    std::vector<int> closedSet;
    std::vector<int> openSet;
    int exploredNodes{1};

};

#endif // PLANNER_HPP
