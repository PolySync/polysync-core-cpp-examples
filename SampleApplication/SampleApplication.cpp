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
 * \example SampleApplication.cpp
 *
 * PolySync Sample Application C++ API example application
 *      Demonstrate how to function as an application node
 *
 */

#include <iostream>

#include <GetOpt.hpp>
#include <PolySyncCore.hpp>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>


using namespace std;


/**
 * @brief SampleApplicationNode class
 *
 * The SampleApplicationNode class allows the user to tap into the existing
 * hooks that are implemented as part of the base Node class. The functions
 * exist in the Node class but are implemented as empty function; the default
 * behavior is to do nothing for a given event.
 * In this sample application, we will overload all of the functions and
 * explain their usage to serve as the basis for future applications.
 */
class SampleApplicationNode : public polysync::Node
{

public:

    /**
     * The messageEvent is the hook that is called whenever an application
     * receives a message from the PolySync bus.  In this application
     * we are sending messages that we have registered for and should receive
     * them here.
     */
    virtual void messageEvent( std::shared_ptr< polysync::Message > message )
    {
        using namespace polysync::datamodel;

        if( auto byteArray = getSubclass< ByteArrayMessage >( message ) )
        {
            byteArray->print();
        }
    }


    /**
     * This function is called by PolySync when the node transitions to the
     * INIT state.  In this instance, we are registering for messages from the
     * bus of a specific type.
     *
     * Other possible initialization functions include connection to a sensor
     * or actuator via a bus, connection to a communication medium, opening a
     * socket, etc.
     */
    virtual void initStateEvent()
    {
        _stateIterationCount = 0;

        cout << NODE_NAME << " is initializing." << endl;

        _messageType = getMessageTypeByName( "ps_byte_array_msg" );

        // Register as a listener for the message type that the publisher
        // is going to send.  Message types are defined in later tutorials.
        registerListener( _messageType );

    }

    /**
     * This is called once after node transitions into the AUTH state.
     * This happens near the start of program, before any PolySync related
     * tasks.
     *
     * If this node is instantiated with argc and argv arguments they will be
     * available for parsing in this function ( ie getopts ).
     *
     * @param [in] commandLineArgs GetOpt object containing option flags, and
     * their arguments.
     */
    virtual void setConfigurationEvent( const GetOpt & commandLineArgs )
    {
        cout << "Printing command line data for " << NODE_NAME << endl << endl;

        // Just a display. This event is where you can handle command line input
        // that make require a valid PolySync node ( ps_node_ref ).
        commandLineArgs.printAll();
    }

    /**
     * PolySync calls the okStateEvent frequently to minimize latency associated
     * any sampling that the function might do.  It is in this function that the
     * the node performs most of its normal processing.  If there is any sensor
     * or actuator interaction, it would happen here; this is the lowest latency
     * of any of the functions in a given node.
     */
    void okStateEvent() 
    {
        // In this instance, like the publisher tutorial, we will send a message
        // on the bus with a sleep to minimize the loading on the bus.

        // Package and ublish a polysync::datamodel::ByteArrayMessage
        publishByteArray( {'P', 'o', 'l', 'y', 'S', 'y', 'n', 'c',
                           ' ', 'S', 'a', 'm', 'p', 'l', 'e' } );

        // Transition state after max iterations
        if( stateHasExpired( ++_stateIterationCount ) )
        {
            changeState( NODE_STATE_WARN );
        }

        // Only allow this event ot happen once per second, this prevents us
        // from flooding the bus
        polysync::sleepMicro( ONE_SECOND );
    }

    /**
     * The fatalStateEvent event is analogous to the release event, except that
     * it occurs after a fatal error has occurred.  If there is anything that
     * absolutely must be done (e.g. shut down a piece of hardware) before the
     * node shuts down this is the place where that can be done.
     */
    virtual void fatalStateEvent()
    {
        cout << NODE_NAME
             << " received a fatal error event and will shut down."  << endl;
    }

    /**
     * The error state event is called when a node enters an error state.  There
     * are a variety of ways that a node can enter this state.  PolySync treats
     * this very much like the @ref okStateEvent() function, calling the
     * function as rapidly as possible.
     *
     * This function sleeps for 1 second to minimize the amount of data being
     * sent to the screen.
     */
    virtual void errorStateEvent()
    {
        cout << NODE_NAME << " is in the error state." << endl;

        polysync::sleepMicro( ONE_SECOND );

        // Transition state after max iterations
        // Note that a FATAL state shuts the node down.
        if( stateHasExpired( ++_stateIterationCount ) )
        {
            changeState( NODE_STATE_FATAL );
        }
    }

    /**
     * The warn state event is called when a node enters an error state. It is
     * specifically that the node can continue to function.  PolySync treats
     * this very much like the @ref okStateEvent() function, calling the
     * function as rapidly as possible.
     *
     * This function sleeps for 1 second to minimize the amount of data being
     * sent to the screen.
     */
    virtual void warnStateEvent()
    {
        // Send a warning and sleep to minimize the loading on the bus.

        // Create the message, set the time and populate the buffer and send it
        publishByteArray( {'W', 'A', 'R', 'N', 'I', 'N', 'G', '!' } );

        // Limit the number of iterations by sleeping for 1 second.
        // The nominal PolySync clock is on a microsecond basis
        polysync::sleepMicro( ONE_SECOND );

        // Transition state after max iterations
        if( stateHasExpired( ++_stateIterationCount ) )
        {
            changeState( NODE_STATE_ERROR );
        }
    }

    /**
     * The release state event is called by PolySync during the normal shutdown
     * sequence.  This application does not require any specific functionality
     * during the shutdown sequence.
     */
    virtual void releaseStateEvent()
    {
        cout << NODE_NAME << " is shutting down." << endl;
    }


private:

    bool stateHasExpired( uint timerTick )
    {
        bool hasExpired = false;

        if( timerTick > ITERATIONS_PER_STATE )
        {
            hasExpired = true;
        }

        return hasExpired;
    }

    void changeState( ps_node_state_kind nextState )
    {
        activateFault( DTC_USAGE, nextState );

        _stateIterationCount = 0;
    }

    void publishByteArray( const std::vector< uchar > && bytes )
    {
        polysync::datamodel::ByteArrayMessage message( *this );

        message.setHeaderTimestamp( polysync::getTimestamp() );

        message.setBytes( bytes );

        message.publish();
    }

public:

    static constexpr uint ITERATIONS_PER_STATE = 4;

    static constexpr auto NODE_NAME = "polysync-sample-application-cpp";

    static constexpr ps_timestamp ONE_SECOND = 1000000;


private:

    /**
     * The application should contain a variable that defines the type (or
     * types) of messages that the application wants to receive from the
     * PolySync bus.
     */
    ps_msg_type _messageType;

    uint _stateIterationCount;

};

/**
 * Entry point for the sample application.  If there are command-line arguments
 * they should be handled here and used to configure the application after
 * instantiation.
 *
 * The "connectPolySync" is a blocking call and does not return.  Use Ctrl-C to
 * exit.
 *
 * @return int - exit code
 */
int main( int argc, char * argv[] )
{
    SampleApplicationNode sampleAppNode;

    sampleAppNode.setNodeName( SampleApplicationNode::NODE_NAME );

    sampleAppNode.setCommandLineOptions( { argc, argv } );

    sampleAppNode.connectPolySync();

    return 0;
}
