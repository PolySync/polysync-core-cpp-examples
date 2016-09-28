//GridMap.hpp
#ifndef GRIDMAP_HPP
#define GRIDMAP_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <cmath>
#include <iostream>
//#include <cstdlib>

using namespace cv;
using namespace std;

class GridMap {

public:

    GridMap( );

    ~GridMap( );

    void moveRobot( int x, int y );

    void moveQuery( int index );

    bool checkGoal( int index );

    bool checkMove( int index, int size );

    int getIndexFromState( int x, int y );

    void getStateFromIndex( int index );

    Mat map;
    Mat robot;
    Mat query;
    Mat gol;
    Mat staticMap;
    Mat pathMap;
    int nRows{ 500 };
    int nCols{ 500 };
    int robSize{ 29 };
    int robLoc[4][2];
    int queLoc[4][2];
    int golLoc[4][2];
    int checkMoveIndexX;
    int checkMoveIndexY;

private:

    void generateMap( );

    void generateRobot( );

    void generateQuery( );

    void generateGoal( );

    void fillQuad(int (&location)[4][2], int size);

    void showMap( );

    void updateMap( );

    bool checkHit( int x, int y, int size );

    string mapID{ "maze2.pgm" };
    string robID{ "robot.jpg" };
    string golID{ "gold.jpg" };
    string queID{ "question.jpg" };

};

#endif // GRIDMAP_HPP
