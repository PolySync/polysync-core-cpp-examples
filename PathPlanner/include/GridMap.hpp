//GridMap.hpp
#ifndef GRIDMAP_HPP
#define GRIDMAP_HPP

#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;


class GridMap
{

public:

    GridMap( );

    ~GridMap( );

    void generateMap( );

    void generateMap( int x, int y );

    void moveRobot( int x, int y );

    bool checkGoal( int index );

    bool checkMove( int index, int size );

    int getIndexFromState( int x, int y );

    void getStateFromIndex( int index );


public:

    Mat map;

    int nRows{ 500 };

    int nCols{ 500 };

    int robSize{ 29 };

    int robLoc[4][2];

    int golLoc[4][2];

    int checkedMoveIndX;

    int checkedMoveIndY;


private:

    void generateRobot( );

    void generateGoal( );

    void generateGoal( int x, int y );

    void fillQuad(int ( &location )[ 4 ][ 2 ], int size);

    void showMap( );

    void updateMap( );

    bool checkHit( int x, int y, int size );


private:

    Mat _robot;

    Mat _gol;

    Mat _staticMap;

    Mat _pathMap;

    string _mapID{ "../res/maze2.pgm" };

    string _robID{ "../res/robot.jpg" };

    string _golID{ "../res/gold.jpg" };

};

#endif // GRIDMAP_HPP
