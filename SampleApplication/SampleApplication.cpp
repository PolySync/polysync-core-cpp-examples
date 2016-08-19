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
#include <PolySyncCore.hpp>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

using namespace std;

const uint ITERATIONS_PER_STATE = 4;
const string NODE_NAME( "polysync-sample-application-cpp" );

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
private:
    /**
     * @brief ps_msg_type
     *
     * The application should contain a variable that defines the type (or
     * types) of messages that the application wants to receive from the
     * PolySync bus.
     */
    ps_msg_type _messageType;
    unsigned int _timerTick;

public:
    /**
     * @brief messageEvent
     *
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
     * @brief okStateEvent
     *
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

        // Create the message, set the time and populate the buffer and send it
        polysync::datamodel::ByteArrayMessage message( *this );
        message.setHeaderTimestamp( polysync::getTimestamp() );
        message.setBytes( {'P', 'o', 'l', 'y', 'S', 'y', 'n', 'c',
                           ' ', 'S', 'a', 'm', 'p', 'l', 'e' } );
        message.publish();

        // Limit the number of iterations by sleeping for 1 second.
        // The nominal PolySync clock is on a microsecond basis
        polysync::sleepMicro( 1000000 );

        // Spend some iterations in this state and transition to the next
        _timerTick++;

        if ( _timerTick > ITERATIONS_PER_STATE )
        {
            activateFault( DTC_USAGE, NODE_STATE_WARN );
            _timerTick = 0;
        }
    }

    /**
     * @brief fatalStateEvent
     *
     * The fatalStateEvent event is analogous to the release event, except that
     * it occurs after a fatal error has occurred.  If there is anything that
     * absolutely must be done (e.g. shut down a piece of hardware) before the
     * node shuts down this is the place where that can be done.
     */
    virtual void fatalStateEvent()
    {
        cout << NODE_NAME << " received a fatal error event and will shut down."  << endl;
    }

    /**
     * @brief errorStateEvent
     *
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

        polysync::sleepMicro( 1000000 );

        // Spend some iterations in this state and transition to the next
        // Note that a FATAL state shuts the node down.
        _timerTick++;

        if ( _timerTick > ITERATIONS_PER_STATE )
        {
            activateFault( DTC_USAGE, NODE_STATE_FATAL );
        }
    }

    /**
     * @brief warnStateEvent
     *
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
        polysync::datamodel::ByteArrayMessage message( *this );
        message.setHeaderTimestamp( polysync::getTimestamp() );
        message.setBytes( {'W', 'A', 'R', 'N', 'I', 'N', 'G', '!' } );
        message.publish();

        // Limit the number of iterations by sleeping for 1 second.
        // The nominal PolySync clock is on a microsecond basis
        polysync::sleepMicro( 1000000 );

        // Spend some iterations in this state and transition to the next
        _timerTick++;

        if ( _timerTick > ITERATIONS_PER_STATE )
        {
            activateFault( DTC_USAGE, NODE_STATE_ERROR );
            _timerTick = 0;
        }
    }

    /**
     * @brief releaseStateEvent
     *
     * The release state event is called by PolySync during the normal shutdown
     * sequence.  This application does not require any specific functionality
     * during the shutdown sequence.
     */
    virtual void releaseStateEvent()
    {
        cout << NODE_NAME << " is shutting down." << endl;
    }

    /**
     * @brief initStateEvent
     *
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
        _timerTick = 0;

        cout << NODE_NAME << " is initializing." << endl;

        _messageType = getMessageTypeByName( "ps_byte_array_msg" );

        // Register as a listener for the message type that the publisher
        // is going to send.  Message types are defined in later tutorials.
        registerListener( _messageType );

    }

    /**
     * @brief setConfigurationEvent
     *
     * This is called once after node transitions into the AUTH state.
     * This happens near the start of program, before any PolySync related
     * tasks.
     *
     * If this node is instantiated with argc and argv arguments they will be
     * available for parsing in this function ( ie getopts ).
     *
     * @param [in] argc Number of strings provided in argv.
     * @param [in] argv Buffer of strings.
     */
    virtual void setConfigurationEvent( int argc, char *argv[] )
    {
        cout << NODE_NAME << " received a configuration event." << endl;

        if (argc > 0)
        {
            for (int i = 0; i < argc; i++)
            {
                cout << "Parameter: " << argc << " : " << argv[i] << endl;
            }
        }
        else
        {
            cout << "No configuration parameters to display." << endl;
        }
    }
};

/**
 * @brief main
 *
 * Entry point for the sample application.  If there are command-line arguments
 * they should be handled here and used to configure the application after
 * instantiation.
 *
 * The "connectPolySync" is a blocking call and does not return.  Use Ctrl-C to
 * exit.
 *
 * @return int - exit code
 */
int main()
{
    SampleApplicationNode sampleAppNode;

    sampleAppNode.setNodeName(NODE_NAME);

    sampleAppNode.connectPolySync();

    return 0;
}
