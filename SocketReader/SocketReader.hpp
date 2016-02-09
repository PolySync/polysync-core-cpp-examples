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
class SocketReader : Reader 
{
public:

    // constructor
    explicit SocketReader();

    // destructor
    ~SocketReader();

    void    connectUdpSocket();

    void    receiveSocket();

private:

    polysync::Socket  *Socket = nullptr;

};

#endif // SOCKET_READER_H
