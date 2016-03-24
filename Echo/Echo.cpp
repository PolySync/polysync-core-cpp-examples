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
 * PolySync Echo C++ API example application
 *      Demonstrates a command line utility for echoing PolySync messges to stdout
 *
 */

#include <iostream>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

using namespace std;

/**
 * @brief PolySyncEcho class
 *
 * The PolySyncEcho class exists to override the functions defined
 * in the base Node class.  The functions exist in the base class but are
 * stubbed out and must be overloaded in order for them to do something.  In
 * this instance the initStateEvent and the messageHandlerEvent are overloaded
 * to register for the messages and receive them, respectively.
 */
class PolySyncEcho : public polysync::Node
{
    
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
        // Register as a listener for the message type that the publisher
        // is going to send. Message types are defined in later tutorials.
        registerListenerToAllMessageTypes();
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
        message->print();
        //message->getHeader();
    }

};

/**
 * @brief main
 *
 * Entry point for this tutorial application
 * The "connectPolySync" begins this node's PolySync execution loop.
 *
 * @param argc - int, the number of parameters on the command-line
 * @param argv - char* [], the parsed command-line arguments
 * 
 * @return int - exit code
 */
int main( int argc, char *argv[] )
{
    // Create an instance of the PolySyncEcho and connect it to PolySync
    PolySyncEcho echo;

    // When the node has been created, it will cause an initStateEvent to
    // to be sent.
    echo.connectPolySync();

    return 0;
}
