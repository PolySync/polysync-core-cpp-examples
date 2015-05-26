#include <iostream>
#include <polysync_message.hpp>
#include <polysync_node.hpp>

int main()
{
    polysync::Node node( "LiDAR Listener" );
    node.registerListener( MSG_TYPE_LIDAR_POINT_STREAM );

    while( ! node.exitSignal() )
    {
        polysync::message::LiDARPointStream message( node.queuePopSafe() );

        if( message.hasData() )
        {
            message.print();
            message.printDataStream( 2 );
        }
    }

    return 0;
}
