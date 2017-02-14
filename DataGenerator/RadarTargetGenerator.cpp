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
 * @file RadarTargetGenerator.cpp
 * @brief RadarTargetGenerator Source
 *
 */

#include "RadarTargetGenerator.hpp"

RadarTargetGenerator::RadarTargetGenerator( polysync::Node & node )
    :
    _message( node )
{
    initializeMessage();
}

void RadarTargetGenerator::initializeMessage()
{
    polysync::datamodel::SensorDescriptor descriptor;

    descriptor.setId( _sensorID );

    descriptor.setTransformParentId( PSYNC_COORDINATE_FRAME_LOCAL );

    descriptor.setType( PSYNC_SENSOR_KIND_NOT_AVAILABLE );

    _message.setSensorDescriptor( descriptor );

    _message.setHeaderTimestamp( polysync::getTimestamp() );

    resetTargets();

    _message.setTargets( _targetVector );
}

void RadarTargetGenerator::updateTargets()
{
    auto time = polysync::getTimestamp();

    // for each target
    for( auto index = 0; index < _targetsCount; ++index )
    {
        // Get time difference from last update
        auto timeDelta = time - _message.getHeaderTimestamp();

        // Convert to seconds
        auto timeDeltaSeconds = static_cast< double >( timeDelta )/ 1000000.0;

        auto position = _targetVector[ index ].getPosition();

        // update position using p = p_0 + v * dt
        _targetVector[ index ].setPosition(
            { position[ 0 ] + _velocityX * timeDeltaSeconds,
              position[ 1 ] + _velocityY * timeDeltaSeconds,
              0.0 } );

        // set timestamp
        _targetVector[ index ].setTimestamp( time );

        if( _targetVector[ index ].getPosition()[ 0 ] > 30.0 )
        {
            _reset = true;
        }
    }

    // if reset flag set
    if( _reset )
    {
        resetTargets();
    }

    _message.setTargets( _targetVector );

    // set header timestamp
    _message.setHeaderTimestamp(  time );
}

void RadarTargetGenerator::resetTargets()
{
    _targetVector = {};
    for( auto index = 0; index < _targetsCount; ++index )
    {
        polysync::datamodel::RadarTarget target;

        target.setSize( { PSYNC_SIZE_NOT_AVAILABLE,
                          PSYNC_SIZE_NOT_AVAILABLE,
                          PSYNC_SIZE_NOT_AVAILABLE } );

        target.setRangeType( RANGE_NOT_AVAILABLE );

        target.setZoneType( ZONE_NOT_AVAILABLE );

        target.setQuality( QUALITY_NOT_AVAILABLE );

        target.setMagnitude( PSYNC_MAGNITUDE_NOT_AVAILABLE );

        target.setAlias( PSYNC_VELOCITY_ALIAS_NOT_AVAILABLE );

        target.setCrossSection( PSYNC_RADAR_CROSS_SECTION_NOT_AVAILABLE );

        target.setScanIndex( 0 );

        target.setId( index + 1 );

        target.setTrackStatus( TRACK_STATUS_ACTIVE );

        target.setTimestamp( polysync::getTimestamp() );

        target.setAmplitude( _amplitude );

        target.setPosition( { ( index + 2 ) * 2.0,
                              ( index + 2 ) * 4.0,
                              0.5 } );

        target.setVelocity( { _velocityX, _velocityY, 0.0 } );

        _targetVector.emplace_back( target );
    }

    _reset = false;
}

void RadarTargetGenerator::publishTargets()
{
    _message.setHeaderTimestamp( polysync::getTimestamp() );

    _message.publish();
}
