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
 * \example Subscribe.cpp
 *
 * Publish/Subscribe Example.
 *
 * Shows how to use publish/subscribe communication model, and the Messaging API
 * routines. This is the second half of the publish/subscribe example. This node
 * subscribes to the message published from the Publish.cpp node.
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
 */
#ifndef NODE_FLAGS_VALUE
#define NODE_FLAGS_VALUE (0)
#endif

/**
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
    const string node_name = "polysync-subscribe-cpp";
    const string platform_motion_msg_name = "ps_platform_motion_msg";

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
        _messageType = getMessageTypeByName( platform_motion_msg_name );
        
        // Register as a listener for the platform motion message
        registerListener( _messageType );
    }
    
    void releaseStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
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
     * Override the base class functionality to send messages when the node
     * reaches the "ok" state. This is the state where the node is in its
     * normal operating mode.
     */
    void okStateEvent() override
    {
        // The ok state is called periodically by the system
        polysync::sleepMicro( 10000 );
    }
    
    /**
     * Extract the information from the provided message
     * 
     * @param std::shared_ptr< Message > - variable containing the message
     */
    void messageEvent( std::shared_ptr< polysync::Message > message ) override
    {
        using namespace polysync::datamodel;
        if( std::shared_ptr < polysync::datamodel::PlatformMotionMessage > platformMotionMsg =
                getSubclass< PlatformMotionMessage >( message ) )
        {  
            //platformMotionMsg->print();

            // or...

            std::cout << "Latitude: " << platformMotionMsg->getLatitude() << std::endl;
            std::cout << "Longitude: " << platformMotionMsg->getLongitude() << std::endl;

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
