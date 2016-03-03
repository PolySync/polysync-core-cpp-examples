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
 * PolySync Hello World Publisher C++ API example application
 *      Demonstrate how to publish a message from a node
 *
 */

#include <iostream>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

using namespace std;

/**
 * @brief HellowWorldPublisherNode class
 *
 * The HelloWorldNode class exists to override the functions defined in the
 * base class.  The functions exist in the base class but are stubbed out
 * and must be overloaded in order for them to do something. In this instance
 * only the okStateEvent function is overloaded to publish a message that the
 * subscriber can receive.
 */
class HelloWorldPublisherNode : public polysync::Node
{
    /**
     * @brief okStateEvent
     *
     * Override the base class functionality to send messages when the node
     * reaches the "ok" state. This is the state where the node is in its
     * normal operating mode.
     * 
     * @param void
     * @return void
     */
    void okStateEvent() override
    {
        // Create a message
        polysync::datamodel::ByteArrayMessage message( *this );
        
        // Set publish time
        message.setHeaderTimestamp( polysync::getTimestamp() );

        // Populate buffer
        message.setBytes( {'H', 'e', 'l', 'l', 'o', ' ',
                           'W', 'o', 'r', 'l', 'd' } );

        // Publish to the PolySync bus
        message.publish();

        // The ok state is called periodically by the system so sleep to reduce
        // the number of messages sent.
        polysync::sleepMicro( 1000000 );
    }
};

/**
 * @brief main
 *
 * Entry point for the publisher side of this tutorial application
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
    // Create an instance of the HelloWorldPublisherNode and connect it to PolySync
    HelloWorldPublisherNode publisherNode;

    // When the node has been created, it will cause an initStateEvent to be
    // sent and then proceed into the okState.  connectToPolySync does not
    // return, use Ctrl-C to exit.
    publisherNode.connectPolySync();

    return 0;
}
