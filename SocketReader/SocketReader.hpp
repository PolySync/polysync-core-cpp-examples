#ifndef SOCKET_READER_H
#define SOCKET_READER_H


#include <map>
#include <future>
#include <memory>

#include <PolySyncNode>
#include <PolySyncMessage>
#include <PolySyncDataModel>

// Socket API utilities
#include <PolySyncSocket.hpp>

#include "Reader.hpp"

using namespace std;

/**
 * @brief The Reader class
 * Subclass of Reader, which is a subclass of polysync::Node.
 *
 * @note Run the Writer example if you need to make data availabe on bus.
 */
class SocketReader : polysync::Socket
{

public:

    // constructor
    explicit SocketReader();
    SocketReader( int domain, int type, int protocol );

    // destructor
    ~SocketReader();

    void    connectUdpSocket();

    void    receiveSocket();



protected:

    int domain = AF_INET;
    int protocol = IPPROTO_UDP;
    int type = SOCK_DGRAM;



private:

    //polysync::Socket  *Socket = nullptr;

};

#endif // SOCKET_READER_H
