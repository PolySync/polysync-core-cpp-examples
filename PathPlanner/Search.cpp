//Search.cpp
#include "Search.hpp"

using namespace arma;
using namespace std;
using namespace polysync::datamodel;

Search::Search( ) {
    world = new GridMap;
    initializeSearchSpace( );
    curLoc = world->getIndexFromState(world->robLoc[0][0], world->robLoc[0][1]);
    curLocU = arma::uword(curLoc);
    path[curLoc].push_back(curLoc);
    globalScore(curLocU) = heuristic(curLocU) + path[curLoc].size();
    pathScore(curLocU) = path[curLoc].size();
    openSet.push_back(curLoc);
}

void Search::initializeSearchSpace( ) {
    searchMap.ones(world->nRows, world->nCols);
    heuristic.zeros(world->nRows, world->nCols);
    globalScore.zeros(world->nRows, world->nCols);
    pathScore.zeros(world->nRows, world->nCols);
    path.resize(world->nRows * world->nCols);
    moves.resize(world->nRows * world->nCols);
    float iDistance;
    float jDistance;
    for (int i = 0; i < world->nCols; i++) {
        for (int j = 0; j < world->nRows; j++) {
            searchMap(i, j) = world->map.at<uchar>(Point(3*i, j));
            iDistance = i - world->golLoc[0][0];
            jDistance = j - world->golLoc[0][1];
            //heuristic(i, j) = ( fabs(iDistance) + fabs(jDistance) ) * epsilon;
            //heuristic(i, j) = pow( (fabs(iDistance) + fabs(jDistance)), epsilon );
            heuristic(i, j) = pow (sqrt( pow(iDistance,2) + pow(jDistance,2) ), epsilon);
            globalScore(i, j) = 1e9;
            pathScore(i, j) = 1e9;
        }
    }
}

int Search::getSearchNode( ) {
    int lowScore = 1e9;
    for (uint i = 0; i < openSet.size(); i++) {
        if ( globalScore(openSet[i]) < lowScore ) {
            lowScore = globalScore(openSet[i]);
            curLoc = openSet[i];
        }
    }
    return curLoc;
}

void Search::getNeighbors( int curLoc ) {
    tempMoves = {curLoc - 1, curLoc + 1, curLoc - world->nRows, curLoc + world->nCols,
              curLoc - 1 - world->nRows, curLoc - 1 + world->nRows,
              curLoc + 1 - world->nRows, curLoc + 1 + world->nRows};
    moves[curLoc].clear();
    for (uint i = 0; i < tempMoves.size(); i++) {
        if ( world->checkMove( tempMoves[i], world->robSize ) ) {
            moves[curLoc].push_back(tempMoves[i]);
            if ( std::find(openSet.begin(), openSet.end(), tempMoves[i]) == openSet.end()
            && std::find(closedSet.begin(), closedSet.end(), tempMoves[i]) == closedSet.end()) {
                openSet.push_back(tempMoves[i]);
            }
        }
    }
}

void Search::searchAStar( int curLoc ) {
    float beginTime = clock();
    exploredNodes = 1;
    cout << endl << "Searching with A* . " << std::flush;
    while ( !endGame ) {
        curLoc = getSearchNode( );
        curLocU = arma::uword(curLoc);
        getNeighbors( curLoc );
        for (uint j = 0; j < moves[curLoc].size(); j++) {
            newLoc = moves[curLoc][j];
            newLocU = arma::uword(newLoc);
            if ( std::find(closedSet.begin(), closedSet.end(), newLoc) != closedSet.end() ) {
                continue;
            }
            if ( world->checkGoal(newLoc) ) {
                path[newLoc] = path[curLoc];
                path[newLoc].push_back(newLoc);
                curLoc = newLoc;
                curLocU = newLocU;
                endGame = true;
                goto doneSearching;
            } else  {
                if ( path[curLoc].size() + 1 < pathScore(newLoc) ) {
                    path[newLoc] = path[curLoc];
                    path[newLoc].push_back(newLoc);
                    globalScore(newLocU) = heuristic(newLocU) + path[newLoc].size();
                    pathScore(newLocU) = path[newLoc].size();
                }
            }
        }
        globalScore(curLocU) = 1e9;
        openSet.erase(std::remove(openSet.begin(), openSet.end(), curLoc), openSet.end());
        closedSet.push_back(curLoc);
        //world->moveQuery( curLoc );
        ++exploredNodes;
        if (exploredNodes % 1000 == 0) {
            cout << ". " << std::flush;
        }
    }
    doneSearching:
    float endTime = float(clock() - beginTime) / float(CLOCKS_PER_SEC);
    cout << endl << "Path Optimized, " << exploredNodes << " nodes expanded.";
    cout << endl <<"Total Optimization time: " << endTime << " seconds" << endl;
}

void Search::plotOptimalPath( ) {
    cout << "Press the return key to execute optimal path." << endl;
    cin.get();
    //world->queLoc[0][0] = 0;
    //world->queLoc[0][1] = 0;
    //world->fillQuad(world->queLoc, world->robSize);
    cout << "Optimal path is " << path[curLoc].size() << " steps long." << endl;
    for( uint k = 0; k < path[curLoc].size(); k++) {
        world->getStateFromIndex( path[curLoc][k] );
        world->moveRobot( world->checkMoveIndexX, world->checkMoveIndexY );
    }
    cout << "GOAL!!" << endl;
}

Search::~Search() {
    delete world;
}
