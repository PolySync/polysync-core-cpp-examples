#include <iostream>

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

#include "LidarPointGenerator.hpp"

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
        polysync::sleepMicro( _updateInterval );
    }

    virtual void initStateEvent()
    {
        _lidarPointGenerator =
                std::unique_ptr< LidarPointGenerator >{
                    new LidarPointGenerator( *this ) };
    }

private:
    ps_timestamp _updateInterval{ 50000 };
    std::unique_ptr< LidarPointGenerator > _lidarPointGenerator;
};

int main(int argc, char *argv[])
{
    DataGenerator dataGenerator;

    dataGenerator.connectPolySync();
    return 0;
}
