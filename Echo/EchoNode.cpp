#include "EchoNode.hpp"
#include "PolySyncGetOpt.hpp"
#include <fstream>

using namespace std;

namespace polysync
{

void PolySyncEcho::initStateEvent()
{
    if ( getOpt.wasSingleMsgFiltered() )
    {
       registerSingleFilteredMessage();
    }

    else
    {
        registerListenerToAllMessageTypes();
    }
}


void PolySyncEcho::registerSingleFilteredMessage()
{
    try
    {
        registerListener( getMessageTypeByName ( getOpt.getMsgName() ) );
    }

    catch ( ... )
    {
        cout << "\nPlease enter a valid PS message type: \n\n";

        printAvailableMessage( getAvailableMessageNames() );

        // getAvailable is all of the messages.
        // let's add a function for "everything that's on the bus."

        disconnectPolySync();
    }
}


void PolySyncEcho::messageEvent( std::shared_ptr< polysync::Message > message )
{    
    if ( getOpt.wasFileSpecified() )
    {
        printToFile( message );
    }

    echoPolySyncMessagesToStdOut( message );
}


void PolySyncEcho::printToFile( std::shared_ptr < polysync:: Message > message )
{
    ofstream openUserFile;

    openUserFile.open( getOpt.getFileName(), ios::app );

    if ( getOpt.wereHeadersRequested() )
    {
        message->printHeader( openUserFile );
    }

    else if ( !getOpt.wereHeadersRequested() )
    {
        message->print( openUserFile );
    }

      openUserFile.close();
}


void PolySyncEcho::echoPolySyncMessagesToStdOut
    ( std::shared_ptr < polysync:: Message > message )
{
    if ( getOpt.wereHeadersRequested() )
    {
        message->printHeader();
    }

    else if ( !getOpt.wereHeadersRequested() )
    {
        message->print();
    }
}


bool PolySyncEcho::optionsParse( const int argc, char * argv[] )
{
    return getOpt.optionsParse( argc, argv );
}


bool PolySyncEcho::wasHelpRequested( )
{
    return getOpt.wasHelpRequested();
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


void PolySyncEcho::printEchoHelpPage()
{
    echoHelp.printEchoHelp();
}


// END polysync::PolySyncEcho class

} // END namespace polysync
