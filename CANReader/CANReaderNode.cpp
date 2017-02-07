/*
 * Copyright (c) 2016 PolySync
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
 * THE SOFTWARE IS PROVIDED "AS IS," WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * \example CANReaderNode.cpp
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

    CANReaderNode( uint channelID )
        :
        _channel( channelID, _flags )
    {
        // empty
    }

    virtual ~CANReaderNode() = default;

protected:

    /**
     * This function is triggered once when the node has initialized in the
     * PolySync context.
     */
    virtual void initStateEvent()
    {
        std::cout << "CANReaderNode::initStateEvent()" << std::endl;

        try
        {
            _channel.setBitRate( _bitRate );

            _channel.goOnBus();
        }
        catch( polysync::DTCException & exception )
        {
            // If interaction with the channel fails, print why and trigger
            // errorStateEvent()
            std::cout << exception.what() << std::endl;

            activateFault( exception.getDtc(), NODE_STATE_ERROR );
        }
    }

    /**
     * Called repeatedly while node is in an operational state. For this
     * example, we will read CAN data and print useful information.
     */
    virtual void okStateEvent()
    {
        try
        {
            // Read data from the device, timeout after one second.
            _channel.read( 1000000 );

            // Output CAN frame data.
            std::cout << "CAN frame - ID: 0x"
                      << _channel.getInputFrameId()
                      << std::endl;

            std::cout << "DLC: "
                      << _channel.getInputFramePayloadSize()
                      << std::endl << std::endl;
        }
        catch( polysync::DTCException & exception )
        {
            auto errorCode = exception.getDtc();

            if( exception.getDtc() != DTC_UNAVAILABLE )
            {
                if( errorCode == DTC_INTR )
                {
                    std::cout << "CAN read was interrupted, application most "
                              << "likely received SIGINT (ctrl-c)."
                              << std::endl;

                    disconnectPolySync();

                    return;
                }

                std::cout << exception.what() << std::endl;

                // Activate a fault state for this node. The NODE_STATE_ERROR
                // will trigger call to errorStateEvent.
                activateFault( exception.getDtc(), NODE_STATE_ERROR );
            }
            else
            {
                std::cout << "Device unavailable. " << std::endl;
            }
        }

        // Sleep for one millisecond
        polysync::sleepMicro( 1000 );
    }

    /**
     * If exceptions occurred in @ref okStateEvent or @ref initStateEvent, we
     * disconnect, which triggers @ref releaseStateEvent and allows for graceful
     * exit.
     */
    virtual void errorStateEvent()
    {
        std::cout << "CANReaderNode::errorStateEvent()" << std::endl;

        disconnectPolySync();
    }


private:

    polysync::CANChannel _channel;

    uint _flags{ PSYNC_CAN_OPEN_ALLOW_VIRTUAL };

    ps_datarate_kind _bitRate{ DATARATE_500K };

};

/**
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
    // Check for shared memory key argument
    if( argc > 1 )
    {
        try
        {
            if( polysync::getChannelCount() > 0 )
            {
                CANReaderNode canReader( std::stoul( argv[ 1 ] ) );

                canReader.setNodeName( "polysync-can-reader-cpp" );

                canReader.connectPolySync();
            }
            else
            {
                std::cout << "No available CAN channels." << std::endl;
            }
        }
        catch( std::exception & e )
        {
            std::cout << "Invalid argument. This example requires valid "
                         "integer input representing a CAN channel."
                      << std::endl
                      << "For example: "
                         "polysync-can-reader-cpp 1"
                      << std::endl;

            return 1;
        }
    }
    else
    {
        std::cout << "Must pass CAN channel argument." << std::endl
                  << "For example: "
                     "polysync-can-reader-cpp 1" << std::endl;

        return 1;
    }

    return 0;
}
