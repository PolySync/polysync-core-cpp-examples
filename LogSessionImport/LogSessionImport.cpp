/*
 * Copyright (c) 2015 PolySync
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
#include <PolySyncApplication.hpp>
#include <PolySyncDataModel.hpp>

#include "LogSessionImport.hpp"


SessionImportExample::SessionImportExample( const std::string & sessionPath )
    :
    _sessionPath( sessionPath ),
    _importer()
{
    // Subscribe to ApplicationEventMessage to determine when
    // the application connects to the PolySync bus.
    connectSubscriberMethod< SessionImportExample >(
            polysync::ApplicationEventMessage::getName(),
            &SessionImportExample::handleEvent,
            this );

    polysync::Application::getInstance()->attachSubscriber( this );
}


void SessionImportExample::handleEvent(
        std::shared_ptr< polysync::Message > message )
{
    if( auto event = polysync::datamodel::getSubclass<
            polysync::ApplicationEventMessage >( message ) )
    {
        auto eventKind = event->getEventKind();

        if( eventKind == polysync::EventKind::Init )
        {
            // This is the actual usage of the import API.
            _importer = std::unique_ptr< polysync::LogSessionImport >{
                    new polysync::LogSessionImport( _sessionPath ) };

            //
            // Assign imported nodes to new destination host.
            //

            // Destination hosts are specified using 'LogSessionTransferTarget'.
            // The import API requires the transfer target to provide one of
            // the following:
            // * Interface Address String ("127.0.0.1")
            // * Manager GUID (562950866709306)
            // * Host's Id in the SDF
            polysync::LogSessionTransferTarget transferTarget;

            for( const auto & availableNode : _importer->getAvailableNodes() )
            {
                std::cout << "assigning "
                          << availableNode.getName()
                          << " to 127.0.0.1" << std::endl;

                transferTarget.setInterfaceAddress("127.0.0.1");

                _importer->updateNodeTarget(
                        transferTarget,
                        availableNode );
            }

            // Import can be started with or without a progress callback.
            // When complete, results will be located at
            // "PSYNC_HOME/rnr_logs/[sessionId]"
            _importer->start(
                    this,
                    &SessionImportExample::handleTransferStatus );
        }
    }
}


void SessionImportExample::handleTransferStatus(
        const polysync::LogSessionTransferState state,
        const std::shared_ptr< polysync::datamodel::FileSyncStatusMessage > & )
{
    std::cout << "State: ";

    switch( state )
    {
        case polysync::LogSessionTransferState::Invalid :
            std::cout << "Invalid" << std::endl;
            break;
        case polysync::LogSessionTransferState::Error :
            std::cout << "Error" << std::endl;
            break;
        case polysync::LogSessionTransferState::Initial :
            std::cout << "Initial" << std::endl;
            break;
        case polysync::LogSessionTransferState::Enumeration :
            std::cout << "Enumeration" << std::endl;
            break;
        case polysync::LogSessionTransferState::TransferringSystemFiles :
            std::cout << "TransferringSystemFiles" << std::endl;
            break;
        case polysync::LogSessionTransferState::TransformingSystemFile :
            std::cout << "TransformingSystemFile" << std::endl;
            break;
        case polysync::LogSessionTransferState::TransferringLogfiles :
            std::cout << "TransferringLogfiles" << std::endl;
            break;
        case polysync::LogSessionTransferState::Complete :
            std::cout << "Complete" << std::endl;
            polysync::Application::getInstance()->disconnectPolySync();
            break;
        default:
            std::cout << "Unknown" << std::endl;
    }
}
