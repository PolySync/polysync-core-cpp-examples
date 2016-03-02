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
 * \example CANReader.cpp
 *
 * PolySync CAN API reader Example.
 *
 * Shows how to use the CAN API to read CAN frames.
 *
 * The example uses the standard PolySync node template and state machine.
 * Send the SIGINT (control-C on the keyboard) signal to the node/process to do a graceful shutdown.
 *
 */

#include <iostream>
#include <memory>

#include <PolySyncNode.hpp>
#include <PolySyncCAN.hpp>
#include <PolySyncDTCException.hpp>
#include <PolySyncDataModel.hpp>

class CANReaderNode : public polysync::Node
{
public:
    /**
     * @brief CANReaderNode constructor
     * Open @ref _channel
     */
    CANReaderNode()
        :
        _channel( _systemID, _flags )
    {}


protected:

    /**
     * @brief initStateEvent
     *
     * This function is triggered once when the node has initialized in the
     * PolySync context.
     */
    virtual void initStateEvent()
    {
        try
        {
            _channel.setBitRate( _bitRate );

            _channel.goOnBus();
        }
        catch( polysync::DTCException & exception )
        {
            // If interaction with the channel fails, print why and trigger
            // errorStateEvent
            std::cout << exception.what() << std::endl;
            activateFault( exception.getDtc(), NODE_STATE_ERROR );
        }
    }

    /**
     * @brief okStateEvent
     *
     * Called repeatedly while node is in an operational state. For this
     * example, we will read CAN data and print useful information.
     */
    virtual void okStateEvent()
    {
        try
        {
            // Read data from the device, we're not using the buffer, here.
            auto CANFrame = _channel.read();

            // Output CAN frame data.
            std::cout << "CAN frame - ID: 0x"
                      << _channel.getInputFrameId() << std::endl;

            std::cout << "DLC: "
                      << _channel.getInputFramePayloadSize() << std::endl;
        }
        catch( polysync::DTCException & exception )
        {
            std::cout << exception.what() << std::endl;

            // Activate a fault state for this node. The NODE_STATE_ERROR
            // will trigger call to errorStateEvent.
            activateFault( exception.getDtc(), NODE_STATE_ERROR );
        }

    }

    /**
     * @brief releaseStateEvent
     *
     * The event is triggered once upon the node's release from PolySync.
     *
     */
    virtual void releaseStateEvent()
    {
        _channel.goOffBus();
    }

    /**
     * @brief errorStateEvent
     * If exceptions occurred in @ref okStateEvent or @ref initStateEvent, we
     * disconnect, which triggers @ref releaseStateEvent and allows for graceful
     * exit.
     */
    virtual void errorStateEvent()
    {
        disconnectPolySync();
    }

private:
    polysync::CANChannel _channel;
    uint _systemID{ 0 };
    uint _flags{ PSYNC_CAN_OPEN_ALLOW_VIRTUAL };
    ps_datarate_kind _bitRate{ DATARATE_500K };
};

/**
 * @brief main
 *
 * Entry point for this example application
 * The "connectPolySync" function begins the node's PolySync execution loop.
 *
 * @param argc - int, the number of parameters on the command-line
 * @param argv - char* [], the parsed command-line arguments
 *
 * @return int - exit code
 */
int main( int argc, char *argv[] )
{
    try
    {
        CANReaderNode canReader;

        canReader.setNodeName( "polysync-can-reader-cpp" );
        canReader.connectPolySync();
    }
    catch( polysync::DTCException & exception )
    {
        std::cout << exception.what() << std::endl;
        std::cout << "Make sure a CAN device is connected to your machine."
                  << std::endl;
    }

    return 0;
}
