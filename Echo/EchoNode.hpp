/**
 * @file PolySyncEcho.hpp
 * @brief PolySync Echo Messages Command Line Utility.
 *
 * PUBLIC_HEADER
 */

#ifndef POLYSYNC_ECHO_HPP
#define POLYSYNC_ECHO_HPP

#include <PolySyncDataModel.hpp>
#include <ApplicationInputHandler.hpp>
#include <EchoHelp.hpp>

/**
 * @namespace polysync
 */
namespace polysync
{

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
     * @brief PolySync C++ ApplicationInputHandler class builds on standard C getopt
     *          command line input handling + validation.
     */
    ApplicationInputHandler inputHandler;

    /**
     * @brief PolySync EchoHelp is a user interaction class to assist
     *          end users with command line arguments/options for Echo utility.
     */
    EchoHelp echoHelp;

    /**
     * @brief initStateEvent
     *
     * Subscribe to a message that the publisher node will send.
     */
    void initStateEvent() override;

    /**
     * @brief Register a single, filtered message type per cmd line input.
     */
    void registerSingleFilteredMessage();

    /**
     * @brief messageEvent
     *
     * Extract the information from the provided message
     *
     * @param std::shared_ptr< Message > - variable containing the message
     */
    virtual void messageEvent( std::shared_ptr< polysync::Message > message );

    /**
     * @brief Prints message(s) to external user specified file and std out.
     * @param PolySync Message.
     */
    void printToFile( std::shared_ptr < polysync:: Message > message );

    /**
     * @brief Echo PolySync message(s) on bus to standard out.
     * Messages will print data, or just headers, based on cmd line input.
     */
    void echoPolySyncMessagesToStdOut
        ( std::shared_ptr < polysync:: Message > message );

    /**
     * @brief Calls GetOpt::optionsParse. Standardizes cmd line handling.
     * @param argv Argument vector.
     * @param argc Argument count.
     * @return Returns true if options and option arguments are valid.
     */
    bool optionsParse( const int argc, char * argv[] );

    /**
     * @brief If help -h is one of arguments, don't start node up.
     * @return Returns true if help was requested.
     *
     */
    bool wasHelpRequested();

    /**
     * @brief Wrapper for encapsulation of the PolySyncEchoHelp class.
     */
    void printEchoHelpPage();

private:

    /**
     * @brief Get messages currently on bus and append to end of list.
     *  @return std::vector< std::string > - variable containing msg name.
     */
    std::vector< std::string > getAvailableMessageNames();

    /**
     * @brief Print messages currently available on bus, append to end of list.
     *  @param std::vector< std:: string > - variable containing msg name.
     */
    void printAvailableMessage( const std::vector< std::string > & messageTypeStrings );

}; // END polysync::EchoNode class

} /*!< end namespace polysync */

#endif // POLYSYNC_ECHO_HPP
