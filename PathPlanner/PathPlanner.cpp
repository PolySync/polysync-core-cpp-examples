//Opencv C++ Program to solve mazes using mathematical morphology
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>
#include <cmath>
#include <iostream>

#include "GridMap.hpp"

using namespace cv;
using namespace std;
using namespace polysync::datamodel;

int main( )
{
    GridMap map;
    /*
    Mat src = imread("maze1.png", CV_LOAD_IMAGE_COLOR);
    Mat map = imread("maze2.pgm", CV_LOAD_IMAGE_COLOR);
    Mat robot = imread("robot.jpg", CV_LOAD_IMAGE_COLOR);
    resize(robot, robot, Size(25,25));
    resize(map, map, Size(400,400));
    threshold(map, map, 254.9, 255, THRESH_BINARY);
    if( !map.data ) { printf("Error loading src \n"); return -1;}
    Mat staticMap = map.clone();

    imshow("test", map);
    waitKey(1);

    printf("%i\n", map.cols);
    printf("%i\n", map.rows);
    //printf("%i\n\n", map[0]);
    printf("Corner 1: %i\n", map.at<uchar>(Point(1,1)));
    printf("Corner 2: %i\n", map.at<uchar>(Point(map.cols-1,map.rows-1)));
    printf("Corner 3: %i\n", map.at<uchar>(Point(1,map.rows-1)));
    printf("Corner 4: %i\n", map.at<uchar>(Point(map.cols-1,1)));
    map.at<uchar>(Point(50,50))= 127;
    map.at<uchar>(Point(100,100))= 127;
    map.at<uchar>(Point(150,150))= 127;
    map.at<uchar>(Point(200,200))= 127;

    robot.copyTo(map(cv::Rect(map.cols-robot.cols, map.rows-robot.rows, robot.cols, robot.rows)));
    imshow("test", map);
    waitKey(1);
    robot.copyTo(map(cv::Rect(1, 1, robot.cols, robot.rows)));
    //merge(robot, map);
    //int startRowInd = 6;
    //int startColInd = 7;
    //robot.copyTo(map.rowRange(startRowInd, robot.rows+startRowInd).colRange(startColInd, robot.cols+startColInd));
    imshow("test", map);
    waitKey(1);
    //map.setTo(staticMap);
    //map = staticMap.clone();
    staticMap.copyTo(map);
    imshow("test", map);
    waitKey(1);
    robot.copyTo(map(cv::Rect(map.cols-robot.cols, map.rows-robot.rows, robot.cols, robot.rows)));
    imshow("test", map);

    for(int i = 375; i < 377; i++) {
        for(int j = 1; j < 300; j++) {
            staticMap.copyTo(map);
            robot.copyTo(map(cv::Rect(i, j, robot.cols, robot.rows)));
            imshow("test", map);
            waitKey(0.1);
        }
    }
    printf("%i\n", robot.cols);
    printf("%i\n", robot.rows);
    //cout << map.at(1);

    Scalar intensity = map.at<int>(Point(50,50));
    printf("%f\n", intensity.val[0]);
    waitKey(0);
    */
    waitKey(0);

    return 0;
}
