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
    GridMap world;
    float epsilon = 1;
    arma::Mat<int>searchMap(world.nRows, world.nCols, fill::ones);
    arma::Mat<float>heuristic(world.nRows, world.nCols, fill::zeros);
    arma::Mat<float>score(world.nRows, world.nCols, fill::zeros);
    int curLoc = world.getIndexFromState(world.robLoc[0][0], world.robLoc[0][1]);
    //int curLoc = world.getIndexFromState( world.robSize-1, world.robSize-1 );
    arma::uword curLocU = arma::uword(curLoc);
    std::vector< std::vector<int> > path(world.nRows * world.nCols);
    std::vector< std::vector<int> > moves(world.nRows * world.nCols);
    std::array<int, 4> tempMoves;
    path[curLoc].push_back(curLoc);
    for (int i = 0; i < world.nCols; i++) {
        for (int j = 0; j < world.nRows; j++) {
            searchMap(i, j) = world.map.at<uchar>(Point(3*i, j));
            heuristic(i, j) = pow(sqrt( (i^2) + (j^2) ), epsilon);
            score(i, j) = 1e9;
        }
    }
    score(curLoc) = heuristic(curLoc) + path[curLoc].size() - 1;
    find(score == score.min());

    arma::uword newLocU;
    int newLoc;
    bool endgame = false;
    tempMoves = {curLoc - 1, curLoc + 1, curLoc - 500, curLoc + 500};
    while ( !endgame ) {
        score.min(curLocU);
        curLoc = int(curLocU);
        cout << score(curLocU) << endl;
        tempMoves = {curLoc - 1, curLoc + 1, curLoc - 500, curLoc + 500};
        for (uint i = 0; i < tempMoves.size(); i++) {
            if ( world.checkMove( tempMoves[i], world.robSize ) ) {
                moves[curLoc].push_back(tempMoves[i]);
                /*cout << moves[curLoc].back() << " & " <<
                        world.checkMoveIndexX << " & " <<
                        world.checkMoveIndexY << " & " <<
                        moves[curLoc].size()<< endl; */
            } else {
                score(arma::uword(tempMoves[i])) = 1e18;
            }
        }
        for (uint j = 0; j < moves[curLoc].size(); j++) {
            newLoc = moves[curLoc][j];
            newLocU = arma::uword(newLoc);
            if ( world.checkGoal(newLoc) ) {
                cout << "GOAL!!" << endl;
                curLoc = newLoc;
                curLocU = newLocU;
                endgame = true;
            } else  {
                path[newLoc] = path[curLoc];
                path[newLoc].push_back(newLoc);
                score(newLocU) = heuristic(newLocU) + path[newLoc].size() - 1;
            }
        }
        score(curLocU) = 1e9;
        world.getStateFromIndex( curLoc );
        cout << curLoc << endl;
        world.moveRobot( world.checkMoveIndexX, world.checkMoveIndexY );
        //endgame = true;
    }

    for( int k = 0; k < 500; ++k) {
        cout << heuristic(k, 499) << endl;
    }
/*
    score[1] = 4;
    score[2] = 4;
    if ( size(find(score == score.min())).n_rows == 1 ) {
        cout << size(find(score == score.min())) << endl;
    } else if ( size(find(score == score.min())).n_rows == 2 ) {
        cout << "2 TIMES!!" << endl;
    }

    uword temp;
    score.min(temp);
    cout << temp << endl;

    cout << curLoc << " & " << path[curLoc].at(0) << " & " << path[curLoc].size() << endl;

    world.getStateFromIndex( 0 );
    cout << world.checkMoveIndexX << " & " << world.checkMoveIndexY << endl;
    world.getStateFromIndex( 500 );
    cout << world.checkMoveIndexX << " & " << world.checkMoveIndexY << endl;
    world.getStateFromIndex( 1 );
    cout << world.checkMoveIndexX << " & " << world.checkMoveIndexY << endl;
    world.getStateFromIndex( 1001 );
    cout << world.checkMoveIndexX << " & " << world.checkMoveIndexY << endl;

    cout << world.getIndexFromState(0, 0) << endl;
    cout << world.getIndexFromState(0, 1) << endl;
    cout << world.getIndexFromState(1, 0) << endl;

    cout << heuristic(0,0) << " " << heuristic(100, 100) << " " << heuristic(200, 200) << endl;
    cout << heuristic(499, 499) << " " << heuristic(200, 2) << " " << heuristic(2, 200) << endl;

    cout << searchMap(0,0) << " " << searchMap(0, 499) << " " << searchMap(499, 0) << endl;
    cout << searchMap(499, 499) << " " << searchMap(250, 100) << " " << searchMap(100, 250) << endl;
    cout << searchMap(100, 350) << " " << searchMap(350, 100) << " " << searchMap(400, 200) << endl;

    cout << searchMap(world.getIndexFromState(0, 0)) << " " <<
            searchMap(world.getIndexFromState(0, 499)) << " " <<
            searchMap(world.getIndexFromState(499, 0)) << endl;
    cout << searchMap(world.getIndexFromState(499, 499)) << " " <<
            searchMap(world.getIndexFromState(250, 100)) << " " <<
            searchMap(world.getIndexFromState(100, 250)) << endl;
            cout << searchMap(world.getIndexFromState(100, 350)) << " " <<
                    searchMap(world.getIndexFromState(350, 100)) << " " <<
                    searchMap(world.getIndexFromState(400, 200)) << endl;
*/
    auto test1 = score.elem(score == score.min());
    //cout << test1 << endl;
    auto test2 = score.elem(find(score==score.min()));
    //cout << test2 << endl;
    auto whereMin = find(score==score.min());

    //cout << score(whereMin) << " is located at index: " << score.elem(score == score.min()) <<endl;

    //cout << score(whereMin) << " is located at index: " << score.elem(find(score==score.min())) <<endl;

    cout << score(whereMin) << " is located at index: " << find(score==score.min()) <<endl;

    cout << path.size() << endl;
    cout << searchMap.n_elem << " " << searchMap.n_rows << " " << searchMap.n_cols << endl;

    //path[newLoc] = path[curLoc];
    //path[newLoc].push_back(newLoc);

    //path[1].size()=0
    //path[1].push_back(1)

    waitKey(0);

    return 0;
}
