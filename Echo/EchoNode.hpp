/**
 * @file PolySyncEcho.hpp
 * @brief PolySync Echo Messages Command Line Utility.
 *
 * PUBLIC_HEADER
 */

#ifndef POLYSYNC_ECHO_HPP
#define POLYSYNC_ECHO_HPP

#include <PolySyncDataModel.hpp>

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
    * @brief Parse arguments: Filter on a single message type.
    *
    * PSR-29-F05 : ( PS-147 , PS-203, PS-149, PS-150 )
    */
    void setConfigurationEvent( int argc, char * argv [] ) override;

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
     * @brief Validates arguments; no nodes connect unless arguments valid.
     * @param argc
     * @param argv
     * @return Returns true if arguments valid and falls through to connect.
     *
     */
    bool validArgs( int argc, char * argv [] );

    /**
     * @brief If help -h is one of arguments, don't start node up.
     * @param argc
     * @param argv
     *
     * PSR-29-F11 : ( PS-153)
     */
    bool helpRequested( int argc, char * argv [] );

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

    std::string _msgName;
    std::string _userFileName;

    bool _filteredForSingleMsgFlag = false;
    bool _echoMessageHeadersOnlyFlag = false;
    bool _echoMessageToFileFlag = false;
    bool _echoHelpFlag = false;

    const std::string _echoHelpOpt = "-h";
    const std::string _filterMessageOpt = "-f";
    const std::string _echoHeadersOnlyOpt = "-H";
    const std::string _echoToFileOpt = "-o";

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
