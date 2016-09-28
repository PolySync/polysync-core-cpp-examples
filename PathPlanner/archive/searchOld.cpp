//Opencv C++ Program to solve mazes using mathematical morphology
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>
#include <cmath>
#include <iostream>
#include <armadillo>

#include "GridMap.hpp"

//using namespace cv;
using namespace arma;
using namespace std;
using namespace polysync::datamodel;

int main( )
{
    const clock_t beginTime = clock();
    GridMap * world = new GridMap;
    float epsilon{2};
    arma::Mat<int>searchMap(world->nRows, world->nCols, fill::ones);
    arma::Mat<float>heuristic(world->nRows, world->nCols, fill::zeros);
    std::vector<float*> heuristic2(world->nRows * world->nCols);
    arma::Mat<float>globalScore(world->nRows, world->nCols, fill::zeros);
    arma::Mat<float>pathScore(world->nRows, world->nCols, fill::zeros);
    int curLoc = world->getIndexFromState(world->robLoc[0][0], world->robLoc[0][1]);
    arma::uword curLocU = arma::uword(curLoc);
    std::vector< std::vector<int> > path(world->nRows * world->nCols);
    std::vector< std::vector<int> > moves(world->nRows * world->nCols);
    std::array<int, 8> tempMoves;
    path[curLoc].push_back(curLoc);

    float iDistance;
    float jDistance;
    for (int i = 0; i < world->nCols; i++) {
        for (int j = 0; j < world->nRows; j++) {
            searchMap(i, j) = world->map.at<uchar>(Point(3*i, j));
            iDistance = i - world->golLoc[0][0];
            jDistance = j - world->golLoc[0][1];
            //heuristic(i, j) = pow(sqrt( (i^2) + (j^2) ), epsilon);
            //heuristic(i, j) = sqrt( (i^2) + (j^2) );
            //heuristic(i, j) = ( fabs(iDistance) + fabs(jDistance) ) * epsilon;
            //heuristic(i, j) = pow( (fabs(iDistance) + fabs(jDistance)), epsilon );
            heuristic(i, j) = pow (sqrt( pow(iDistance,2) + pow(jDistance,2) ), epsilon);
            if (iDistance < 0) {
                //heuristic(i, j) = floor( heuristic(i, j) / 2 );
            }
            globalScore(i, j) = 1e9;
            pathScore(i, j) = 1e9;
        }
    }
    globalScore(curLocU) = heuristic(curLocU) + path[curLoc].size();
    pathScore(curLocU) = path[curLoc].size();
    find(globalScore == globalScore.min());

    arma::uword newLocU;
    int newLoc;
    bool endgame = false;
    tempMoves = {curLoc - 1, curLoc + 1, curLoc - world->nRows, curLoc + world->nCols,
                curLoc - 1 - world->nRows, curLoc - 1 + world->nRows,
                curLoc + 1 - world->nRows, curLoc + 1 + world->nRows};
    std::vector<int> closedSet;
    std::vector<int> openSet;
    openSet.push_back(curLoc);
    int nNodes = 1;
    cout << "Searching . " << std::flush;
    while ( !endgame ) {
        int lowScore = 1e9;
        for (uint i = 0; i < openSet.size(); i++) {
            if ( globalScore(openSet[i]) < lowScore ) {
                lowScore = globalScore(openSet[i]);
                curLoc = openSet[i];
                //cout << i << " " << openSet[i] << " " << lowScore <<endl;
            }
        }
        globalScore.min(curLocU);
        //curLocU = arma::uword(curLoc);
        curLoc = int(curLocU);
        //cout << globalScore(curLocU) << endl;
        tempMoves = {curLoc - 1, curLoc + 1, curLoc - world->nRows, curLoc + world->nCols,
                    curLoc - 1 - world->nRows, curLoc - 1 + world->nRows,
                    curLoc + 1 - world->nRows, curLoc + 1 + world->nRows};
        moves[curLoc].clear();
        for (uint i = 0; i < tempMoves.size(); i++) {
            if ( world->checkMove( tempMoves[i], world->robSize ) ) {
                moves[curLoc].push_back(tempMoves[i]);
                if ( std::find(openSet.begin(), openSet.end(), tempMoves[i]) != openSet.end() ) {
                    openSet.push_back(tempMoves[i]);
                }
                /*cout << moves[curLoc].back() << " & " <<
                        world->checkMoveIndexX << " & " <<
                        world->checkMoveIndexY << " & " <<
                        moves[curLoc].size()<< endl; */
            }
        }
        for (uint j = 0; j < moves[curLoc].size(); j++) {
            newLoc = moves[curLoc][j];
            newLocU = arma::uword(newLoc);
            if ( std::find(closedSet.begin(), closedSet.end(), newLoc) != closedSet.end() ) {
                continue;
            }
            if ( world->checkGoal(newLoc) ) {
                cout << endl << "GOAL!!" << endl;
                path[newLoc] = path[curLoc];
                path[newLoc].push_back(newLoc);
                curLoc = newLoc;
                curLocU = newLocU;
                endgame = true;
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
        //cout << "Index (" << curLoc << ") has score: " << globalScore(curLocU) << endl;
        globalScore(curLocU) = 1e9;
        openSet.erase(std::remove(openSet.begin(), openSet.end(), curLoc), openSet.end());
        closedSet.push_back(curLoc);
        //world->getStateFromIndex( curLoc );
        //world->moveRobot( world->checkMoveIndexX, world->checkMoveIndexY );
        //world->moveQuery( curLoc );
        ++nNodes;
        if (nNodes % 1000 == 0) {
            cout << ". " << std::flush;
        }
        //endgame = true;
    }

    doneSearching:
    const clock_t endTime = float(clock() - beginTime) / CLOCKS_PER_SEC;
    cout << "Path Optimized, " << nNodes << " nodes expanded." << endl;
    cout << "Total Optimization time: " << endTime << " seconds" << endl;
    cout << "Press any key to execute optimized path." << endl;
    cin.get();
    world->queLoc[0][0] = 0;
    world->queLoc[0][1] = 0;
    world->fillQuad(world->queLoc, world->robSize);
    for( uint k = 0; k < path[curLoc].size(); k++) {
        world->getStateFromIndex( path[curLoc][k] );
        world->moveRobot( world->checkMoveIndexX, world->checkMoveIndexY );
    }

    waitKey(0);

    return 0;
}
