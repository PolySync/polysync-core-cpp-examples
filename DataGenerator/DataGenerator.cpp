#include <iostream>

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

#include "LidarPointGenerator.hpp"

using namespace polysync::datamodel;

class DataGenerator : public polysync::Node
{
protected:

    virtual void okStateEvent()
    {

    }

    virtual void initStateEvent()
    {

    }

private:

    LidarPointGenerator lidarPointGenerator;
};

int main(int argc, char *argv[])
{

    return 0;
}
