#include "EchoNode.hpp"
#include <iostream>
#include <fstream>


using namespace std;


namespace polysync
{


void PolySyncEcho::initStateEvent()
{
    if ( _filteredForSingleMsgFlag )
    {
        try
        {
            registerListener( getMessageTypeByName ( _msgName ) );
        }

        catch ( ... )
        {
            std::cout << "\nPlease enter a valid PS message type. \n";

            printAvailableMessage( getAvailableMessageNames() );

            disconnectPolySync();
        }
    }

    else
    {
        registerListenerToAllMessageTypes();
    }
}


void PolySyncEcho::setConfigurationEvent( int argc, char * argv [] )
{
    std::string filterFlag = "-filterMessage";
    std::string echoHeadersOnly = "-echoHeaders";
    std::string echoToFile = "-toFile";
    std::string echoHelp = "-h";

    if( argc >= 2 )
    {
        for( auto idx = 1; idx < argc ; ++idx )
        {
            if( argv[ idx ] == filterFlag
                  && ( argv[ idx + 1 ] ) )
            {
                _filteredForSingleMsgFlag = true;

               _msgName = argv[ idx + 1 ];
            }

            else if( argv[ idx ] == filterFlag
                  && ( ! argv[ idx + 1 ] ) )
            {
                cout << "\n\nPlease include message type after filter option."
                     << endl
                     << "Here is an example: \n"
                     << "$ polysync-echo -filterMessage ps_diagnostic_trace_msg"
                     <<endl;
                disconnectPolySync();
            }

            if ( argv[ idx ] == echoHeadersOnly )
            {
                _echoMessageHeadersOnly = true;
            }

            if ( argv[ idx ] == echoToFile
                 && ! argv[ idx + 1 ] )
            {
                cout << "\n\nPlease include file type after file option."
                     << endl
                     <<"Here is an example: print all msg types to file: \n"
                     << "$ polysync-echo -toFile yourFileName.txt"
                     <<endl <<endl
                     << "Another example: print one msg type's headers' to file: \n"
                     << "$ polysync-echo -filterMessage ps_diagnostic_trace_msg "
                     << "-echoHeaders -toFile yourFileName.txt"
                     <<endl <<endl;
                disconnectPolySync();
            }

            else if ( argv[ idx ] == echoToFile
                 &&  argv[ idx + 1 ] )
            {
                 _echoMessageToFile = true;

                 _userFileName = argv [ idx + 1 ];
            }

            if ( argv[ idx ] == echoHelp )
            {
                _echoHelp = true;
            }
        }
    }
}


void PolySyncEcho::messageEvent( std::shared_ptr< polysync::Message > message )
{
    if ( _echoMessageToFile )
    {
        streambuf *fileStreambuf, *coutStreambuf;

        ofstream outputOfstream;

        outputOfstream.open( _userFileName, ios::app );

        coutStreambuf = cout.rdbuf();

        fileStreambuf = outputOfstream.rdbuf();

        cout.rdbuf( fileStreambuf );

        if ( _echoMessageHeadersOnly )
        {
            message->printHeader();
        }

        else
        {
            message->print();
        }

        cout.rdbuf( coutStreambuf );

        outputOfstream.close();
    }

    if ( _echoMessageHeadersOnly && !_echoHelp )
    {
        message->printHeader();
    }

    else if ( !_echoMessageHeadersOnly && !_echoHelp )
    {
        message->print();
    }

    else if ( _echoHelp )
    {
        disconnectPolySync();

        printHelp( getHelpFlags() , getHelpDescriptions() );
    }
}


std::vector< std::string > PolySyncEcho::getAvailableMessageNames()
{
    std::vector< std::string > messageNames;

    for( auto index = 1;
         index < getAvailableMessageCount() + 1;
         ++index )
    {
        messageNames.emplace_back( getMessageNameByType( index ) );
    }
    return messageNames;
}


void PolySyncEcho::printAvailableMessage( const std::vector< std::string > & messageTypeStrings )
{
    for( auto messageTypeString : messageTypeStrings )
    {
        cout << "    " << messageTypeString << endl;
    }
}


std::vector< std::string > PolySyncEcho::getHelpFlags()
{
    std::vector< std::string > cmdLineFlagsHelp;

        cmdLineFlagsHelp.emplace_back ( "-h" );
        cmdLineFlagsHelp.emplace_back ( "-filterMessage messageType" );
        cmdLineFlagsHelp.emplace_back ( "-echoHeaders" );
        cmdLineFlagsHelp.emplace_back ( "-toFile yourFileName.txt" );

    return cmdLineFlagsHelp;
}


std::vector< std::string > PolySyncEcho::getHelpDescriptions()
{
    std::vector< std::string > flagDescriptionsHelp;

    flagDescriptionsHelp.emplace_back
        ( "Show this help message [optional]." );

    flagDescriptionsHelp.emplace_back
        ( "Filter for a single message type [optional]. \n"
        "  Example: $ polysync-echo -filterMessage ps_diagnostic_trace_msg" );

    flagDescriptionsHelp.emplace_back
        ( "Echo only message headers [optional]. \n"
        "  Example 1: echo headers for all message types: \n"
        "  $ polysync-echo -echoHeaders \n"
        "  Example 2: echo headers for single message types: \n"
        "  $ polysync-echo -echoHeaders -filterMessage ps_lidar_points_msg" );

    flagDescriptionsHelp.emplace_back
        ( "Specify an output file for printed message data [optional], \n"
        "  in addition to standard output. \n"
        "  Example: print only headers for single message types to file: \n"
        "  $ polysync-echo -echoHeaders -filterMessage ps_lidar_points_msg \n"
        "  -toFile yourFileName.txt \n\n"
        "  Note that file prints to this location of current directory \n"
        "  unless otherwise specified, as such: \n"
        "  Example: store your file in one directory up: \n"
        "  $ polysync-echo -toFile ../-yourFile.txt \n\n"
        "  Unless you specify a new file each time, each session will \n"
        "  append to the end of your specified file.");

    return flagDescriptionsHelp;
}


void PolySyncEcho::printHelp(const std::vector< std::string > & helpFlags ,
                             const std::vector< std::string> & helpDescriptions)
{
    cout << "\n\nPolySync Echo \n";
    cout <<        "Standard behavior (no options) is to echo all messages \n";
    cout <<        "currently on the bus. \n\n";

    cout << "usage: \n $polysync-echo [options] \n\n";

    for( auto index = 0; index < 4; ++index )
    {
        cout  << helpFlags[ index ] <<endl;
        cout  << helpDescriptions[ index ] <<endl <<endl;
    }
}


// END polysync::PolySyncEcho class

} // END namespace polysync
