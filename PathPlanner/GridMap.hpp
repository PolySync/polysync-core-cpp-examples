//Opencv C++ Program to solve mazes using mathematical morphology
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

    void generateMap( );

    void generateRobot( );

    void generateGoal( );

    void moveRobot( int x, int y );

    void fillQuad(int (&location)[4][2], int size);

    void updateMap( );

    bool checkHit( int x, int y, int size );

    bool checkGoal( int index );

    bool checkMove( int index, int size );

    int getIndexFromState( int x, int y );

    void getStateFromIndex( int index );

    Mat map;
    Mat robot;
    Mat gol;
    Mat staticMap;
    Mat pathMap;
    int nRows{ 500 };
    int nCols{ 500 };
    int robSize{ 29 };
    int robLoc[4][2];
    int golLoc[4][2];
    int checkMoveIndexX;
    int checkMoveIndexY;

private:

    string mapID{ "maze2.pgm" };
    string robID{ "robot.jpg" };
    string golID{ "gold.jpg" };

    /*
    float _relativeTime{ 0.0 };
    const float _gridScale{ 10.0 };
    const ulong _gridSideLength{ 100 };
    const ulong _sensorID{ 11 };
    const ulong _numberOfPoints{ 10000 };
    const float _sineFrequency{ 4.0 };
    */
};

#endif // GRIDMAP_HPP
