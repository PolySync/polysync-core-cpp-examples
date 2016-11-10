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
 * \example FileTransfer.cpp
 *
 * File Transfer example.
 *
 * Demonstrates how to use the PolySync file transfer API to initiate and
 * support file transfer operations.
 *
 * The need to transfer files across the PolySync domain arises rarely.
 * Most data should be transmitted via traditional PolySync message types.
 * Occasionally however, file transfer is useful for high level tasks
 * such as system administration or file management.
 *
 */

#include <iostream>
#include <PolySyncCore.hpp>
#include <PolySyncDataModel.hpp>
#include <PolySyncApplication.hpp>
#include <PolySyncFileTransfer.hpp>

using namespace std;
using namespace polysync;
using namespace datamodel;

/**
 * @brief TransferExample class
 *
 * The TransferExample class enumerates 'polysync-manager' nodes
 * by publishing a PSYNC_COMMAND_GET_MANAGER_STATUS CommandMessage.
 * For each ManagerStatusMessage response, the TransferExample then
 * initiates a file transfer of EULA.txt from that 'polysync-manager'.
 */
class TransferExample : public DataSubscriber
{

public:

    TransferExample()
        :
            _transferHandler( FileTransferHandler::getInstance() )
    {
        // Subscribe to ApplicationEventMessage to determine when
        // the application connects to the PolySync bus.
        connectSubscriberMethod< TransferExample >(
                ApplicationEventMessage::getName(),
                &TransferExample::handleEvent,
                this );

        // Subscribe to ManagerStatusMessage to initiate file transfers
        // from responding polysync-manager nodes.
        connectSubscriberMethod< TransferExample >(
                ManagerStatusMessage::getName(),
                &TransferExample::handleManagerStatus,
                this );

        _application = Application::getInstance();

        _application->attachSubscriber( this );
    }

private:

    void handleEvent( shared_ptr< Message > message )
    {
        if( auto event = getSubclass< ApplicationEventMessage >( message ) )
        {
            // If the application has connected to the PolySync bus then
            // publish a PSYNC_COMMAND_GET_MANAGER_STATUS CommandMessage.
            if( event->getEventKind() == EventKind::Init )
            {
                CommandMessage managerStatusCommand{ *_application };

                managerStatusCommand.setId( PSYNC_COMMAND_GET_MANAGER_STATUS );

                auto managerStatusMessageType = _application->getMessageTypeByName(
                        ManagerStatusMessage::getName() );

                // Set the subscription quality of service to reliable
                // to prevent missing any responses from polysync-manager nodes.
                _application->setSubscriberReliabilityQos(
                        managerStatusMessageType,
                        RELIABILITY_QOS_RELIABLE );

                managerStatusCommand.publish();
            }
        }
    }

    void handleManagerStatus( shared_ptr< Message > message )
    {
        if( auto managerStatus = getSubclass< ManagerStatusMessage >( message ) )
        {
            // Build the string path of the source file to be transferred.
            // e.g. "/usr/local/polysync/doc/EULA.txt"
            string sourceFile{ getenv( "PSYNC_HOME" ) };

            sourceFile += "/doc/EULA.txt";

            // Build the destination path for the transferred file.
            // e.g. "{Current Directory}/copy_of_license_from_{GUID}.txt"
            string destinationFile{
                "copy_of_license_from_" +
                to_string( managerStatus->getSourceGuid() )+
                ".txt"
            };

            // Set the options of the file transfer operation using the
            // above paths, polysync-manager's GUID, and the local node's GUID.
            FileTransferOptions transferOptions {
                managerStatus->getSourceGuid(),
                sourceFile,
                _application->getGuid(),
                destinationFile
            };

            // Subscribe to progress updates from the file transfer.
            auto transferSubscription =
                    make_shared< FileTransferSubscription >( transferOptions );

            transferSubscription->setOnProgress(
                    this,
                    &TransferExample::handleTransferProgress );

            _transferHandler.attachSubscription( transferSubscription );

            _transferHandler.start( transferOptions );
        }
    }

    void handleTransferProgress(
            const FileTransferState & state,
            const FileTransferOptions & options )
    {
        // Check if the file transfer has encountered an error.
        if( state.getDtc() != DTC_NONE )
        {
            cout << "File transfer has encountered an error" <<
                    options << endl;
        }
        // If the final file chunk has arrived, then file transfer is complete.
        else if( state.getCurrentChunkId() == state.getTotalChunks() )
        {
            _application->disconnectPolySync();
        }
    }

    // The FileTransferHandler singleton is required to participate in any
    // PolySync file transfer operations. It is used to start, abort,
    // and subscribe to file transfer operations.
    FileTransferHandler & _transferHandler;

    Application * _application;

};


int main()
{
    TransferExample transferExample;

    auto application = Application::getInstance();

    application->setNodeName( "polysync-file-transfer-cpp" );

    application->connectPolySync();
}
