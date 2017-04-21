#include <iostream>

#include <PolySyncApplication.hpp>

#include "LogSessionImport.hpp"


int main( int argc, char * argv[] )
{
    if( argc != 2 )
    {
        std::cerr << "Usage: " << argv[ 0 ] << " [sessionId]" << std::endl;
        return -1;
    }

    SessionImportExample importExample{ argv[ 1 ] };

    auto application = polysync::Application::getInstance();

    application->setNodeName( "polysync-log-session-import-cpp" );

    application->connectPolySync();
}
