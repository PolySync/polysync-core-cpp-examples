//GridMap.hpp
#ifndef GRIDMAP_HPP
#define GRIDMAP_HPP

#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;

class GridMap {

public:

    GridMap( );

    ~GridMap( );

    void generateMap( );

    void generateMap( int x, int y );

    void moveRobot( int x, int y );

    void moveQuery( int index );

    bool checkGoal( int index );

    bool checkMove( int index, int size );

    int getIndexFromState( int x, int y );

    void getStateFromIndex( int index );

    Mat map;
    int nRows{ 500 };
    int nCols{ 500 };
    int robSize{ 29 };
    int robLoc[4][2];
    int queLoc[4][2];
    int golLoc[4][2];
    int checkedMoveIndX;
    int checkedMoveIndY;

private:

    void generateRobot( );

    void generateQuery( );

    void generateGoal( );

    void generateGoal( int x, int y );

    void fillQuad(int (&location)[4][2], int size);

    void showMap( );

    void updateMap( );

    bool checkHit( int x, int y, int size );

    Mat robot;
    Mat query;
    Mat gol;
    Mat staticMap;
    Mat pathMap;
    string mapID{ "resources/maze2.pgm" };
    string robID{ "resources/robot.jpg" };
    string golID{ "resources/gold.jpg" };
    string queID{ "resources/question.jpg" };

};

#endif // GRIDMAP_HPP
