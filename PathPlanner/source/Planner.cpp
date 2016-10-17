//Planner.cpp
#include <cmath>
#include <iostream>

#include "Planner.hpp"


using namespace std;


Planner::Planner( )
    :
    world( ),
    path( )
    {
    world.generateMap( );
    initializeSearchSpace( );
}

void Planner::initializeSearchSpace( ) {
    int i = 0;
    int j = 0;
    float iDistance;
    float jDistance;
    for (int k = 0; k < (world.nCols * world.nRows); k++) {
        world.getStateFromIndex( k );
        i = world.checkedMoveIndX;
        j = world.checkedMoveIndY;
        iDistance = i - world.golLoc[0][0];
        jDistance = j - world.golLoc[0][1];
        searchMap.push_back( world.map.at<uchar>(Point(3*i, j)) );
        heuristic.push_back( pow (sqrt( pow(iDistance,2) + pow(jDistance,2) ), epsilon));
        globalScore.push_back( 1e9 );
        pathScore.push_back( 1e9 );
    }
    path.resize(world.nRows * world.nCols);
    moves.resize(world.nRows * world.nCols);
    curLoc = world.getIndexFromState(world.robLoc[0][0], world.robLoc[0][1]);
    path[curLoc].push_back(curLoc);
    openSet.push_back(curLoc);
}

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

void Planner::searchAStar( int curLoc ) {
    float beginTime = clock();
    exploredNodes = 1;
    cout << endl << "Searching with A* . " << std::flush;
    //cout << "openSet[0] = " << openSet[0] << endl;
    while ( !endGame ) {
        curLoc = getSearchNode( );
        world.getStateFromIndex( curLoc );
        //cout << "Current search node is: " << world.checkedMoveIndX << " & " << world.checkedMoveIndY << endl;
        //cout << "openSet.size() = " << openSet.size() << endl;
        getNeighbors( curLoc );
        for (uint j = 0; j < moves[curLoc].size(); j++) {
            newLoc = moves[curLoc][j];
            if ( std::find( closedSet.begin(), closedSet.end(), newLoc)
                            != closedSet.end() ) {
                continue;
            }
            if ( world.checkGoal( newLoc )) {
                path[newLoc] = path[curLoc];
                path[newLoc].push_back(newLoc);
                curLoc = newLoc;
                endGame = true;
                goto doneSearching;
            } else  {
                if ( path[curLoc].size() + 1 < pathScore[newLoc] ) {
                    path[newLoc] = path[curLoc];
                    path[newLoc].push_back(newLoc);
                    globalScore[newLoc] = heuristic[newLoc] + path[newLoc].size();
                    pathScore[newLoc] = path[newLoc].size();
                }
            }
        }
        globalScore[curLoc] = 1e9;
        openSet.erase(std::remove(openSet.begin(), openSet.end(), curLoc), openSet.end());
        closedSet.push_back(curLoc);
        //world.moveQuery( curLoc );
        ++exploredNodes;
        if (exploredNodes % 1000 == 0) {
            cout << ". " << std::flush;
        }
    }
    doneSearching:
    float endTime = float(clock() - beginTime) / float(CLOCKS_PER_SEC);
    cout << endl << "Path Optimized, " << exploredNodes << " nodes expanded.";
    cout << endl << "Total Optimization time: " << endTime << " seconds" << endl;
    cout << endl << "Press the return key to execute optimal path." << endl;
    cin.get();
    cout << "Optimal path is " << path[curLoc].size() << " steps long." << endl;
}

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

int Planner::getGoalX( ) {
    return world.golLoc[0][0];
}

int Planner::getGoalY( ) {
    return world.golLoc[0][1];
}

int Planner::getNextWaypoint( int index ) {
    return path[curLoc][index];
}

Planner::~Planner() {

}
