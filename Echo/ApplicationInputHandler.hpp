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
    int getFlagIndex( const char optionArgumentIndex );

    /**
     * @brief Parses cmd line arguments in C getopt style.
     * @param argv Argument vector.
     * @param argc Argument count.
     * @return Returns true if options were parsed successfully.
     */
    bool optionsParse( const int argc, char * argv[] );

    /**
     * @brief Member variable getter for message(s) name(s).
     * @return Returns std::string containing message name.
     */
    std::string getSingleMessageName() const;

    /**
     * @brief Member variable getter for multiple filtered message names.
     * @return Returns std::vector <std::string> containing multiple message names.
     */
    std::vector < std::string > getMultipleMessageNames() const;

    /**
     * @brief Member variable getter for user-defined file name, so that
     *        user can print Echo output to file.
     * @return Returns std::string containing user-defined file name.
     */
    std::string getFileName() const;

    /**
     * @brief Member variable getter for command line getopt handling.
     * @return Returns true if a single message type was filtered.
     */
    bool singleMessageWasFiltered() const;

    /**
     * @brief Member variable getter for command line getopt handling.
     * @return Returns true if multiple message types were filtered.
     */
    bool multipleMessagesWereFiltered() const;

    /**
     * @brief Member variable getter for command line getopt handling.
     * @return Returns true if only message headers (not data) were requested.
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

    /**
     * @brief Member veriable getter for command line getopt handling.
     * @return Returns true if user specified a specific runtime for Echo.
     */
    bool wasRunTimeSpecified() const;

    /**
     * @brief Member variable getter for command line getopt handling.
     * @return Returns unsigned long long to compute difference in UTC microsec.
     */
    unsigned long long getUserRunTime() const;

private:

    std::string _messageName;
    std::string _userFileName;
 //   std::string _userRunTime; // cast to unsigned long long

    unsigned long long _echoRunTime;

    bool _filteredForSingleMessageFlag = false;
    bool _filteredForMultipleMessagesFlag = false;
    bool _echoMessageHeadersOnlyFlag = false;
    bool _echoMessageToFileFlag = false;
    bool _getOptHelpFlag = false;
    bool _runTimeSpecifiedFlag = false;

    const std::vector < char > _optionInputFlags
    {
      'f', 'h', 'H', 'o', 't'
    };

    std::vector < std::string > _multipleFilteredMessageNames;

}; // END polysync::ApplicationInputHandler class

} /*!< end namespace polysync */

#endif // APPLICATIONINPUTHANDLER_HPP
