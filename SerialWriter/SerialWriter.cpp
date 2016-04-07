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
 * \example SerialWriter.cpp
 *
 * PolySync Serial Writer C++ API example application
 *      Demonstrate the user of a serial writer node
 */

#include <iostream>
#include <string>
#include <PolySyncNode.hpp>
#include <PolySyncSerial.hpp>

using namespace std;

// Constant values for use in the example
const ps_datarate_kind MY_DATARATE = DATARATE_9600;
const string SERIAL_PORT( "/dev/ttyUSB0" );
const string NODE_NAME( "polysync-serial-writer-cpp" );

/**
 * @brief SerialWriterNode class
 *
 * The SerialWriterNode class exists to demonstrate how to use the node class
 * to perform serial write functions.
 */
class SerialWriterNode : public polysync::Node
{
private:
    polysync::Serial myDevice;

public:
    SerialWriterNode()
    : myDevice( SERIAL_PORT )
    {
    }

    /**
     * @brief initStateEvent
     *
     * Initialize the serial port here
     *
     */
    void initStateEvent() override
    {
        myDevice.open();
        myDevice.setDataRate( MY_DATARATE );
        myDevice.applySettings();
    }

    /**
     * @brief okStateEvent
     *
     * Perform a read here and process any data that has been read.
     *
     */
    void okStateEvent() override
    {
        vector< uchar > writeBuffer;

        writeBuffer.resize( NODE_NAME.size() + 1, 0 );

        memcpy(writeBuffer.data(), NODE_NAME.data(), NODE_NAME.size() );

        ulong bytesWritten = myDevice.write( writeBuffer );

        // Up to the user on what to do with the data here.
        // Options include: print it, consume it, publish it, transform it...
    }

    /**
     * @brief releaseStateEvent
     *
     * Perform the serial port cleanup here.
     *
     */
    void releaseStateEvent() override
    {
        myDevice.close();
    }

};

/**
 * @brief main
 *
 * The "connectPolySync" function begins the node's PolySync execution loop.
 *
 * @param argc - int, the number of parameters on the command-line
 * @param argv - char* [], the parsed command-line arguments
 * 
 * @return int - exit code
 */
int main(int argc, char *argv[])
{
    // Create an instance of the SerialReaderNode and connect it to PolySync
    SerialWriterNode serialWriterNode;
    serialWriterNode.setNodeName(NODE_NAME);
    serialWriterNode.connectPolySync();

    return 0;
}
