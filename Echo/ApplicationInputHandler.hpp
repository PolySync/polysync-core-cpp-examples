/**
 * @file ApplicationInputHandler.hpp
 * @brief Input Handling for Echo Command Line Utility.
 *
 * PUBLIC_HEADER
 */

#ifndef APPLICATIONINPUTHANDLER_HPP
#define APPLICATIONINPUTHANDLER_HPP

/**
 * @namespace polysync
 */
namespace polysync
{

/**
 * @brief ApplicationInputHandler class
 *
 * The ApplicationInputHandler C++ class exists to build upon the C-based getopt class.
 * Getopt parses command line input, and validates that input with options,
 * arguments, and option-arguments.
 */
class ApplicationInputHandler
{

public:

    /**
     * @brief Determines whether user supplied cmd line options are supported.
     * @param optchar Character of user supplied option on cmd line.
     * @return Returns -1 if not supported; returns index if supported (O-N)
     */
    int getFlagIndex( const char optret );

    /**
     * @brief Parses cmd line arguments in C getopt style.
     * @param argv Argument vector.
     * @param argc Argument count.
     * @return
     */
    bool optionsParse( const int argc, char * argv[] );

    /**
     * @brief Member variable getter for message(s) name(s).
     * @return Returns char * variable containing message name.
     */
    std::string getMessageName() const;

    /**
     * @brief Member variable getter for user defined file name.
     * @return Returns char * variable containing message name.
     */
    std::string getFileName() const;

    /**
     * @brief Member variable getter for command line getopt handling.
     * @return Returns boolean: true if a single message type was filtered.
     */
    bool singleMessageWasFiltered() const;

    /**
     * @brief Member variable getter for command line getopt handling.
     * @return Returns boolean: true if only message headers were requested.
     */
    bool headersWereRequested() const;

    /**
     * @brief Member variable getter for command line getopt handling.
     * @return Returns boolean: true if user specified external file for output.
     */
    bool fileWasSpecified() const;

    /**
     * @brief Member variable getter for command line getopt handling.
     * @return Returns boolean: true if user either requested help directly,
     *          or user entered invalid options/arguments on command line.
     *
     * If help flag true, PolySync nodes do not start up; help is displayed.
     */
    bool helpWasRequested() const;

private:

    std::string _messageName;
    std::string _userFileName;

    bool _filteredForSingleMessageFlag = false;
    bool _echoMessageHeadersOnlyFlag = false;
    bool _echoMessageToFileFlag = false;
    bool _getOptHelpFlag = false;

    const std::vector < char > _optIoninputflags
    {
      'f', 'h', 'H', 'o'
    };

}; // END polysync::ApplicationInputHandler class

} /*!< end namespace polysync */

#endif // APPLICATIONINPUTHANDLER_HPP
