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
 */

#include <iostream>
#include <PolySyncCore.hpp>
#include <PolySyncDataModel.hpp>
#include <PolySyncApplication.hpp>
#include <PolySyncFileTransfer.hpp>

using namespace std;
using namespace polysync;
using namespace datamodel;

class TransferExample : public DataSubscriber
{

public:

    TransferExample()
    {
        connectSubscriberMethod< TransferExample >(
                ApplicationEventMessage::getName(),
                &TransferExample::handleEvent,
                this );

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
            if( event->getEventKind() == EventKind::Init )
            {
                CommandMessage managerStatusCommand{ *_application };

                managerStatusCommand.setId( PSYNC_COMMAND_GET_MANAGER_STATUS );

                auto managerStatusMessageType = _application->getMessageTypeByName(
                        ManagerStatusMessage::getName() );

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
            string sourceFile{ getenv( "PSYNC_HOME" ) };

            sourceFile += _eulaPath;

            FileTransferOptions transferOptions {
                managerStatus->getSourceGuid(),
                sourceFile,
                _application->getGuid(),
                _destinationPath
            };

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
        notUsed( options );

        if( state.getCurrentChunkId() == state.getTotalChunks() )
        {
            _application->disconnectPolySync();
        }
    }

    static constexpr auto _eulaPath = "/doc/EULA.txt";

    static constexpr auto _destinationPath = "copy_of_license.txt";

    FileTransferHandler _transferHandler;

    Application * _application;

};


int main()
{
    TransferExample transferExample;

    Application::getInstance()->connectPolySync();
}
