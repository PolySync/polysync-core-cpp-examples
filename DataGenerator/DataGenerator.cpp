#include <iostream>

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

using namespace std;

class DataGenerator : public polysync::Node
{
    virtual void okStateEvent()
    {
        polysync::datamodel::LidarPointsMessage lidarMessage( *this );

        polysync::datamodel::SensorDescriptor descriptor;

        descriptor.setId( 11 );
        descriptor.setTransformParentId( PSYNC_COORDINATE_FRAME_LOCAL );
        descriptor.setType( PSYNC_SENSOR_KIND_NOT_AVAILABLE );

        lidarMessage.setSensorDescriptor( descriptor );

        auto time = polysync::getTimestamp();
        lidarMessage.setHeaderTimestamp( time );

        for( auto pointNum = 0; pointNum < 10000; ++pointNum )
        {
            lidarMessage.setStartTimestamp( time );
            lidarMessage.setEndTimestamp( time );

            polysync::datamodel::LidarPoint point;
            point.setIntensity( 255 );

            auto x = pointNum % 100;
            auto y = pointNum / 100;

            double u = static_cast< double >( x )/ 100.0;
            double v = static_cast< double >( y ) / 100.0;

//            auto w = sin((u * 4.0) + global_relative_time) * cos((v * sine_freq) + global_relative_time) * 0.5;

//            point.setPosition( { u*10, v*10, w*10 } );
        }
    }

    virtual void initStateEvent()
    {

    }
};

int main(int argc, char *argv[])
{

    return 0;
}
