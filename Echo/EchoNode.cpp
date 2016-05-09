#include "EchoNode.hpp"
#include "PolySyncGetOpt.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;


namespace polysync
{

PolySyncGetOpt getOpt;

void PolySyncEcho::initStateEvent()
{

    // void PolySyncEcho::singleMsgFilterRegister()
    if ( _filteredForSingleMsgFlag )
    {
        try
        {
            registerListener( getMessageTypeByName ( _msgName ) );
            // registerListener( getMessageTypeByName ( getOpt.getMsgName() ) );
        }

        catch ( ... )
        {
            cout << "\nPlease enter a valid PS message type. \n\n"
                 << "For help + usage guide: $ polysync-echo -h \n\n";

            printAvailableMessage( getAvailableMessageNames() );

            disconnectPolySync();
        }
    }

    else
    {
        registerListenerToAllMessageTypes();
    }
    // end PolySyncEcho::singleMsgFilterRegister()
}


void PolySyncEcho::messageEvent( std::shared_ptr< polysync::Message > message )
{    
    if ( _echoMessageToFileFlag )
    {
        printMsgToFile( message );
    }

    if ( _echoMessageHeadersOnlyFlag )
    {
        message->printHeader();
    }

    else if ( !_echoMessageHeadersOnlyFlag )
    {
        message->print();
    }
}


void PolySyncEcho::printMsgToFile( std::shared_ptr<polysync::Message> message )
{
    ofstream openUserFile;

    openUserFile.open( _userFileName, ios::app );

    if ( _echoMessageHeadersOnlyFlag )
    {
        message->printHeader( openUserFile );
    }

    else if ( !_echoMessageHeadersOnlyFlag )
    {
        message->print( openUserFile );
    }

      openUserFile.close();
}

// class GetOpt::getOptIdx

int PolySyncEcho::getOptIdx( const char optret )
{
    if( std::find( std::begin( _optChars ), std::end( _optChars ), optret )
            != std::end( _optChars ) )
    {
         // success: container end not reached.
    }
    else
    {
        return -1;
    }

}

/*

class GetOpt::
needs member variables / persistent data:

    bool _filteredForSingleMsgFlag
    bool _echoMessageHeadersOnlyFlag
    bool _echoMessageToFileFlag
    bool _echoHelpFlag

char * _msgName
char * _userFileName;

// use getters inside of the Echo class:
char * GetOpt::getMsgName() { return _msgName; }
char * GetOpt::getUserFileName() { return _userFileName; }

*/

bool PolySyncEcho::optionsParse( const int argc, char * argv[] )
{
    bool parsedOptSuccess = true;

    int optNeedsArgCount = 0;

    int option_idx = -1;
    int optret = 0;

    // reset scanner
    optind = 0;

    int fFlag = 0;
    int oFlag = 0;
    int hFlag = 0;
    int HFlag = 0;
    int idx;

    opterr = 0;

    while ((optret = getopt( argc, argv, "o:f:hH")) != -1)
    {
        option_idx = getOptIdx( (const char) optret );

        if ( option_idx == -1 )
        {
            cout << "\n\nUsage: invalid options. Usage guide follows." <<endl;

            _echoHelpFlag = true;
        }

         else if ( option_idx != -1 )
        {
            switch (optret)
            {

            case 'f':

                fFlag = 1;

                _msgName = optarg;

                parsedOptSuccess = true;

                break;

            case 'o':

                if ( (*argv[ optind - 1 ])
                     && (*argv[ optind - 1 ] == '-') )
                {
                    cout <<"\n\nInvalid usage for option -o external file:\n"
                         <<"-o should be followed by a filename yourfile.txt,\n"
                         << "and not by another -option.\n"
                         <<"A usage guide follows." << endl;

                    _echoHelpFlag = true;
                }

                else
                {
                    oFlag = 1;

                    _userFileName = optarg;

                    parsedOptSuccess = true;
                }
                break;

            case 'h':

                hFlag = 1;

                _echoHelpFlag = true;

                parsedOptSuccess = true;

                break;

            case 'H':

                HFlag = 1;

                _echoMessageHeadersOnlyFlag = true;

                parsedOptSuccess = true;

                break;

            case '?':

                if ( optopt == 'f' )
                {
                    parsedOptSuccess = false;

                    _echoHelpFlag = true;

                    fFlag = 0;

                    ++optNeedsArgCount;

                    cout << "\n\n optNeedsArgCount = " << optNeedsArgCount <<endl;

                    cout<<"\n\nUsage: option -f requires an argument." <<endl;
                    cout << "Usage: include message type after filter option -f."
                         << endl <<endl
                         << "Usage example:" <<endl
                         << "$ polysync-echo -f ps_diagnostic_trace_msg"
                         <<endl <<endl;
                }

                else if ( optopt == 'o' )
                {
                    parsedOptSuccess = false;

                    _echoHelpFlag = true;

                    oFlag = 0;

                    ++optNeedsArgCount;

                    cout << "\n\n optNeedsArgCount = " << optNeedsArgCount <<endl;

                    cout << "\n\nUsage: option -o requires an argument." <<endl
                         << "Usage: include a file name after file option -o."
                         << endl <<endl
                         <<"Usage example: print all msg types to file:" <<endl
                         << "$ polysync-echo -o yourFileName.txt"
                         <<endl <<endl
                         << "Usage example: print one msg type headers' to file:\n"
                         << "$ polysync-echo -f ps_diagnostic_trace_msg "
                         << "-H -o yourFileName.txt"
                         <<endl <<endl;
                }

            default:
                parsedOptSuccess = true;

            }
        }
    }


    for ( idx = optind; idx < argc; ++idx )
    {
        cout << "\n\nUsage: Non option argument: " << argv[ idx ] <<endl
             << "A usage guide follows." <<endl;

        _echoHelpFlag = true;
    }

    if ( fFlag == 1 )
    {
        _filteredForSingleMsgFlag = true;

        parsedOptSuccess = true;
    }

    if ( oFlag == 1 )
    {
        _echoMessageToFileFlag = true;

        parsedOptSuccess = true;
    }

    return parsedOptSuccess;
}


// GetOpt::
bool PolySyncEcho::wasSingleMsgFiltered()
{
    return _filteredForSingleMsgFlag;
}


// GetOpt::
bool PolySyncEcho::wereHeadersRequested()
{
    return _echoMessageHeadersOnlyFlag;
}


// GetOpt::
bool PolySyncEcho::wasFileSpecified()
{
    return _echoMessageToFileFlag;
}


//  GetOpt::
bool PolySyncEcho::wasHelpRequested( )
{
    //return _echoHelpFlag;
    return getOpt.wasHelpRequested();
}


// PolySyncEcho:: OR PolySyncHelp::
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


//  PolySyncHelp::
void PolySyncEcho::printAvailableMessage( const std::vector< std::string > & messageTypeStrings )
{
    for( auto messageTypeString : messageTypeStrings )
    {
        cout << "    " << messageTypeString << endl;
    }
}

//  PolySyncHelp::
std::vector< std::string > PolySyncEcho::getHelpFlags()
{
    std::vector< std::string > cmdLineFlagsHelp;

        cmdLineFlagsHelp.emplace_back ( "-h" );
        cmdLineFlagsHelp.emplace_back ( "-f <MESSAGE_TYPE>" );
        cmdLineFlagsHelp.emplace_back ( "-H" );
        cmdLineFlagsHelp.emplace_back ( "-o <FILE_NAME>" );

    return cmdLineFlagsHelp;
}

//  PolySyncHelp::

std::vector< std::string > PolySyncEcho::getHelpDescriptions()
{
    std::vector< std::string > flagDescriptionsHelp;

    flagDescriptionsHelp.emplace_back
        ( " Show this help message [optional]." );

    flagDescriptionsHelp.emplace_back
        ( " Filter for a single message type [optional]. \n"
        " Usage Example: $ polysync-echo -f ps_diagnostic_trace_msg" );

    flagDescriptionsHelp.emplace_back
        ( " Echo only message headers [optional]. \n"
        " Usage Example 1: echo headers for all message types: \n"
        " $ polysync-echo -H \n"
        " Usage Example 2: echo headers for single message type: \n"
        " $ polysync-echo -f ps_lidar_points_msg -H" );

    flagDescriptionsHelp.emplace_back
        ( " Specify an external output file for printed message data, \n"
        " in addition to standard output [optional]. \n"
        " Usage Example: print only headers for single message type to file: \n"
        " $ polysync-echo -f ps_lidar_points_msg -H \n"
        " -o yourFileName.txt \n\n"
        " Note: Unless you specify a new file each time, each session will \n"
        " append to the end of your specified file.");

    return flagDescriptionsHelp;
}


//  PolySyncHelp::

void PolySyncEcho::printHelp(const std::vector< std::string > & helpFlags ,
                             const std::vector< std::string> & helpDescriptions)
{
    /* previously in messageEvent function:
    // void PolySyncEcho::printHelp()

    if ( _echoHelpFlag )
    {
        disconnectPolySync();

        printHelp( getHelpFlags() , getHelpDescriptions() );
    }

    // end void PolySyncEcho::printHelp()
    */

    cout << "\n\nPolySync Echo \n";
    cout << "Standard behavior (no options) is to echo all messages \n";
    cout << " currently on the bus. \n\n";

    cout << "Usage: \n $polysync-echo [options] \n\n";

    for( auto index = 0; index < 3; ++index )
    {
        cout  << helpFlags[ index ] <<endl;
        cout  << helpDescriptions[ index ] <<endl <<endl;
    }
}


// END polysync::PolySyncEcho class

} // END namespace polysync
