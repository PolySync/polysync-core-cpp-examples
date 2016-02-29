#ifndef RADARTARGETGENERATOR_H
#define RADARTARGETGENERATOR_H

#include <PolySyncDataModel.hpp>
#include <PolySyncNode.hpp>

class RadarTargetGenerator
{
public:
    RadarTargetGenerator( polysync::Node & );

    void initializeMessage();
    void updateTargets();
    void publishTargets();
    void resetTargets();

private:
    polysync::datamodel::RadarTargetsMessage _message;

    std::vector< polysync::datamodel::RadarTarget > _targetVector;

    const ps_identifier _sensorID { 11 };
    const int _targetsCount{ 2 };
    const double _amplitude{ 2.0 };
    const double _velocityX{ 3.5 };
    const double _velocityY{ 3.5 };
    bool _reset{ false };
};

#endif // RADARTARGETGENERATOR_H
