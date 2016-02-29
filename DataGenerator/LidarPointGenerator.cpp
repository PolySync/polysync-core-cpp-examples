#include "LidarPointGenerator.hpp"

using namespace std;
using namespace polysync::datamodel;

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

    _message.setEndTimestamp( 234 );
    _message.setSensorDescriptor( descriptor );

    auto time = polysync::getTimestamp();
    _message.setHeaderTimestamp( time );
    _message.setStartTimestamp( time );

    updatePoints();
}

void LidarPointGenerator::updatePoints()
{
    auto time = polysync::getTimestamp();

    auto timeDelta = time - _message.getStartTimestamp();

    auto timeDeltaSeconds = static_cast< float >( timeDelta ) / 1000000.0;

    _relativeTime += timeDeltaSeconds;

    _message.setStartTimestamp( time );
    _message.setEndTimestamp( time );

    std::vector< LidarPoint > outputPoints;
    outputPoints.reserve( _numberOfPoints );

    for( auto pointNum = 0; pointNum < _numberOfPoints; ++pointNum )
    {
        polysync::datamodel::LidarPoint point;
        point.setIntensity( 255 );

        auto x = pointNum % 100;
        auto y = pointNum / 100;

        float u = static_cast< float >( x )/ 100.0;
        float v = static_cast< float >( y ) / 100.0;

        // center u/v at origin
        u = ( u * 2.0 ) - 1.0;
        v = ( v * 2.0 ) - 1.0;

        float w = sin( ( u * _sineFrequency ) + _relativeTime )
                * cos( ( v * _sineFrequency ) + _relativeTime )
                * 0.5;

        point.setPosition( { u * 10, v * 10, w * 10 } );
        outputPoints.emplace_back( point );
    }

    _message.setPoints( outputPoints );
}

void LidarPointGenerator::publishPoints()
{
    _message.setHeaderTimestamp( polysync::getTimestamp() );
    _message.publish();
}
