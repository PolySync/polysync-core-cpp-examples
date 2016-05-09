/**
 * @file PolySyncEcho.hpp
 * @brief PolySync Echo Messages Command Line Utility.
 *
 * PUBLIC_HEADER
 */

#ifndef POLYSYNC_ECHO_HPP
#define POLYSYNC_ECHO_HPP

#include <PolySyncDataModel.hpp>
//#include <unistd.h>
//#include <stdlib.h>
#include <getopt.h>


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
     * @brief initStateEvent
     *
     * Subscribe to a message that the publisher node will send.
     *
     * @param void
     * @return void
     */
    void initStateEvent() override;

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
    virtual void messageEvent( std::shared_ptr< polysync::Message > message );

    /**
     * @brief Print msgs to external file and standard out with -o option.
     * @param PolySync Message(s).
     */
    void printMsgToFile( std::shared_ptr< polysync::Message > message );

    /**
     * @brief Determines whether user supplied cmd line options are supported.
     * @param optchar Character of user supplied option on cmd line.
     * @return Returns -1 if not supported; returns index if supported (O-N)
     */
    int getOptIdx( const char optchar );

    /**
     * @brief Parses cmd line arguments in C getopt style.
     * @param argv Argument vector.
     * @param argc Argument count.
     * @return
     */
    bool optionsParse( const int argc, char * argv[] );


    // GetOpt:: getters
    bool wasSingleMsgFiltered();

    bool wereHeadersRequested();

    bool wasFileSpecified();

    bool wasHelpRequested();

    /**
     * @brief Get strings for help page display of cmd line flags.
     * @return std::vector< std::string > - variable contains help descriptions.
     *
     * PSR-29-F11 : ( PS-153)
     */
    std::vector< std::string > getHelpFlags();

    /**
     * @brief Get strings for help page display of descriptions for cmd line flags.
     * @return std::vector< std::string > - variable contains help descriptions.
     *
     * PSR-29-F11 : ( PS-153)
     */
    std::vector< std::string > getHelpDescriptions();

    /**
     * @brief Print strings for help page display, append to end of list.
     * @param std::vector< std:: string > - variable containing help string.
     */
    void printHelp( const std::vector< std::string > & helpFlags,
                    const std::vector< std::string > & helpDescriptions);

private:

    // GetOpt::
    char * _msgName;
    char * _userFileName;
    /*
    char * GetOpt::getMsgName() { return _msgName; }
    char * GetOpt::getUserFileName() { return _userFileName; }
    */

    // GetOpt:: member variables, and use getters.
    bool _filteredForSingleMsgFlag = false;
    bool _echoMessageHeadersOnlyFlag = false;
    bool _echoMessageToFileFlag = false;
    bool _echoHelpFlag = false;

    // GetOpt::
    const std::vector < char > _optChars
    {
      'f', 'h', 'H', 'o'
    };

    /** @brief Get messages currently on bus and append to end of list.
     *  @return std::vector< std::string > - variable containing msg name.
     *
     *  PSR-29-F05 : ( PS-203 )
     */
    std::vector< std::string > getAvailableMessageNames();

    /** @brief Print messages currently available on bus, append to end of list.
     *  @param std::vector< std:: string > - variable containing msg name.
     *
     *  PSR-29-F05 : ( PS-203 )
     */
    void printAvailableMessage( const std::vector< std::string > & messageTypeStrings );
};

} /*!< end namespace polysync */

#endif // POLYSYNC_ECHO_HPP
