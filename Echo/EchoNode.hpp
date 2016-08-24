/**
 * @file PolySyncEcho.hpp
 * @brief PolySync Echo Messages Command Line Utility.
 *
 * PUBLIC_HEADER
 */

#ifndef POLYSYNC_ECHO_HPP
#define POLYSYNC_ECHO_HPP

#include <PolySyncDataModel.hpp>

#include "ApplicationInputHandler.hpp"
#include "EchoHelp.hpp"


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
     * @brief Override polysync::Node::initStateEvent()
     *
     * Subscribe to a message that the publisher node will send.
     */
    void initStateEvent() override;

    /**
     * @brief Override polysync::Node::okStateEvent()
     * Called continuously while in ok state.
     */
    void okStateEvent() override;

    /**
     * @brief Register filtered message type(s) per cmd line input.
     */
    void registerFilteredMessages();

    /**
     * @brief Safely register message listeners.
     * Called by registerSingle/MultipleFilteredMessages.
     *
     * @param std::string messageName.
     */
    void tryCatchRegisterAMessageListener ( std::string );

    /**
     * @brief Extract the information from the provided message
     *
     * @param std::shared_ptr< Message > - variable containing the message
     */
    virtual void messageEvent( std::shared_ptr< polysync::Message > message );

    /**
     * @brief Prints message(s) to external user specified file and std out.
     *
     * @param PolySync Message.
     */
    void printToFile( std::shared_ptr < polysync:: Message > message );

    /**
     * @brief Echo PolySync message(s) on bus to standard out.
     * Messages will print data, or just headers, based on cmd line input.
     */
    void printMessage( std::shared_ptr < polysync:: Message > message ) const;

    /**
     * @brief Calls GetOpt::optionsParse. Standardizes cmd line handling.
     *
     * @param argv Argument vector.
     * @param argc Argument count.
     *
     * @return Returns true if options and option arguments are valid.
     */
    bool optionsParse( const int argc, char * argv[] );

    /**
     * @brief If help -h is one of arguments, don't start node up.
     *
     * @return Returns true if help was requested.
     */
    bool helpWasRequested() const;

    /**
     * @brief Wrapper for encapsulation of the EchoHelp class.
     */
    void printEchoHelpPage() const;


private:

    /**
     * @brief Get messages currently on bus and append to end of list.
     *
     *  @return std::vector< std::string > - variable containing msg name.
     */
    std::vector< std::string > getAvailableMessageNames() const;

    /**
     * @brief Print messages currently available on bus, append to end of list.
     *
     *  @param std::vector< std:: string > - variable containing msg name.
     */
    void printAvailableMessage(
            const std::vector< std::string > & messageTypeStrings ) const;

    /**
     * Output list of message types that have been published by the current
     * PolySync runtime.
     *
     * @note This does not print all types available in the PolySync data model.
     */
    void printActiveMessageTypes() const;

    /**
     * Has this node's duration, updated in the okStateEvent, reached the
     * duration input from the command line (if any).
     *
     * @return true if time is up
     */
    bool durationReached() const;

    /**
     * @return Difference between start time and current time.
     */
    ps_timestamp getDuration() const;


private:

    /**
     * Magic number wrapper. One second == 1000000 micro.
     */
    static constexpr ps_timestamp _oneSecond = 1e6;

    /**
     * Manage command line input arguments.
     */
    ApplicationInputHandler inputHandler;

    /**
     * Wrap notification to std::cout
     */
    EchoHelp echoHelp;

    /**
     * @brief Member variables to run Echo for a particular time.
     * @note getTimestamp() is a PolySync function that returns message header
     *    timestamps in UTC microseconds.
     * Start run time: see PolySyncEcho's overridden initState() function.
     * Current run time: see PolySyncEcho's overridden okStateEvent() function.
     */
    ps_timestamp _applicationStartTime;

}; // END polysync::EchoNode class


#endif // POLYSYNC_ECHO_HPP
