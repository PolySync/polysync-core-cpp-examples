//PathPlanner
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>
#include <cmath>
#include <iostream>

#include "Search.hpp"
#include "GridMap.hpp"

using namespace cv;
using namespace std;
using namespace polysync::datamodel;

int main( )
{
    Search * searcher = new Search;
    searcher->searchAStar( searcher->curLoc );
    searcher->plotOptimalPath( );

    waitKey(0);

    delete searcher;
    return 0;
}
