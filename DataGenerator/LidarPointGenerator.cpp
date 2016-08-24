/*
 * Copyright (c) 2016 PolySync
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file LidarPointGenerator.cpp
 * @brief LidarPointGenerator Source
 *
 */

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

    _message.setSensorDescriptor( descriptor );

    auto time = polysync::getTimestamp();

    _message.setHeaderTimestamp( time );

    _message.setStartTimestamp( time );

    _message.setEndTimestamp( time );


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

    for( auto pointNum = 0U; pointNum < _numberOfPoints; ++pointNum )
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

        float w = sin( ( u * _sineFrequency ) + _relativeTime ) *
                  cos( ( v * _sineFrequency ) + _relativeTime ) *
                  0.5;

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
