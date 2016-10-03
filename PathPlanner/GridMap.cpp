//GridMap.cpp
#include "GridMap.hpp"
#include <cmath>
#include <iostream>
#include <unistd.h>


using namespace cv;
using namespace std;

GridMap::GridMap( ) {
    srand(time(NULL));
    //generateMap( );
    /*for(int i = 1; i < nCols - robSize-1; i++) {
        moveRobot(nRows-robSize-1, i);
    }*/
    //moveRobot(1,1);
    //moveRobot(nRows - robSize - 1, nCols - robSize - 1);
}

GridMap::~GridMap( ) {

}

void GridMap::generateMap( ) {
    map = imread(mapID, CV_LOAD_IMAGE_COLOR);
    resize(map, map, Size(nRows, nCols));
    threshold(map, map, 254.9, 255, THRESH_BINARY);
    if( !map.data ) {
        printf("Error loading src \n");
        return;
    }
    generateGoal( );
    staticMap = map.clone();
    pathMap = map.clone();
    //generateQuery( );
    generateRobot( );
    showMap( );
}

void GridMap::generateMap( int x, int y ) {
    map = imread(mapID, CV_LOAD_IMAGE_COLOR);
    resize(map, map, Size(nRows, nCols));
    threshold(map, map, 254.9, 255, THRESH_BINARY);
    if( !map.data ) {
        printf("Error loading src \n");
        return;
    }
    generateGoal( x, y );
    staticMap = map.clone();
    pathMap = map.clone();
    //generateQuery( );
    generateRobot( );
    showMap( );
}

void GridMap::generateRobot( ) {
    robot = imread(robID, CV_LOAD_IMAGE_COLOR);
    resize(robot, robot, Size(robSize, robSize));
    robLoc[0][0] = nCols - robSize - 1;
    robLoc[0][1] = nRows - robSize - 1;
    fillQuad(robLoc, robSize);
    robot.copyTo(map(cv::Rect(robLoc[0][0], robLoc[0][1], robSize, robSize)));
    //cout << robLoc[3][0] << " " << robLoc[3][1] << endl;
}

void GridMap::generateQuery( ) {
    query = imread(queID, CV_LOAD_IMAGE_COLOR);
    resize(query, query, Size(robSize, robSize));
    queLoc[0][0] = nCols - robSize - 1;
    queLoc[0][1] = nRows - robSize - 1;
    fillQuad(queLoc, robSize);
    query.copyTo(map(cv::Rect(queLoc[0][0], queLoc[0][1], robSize, robSize)));
}

void GridMap::generateGoal( ) {
    gol = imread(golID, CV_LOAD_IMAGE_COLOR);
    resize(gol, gol, Size(robSize, robSize));
    //golLoc[0][0] = 0;
    //golLoc[0][1] = 0;
    golLoc[0][0] = rand() % 50 + 150;
    golLoc[0][1] = rand() % 50 + 1;
    fillQuad(golLoc, robSize);
    gol.copyTo(map(cv::Rect(golLoc[0][0], golLoc[0][1], robSize, robSize)));
}

void GridMap::generateGoal( int x, int y ) {
    gol = imread(golID, CV_LOAD_IMAGE_COLOR);
    resize(gol, gol, Size(robSize, robSize));
    golLoc[0][0] = x;
    golLoc[0][1] = y;
    fillQuad(golLoc, robSize);
    gol.copyTo(map(cv::Rect(golLoc[0][0], golLoc[0][1], robSize, robSize)));
}

void GridMap::moveRobot( int x, int y ){
    bool hit = checkHit( x, y, robSize );
    if (hit == false) {
        robLoc[0][0] = x;
        robLoc[0][1] = y;
        fillQuad(robLoc, robSize);
        updateMap( );
    } else {
        return;
    }
    checkGoal( getIndexFromState(robLoc[0][0], robLoc[0][1]) );
}

void GridMap::moveQuery( int index ){
    getStateFromIndex( index );
    queLoc[0][0] = checkMoveIndexX;
    queLoc[0][1] = checkMoveIndexY;
    fillQuad(queLoc, robSize);
    updateMap( );
}

void GridMap::fillQuad( int (&location)[4][2], int size ){
    location[1][0] = location[0][0] + size;
    location[1][1] = location[0][1];
    location[2][0] = location[0][0];
    location[2][1] = location[0][1] + size;
    location[3][0] = location[1][0];
    location[3][1] = location[2][1];
}

void GridMap::showMap( ) {
    map = staticMap.clone();
    //query.copyTo(map(cv::Rect(queLoc[0][0], queLoc[0][1], robSize, robSize)));
    robot.copyTo(map(cv::Rect(robLoc[0][0], robLoc[0][1], robSize, robSize)));
    imshow("PolySync Path Planner Example", map);
    waitKey(50);
}

void GridMap::updateMap( ) {
    int xCenter = int( floor( robLoc[0][0] + (robSize/2) ));
    int yCenter = int( floor( robLoc[0][1] + (robSize/2) ));
    map = pathMap.clone();
    pathMap.at<uchar>( Point(3*xCenter-1, yCenter) )= 0;
    //query.copyTo(map(cv::Rect(queLoc[0][0], queLoc[0][1], robSize, robSize)));
    robot.copyTo(map(cv::Rect(robLoc[0][0], robLoc[0][1], robSize, robSize)));
    imshow("PolySync Path Planner Example", map);
    waitKey(1);
}

bool GridMap::checkHit( int x, int y, int size ) {
    int tempLoc[4][2]{x, y, 0, 0, 0, 0, 0, 0};
    fillQuad(tempLoc, size);
    for (int i = 0; i < 4; i++) {
        if (map.at<uchar>(Point(3*tempLoc[i][0], tempLoc[i][1])) == 0){
            //printf("HIT!! %i, %i, %i\n", tempLoc[i][0], tempLoc[i][1], map.at<uchar>(Point(tempLoc[i][0], tempLoc[i][1])));
            return true;
        } else if ( tempLoc[i][0] < 0 || tempLoc[i][1] < 0 ||
                    tempLoc[i][0] >= nCols || tempLoc[i][1] >= nRows ) {
            //printf("HIT!! %i, %i, %i\n", tempLoc[i][0], tempLoc[i][1], map.at<uchar>(Point(tempLoc[i][0], tempLoc[i][1])));
            return true;
        }
    }
    return false;
}

bool GridMap::checkGoal( int index ) {
    getStateFromIndex( index );
    if( abs(checkMoveIndexX - golLoc[0][0]) <= robSize-1
        && abs(checkMoveIndexY - golLoc[0][1]) <= robSize-1 ) {
        //printf("GOAL!! %i, %i, %i\n", checkMoveIndexX, checkMoveIndexY,
            //map.at<uchar>(Point(checkMoveIndexX, checkMoveIndexY)));
        return true;
    }
    return false;
}

bool GridMap::checkMove( int index, int size ) {
    getStateFromIndex( index );
    if ( !checkHit( checkMoveIndexX, checkMoveIndexY, size ) ) {
        return true;
    }
    return false;
}

int GridMap::getIndexFromState( int x, int y ) {
    return int(y * nRows + x);
}

void GridMap::getStateFromIndex( int index ) {
    checkMoveIndexX = index % nRows;
    checkMoveIndexY = index / nCols;
    return;
    if (checkMoveIndexX == 0) {
        checkMoveIndexY = 0;
    } else {
        checkMoveIndexY = index - nCols + checkMoveIndexX;
    }
}
