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

/*
 * PolySync Hello World Subscriber C++ API example application
 *      Demonstrate how to subscribe a node to a message
 *
 */

#include <iostream>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

using namespace std;

/**
 * @brief HellowWorldSubscriberNode class
 *
 * The HelloWorldSubscriberNode class exists to override the functions defined
 * in the base Node class.  The functions exist in the base class but are
 * stubbed out and must be overloaded in order for them to do something.  In
 * this instance the initStateEvent and the messageHandlerEvent are overloaded
 * to register for the messages and receive them, respectively.
 */
class HelloWorldSubscriberNode : public polysync::Node
{
private:
    ps_msg_type _messageType;
    
public:
    /**
     * @brief initStateEvent
     *
     * Subscribe to a message that the publisher node will send.
     *
     * @param void
     * @return void
     */
    void initStateEvent() override
    {
        _messageType = getMessageTypeByName( "ps_byte_array_msg" );

        // Register as a listener for the message type that the publisher
        // is going to send.  Message types are defined in later tutorials.
        registerListener( _messageType );
    }
    
    /**
     * @brief messageEvent
     * 
     * Extract the information from the provided message
     * 
     * @param std::shared_ptr< Message > - variable containing the message
     * @return void
     */
    virtual void messageEvent( std::shared_ptr< polysync::Message > message )
    {
        using namespace polysync::datamodel;
        if( auto byteArray = getSubclass< ByteArrayMessage >( message ) )
        {  
            byteArray->print();
        }
    }

};

/**
 * @brief main
 *
 * Entry point for this tutorial application
 * The "connectToPolySync" is a blocking call, users must use Ctrl-C to exit
 * this function.
 *
 * @param argc - int, the number of parameters on the command-line
 * @param argv - char* [], the parsed command-line arguments
 * 
 * @return int - exit code
 */
int main( int argc, char *argv[] )
{
    // Create an instance of the HelloWorldNode and connect it to PolySync
    HelloWorldSubscriberNode subscriberNode;

    // When the node has been created, it will cause an initStateEvent to
    // to be sent.
    subscriberNode.connectToPolySync();

    return 0;
}
