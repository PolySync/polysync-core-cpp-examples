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
 * \example ParameterGetSet.cpp
 * 
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
 * In order to demonstrate the 'set' ability, you should define a single video-device
 * node in the SDF. The example will locate and get the published image width,
 * then set the active coordinate frame identifier. More importantly, it demonstrates
 * the process to follow to get and set any parameter.
 *
 */

#include <iostream>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

#include <vector>

using namespace std;

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
    const string _nodeName = "polysync-parameter-get-set-cpp";
    const string _parameterMessageName = "ps_parameters_msg";
    
    ps_msg_type _messageType;
    ps_guid _destGUID;
    bool _setPerformed = false;
    bool _setShouldBePerformed = false;
    
public:
    
    ParameterGetSet()
    {
        setNodeType( PSYNC_NODE_TYPE_API_USER );
        setDomainID( PSYNC_DEFAULT_DOMAIN );
        setSDFID( PSYNC_SDF_ID_INVALID );
        setFlags( PSYNC_INIT_FLAG_STDOUT_LOGGING );
        setNodeName( _nodeName );
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
     * all parameters from all nodes on the PolySync bus. We can print all
     * incoming parameters in the message event handler
     *
     * @param void
     * @return void
     */

    void initStateEvent() override
    {
        // get parameter message type
        _messageType = getMessageTypeByName( _parameterMessageName );
        
        // register as a listener for parameter messages that any
        // node may send
        // data is available in the messageEvent function below
        registerListener( _messageType );

        // set quality of service for the Parameter message type subscriber
        setSubscriberReliabilityQOS( _messageType, RELIABILITY_QOS_RELIABLE );

        // set quality of service for the Parameter message type subscriber
        setPublisherReliabilityQOS( _messageType, RELIABILITY_QOS_RELIABLE );

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
        //
        // create a parameter message that requests parameters from
        // all active nodes on the PolySync bus
        //

        // create a message
        polysync::datamodel::ParametersMessage parameterMessage( *this );

        // set message header timestamp
        parameterMessage.setHeaderTimestamp( polysync::getTimestamp() );

        // PSYNC_GUID_INVALID requests all nodes parameters, rather than a single GUID
        parameterMessage.setDestGuid( PSYNC_GUID_INVALID );

        // create a parameter which will hold the Parameter ID
        polysync::datamodel::Parameter param;

        // request all parameters, rather than a single ID
        param.setId( PSYNC_PARAM_ID_ALL );

        // add our 'get all' parameter to the parameter messages list
        parameterMessage.setParameters( { param } );

        // set parameter message type to get all parameters
        // you can filter for specific message types/ID's here
        parameterMessage.setType( PARAMETER_MESSAGE_GET_ALL );

        // publish to the PolySync bus
        parameterMessage.publish();


        //
        // use a parameter message to set a specific nodes parameter value
        //

        // have we seen the video device on bus yet?
        // the video deivce GUID is extracted from the incoming parameter message in the event handler
        if( _setShouldBePerformed == true )
        {
            // have we already set the value?
            if( _setPerformed == false )
            {
                //
                _setPerformed = true;

                //
                // demonstrate the 'set' operations for the IPC Parameter API
                //

                //
                // create a message
                polysync::datamodel::ParametersMessage parameterSetMessage( *this );

                // set message data
                parameterSetMessage.setHeaderTimestamp( polysync::getTimestamp() );

                // set the node GUID we which to receive this message
                parameterSetMessage.setDestGuid( _destGUID );

                // set parameter message type to get all parameters
                // you can filter for specific message types/ID's here
                // currently step, min and max are not used, though they are functional for custom use
                parameterSetMessage.setType( PARAMETER_MESSAGE_SET_VALUE );


                //
                // create a list of parameters to set
                std::vector< polysync::datamodel::Parameter > parameters;

                // create a parameter and set the ID for the parameter to set the value of
                polysync::datamodel::Parameter myParam;
                myParam.setId( PSYNC_PARAM_ID_COORDINATE_FRAME );


                //
                polysync::datamodel::ParameterValue paramValue;

                // set the descriminator type
                // options are: unsigned long long, long long, double, and string
                paramValue.setParameterValueKind( PARAMETER_VALUE_ULONGLONG );

                // set the parameter-value value field
                paramValue.setUllValue( PSYNC_COORDINATE_FRAME_PLATFORM );

                // set the parameters value
                myParam.setValue( paramValue );

                // insert the parameter into the list
                parameters.emplace_back( myParam );

                // set the parameter list we cretaed
                parameterSetMessage.setParameters( parameters );

                // Publish to the PolySync bus
                parameterSetMessage.publish();
            }
        }

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
        //
        // handle the newly received message
        //

        using namespace polysync::datamodel;

        if( std::shared_ptr< ParametersMessage > parameterMsg =
                getSubclass< ParametersMessage >( message ) )
        {  
            // print all parameter messages seen on the PolySync bus
            //parameterMsg->print();

            // create a list of parameters, and fill with the incoming parameter message buffer
            std::vector< polysync::datamodel::Parameter > parameters = parameterMsg->getParameters();
            for( polysync::datamodel::Parameter param : parameters )
            {
                // check the incoming parameter ID
                if( param.getId() == PSYNC_PARAM_ID_VIDEO0_PUBLISHED_HEIGHT )
                {
                    // set destination GUID, which is the node that published this message
                    _destGUID = parameterMsg->getSourceGUID();
                    if( _setPerformed == false  )
                    {
                        // should we 'set' the parameter (in the ok state)
                        _setShouldBePerformed = true;
                    }

                    // extract the value from the parameter
                    ParameterValue width = param.getValue();

                    printf("Video device - GUID: %llu - Published image pixel width: %llu\n", _destGUID, width.getUllValue() );
                }
            }
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
    // create an instance of the ParameterGetSet and connect it to
    // PolySync.
    ParameterGetSet parameterGetSetNode;

    // when the node has been created, it will cause an initStateEvent to be
    // sent and then proceed into the okState.  connectToPolySync does not
    // return, use Ctrl-C to exit.
    parameterGetSetNode.connectPolySync();

    return 0;
}
