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

/**
 * Parameter Get/Set Example.
 *
 * Shows how to get/set parameters from the PolySync bus. Example publishes
 * a single parameter message to the bus requesting all nodes parameters.
 * A subscriber is created to recieve parameter messages and prints all
 * data stored within the message.
 *
 * The example uses the standard PolySync node template and state machine.
 * Send the SIGINT (control-C on the keyboard) signal to the node/process to do 
 * a graceful shutdown.
 *
 */

#include <iostream>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

#include <vector>

using namespace std;

/**
 * @brief Node flags to be OR'd with driver/interface flags.
 *
 * Provided by the compiler so Harbrick can add build-specifics as needed.
 *
 */
#ifndef NODE_FLAGS_VALUE
#define NODE_FLAGS_VALUE (0)
#endif

/**
 * @brief ParameterGetSet class
 *
 * The ParameterGetSet class exists to override the functions defined in the
 * base class. The functions exist in the base class but are stubbed out
 * and must be overriden to do anything useful. In this instance
 * we are overriding the main functions for sending and receiving messages as 
 * well as responding to error and warning events.
 * 
 */
class ParameterGetSet : public polysync::Node
{
private:
    const string node_name = "polysync-parameter-get-set-cpp";
    const string parameter_msg_name = "ps_parameters_msg";
    
    ps_msg_type _messageType;
    
public:
    
    ParameterGetSet()
    {
        setNodeType( PSYNC_NODE_TYPE_API_USER );
        setDomainID( PSYNC_DEFAULT_DOMAIN );
        setSDFID( PSYNC_SDF_ID_INVALID );
        setFlags( NODE_FLAGS_VALUE | PSYNC_INIT_FLAG_STDOUT_LOGGING );
        setNodeName( node_name );
    }
    
    ~ParameterGetSet()
    {
        
    }

    /**
     * @brief initStateEvent
     *
     * Override the base class functionality to send messages when the node
     * reaches the "init" state. This state is only called once, and will
     * do two things: register listener for parameter messages and request
     * all parameters from all nodes on the PolySync bus
     *
     * @param void
     * @return void
     */

    void initStateEvent() override
    {
        // get parameter message type
        _messageType = getMessageTypeByName( parameter_msg_name );
        
        // Register as a listener for parameter messages that any node may
        // send
        // Data is available in the messageEvent function below
        registerListener( _messageType );



        // Create a message
        polysync::datamodel::ParametersMessage parameterMessage( *this );

        // Set message data
        parameterMessage.setHeaderTimestamp( polysync::getTimestamp() );

        // PSYNC_GUID_INVALID requests all nodes parameters, rather than a single GUID
        parameterMessage.setDestGuid( PSYNC_GUID_INVALID );

        // Set parameter message type to get all parameters
        // You can filter for specific message types/ID's here
        parameterMessage.setType( PARAMETER_MESSAGE_GET_ALL );

        // Publish to the PolySync bus
        parameterMessage.publish();

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
    
    void okStateEvent() override
    {
        // The ok state is called periodically by the system so sleep to reduce
        // the number of messages sent.
        polysync::sleepMicro( 1000000 );
    }
    
    /**
     * @brief messageEvent
     * 
     * Extract the information from the provided message by overriding the
     * callback function
     * 
     * @param std::shared_ptr< Message > - variable containing the message
     * @return void
     */
    void messageEvent( std::shared_ptr< polysync::Message > message ) override
    {
        using namespace polysync::datamodel;
        if( auto parameterMsg = 
                getSubclass< ParametersMessage >( message ) )
        {  
            parameterMsg->print();
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
    // Create an instance of the ParameterGetSet and connect it to 
    // PolySync.
    ParameterGetSet parameterGetSetNode;

    // When the node has been created, it will cause an initStateEvent to be
    // sent and then proceed into the okState.  connectToPolySync does not
    // return, use Ctrl-C to exit.
    parameterGetSetNode.connectPolySync();

    return 0;
}
