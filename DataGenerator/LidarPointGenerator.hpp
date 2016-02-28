#ifndef LIDARPOINTS_H
#define LIDARPOINTS_H

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>


class LidarPointGenerator
{
public:
    LidarPointGenerator( polysync::Node & );

    const polysync::datamodel::LidarPointsMessage & updatePoints();

private:
    void initializeMessage();

    polysync::datamodel::LidarPointsMessage _message;

    double _relativeTime{ 0.0 };
    ulong _sensorID{ 11 };
    ulong _gridSideLength{ 100 };
    double _gridScale{ 10.0 };
    ulong _numberOfPoints;
    const double _sineFrequency{ 4.0 };
};

#endif // LIDARPOINTS_H
