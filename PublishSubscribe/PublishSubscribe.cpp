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
 * \example PublishSubscribe.cpp
 *
 * Publish/Subscribe Example.
 *
 * Shows how to use publish/subscribe routines.
 * The example code registers a subscriber to PolySync diagnostic messages and
 * publishes event messages.
 *
 * The example uses the standard PolySync node template and state machine.
 * Send the SIGINT (control-C on the keyboard) signal to the node/process to do 
 * a graceful shutdown.
 *
 */

#include <iostream>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

using namespace std;

/**
 * @brief Node flags to be OR'd with driver/interface flags.
 *
 * Provided by the compiler so PolySync can add build-specifics as needed.
 *
 */
#ifndef NODE_FLAGS_VALUE
#define NODE_FLAGS_VALUE (0)
#endif

/**
 * @brief PublisherSubscriberNode class
 *
 * The PublisherSubscriberNode class exists to override the functions defined in the
 * base class. The functions exist in the base class but are stubbed out
 * and must be overloaded in order for them to do something. In this instance
 * we are overriding the main functions for sending and receiving messages as 
 * well as responding to error and warning type events.
 * 
 */
class PublisherSubscriberNode : public polysync::Node
{
private:
    const string node_name = "polysync-publish-subscribe-cpp";
    const string diagnostic_trace_msg_name = "ps_diagnostic_trace_msg";
    const string event_msg_name = "ps_event_msg";
    
    ps_msg_type _messageType;
    
public:
    
    PublisherSubscriberNode()
    {
        setNodeType( PSYNC_NODE_TYPE_API_USER );
        setDomainID( PSYNC_DEFAULT_DOMAIN );
        setSDFID( PSYNC_SDF_ID_INVALID );
        setFlags( NODE_FLAGS_VALUE | PSYNC_INIT_FLAG_STDOUT_LOGGING );
        setNodeName( node_name );
    }
    
    ~PublisherSubscriberNode()
    {
        
    }

    void initStateEvent() override
    {
        _messageType = getMessageTypeByName( diagnostic_trace_msg_name );
        
        // Register as a listener for diagnostic messages that any nodes may 
        // send.
        registerListener( _messageType );
    }
    
    void releaseStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro( 10000 );
    }
    
    void errorStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro( 10000 );
    }
    
    void fatalStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro( 10000 );
    }
    
    void warnStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro( 10000 );
    }
    
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
        polysync::datamodel::EventMessage message( *this );
        
        // Set message data
        message.setHeaderTimestamp( polysync::getTimestamp() );
        message.setId(0);

        // Publish to the PolySync bus
        message.publish();

        // The ok state is called periodically by the system so sleep to reduce
        // the number of messages sent.
        polysync::sleepMicro( 1000000 );
    }
    
    /**
     * @brief messageEvent
     * 
     * Extract the information from the provided message
     * 
     * @param std::shared_ptr< Message > - variable containing the message
     * @return void
     */
    void messageEvent( std::shared_ptr< polysync::Message > message ) override
    {
        using namespace polysync::datamodel;
        if( auto diagnosticMsg = 
                getSubclass< DiagnosticTraceMessage >( message ) )
        {  
            diagnosticMsg->print();
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
 * @return int - exit code
 */
int main()
{
    // Create an instance of the PublisherSubscriberNode and connect it to 
    // PolySync.
    PublisherSubscriberNode publisherSubscriberNode;

    // When the node has been created, it will cause an initStateEvent to be
    // sent and then proceed into the okState.  connectToPolySync does not
    // return, use Ctrl-C to exit.
    publisherSubscriberNode.connectPolySync();

    return 0;
}
