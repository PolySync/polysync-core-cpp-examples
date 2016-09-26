//Opencv C++ Program to solve mazes using mathematical morphology
#include "GridMap.hpp"

using namespace cv;
using namespace std;

GridMap::GridMap( ) {
    srand(time(NULL));
    generateMap( );
    //generateRobot( );
    /*for(int i = 1; i < nCols - robSize-1; i++) {
        moveRobot(nRows-robSize-1, i);
    }*/
    //moveRobot(0,0);
    //moveRobot(1,1);
    //moveRobot(robSize,robSize);
    //moveRobot(robSize*2,robSize*2);
    //moveRobot(robSize*2-1,robSize*2-1);
    //moveRobot(robSize-1,robSize-1);
    //moveRobot(nRows - robSize - 1, nCols - robSize - 1);
    //moveRobot(robSize*2-1,robSize*2);
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
    //gol.copyTo(map(cv::Rect(golLoc[0][0], golLoc[0][1], robSize, robSize)));
    staticMap = map.clone();
    pathMap = map.clone();
    generateRobot( );
}

void GridMap::generateRobot( ) {
    robot = imread(robID, CV_LOAD_IMAGE_COLOR);
    resize(robot, robot, Size(robSize, robSize));
    robLoc[0][0] = nCols - robSize - 1;
    robLoc[0][1] = nRows - robSize - 1;
    fillQuad(robLoc, robSize);
    robot.copyTo(map(cv::Rect(robLoc[0][0], robLoc[0][1], robSize, robSize)));
    updateMap( );
    cout << robLoc[3][0] << " " << robLoc[3][1] << endl;
}

void GridMap::generateGoal( ) {
    gol = imread(golID, CV_LOAD_IMAGE_COLOR);
    resize(gol, gol, Size(robSize, robSize));
    golLoc[0][0] = 0;
    golLoc[0][1] = 0;
    //golLoc[0][0] = rand() % 50 + 275;
    //golLoc[0][1] = rand() % 50 + 1;
    fillQuad(golLoc, robSize);
    gol.copyTo(map(cv::Rect(golLoc[0][0], golLoc[0][1], robSize, robSize)));
}

void GridMap::moveRobot( int x, int y ){
    bool hit = checkHit( x, y, robSize );
    if (hit == false) {
        robLoc[0][0] = x;
        robLoc[0][1] = y;
        fillQuad(robLoc, robSize);
        //cout << robLoc[3][0] << " " << robLoc[3][1] << endl;
        updateMap( );
    } else {
        //printf("HIT!! %i, %i, %i\n", x, y, map.at<uchar>(Point(x, y)));
        //waitKey(10);
        return;
    }
    checkGoal( getIndexFromState(robLoc[0][0], robLoc[0][1]) );
}

void GridMap::fillQuad( int (&location)[4][2], int size ){
    location[1][0] = location[0][0] + size;
    location[1][1] = location[0][1];
    location[2][0] = location[0][0];
    location[2][1] = location[0][1] + size;
    location[3][0] = location[1][0];
    location[3][1] = location[2][1];
}

void GridMap::updateMap( ) {
    map = staticMap.clone();
    robot.copyTo(map(cv::Rect(robLoc[0][0], robLoc[0][1], robSize, robSize)));
    imshow("test", map);
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
        printf("GOAL!! %i, %i, %i\n", checkMoveIndexX, checkMoveIndexY,
            map.at<uchar>(Point(checkMoveIndexX, checkMoveIndexY)));
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
    return int(y * map.rows + x);
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
/*
printf("map.rows - robot.rows: %i\n", map.rows - robot.rows);

for(int i = map.cols - robot.cols; i <= map.cols - robot.cols; i++) {
//for( int i = 1; i <= 300; i++) {
    for(int j = 1; j < map.rows - robot.rows; j++) {
        map = staticMap.clone();
        //map.at<uchar>(Point(3*i,j))= 127;
        cout << static_cast <short> (staticMap.at<uchar>(Point(3*i,j))) << " "
        << static_cast <short> (map.at<uchar>(Point(3*i,j))) << endl;
        //staticMap.copyTo(map);
        checkHit(i, j);
        robot.copyTo(map(cv::Rect(i, j, robot.cols, robot.rows)));
        staticMap.at<uchar>(Point(3*i,j))= 127;
        //printf("vals: %i\n", map.at<uchar>(Point(i+100, j)));
        imshow("test", map);
        waitKey(1);
    }
}
*/
