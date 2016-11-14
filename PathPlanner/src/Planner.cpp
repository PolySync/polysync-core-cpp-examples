/*
 * Copyright (c) 2016 PolySync
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <cmath>
#include <iostream>

#include "Planner.hpp"

using namespace std;

/**
 * \example Planner.cpp
 *
 * PolySync Path Planner Example.
 *
 * Use A* Search to find the optimal path to the goal location.  This is a class
 * that SearchNode uses.  Once searchAStar() is called, SearchNode sends the
 * next waypoint in path vector until the robot is at the goal state.
 *
 * @file Planner.cpp
 * @brief Planner Source
 *
 */
Planner::Planner( )
    :
    world( ),
    path( )
{
    world.generateMap( );

    initializeSearchSpace( );
}

/**
 * @brief initializeSearchSpace
 *
 * Initialize vectors needed to perform A* search.  searchMap is a vector which
 * stores '0' or '255' at each index, signifying whether that cell is black or
 * white.  heuristic is a vector storing the Euclidean distance from the goal
 * state.  globalScore is a vector which stores the global score at each cell.
 * By default, it is set very high to encourage exploration.  pathScore is a
 * vector which scores the length of the shortest path to that cell.
 *
 */
void Planner::initializeSearchSpace( )
{

    int i;
    int j;

    float iDistance;
    float jDistance;

    for (int k = 0; k < (world.nCols * world.nRows); k++)
    {
        world.getStateFromIndex( k );

        i = world.checkedMoveIndX;
        j = world.checkedMoveIndY;

        iDistance = i - world.golLoc[ 0 ][ 0 ];
        jDistance = j - world.golLoc[ 0 ][ 1 ];

        _searchMap.push_back( world.map.at< uchar >(Point(3*i, j)) );

        _heuristic.push_back( pow ( sqrt( pow(iDistance,2) + pow(jDistance,2) ),
                                    _epsilon ) );

        _globalScore.push_back( 1e9 );

        _pathScore.push_back( 1e9 );

    }
    path.resize( world.nRows * world.nCols );

    _moves.resize( world.nRows * world.nCols );

    _curLoc = world.getIndexFromState(   world.robLoc[ 0 ][ 0 ],
                                        world.robLoc[ 0 ][ 1 ] );

    path[ _curLoc ].push_back( _curLoc );

    _openSet.push_back( _curLoc );

}

/**
 * @brief getSearchNode
 *
 * Get next node to expand by returning the node with the current lowest global
 * score from the openSet (nodes to be expanded).
 *
 * @return int - index of next node to search
 */
int Planner::getSearchNode( )
{

    int lowScore = 1e9;

    for (uint i = 0; i < _openSet.size(); i++)
    {
        if ( _globalScore[ _openSet[ i ] ] < lowScore )
        {
            lowScore = _globalScore[ _openSet[ i ] ];

            _curLoc = _openSet[ i ];
        }
    }

    return _curLoc;

}

/**
 * @brief getNeighbors
 *
 * Populate the moves vector with all legal moves at the current index.
 *
 * @param int - current search node index
 */
void Planner::getNeighbors( int _curLoc )
{

    _tempMoves = {   _curLoc - 1,
                    _curLoc + 1,
                    _curLoc - world.nRows,
                    _curLoc + world.nCols,
                    _curLoc - 1 - world.nRows,
                    _curLoc - 1 + world.nRows,
                    _curLoc + 1 - world.nRows,
                    _curLoc + 1 + world.nRows };

    _moves[ _curLoc ].clear();

    for (uint i = 0; i < _tempMoves.size(); i++)
    {
        if ( world.checkMove( _tempMoves[ i ], world.robSize ) )
        {
            _moves[ _curLoc ].push_back( _tempMoves[ i ] );

            if ( std::find( _openSet.begin(),
                            _openSet.end(),
                            _tempMoves[ i ] )
                            == _openSet.end() &&
                std::find(  _closedSet.begin(),
                            _closedSet.end(),
                            _tempMoves[ i ] )
                            == _closedSet.end() )
            {
                _openSet.push_back( _tempMoves[ i ] );
            }
        }
    }
}

/**
 * @brief searchAStar
 *
 * Search the grid map for the optimal path.
 *
 * @param int - start location to search from
 * @return int - number of waypoints in optimal path
 */
