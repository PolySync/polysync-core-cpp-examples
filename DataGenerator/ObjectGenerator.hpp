#ifndef OBJECTGENERATOR_H
#define OBJECTGENERATOR_H

#include <PolySyncDataModel.hpp>
#include <PolySyncNode.hpp>

class ObjectGenerator
{
public:
    ObjectGenerator( polysync::Node & );

    void initializeMessage();
    void resetObjects();
    void updateObjects();
    void publishObjects();

private:
    polysync::datamodel::ObjectsMessage _message;
    std::vector< polysync::datamodel::Object > _objectVector;

    const ps_identifier _sensorID{ 12 };
    const int _objectCount{ 2 };
    const double _objectSizeX{ 2.0 };
    const double _objectSizeY{ 1.5 };
    const double _objectSizeZ{ 1.0 };
    const double _velocityX{ 3.5 };
    const double _velocityY{ -3.5 };
    bool _reset{ false };
};

#endif // OBJECTGENERATOR_H
