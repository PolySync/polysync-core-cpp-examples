#include <utility>
#include <cmath>

#include "occupancygrid.h"
#include <polysync_core.h>

OccupancyGrid::OccupancyGrid() : Node( "Occupancy Grid" )
{
    registerListener( MSG_TYPE_RADAR_TRACK_STREAM );
    registerListener( MSG_TYPE_LIDAR_POINT_STREAM );
}

void OccupancyGrid::exec()
{
    while( ! exitSignal() )
    {
        if( _gotPoints && _gotTracks)
        {
            publishGrid();
        }

        void* msg = queuePopSafe( true );

        if( msg != nullptr )
        {
            handleMessage( msg );
            usleep( 1000 );
        }
    }
}

void OccupancyGrid::handleMessage( void* msg )
{
    ps_msg_type type = polysync::message::getType( msg );

    if( type == MSG_TYPE_RADAR_TRACK_STREAM )
    {
        gotTracks( polysync::message::RadarTrackStream( msg ) );
    }
    else if( type == MSG_TYPE_LIDAR_POINT_STREAM )
    {
        gotPoints( polysync::message::LiDARPointStream( msg ) );
    }
}

void OccupancyGrid::gotTracks( const polysync::message::RadarTrackStream& msg )
{
    _gotTracks = true;
    std::vector< ps_radar_track > vector = msg.dataStreamVector();

    for( auto track : vector )
    {
        updateGrid( track.pos );
    }
}

void OccupancyGrid::gotPoints( const polysync::message::LiDARPointStream& msg )
{
    _gotPoints = true;
    std::vector< ps_lidar_point > vector = msg.dataStreamVector();

    for( auto point : vector )
    {
        updateGrid( point.pos );
    }
}

void OccupancyGrid::updateGrid( double* position )
{
    int x = static_cast< int>( std::floor( position[ 0 ] ) );
    int y = static_cast< int>( std::floor( position[ 1 ] ) );
    int z = static_cast< int>( std::floor( position[ 2 ] ) );

    auto xRange = grid.equal_range( x );

    for( auto elementX = xRange.first; elementX != xRange.second; ++elementX )
    {
        auto yRange = elementX->second.equal_range( y );

        for( auto elementY = yRange.first; elementY != yRange.second; ++elementY )
        {
            if( elementY->second.first == z )
            {
                ++elementY->second.second;
                return;
            }
        }
    }

    addToGrid( x, y, z );
}

void OccupancyGrid::addToGrid( int x, int y, int z )
{
    std::multimap< int, std::pair< int, int > > data;
    data.emplace( std::make_pair( y, std::make_pair( z, 1 ) ) );
    grid.emplace( std::make_pair( x, data ) );
}

void OccupancyGrid::publishGrid()
{
    polysync::message::ObjectStream objects;
    objects.setStreamLength( grid.size() );

    ps_object_descriptor object;
    object.classification_type = CLASSIFICATION_UNKNOWN;
    object.size[ 0 ] = 1.0;
    object.size[ 1 ] = 1.0;
    object.size[ 2 ] = 1.0;

    int index = 0;
    for( auto elementX : grid )
    {
        for( auto elementY : elementX.second )
        {
            if( elementY.second.second > OCCUPIED_MINIMUM )
            {
                object.id = index++;
                object.pos[ 0 ] = elementX.first;
                object.pos[ 1 ] = elementY.first;
                object.pos[ 2 ] = elementY.second.first;
            }
            objects.streamPushBack( object );
        }
    }

    polysync::message::publish( objects );

    grid.clear();
    resetFlags();
}

void OccupancyGrid::resetFlags()
{
    _gotPoints = !_gotPoints;
    _gotTracks = !_gotTracks;
}

bool OccupancyGrid::filter( ps_msg_type type, void* msg )
{
    if( type == MSG_TYPE_RADAR_TRACK_STREAM )
    {
        polysync::message::RadarTrackStream track( msg, false );
        if( track.serialNumber() == ESR_SN )
        {
            return true;
        }
    }
    else
    {
        return true;
    }

    return false;
}
