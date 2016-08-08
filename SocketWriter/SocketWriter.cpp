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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * \example SocketWriter.cpp
 *
 * PolySync SocketWriter C++ API example application
 *      Connect to a UDP socket and send data
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
const string NODE_NAME( "polysync-socket-writer-cpp" );

/**
 * @brief SocketWriterNode class
 *
 * The SocketWriterNode class exists to demonstrate how to use the node class
 * to perform UDP socket write functions.
 */
class SocketWriterNode : public polysync::Node
{
private:
    polysync::Socket mySocket;


public:
    SocketWriterNode()
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
        const vector< char > addr { 127, 0, 0, 1 };

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
     * Perform a write to localhost here.
     *
     */
    void okStateEvent() override
    {
        vector< uchar > sendBuffer
        { 'S', 'o', 'c', 'k', 'e', 't', ' ', 'W', 'r', 'i', 't', 'e', 'r' };

        // returns number of bytes sent
        mySocket.send( sendBuffer );
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
    // Create an instance of the SocketWriterNode and connect it to PolySync
    SocketWriterNode socketWriterNode;
    socketWriterNode.setNodeName(NODE_NAME);
    socketWriterNode.connectPolySync();

    return 0;
}


