#include <iostream>
#include "Reader.hpp"

Reader::Reader()
    :
      polysync::Node( "Example Reader" )
{
    buildTypeList();
    registerListeners();

    printTypes();
}

void Reader::buildTypeList()
{
    ulong count = 0;
    polysync::message::getCount( reference(), count );

    // register backend to listen for all available types
    for( auto i = 1; i < count+1; ++i )
    {
        int ret = DTC_NONE;
        string name;
        if( ( ret = polysync::message::getNameByType( reference(),
                                                      i,
                                                      name,
                                                      128 ) )
        != DTC_NONE )
        {
            polysync::logMessage( LOG_LEVEL_ERROR,
            { "Example Reader failed to register listener: ",
              to_string( i ) } );
        }
        else
        {
            _typeNameMap.insert( { name, i } );
        }


    }
}

void Reader::registerListeners()
{
    cout << endl << "Registering Callback To: " << endl;
    for( auto type : _typeNameMap )
    {
        if( type.first == "ps_objects_msg" or
            type.first == "ps_radar_targets_msg" or
            type.first == "ps_lidar_points_msg" )
        {
            cout << '\t' << type.first << endl;
            int ret = DTC_NONE;
            if( ( ret = polysync::message::registerListener( reference(),
                                                             type.second,
                                                             polysyncCallback,
                                                             this ) )
            != DTC_NONE )
            {
                polysync::logMessage( LOG_LEVEL_ERROR,
                { "Example Reader failed to register listener: ",
                  std::to_string( type.second ) } );
            }
        }
    }
}

void Reader::printTypes()
{
    cout << endl << "Existing Data Model Types: " << endl;
    for( auto type : _typeNameMap )
    {
        cout << "\t"
             << "Name: " << type.first
             << '\t'
             << "Value: " << type.second << endl;
    }
    cout << endl;
}

void Reader::printMessage( polysync::Message *message )
{
    cout << endl;
    if( message->messageType() ==
            _typeNameMap.at( "ps_objects_msg" ) )
    {
        printSensorData< polysync::ObjectsMessage >(
            message, "ps_objects_msg" );
    }
    else if( message->messageType() ==
             _typeNameMap.at( "ps_radar_targets_msg" ) )
    {
        printSensorData< polysync::RadarTargetsMessage >(
            message, "ps_radar_targets_msg" );
    }
    else if( message->messageType() ==
             _typeNameMap.at( "ps_lidar_points_msg" ) )
    {
        printSensorData< polysync::LidarPointsMessage >(
            message, "ps_lidar_points_msg" );
    }
}

template< typename T >
void Reader::printSensorData( polysync::Message *message, string typeName )
{
    // Safe cast, if it fails. Our class is not a sub-class os polysync::Message
    if( auto castedMessage = dynamic_cast< T* >( message ) )
    {
        auto sensorDescriptor = castedMessage->getSensorDescriptor();

        cout << typeName << endl;
        cout << '\t' << "Sensor ID: " << sensorDescriptor.id << endl;
        cout << '\t' << "Sensor Type: " << sensorDescriptor.type << endl;
    }
}

void Reader::execute()
{
    while( 1 )
    {
        auto message = queuePop();

        if( message )
        {
            printMessage( message.get() );
        }

        // Sleep for 1/10 of a second (Default)
        polysync::sleepMicro( _sleepInterval );
    }
}
