/*
 * PolySync SocketWriter C++ API example application
 *      Connect to a UDP socket and write data
 *
 *  Socket Class definition
 *      file:///usr/local/polysync/doc/cpp_api_docs/html/d9/d32/classpolysync_1_1_socket.html
 */

#include <iostream>

#include <PolySyncCore.hpp>
#include <PolySyncSocket.hpp>


// set socket re-use enabled flag
#define ENABLED 1


using namespace std;

int main()
{
    // required initalizer parameters
    int domain = AF_INET;
    int protocol = IPPROTO_UDP;
    int type = SOCK_DGRAM;

    // error exit flag for reading/receiving socket data
    auto errExit = 0;

    // buffer of data to send to the socket
    std::vector< uchar > socketSendBuffer;

    // create our local socket instance
    polysync::Socket socket( domain, type, protocol );


    //
    // set address and port to connect to

    // IP address we want to connect to (localhost)
    std::string ipAddr = "127.0.0.1";

    // copy from string into the vector of char
    const std::vector< char > addr( ipAddr.begin(), ipAddr.end() );

    // port to connect to
    ulong port = 1197;

    // set the address and port for our object and bind to the socket
    socket.setAddress( addr, port );
    // set socket reuse option for multiple connections
    socket.setReuse( ENABLED );
    socket.bind();

    // connect to the socket
    socket.connect();

    // set buffer here
    std::copy( ipAddr.begin(), ipAddr.end(), std::back_inserter(socketSendBuffer) );

    // number of bytes sent from socket.send()
    auto bytesSent = 0;

    // busy write loop
    while( errExit >= 0 )
    {
        bytesSent = 0;

        bytesSent = socket.send( socketSendBuffer );

        //
        // handle DTC exceptions thrown here


        //
        // do something with the data

    }


    socket.release();


    return 0;
}


