/**
 * @file PolySyncGetOpt.hpp
 * @brief PolySync GetOpt Command Line Utility.
 *
 * PUBLIC_HEADER
 */

#ifndef POLYSYNCGETOPT_HPP
#define POLYSYNCGETOPT_HPP

#include <PolySyncDataModel.hpp>
#include <getopt.h>

/**
 * @namespace polysync
 */
namespace polysync
{

/**
 * @brief PolySyncGetOpt class
 *
 * The PolySyncGetOpt C++ class exists to build upon the C-based getopt class.
 * Getopt parses command line input, and validates that input with options,
 * arguments, and option-arguments.
 */
class PolySyncGetOpt
{

public:

    /**
     * @brief Determines whether user supplied cmd line options are supported.
     * @param optchar Character of user supplied option on cmd line.
     * @return Returns -1 if not supported; returns index if supported (O-N)
     */
    int getOptIdx( const char optret );

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
    char * getMsgName()
    { return _msgName; }

    /**
     * @brief Member variable getter for user defined file name.
     * @return Returns char * variable containing message name.
     */
    char * getFileName()
    { return _userFileName; }

    /**
     * @brief Member variable getter for command line getopt handling.
     * @return Returns boolean: true if a single message type was filtered.
     */
    bool wasSingleMsgFiltered()
    { return _filteredForSingleMsgFlag; }

    /**
     * @brief Member variable getter for command line getopt handling.
     * @return Returns boolean: true if only message headers were requested.
     */
    bool wereHeadersRequested()
    { return _echoMessageHeadersOnlyFlag; }

    /**
     * @brief Member variable getter for command line getopt handling.
     * @return Returns boolean: true if user specified external file for output.
     */
    bool wasFileSpecified()
    { return _echoMessageToFileFlag; }

    /**
     * @brief Member variable getter for command line getopt handling.
     * @return Returns boolean: true if user either requested help directly,
     *          or user entered invalid options/arguments on command line.
     *
     * If help flag true, PolySync nodes do not start up; help is displayed.
     */
    bool wasHelpRequested()
    { return _getOptHelpFlag; }

private:

    char * _msgName;
    char * _userFileName;

    bool _filteredForSingleMsgFlag = false;
    bool _echoMessageHeadersOnlyFlag = false;
    bool _echoMessageToFileFlag = false;
    bool _getOptHelpFlag = false;

    const std::vector < char > _optChars
    {
      'f', 'h', 'H', 'o'
    };

}; // END polysync::PolySyncGetOpt class

} /*!< end namespace polysync */

#endif // POLYSYNCGETOPT_HPP
