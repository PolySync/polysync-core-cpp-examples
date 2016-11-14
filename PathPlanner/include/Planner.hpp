#ifndef PLANNER_HPP
#define PLANNER_HPP

#include "GridMap.hpp"


using namespace std;


class Planner
{

public:

    Planner( );

    ~Planner( );

    int searchAStar( int startLoc );

    void plotOptimalPath( );

    int getGoalX( );

    int getGoalY( );

    int getNextWaypoint( int index );

    int getPathSize( );


public:

    GridMap world;

    std::vector< std::vector< int > > path;


private:

    void initializeSearchSpace( );

    int getSearchNode( );

    void getNeighbors( int index );


private:

    std::vector< int > _searchMap;

    std::vector< float > _heuristic;

    std::vector< float > _globalScore;

    std::vector< float > _pathScore;

    float _epsilon{ 2 };

    bool _endGame{ false };

    int _curLoc;

    int _newLoc;

    std::vector< std::vector< int > > _moves;

    std::array< int, 8 > _tempMoves;

    std::vector< int > _closedSet;

    std::vector< int > _openSet;

    int _expandedNodes{ 1 };

};

#endif // PLANNER_HPP
