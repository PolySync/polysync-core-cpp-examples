#ifndef OCCUPANCYGRID_H
#define OCCUPANCYGRID_H

#include <polysync_node.hpp>
#include <array>

#define ESR_SN 59138
#define OCCUPIED_MINIMUM 5U

class OccupancyGrid : public polysync::Node
{
public:

    OccupancyGrid();

    void exec();

    void handleMessage( void* msg );
    void gotTracks( const polysync::message::RadarTrackStream& msg );
    void gotPoints( const polysync::message::LiDARPointStream& msg );
    void updateGrid( double* position );
    void addToGrid( int x, int y, int z );
    void resetFlags();
    void publishGrid();

    bool filter( ps_msg_type type, void *msg ) override;

private:

    std::multimap< int, std::multimap< int, std::pair< int, int > > > grid;
    bool _gotTracks = false;
    bool _gotPoints = false;
};

#endif // OCCUPANCYGRID_H
