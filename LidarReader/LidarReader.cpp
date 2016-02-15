#include <iostream>
#include "LidarReader.hpp"



LidarReader::LidarReader()
    : polysync::Node( "LidarReader" )
{
    buildTypeList();
    registerListeners();

    printTypes();
}



void LidarReader::buildTypeList()
{
    ulong messageCount = 0;
    polysync::message::getCount( reference(), messageCount );

    // register backend to listen for all available types
    for( auto i = 1; i < messageCount+1; ++i )
    {
        int ret = DTC_NONE;
        string psMessageName;
        if( ( ret = polysync::message::getNameByType( reference(),
                                                      i,
                                                      psMessageName,
                                                      128 ) )
        != DTC_NONE )
        {
            polysync::logMessage( LOG_LEVEL_ERROR,
            { "LidarReader failed to register listener: ",
              to_string( i ) } );
        }
        else
        {
            _typeNameMap.insert( { psMessageName, i } );
        }
    }
}



void LidarReader::registerListeners()
{
    // you can filter for specific message types here
    // this example only listens for LiDAR point messages

    cout << endl << "Registering callback to: " << endl;
    for( auto type : _typeNameMap )
    {
        if( type.first == "ps_lidar_points_msg" )
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
                { "Failed to register listener: ",
                  std::to_string( type.second ) } );
            }
        }
    }
}

void LidarReader::printTypes()
{
    // print the loaded PolySync Data Model message names and their associated value
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



template< typename T >
void LidarReader::filterLidarPoints( polysync::Message *message )
{

    // safe cast
    if( auto castedMessage = static_cast< T* >( message ) )
    {

        // class definition:
        // file:///usr/local/polysync/doc/cpp_api_docs/html/d0/d17/classpolysync_1_1_lidar_points_message.html
        std::vector< ps_lidar_point > lidarPoints = castedMessage->getPoints();

        // iterate through our local vector of ps_lidar_pts
        for( auto point : lidarPoints )
        {
            // intensity : [0:255]
            //      0   : invalid or unknown
            //      1   : lowest intensity
            //      255 : highest intensity

            // ignore points of unknown and not available
            // filter points with intsity lower than an arbitrary number for now
            if( point.intensity >= 50 )
            {
                cout << "Intensity: " << static_cast< short > (point.intensity) << endl;
                // print the xyz position (in reference to the vehicle) of each filtered point
                cout << "Point :" << endl;
                cout << '\t' << "X: " << static_cast< short > (point.position[0]) << endl;
                cout << '\t' << "Y: " << static_cast< short > (point.position[1]) << endl;
                cout << '\t' << "Z: " << static_cast< short > (point.position[2]) << endl;
            }
        }
    }
}



void LidarReader::execute()
{
    while( 1 )
    {
        // get a message from the queue
        auto message = queuePop();

        // validate message
        if( message )
        {
            // access and filter lidar sensor data
            filterLidarPoints < polysync::LidarPointsMessage >( message.get() );
        }

        // sleep for 1/10 of a second (default)
        polysync::sleepMicro( _sleepInterval );
    }
}
