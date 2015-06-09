#include <iostream>
#include <polysync_node.hpp>
#include <polysync_message.hpp>

//Check a message to see if it came from an ESR
bool isESR( const polysync::message::ParameterStream& msg )
{
    //Pull our data stream
    std::vector< polysync::Parameter > dataStream = msg.dataStreamVector();

    //For each item in the stream
    for( auto i : dataStream )
    {
        //If the item is a sensor type
        if( i.id == polysync::parameter::ID_SENSOR_TYPE )
        {
            //Check the value
            if( i.value._u.ull_value == PSYNC_SENSOR_TYPE_DELPHI_ESR )
            {
                return true;
            }
        }
    }

    //Not an ESR
    return false;
}

int main()
{
    polysync::Node node( "PolySync Get Set" );
    node.registerListener( MSG_TYPE_PARAMETER_STREAM );

    //Get all available parameters from all nodes on bus
    polysync::parameter::getAll();

    //Used for our parameter value
    auto idx = 0.0;

    while( ! node.exitSignal() )
    {
        //Get ID_SENSOR_TYPE parameter from all nodes on bus
        polysync::parameter::get( polysync::parameter::ID_SENSOR_TYPE );

        //Pull a message from listener queue
        polysync::message::ParameterStream msg{ node.queuePopSafe() };

        //If the message has data
        if( msg.hasData() )
        {
            //Print message header and data members, if any
            msg.print();

            //Print data from each item in the data stream
            msg.printDataStream( msg.streamLength() );

            //If the message came from an ESR sensor
            if( isESR( msg ) )
            {
                //Adjust this sensor's Mount position x to idx*5.0
                polysync::parameter::set( msg.srcGuid(), polysync::parameter::ID_SENSOR_MOUNT_POSITION_X, ++idx*5.0 );
            }
        }

        //Reset index
        if( idx > 4.0 )
        {
            idx = 0.0;
        }

        //Wait for a second before we repeat
        sleep( 1 );
    }

    return 0;
}

