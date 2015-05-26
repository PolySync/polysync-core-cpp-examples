#include <iostream>
#include <polysync_message.hpp>
#include <polysync_node.hpp>

int main()
{
    polysync::Node node( "LiDAR Publisher" );

    while( ! node.exitSignal() )
    {
        polysync::message::LiDARPointStream pointStream;

        pointStream.setStreamLength( 10 );

        for( auto i = 0.0; i < 10.0; ++i )
        {
            pointStream.streamPushBack
            (
                { LAYER_NONE, ECHO_NONE, POINT_NONE, 0.0, { i, 0.0, 5.0 } }
            );
        }

        polysync::message::publish( pointStream );

        sleep( 1 );
    }

    return 0;
}
