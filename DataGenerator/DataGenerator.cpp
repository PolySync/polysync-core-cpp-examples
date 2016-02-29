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
        polysync::sleepMicro( 100000 );
    }

    virtual void initStateEvent()
    {
        _lidarPointGenerator =
                std::unique_ptr< LidarPointGenerator >{
                    new LidarPointGenerator( *this ) };
    }

private:
    bool init{ false };
    std::unique_ptr< LidarPointGenerator > _lidarPointGenerator;
};

int main(int argc, char *argv[])
{
    DataGenerator dataGenerator;

    dataGenerator.connectPolySync();
    return 0;
}
