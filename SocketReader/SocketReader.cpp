#include <iostream>
#include "SocketReader.hpp"



SocketReader::SocketReader()
{
    int domain = AF_INET;

    int protocol = IPPROTO_UDP;
    int type = SOCK_DGRAM;

    polysync::socket::init( Socket, domain, type, protocol );

//    std::vector< char* > addr;
//    addr.assign("127.0.0.1");
//    polysync::socket::setAddress( Socket, addr, PSYNC_SOCKET_PORT_ANY );

}



SocketReader::~SocketReader()
{
    polysync::socket::release( Socket );
}



void SocketReader::connectUdpSocket()
{
    // connect to UDP socket
    polysync::socket::connect( Socket );
}



void SocketReader::receiveSocket()
{

    std::vector< uchar > buffer;

    auto bufferLength = buffer.max_size();

    // indication of total bytes read
    ulong bytesRead = 0;
    polysync::Timestamp rxTimestamp;

    // recevice the data to be parsed/printed later
    polysync::socket::receive( Socket, buffer, bufferLength, bytesRead, rxTimestamp);

   if( bytesRead > 0 )
   {
       // work with incoming data here
   }

}
