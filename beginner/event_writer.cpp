#include <iostream>

#include <polysync_node.hpp>
#include <polysync_message.hpp>

using namespace std;

int main()
{
    polysync::Node node( "Event Publisher" );

    int id = 500;
    bool flag = true;
    while ( ! node.exitSignal() )
    {
        double value = 0;

        if( flag )
        {
            value = 1.0;
        }

        if( id == 504 )
        {
            flag = !flag;
            id = 500;
        }

        polysync::message::Event msg;
        msg.setId( id++ );
        msg.setValue( value );

        //publish to the bus
        polysync::message::publish( msg );

        //no need to print too often in this case
        //let this process sleep for one second
        sleep( 1 );
    }

    return 0;
}

