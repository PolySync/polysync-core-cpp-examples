#include <iostream>

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

#include "LidarPointGenerator.hpp"
#include "RadarTargetGenerator.hpp"


using namespace polysync::datamodel;

class DataGenerator : public polysync::Node
{
public:

    DataGenerator()
    {}

protected:

    virtual void okStateEvent()
    {
        _lidarPointGenerator->updatePoints();
        _lidarPointGenerator->publishPoints();

        _radarTargetGenerator->updateTargets();
        _radarTargetGenerator->publishTargets();

        polysync::sleepMicro( _updateInterval );
    }

    virtual void initStateEvent()
    {
        _lidarPointGenerator =
                std::unique_ptr< LidarPointGenerator >{
                    new LidarPointGenerator( *this ) };

        _radarTargetGenerator =
                std::unique_ptr< RadarTargetGenerator >{
                    new RadarTargetGenerator( *this ) };
    }

private:
    ps_timestamp _updateInterval{ 50000 };
    std::unique_ptr< LidarPointGenerator > _lidarPointGenerator;
    std::unique_ptr< RadarTargetGenerator > _radarTargetGenerator;
};

int main(int argc, char *argv[])
{
    DataGenerator dataGenerator;

    dataGenerator.connectPolySync();
    return 0;
}
