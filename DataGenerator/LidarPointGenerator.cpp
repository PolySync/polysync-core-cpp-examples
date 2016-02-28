#include "LidarPointGenerator.hpp"


LidarPointGenerator::LidarPointGenerator( polysync::Node & node )
    :
    _message( node ),
    _numberOfPoints( _gridSideLength * _gridSideLength )
{
    initializeMessage();
}

void LidarPointGenerator::initializeMessage()
{
    polysync::datamodel::SensorDescriptor descriptor;

    descriptor.setId( 11 );
    descriptor.setTransformParentId( PSYNC_COORDINATE_FRAME_LOCAL );
    descriptor.setType( PSYNC_SENSOR_KIND_NOT_AVAILABLE );

    _message.setSensorDescriptor( descriptor );

    auto time = polysync::getTimestamp();
    _message.setHeaderTimestamp( time );
    _message.setStartTimestamp( time );
    _message.setEndTimestamp( time );
}

const polysync::datamodel::LidarPointsMessage & LidarPointGenerator::updatePoints()
{
    for( auto pointNum = 0; pointNum < 10000; ++pointNum )
    {
        polysync::datamodel::LidarPoint point;
        point.setIntensity( 255 );

        auto x = pointNum % 100;
        auto y = pointNum / 100;

        double u = static_cast< double >( x )/ 100.0;
        double v = static_cast< double >( y ) / 100.0;

        // center u/v at origin
        u = ( u * 2.0 ) - 1.0;
        v = ( v * 2.0 ) - 1.0;

        auto w = sin( ( u * _sineFrequency ) + _relativeTime )
                * cos( ( v * _sineFrequency ) + _relativeTime )
                * 0.5;

        point.setPosition( { u * 10, v * 10, w * 10 } );
    }
}
