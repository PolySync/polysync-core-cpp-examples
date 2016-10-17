//A* Searcher
#ifndef PLANNER_HPP
#define PLANNER_HPP

#include "GridMap.hpp"


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
    std::vector< std::vector<int> > path;

private:

    void initializeSearchSpace( );

    int getSearchNode( );

    void getNeighbors( int index );

    std::vector< int > searchMap;
    std::vector< float > heuristic;
    std::vector< float > globalScore;
    std::vector< float > pathScore;
    float epsilon{2};
    bool endGame{false};
    int curLoc;
    int newLoc;
    std::vector< std::vector<int> > moves;
    std::array<int, 8> tempMoves;
    std::vector<int> closedSet;
    std::vector<int> openSet;
    int exploredNodes{1};

};

#endif // PLANNER_HPP
