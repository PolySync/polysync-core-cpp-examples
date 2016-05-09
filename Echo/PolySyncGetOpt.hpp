/**
 * @file PolySyncGetOpt.hpp
 * @brief PolySync GetOpt Command Line Utility.
 *
 * PUBLIC_HEADER
 */

#ifndef POLYSYNCGETOPT_HPP
#define POLYSYNCGETOPT_HPP

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
class PolySyncGetOpt
{
public:
    PolySyncGetOpt();

    // GetOpt:: getters
    bool wasSingleMsgFiltered();

    bool wereHeadersRequested();

    bool wasFileSpecified();

    bool wasHelpRequested();

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

};




} // namespace polysync


#endif // POLYSYNCGETOPT_HPP
