//Opencv C++ Program to solve mazes using mathematical morphology
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <cmath>
#include <iostream>

using namespace cv;
using namespace std;

int main( )
{
    Mat src = imread("maze1.png", CV_LOAD_IMAGE_COLOR);
    Mat map = imread("maze2.pgm", CV_LOAD_IMAGE_COLOR);
    Mat robot = imread("robot.jpg", CV_LOAD_IMAGE_COLOR);
    resize(robot, robot, map.size());
    resize(map, map, src.size());
    threshold(map, map, 254.9, 255, THRESH_BINARY);
    if( !src.data ) { printf("Error loading src \n"); return -1;}
    Mat staticMap = map.clone();
 
 //Convert the given image into Binary Image
    Mat bw;
    cvtColor(src, bw, CV_BGR2GRAY);
    threshold(bw, bw, 10, 255, CV_THRESH_BINARY_INV);

 //Detect Contours in an Image
    vector<std::vector<cv::Point> > contours;
    findContours(bw, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    if (contours.size() != 2)
    {
        // "Perfect maze" should have 2 walls
        std::cout << "This is not a 'perfect maze' with just 2 walls!" << std::endl;
        return -1;
    }

    Mat path = Mat::zeros(src.size(), CV_8UC1);
    drawContours(path, contours, 0, CV_RGB(255,255,255), CV_FILLED);

 //Dilate the Image
    Mat kernel = Mat::ones(21, 21, CV_8UC1);
    dilate(path, path, kernel);

 //Erode the Image
    Mat path_erode;
    erode(path, path_erode, kernel);

 //Subtract Eroded Image from the Dilate One
    absdiff(path, path_erode, path);

 //Draw the Path by Red Color
    vector<Mat> channels;
    split(src, channels);
    channels[0] &= ~path;
    channels[1] &= ~path;
    channels[2] |= path;

    Mat dst;
    merge(channels, dst);

    imshow("test", map);
    waitKey(1);

    //imshow("solution", dst);
    //printf("%f\n", map.size());
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

    for(int i = 1; i < 200; i++) {
        for(int j = 1; j < 200; j++) {
            staticMap.copyTo(map);
            robot.copyTo(map(cv::Rect(i, j, robot.cols, robot.rows)));
            imshow("test", map);
            waitKey(1);
        }
    }

    //cout << map.at(1);

    Scalar intensity = map.at<int>(Point(50,50));
    printf("%f\n", intensity.val[0]);
    waitKey(0);

    return 0;
}
