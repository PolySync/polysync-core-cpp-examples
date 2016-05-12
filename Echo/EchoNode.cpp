#include <fstream>

#include "EchoNode.hpp"
#include "ApplicationInputHandler.hpp"


using namespace std;

namespace polysync
{

void PolySyncEcho::initStateEvent()
{
    if ( inputHandler.singleMessageWasFiltered() )
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
        registerListener( getMessageTypeByName ( inputHandler.getMessageName() ) );
    }
    catch ( ... )
    {
        cout << "\nPlease enter a valid PS message type: \n\n";

        printAvailableMessage( getAvailableMessageNames() );

        disconnectPolySync();
    }
}


void PolySyncEcho::messageEvent( std::shared_ptr< polysync::Message > message )
{    
    if ( inputHandler.fileWasSpecified() )
    {
        printToFile( message );
    }

    echoPolySyncMessagesToStdOut( message );
}


void PolySyncEcho::printToFile( std::shared_ptr < polysync:: Message > message )
{
    ofstream openUserFile;

    openUserFile.open( inputHandler.getFileName(), ios::app );

    if ( inputHandler.headersWereRequested() )
    {
        message->printHeader( openUserFile );
    }

    else if ( !inputHandler.headersWereRequested() )
    {
        message->print( openUserFile );
    }

    openUserFile.close();
}


void PolySyncEcho::echoPolySyncMessagesToStdOut
    ( std::shared_ptr < polysync:: Message > message )
{
    if ( inputHandler.headersWereRequested() )
    {
        message->printHeader();
    }

    else
    {
        message->print();
    }
}


bool PolySyncEcho::optionsParse( const int argc, char * argv[] )
{
    return inputHandler.optionsParse( argc, argv );
}


bool PolySyncEcho::wasHelpRequested( )
{
    return inputHandler.helpWasRequested();
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
