/*
 * Copyright (c) 2015 HARBRICK TECHNOLOGIES, INC
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * \example LogSessionExport.cpp
 *
 * Log session export example.
 *
 * Demonstrates how to use the PolySync log session transfer API to export a previously
 * recorded log session from a new distributed system.
 *
 */

#include <iostream>
#include <PolySyncCore.hpp>
#include <PolySyncDataModel.hpp>
#include <PolySyncApplication.hpp>
#include <PolySyncLogSessionTransfer.hpp>

using namespace std;
using namespace polysync;
using namespace datamodel;

/**
 * @brief SessionExportExample class
 */
class SessionExportExample : public DataSubscriber
{

public:

    SessionExportExample(
            int sessionId )
        :
        _sessionId( sessionId )
    {
        // Subscribe to ApplicationEventMessage to determine when
        // the application connects to the PolySync bus.
        connectSubscriberMethod< SessionExportExample >(
                ApplicationEventMessage::getName(),
                &SessionExportExample::handleEvent,
                this );

        _application = Application::getInstance();

        _application->attachSubscriber( this );
    }

private:

    void handleEvent( shared_ptr< Message > message )
    {
        if( auto event = getSubclass< ApplicationEventMessage >( message ) )
        {
            if( event->getEventKind() == EventKind::Init )
            {
                // This is the actual usage of the export API.
                _exporter = new LogSessionExport( _sessionId );

                // Export can be started with or without a progress callback.
                // When complete, results will be located at "PSYNC_HOME/rnr_logs/export/[sessionId]"
                _exporter->start(
                        this,
                        &SessionExportExample::handleTransferStatus );
            }
        }
    }

    void handleTransferStatus(
            const LogSessionTransferStatus & status )
    {
        // Status has other information available as well
        auto state = status.getState();

        cout << "State: ";

        switch( state )
        {
            case LogSessionTransferState::Invalid : cout << "Invalid" << endl; break;
            case LogSessionTransferState::Error : cout << "Error" << endl; break;
            case LogSessionTransferState::Initial : cout << "Initial" << endl; break;
            case LogSessionTransferState::Enumeration : cout << "Enumeration" << endl; break;
            case LogSessionTransferState::TransferringSystemFiles : cout << "TransferringSystemFiles" << endl; break;
            case LogSessionTransferState::TransformingSystemFile : cout << "TransformingSystemFile" << endl; break;
            case LogSessionTransferState::TransferringLogfiles : cout << "TransferringLogfiles" << endl; break;
            case LogSessionTransferState::Complete : cout << "Complete" << endl; _application->disconnectPolySync(); break;
        }
    }

    int _sessionId;

    LogSessionExport * _exporter;

    Application * _application;

};


int main( int argc, char ** argv )
{
    if( argc != 2 )
    {
        cerr << "Usage: " << argv[0] << " [sessionId]" << endl;
        return -1;
    }

    SessionExportExample exportExample{ atoi(argv[1]) };

    auto application = Application::getInstance();

    application->setNodeName( "polysync-log-session-export-cpp" );

    application->connectPolySync();
}