int Planner::searchAStar( int _curLoc )
{

    // start the clock to time performance.
    float beginTime = clock();

    // reset expandedNodes counter
    _expandedNodes = 1;

    cout << endl << "Searching with A* . " << std::flush;

    while ( _endGame != true )
    {

        // expand the node in the openSet with lowest current score.
        _curLoc = getSearchNode( );

        // get x and y coordinates
        world.getStateFromIndex( _curLoc );

        // find all legal moves at the current node
        getNeighbors( _curLoc );

        // grade all legal moves to fully expand a node
        for (uint j = 0; j < _moves[ _curLoc ].size(); j++)
        {

            // set move to be graded
            _newLoc = _moves[ _curLoc ][ j ];

            // A* guarantees optimality along all expanded nodes, so there is no
            // need to re-check.  if move has been expanded, move along
            if ( std::find( _closedSet.begin(), _closedSet.end(), _newLoc )
                            != _closedSet.end() )
            {
                continue;
            }

            // if move is at goal state, end game and send back path
            if ( world.checkGoal( _newLoc ))
            {
                // path @ newLoc will be the optimal path
                path[ _newLoc ] = path[ _curLoc ];
                path[ _newLoc ].push_back( _newLoc );
                _curLoc = _newLoc;
                _endGame = true;
                goto doneSearching;

            }
            else
            {

                // do nothing unless the new path to the index is less than the
                // current shortest path.  This ensures that the global score is
                // only modified if the searcher has found a shorter path.
                if ( path[ _curLoc ].size() + 1 < _pathScore[ _newLoc ] )
                {
                    path[ _newLoc ] = path[ _curLoc ];

                    path[ _newLoc ].push_back( _newLoc );

                    _globalScore[ _newLoc ] =
                                    _heuristic[ _newLoc ]
                                    + path[ _newLoc ].size();

                    _pathScore[ _newLoc ] = path[ _newLoc ].size();
                }
            }
        }

        // once fully expanded, set the globalScore at the expanded node to a
        // large number.  This is done to discourage searching the same node.
        _globalScore[ _curLoc ] = 1e9;

        // erase the current node from the openSet (nodes to be expanded).
        _openSet.erase(
                std::remove( _openSet.begin(), _openSet.end(), _curLoc ),
                _openSet.end() );

        // add the current node to the closedSet (nodes that have been expanded)
        _closedSet.push_back( _curLoc );

        // increase expanded node counter
        ++_expandedNodes;

        // print a "." for every 1000 nodes expanded as a progress meter
        if ( _expandedNodes % 1000 == 0 )
        {
            cout << ". " << std::flush;
        }
    }

    // what to do at endGame
    doneSearching:

    float endTime = float( clock() - beginTime ) / float( CLOCKS_PER_SEC );

    cout << endl << "Path Optimized, " << _expandedNodes << " nodes expanded.";
    cout << endl << "Total Optimization time: " << endTime << " seconds"<< endl;
    cout << endl << "Press the return key to execute optimal path." << endl;

    cin.get();

    cout << "Optimal path is " << path[ _curLoc ].size() << " steps long.";
    cout << endl;

    return path[ _curLoc ].size();

}

/**
 * @brief plotOptimalPath
 *
 * Defunct; Iterate through path and plot waypoints.
 *
 */
void Planner::plotOptimalPath( )
{
    cout << "Press the return key to execute optimal path." << endl;

    cin.get();

    cout << "Optimal path is " << path[ _curLoc ].size() << " steps long.";
    cout << endl;

    for( uint k = 0; k < path[ _curLoc ].size(); k++)
    {
        world.getStateFromIndex( path[ _curLoc ][ k ] );

        world.moveRobot( world.checkedMoveIndX, world.checkedMoveIndY );
    }

    cout << "GOAL!!" << endl;
}

/**
 * @brief getGoalX
 *
 * return x-coordinate for the goal location
 *
 * @return int - x-coordinate for goal location
 */
int Planner::getGoalX( )
{
    return world.golLoc[ 0 ][ 0 ];
}

/**
 * @brief getGoalY
 *
 * return y-coordinate for the goal location
 *
 * @return int - y-coordinate for goal location
 */
int Planner::getGoalY( )
{
    return world.golLoc[ 0 ][ 1 ];
}

/**
 * @brief getNextWaypoint
 *
 * return next waypoint from input location
 *
 * @param int - waypoint ID that robot is at
 * @return int - index at waypoint ID
 */
int Planner::getNextWaypoint( int index )
{
    return path[ _curLoc ][ index ];
}

/**
 * @brief getNextWaypoint
 *
 * return next waypoint from input location
 *
 * @return int - index at waypoint ID
 */
int Planner::getPathSize( )
{
    return path[ _curLoc ].size();
}

Planner::~Planner() {

}
