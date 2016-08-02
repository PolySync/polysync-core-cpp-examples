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
 * \example SocketReader.cpp
 *
 * PolySync SocketReader C++ API example application
 *      Connect to a UDP socket and receive data
 */

#include <iostream>

#include <PolySyncCore.hpp>
#include <PolySyncNode.hpp>
#include <PolySyncSocket.hpp>


// set socket re-use enabled flag
#define ENABLED 1


using namespace std;

const int SOCKET_DOMAIN = AF_INET;
const int SOCKET_PROTOCOL = IPPROTO_UDP;
const int SOCKET_TYPE = SOCK_DGRAM;
const string NODE_NAME( "polysync-socket-reader-cpp" );

/**
 * @brief SocketReaderNode class
 *
 * The SocketReaderNode class exists to demonstrate how to use the node class
 * to perform UDP socker read functions.
 */
class SocketReaderNode : public polysync::Node
{
private:
    polysync::Socket mySocket;

public:
    SocketReaderNode()
    : mySocket( SOCKET_DOMAIN, SOCKET_TYPE, SOCKET_PROTOCOL )
    {
    }

    /**
     * @brief initStateEvent
     *
     * Initialize the socket here
     *
     */
    void initStateEvent() override
    {
        // connect to (localhost)
        std::string ipAddr = "127.0.0.1";

        // copy from string into the vector of char
        const std::vector< char > addr( ipAddr.begin(), ipAddr.end() );

        ulong port = 1197;

        // set the address and port for our object and bind to the socket
        mySocket.setAddress( addr, port );

        // set socket reuse option for multiple connections
        mySocket.setReuse( ENABLED );
        mySocket.bind();

        // connect to the socket
        mySocket.connect();
    }

    /**
     * @brief okStateEvent
     *
     * Perform a read here and process any data that has been read.
     *
     */
    void okStateEvent() override
    {
        vector< uchar > receiveBuffer;
        ps_timestamp timestamp;

        // Returns number of bytes received.
        mySocket.receive( receiveBuffer, timestamp );

        // Up to the user on what to do with the data here.
        // Options include: print it, consume it, publish it, transform it...
    }

    /**
     * @brief releaseStateEvent
     *
     * Perform the socket cleanup here.
     *
     */
    void releaseStateEvent() override
    {
        mySocket.release();
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
    // Create an instance of the SocketReaderNode and connect it to PolySync
    SocketReaderNode socketReaderNode;
    socketReaderNode.setNodeName(NODE_NAME);
    socketReaderNode.connectPolySync();

    return 0;
}

