#ifndef LIDARPOINTS_H
#define LIDARPOINTS_H

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>


class LidarPointGenerator
{
public:
    LidarPointGenerator( polysync::Node & );

    void updatePoints();

    void publishPoints();
    void initializeMessage();

private:

    polysync::datamodel::LidarPointsMessage _message;

    float _relativeTime{ 0.0 };
    ulong _sensorID{ 11 };
    ulong _gridSideLength{ 100 };
    float _gridScale{ 10.0 };
    ulong _numberOfPoints{ 10000 };
    const float _sineFrequency{ 4.0 };
};

#endif // LIDARPOINTS_H
