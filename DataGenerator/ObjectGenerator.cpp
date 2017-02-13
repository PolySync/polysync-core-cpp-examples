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
 * @file ObjectGenerator.cpp
 * @brief ObjectGenerator Source
 *
 */

#include "ObjectGenerator.hpp"

ObjectGenerator::ObjectGenerator( polysync::Node & node )
    :
    _message( node )
{
    initializeMessage();
}

void ObjectGenerator::initializeMessage()
{
    polysync::datamodel::SensorDescriptor descriptor;

    descriptor.setId( _sensorID );

    descriptor.setTransformParentId( PSYNC_COORDINATE_FRAME_LOCAL );

    descriptor.setType( PSYNC_SENSOR_KIND_NOT_AVAILABLE );

    _message.setSensorDescriptor( descriptor );

    _message.setHeaderTimestamp( polysync::getTimestamp() );

    resetObjects();

    _message.setObjects( _objectVector );
}

void ObjectGenerator::updateObjects()
{
    auto time = polysync::getTimestamp();

    // for each target
    for( auto index = 0; index < _objectCount; ++index )
    {
        // Get time difference from last update
        auto timeDelta = time - _message.getHeaderTimestamp();

        // Convert to seconds
        auto timeDeltaSeconds = static_cast< double >( timeDelta )/ 1000000.0;

        auto position = _objectVector[ index ].getPosition();

        // update position using p = p_0 + v * dt
        _objectVector[ index ].setPosition(
            { position[ 0 ] + _velocityX * timeDeltaSeconds,
              position[ 1 ] + _velocityY * timeDeltaSeconds,
              0.0 } );

        // set timestamp
        _objectVector[ index ].setTimestamp( time );

        if( _objectVector[ index ].getPosition()[ 0 ] > 30.0 )
        {
            _reset = true;
        }
    }

    // if reset flag set
    if( _reset )
    {
        resetObjects();
    }

    _message.setObjects( _objectVector );

    // set header timestamp
    _message.setHeaderTimestamp(  time );
}

void ObjectGenerator::resetObjects()
{
    _objectVector = {};
    for( auto index = 0; index < _objectCount; ++index )
    {
        polysync::datamodel::Object object;

        object.setPosition( { ( index + 2 ) * 2.0,
                              ( index + 2 ) * 4.0,
                              0.5 } );

        object.setSize( { _objectSizeX,
                          _objectSizeY,
                          _objectSizeZ } );

        object.setCourseAngle( atan2( _velocityY, _velocityX ) );

        object.setVelocity( { _velocityX, _velocityY, 0.0 } );

        object.setClassification( OBJECT_CLASSIFICATION_CAR );

        object.setClassificationQuality( QUALITY_9 );

        _objectVector.emplace_back( object );
    }

    _reset = false;
}

void ObjectGenerator::publishObjects()
{
    _message.setHeaderTimestamp( polysync::getTimestamp() );

    _message.publish();
}
