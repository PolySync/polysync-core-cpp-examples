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
 * \example Echo.cpp
 *
 * PolySync Echo C++ API example application
 * 
 * Demonstrates a command line utility for echoing PolySync messges to stdout
 *
 * @file Echo.cpp
 * @brief Echo Source
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
/*
    PolySyncEcho ( int echoArgc, char * echoArgv [] )
        : Node(echoArgc, echoArgv)
    {

    }
*/

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
        // if user filters -filterMessage ps_lidar_etc,
        // then register as listener for event type publisher will send.
        if ( _filteredForSingleMsgFlag )
        {
            try 
            {    
                registerListener( getMessageTypeByName ( _msgName ) );
            }

            catch ( ... ) 
            {        
                std::cout << "\nPlease enter a valid PS message type. \n";

                printAvailableMessage( getAvailableMessageNames() );

                disconnectPolySync();
            }
        }

        // else register listener to all message types.
        else 
        {        
            registerListenerToAllMessageTypes();
        }
    }

    /*
    * @brief Parse arguments: Filter on a single message type.
    * 
    * PSR-29-F05 : ( PS-147 , PS-203, PS-149, PS-150 )
    */
    void setConfigurationEvent( int argc, char * argv [] ) override
    {
        std::string filterFlag = "-filterMessage";
        std::string echoHeadersOnly = "-echoHeaders";

        // check if argv to filtermessage is used.  
        //std::cout << "argc is number" << argc << "\n" ;
        if( argc > 2 )
        {
            for( auto idx = 1; idx < argc ; ++idx )
            {
                if( ( argv[ idx ] == filterFlag )
                      && ( argv[ idx + 1 ] == echoHeadersOnly ) )
                {
                    _filteredForSingleMsgFlag = true;

                    _echoMessageHeadersOnly = true;

                   _msgName = argv[ idx + 2 ];

                }

                else if( ( argv[ idx ] == filterFlag )
                           && ( argv[ idx +1 ] != echoHeadersOnly ) )
                {
                    _filteredForSingleMsgFlag = true;

                    _echoMessageHeadersOnly = false;
		    	 
                   _msgName = argv[ idx + 1 ];

                }

            }
        }

        else if ( argc == 1)
        {
            _filteredForSingleMsgFlag = false;
        }

        else
        {
            cout << "Please include message type after filter option." << endl
                 << "Example: \n"
                 << "polysync-echo -filterMessage ps_diagnostic_trace_msg" <<endl;
            disconnectPolySync();
        }
    }
    
    /**
     * @brief messageEvent
     * 
     * Extract the information from the provided message
     * 
     * @param std::shared_ptr< Message > - variable containing the message
     * @return void
     *
     * PSR-29-F05 : ( PS-149, PS-150 )
     */
    virtual void messageEvent( std::shared_ptr< polysync::Message > message )
    {
        if ( _echoMessageHeadersOnly )
        {
            message->printHeader();
        }

        else
        {
            message->print();
        }
        //message->getHeader();

    }

private:

    std::string _msgName;  
    bool _filteredForSingleMsgFlag = false;
    bool _echoMessageHeadersOnly = false;

    /** @brief Get messages currently on bus and append to end of list.
     *  @return std::vector< std::string > - variable containing msg name.
     *
     *  PSR-29-F05 : ( PS-203 )
     */
    std::vector< std::string > getAvailableMessageNames()
    {
        std::vector< std::string > messageNames;

        for( auto index = 1;
             index < getAvailableMessageCount() + 1;
             ++index )
        {
            messageNames.emplace_back( getMessageNameByType( index ) );
        }

        return messageNames;
    }

    /** @brief Print messages currently available on bus, append to end of list.
     *  @param std::vector< std:: string > - variable containing msg name.
     *
     *  PSR-29-F05 : ( PS-203 )
     */
    void printAvailableMessage( const std::vector< std::string > & messageTypeStrings )
    {
        for( auto messageTypeString : messageTypeStrings )
        {
            cout << "    " << messageTypeString << endl;
        }
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

    // PSR-29-F05 : ( PS-147, PS-203, PS-149, PS-150 ) Filter single msg type.
    echo.setArgumentCount ( argc );
    echo.setArgumentBuffer ( argv );

   // When the node has been created, it will cause an initStateEvent to
    // to be sent.
    echo.connectPolySync();

    return 0;
}
