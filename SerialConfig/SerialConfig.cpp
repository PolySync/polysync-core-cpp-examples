/*
 * Copyright (c) 2016 HARBRICK TECHNOLOGIES, INC
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
 * \example SerialConfig.cpp
 *
 * PolySync Serial Configuration C++ API example application
 * 
 *      Demonstrate the configuration of the serial port for cases where the
 *      connection is using something other than no flow control, no parity,
 *      8 data bits, and 1 stop bit
 *      
 *      To exercise this example requires two serial ports, they can be on the
 *      same computer.  One of the serial ports is used by this application;
 *      this application sets the serial port to a 7E2 configuration.  The
 *      other serial port should be configured the same way.  The user can then
 *      send strings over the serial port until they send "quit" at which point
 *      the application will exit.
 *      
 *      Both serial ports must be capable of running at 7 Data bits, 2 Stop
 *      bits with even parity and support RTS/CTS hardware flow control.
 */

#include <iostream>
#include <string>
#include <PolySyncNode.hpp>
#include <PolySyncSerial.hpp>
#include <PolySyncDTCException.hpp>

using namespace std;
using namespace polysync;

// Constant values for use in the example
const ps_datarate_kind MY_DATARATE = DATARATE_9600;
const string SERIAL_PORT( "/dev/ttyUSB0" );
const string NODE_NAME( "polysync-serial-config-cpp" );

/**
 * @brief SerialConfigNode class
 *
 * The SerialConfigNode class exists to demonstrate how to use the node class
 * to perform serial read functions.
 */
class SerialConfigNode : public Node
{
private:
    Serial myDevice;

public:
    SerialConfigNode()
    : myDevice( SERIAL_PORT )
    {
    }

    /**
     * @brief initStateEvent
     *
     * Initialize the serial port here
     * This example will specifically set the device to use 7 data bits, 2 stop
     * bits, even parity and hardware flow control using RTS/CTS
     *
     */
    void initStateEvent() override
    {
        myDevice.open();
        myDevice.setDataRate( MY_DATARATE );
        
        myDevice.setDataBits( dataBits7 );
        myDevice.setStopBits( stopBits2 );
        myDevice.setParity( parityEven );
        myDevice.setHardwareFlowControl( hardwareFlowControlRtscts );
        
        myDevice.applySettings();
    }

    /**
     * @brief okStateEvent
     *
     * Perform a read here and process any data that has been read.
     * This requires an external device configured to the same serial
     * configuration to communicate.  Any messages sent over that connection
     * will be printed unless the user sends "quit" at which point the program
     * will exit.
     *
     */
    void okStateEvent() override
    {
        vector< uchar > readBuffer;
        ps_timestamp ts;

        // Returns the number of bytes read.
        myDevice.read( readBuffer, ts );

        string command;
        
        for ( unsigned int i = 0; i < readBuffer.size(); i++ )
        {
            command.push_back( (char)readBuffer[i] );
        }
        
        if ( command == "quit" )
        {
            disconnectPolySync();
        }
        else
        {
            // User is free to add their own commands here
            cout << "Received the following command: " << command << endl;
        }
    }

    /**
     * @brief releaseStateEvent
     *
     * Perform the serial port cleanup here.
     * When the user sends the "quit" command the node executes the release
     * state at which point the program resets the serial port back to N,8,1
     * with no flow control
     *
     */
    void releaseStateEvent() override
    {
        myDevice.setDataBits( dataBits8 );
        myDevice.setStopBits( stopBits1 );
        myDevice.setParity( parityNone );
        myDevice.setHardwareFlowControl( hardwareFlowControlNone );
        
        myDevice.applySettings();

        myDevice.close();
    }

};

/**
 * @brief main
 *
 * The "connectPolySync" function begins the node's PolySync execution loop.
 *
 * @return int - exit code
 */
int main()
{
    
    try
    {
        // Create an instance of the SerialConfigNode and connect it to PolySync
        SerialConfigNode SerialConfigNode;
        SerialConfigNode.setNodeName(NODE_NAME);
        SerialConfigNode.connectPolySync();
    }
    catch( polysync::DTCException & e )
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
