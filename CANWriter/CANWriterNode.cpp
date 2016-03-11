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
 * \example CANWriter.cpp
 *
 * PolySync CAN API writer Example.
 *
 * Shows how to use the CAN API to write CAN frames.
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

class CANWriterNode : public polysync::Node
{
public:
    /**
     * @brief CANReaderNode constructor
     * Open @ref _channel
     */
    CANWriterNode( uint channelID )
        :
        _flags( PSYNC_CAN_OPEN_ALLOW_VIRTUAL ),
        _bitRate( DATARATE_500K ),
        _channel( channelID, _flags )
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
        std::cout << "CANWriterNode::initStateEvent()" << std::endl;
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
     * example, 1 byte is written to the CAN channel passed in.
     */
    virtual void okStateEvent()
    {
        try
        {
           _channel.setOutputFrameId( 0x456 );
           _channel.setOutputFramePayloadSize( 1 );

            std::cout << "Writing CAN frame - ID: "
                      << _channel.getOutputFrameId()
                      << " - Output payload size: "
                      << _channel.getOutputFramePayloadSize()
                      << std::endl;

            std::array< uchar, 8 > outputData{ 0 };
            outputData[ 0 ] = 0xFF;

            _channel.write( outputData );

            polysync::sleepMicro( 100000 );
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
     * @brief errorStateEvent
     * If exceptions occurred in @ref okStateEvent or @ref initStateEvent, we
     * disconnect, which triggers @ref releaseStateEvent and allows for graceful
     * exit.
     */
    virtual void errorStateEvent()
    {
        std::cout << "CANWriterNode::errorStateEvent()" << std::endl;
        disconnectPolySync();
    }

private:
    uint _flags;
    ps_datarate_kind _bitRate;
    polysync::CANChannel _channel;
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
    if( argc > 1 )
    {
        ulong channel = 0;

        try
        {
            channel = std::stoul( argv[ 1 ] );
        }
        catch( ... )
        {
            std::cout << "Invalid argument, example expects an integer "
                         "representing a CAN channel." << std::endl;
            return 1;
        }

        try
        {
            CANWriterNode canWriter( channel );

            canWriter.setNodeName( "polysync-can-writer-cpp" );
            canWriter.connectPolySync();
        }
        catch( polysync::DTCException & exception )
        {
            std::cout << exception.what() << std::endl;
            std::cout << "Make sure a CAN device is connected to your machine."
                      << std::endl;

            return 1;
        }
    }
    else
    {
        std::cout << "Example expects an integer "
                     "representing a CAN channel."
                  << std::endl
                  << "For example: "
                  << std::endl
                  << "./polysync-can-writer-cpp 1"
                  << std::endl;
        return 1;
    }

    return 0;
}
