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
 * @param void
 * @return void
 */
void Planner::initializeSearchSpace( ) {
    int i;
    int j;
    float iDistance;
    float jDistance;
    for (int k = 0; k < (world.nCols * world.nRows); k++) {
        world.getStateFromIndex( k );
        i = world.checkedMoveIndX;
        j = world.checkedMoveIndY;
        iDistance = i - world.golLoc[0][0];
        jDistance = j - world.golLoc[0][1];
        searchMap.push_back( world.map.at<uchar>(Point(3*i, j)) );
        heuristic.push_back( pow ( sqrt( pow(iDistance,2) + pow(jDistance,2) ),
                            epsilon ) );
        globalScore.push_back( 1e9 );
        pathScore.push_back( 1e9 );
    }
    path.resize(world.nRows * world.nCols);
    moves.resize(world.nRows * world.nCols);
    curLoc = world.getIndexFromState(world.robLoc[0][0], world.robLoc[0][1]);
    path[curLoc].push_back(curLoc);
    openSet.push_back(curLoc);
}

/**
 * @brief getSearchNode
 *
 * Get next node to expand by returning the node with the current lowest global
 * score from the openSet (nodes to be expanded).
 *
 * @param void
 * @return int - index of next node to search
 */
int Planner::getSearchNode( ) {
    int lowScore = 1e9;
    for (uint i = 0; i < openSet.size(); i++) {
        if ( globalScore[ openSet[i] ] < lowScore ) {
            lowScore = globalScore[ openSet[i] ];
            curLoc = openSet[i];
        }
    }
    return curLoc;
}

/**
 * @brief getNeighbors
 *
 * Populate the moves vector with all legal moves at the current index.
 *
 * @param int - current search node index
 * @return void
 */
void Planner::getNeighbors( int curLoc ) {
    tempMoves = { curLoc - 1, curLoc + 1,
                    curLoc - world.nRows, curLoc + world.nCols,
                    curLoc - 1 - world.nRows, curLoc - 1 + world.nRows,
                    curLoc + 1 - world.nRows, curLoc + 1 + world.nRows };
    moves[curLoc].clear();
    for (uint i = 0; i < tempMoves.size(); i++) {
        if ( world.checkMove( tempMoves[i], world.robSize ) ) {
            moves[curLoc].push_back(tempMoves[i]);
            if ( std::find( openSet.begin(), openSet.end(), tempMoves[i])
                            == openSet.end()
                && std::find( closedSet.begin(), closedSet.end(), tempMoves[i])
                            == closedSet.end() ) {
                openSet.push_back(tempMoves[i]);
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
int Planner::searchAStar( int curLoc ) {

    // start the clock to time performance.
    float beginTime = clock();

    // reset expandedNodes counter
    expandedNodes = 1;

    cout << endl << "Searching with A* . " << std::flush;
    //cout << "openSet[0] = " << openSet[0] << endl;

    while ( !endGame ) {

        // expand the node in the openSet with lowest current score.
        curLoc = getSearchNode( );

        // get x and y coordinates
        world.getStateFromIndex( curLoc );
        //cout << "Current search node is: " << world.checkedMoveIndX;
        //cout << " & " << world.checkedMoveIndY << endl;
        //cout << "openSet.size() = " << openSet.size() << endl;

        // find all legal moves at the current node
        getNeighbors( curLoc );

        // grade all legal moves to fully expand a node
        for (uint j = 0; j < moves[curLoc].size(); j++) {

            // set move to be graded
            newLoc = moves[curLoc][j];

            // A* guarantees optimality along all expanded nodes, so there is no
            // need to re-check.  if move has been expanded, move along
            if ( std::find( closedSet.begin(), closedSet.end(), newLoc)
                            != closedSet.end() ) {
                continue;
            }

            // if move is at goal state, end game and send back path
            if ( world.checkGoal( newLoc )) {
                path[newLoc] = path[curLoc];
                path[newLoc].push_back(newLoc);
                curLoc = newLoc;
                endGame = true;
                goto doneSearching;
            } else  {

                // do nothing unless the new path to the index is less than the
                // current shortest path.  This ensures that the global score is
                // only modified if the searcher has found a shorter path.
                if ( path[curLoc].size() + 1 < pathScore[newLoc] ) {
                    path[newLoc] = path[curLoc];
                    path[newLoc].push_back( newLoc );
                    globalScore[newLoc] = heuristic[newLoc] + path[newLoc].size();
                    pathScore[newLoc] = path[newLoc].size();
                }
            }
        }

        // once fully expanded, set the globalScore at the expanded node to a
        // large number.  This is done to discourage searching the same node.
        globalScore[curLoc] = 1e9;

        // erase the current node from the openSet (nodes to be expanded).
        openSet.erase(
                std::remove( openSet.begin(), openSet.end(), curLoc ),
                openSet.end() );

        // add the current node to the closedSet (nodes that have been expanded)
        closedSet.push_back( curLoc );

        //world.moveQuery( curLoc );

        // increase expanded node counter
        ++expandedNodes;
        if (expandedNodes % 1000 == 0) {
            cout << ". " << std::flush;
        }
    }

    // what to do at endGame
    doneSearching:
    float endTime = float(clock() - beginTime) / float(CLOCKS_PER_SEC);
    cout << endl << "Path Optimized, " << expandedNodes << " nodes expanded.";
    cout << endl << "Total Optimization time: " << endTime << " seconds" << endl;
    cout << endl << "Press the return key to execute optimal path." << endl;
    cin.get();
    cout << "Optimal path is " << path[curLoc].size() << " steps long." << endl;

    return path[curLoc].size();
}

/**
 * @brief plotOptimalPath
 *
 * Defunct; Iterate through path and plot waypoints.
 *
 * @param void
 * @return void
 */
void Planner::plotOptimalPath( ) {
    cout << "Press the return key to execute optimal path." << endl;
    cin.get();
    //world.queLoc[0][0] = 0;
    //world.queLoc[0][1] = 0;
    //world.fillQuad(world.queLoc, world.robSize);
    cout << "Optimal path is " << path[curLoc].size() << " steps long." << endl;
    for( uint k = 0; k < path[curLoc].size(); k++) {
        world.getStateFromIndex( path[curLoc][k] );
        world.moveRobot( world.checkedMoveIndX, world.checkedMoveIndY );
    }
    cout << "GOAL!!" << endl;
}

/**
 * @brief getGoalX
 *
 * return x-coordinate for the goal location
 *
 * @param void
 * @return int - x-coordinate for goal location
 */
int Planner::getGoalX( ) {
    return world.golLoc[0][0];
}

/**
 * @brief getGoalY
 *
 * return y-coordinate for the goal location
 *
 * @param void
 * @return int - y-coordinate for goal location
 */
int Planner::getGoalY( ) {
    return world.golLoc[0][1];
}

/**
 * @brief getNextWaypoint
 *
 * return next waypoint from input location
 *
 * @param int - waypoint ID that robot is at
 * @return int - index at waypoint ID
 */
int Planner::getNextWaypoint( int index ) {
    return path[curLoc][index];
}

/**
 * @brief getNextWaypoint
 *
 * return next waypoint from input location
 *
 * @param void
 * @return int - index at waypoint ID
 */
int Planner::getPathSize( ) {
    return path[curLoc].size();
}

Planner::~Planner() {

}
