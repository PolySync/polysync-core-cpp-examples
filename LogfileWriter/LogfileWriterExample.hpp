/**
 * @file LogfileWriterExample.hpp
 *
 * PUBLIC_HEADER
 */

#ifndef LOGFILEWRITERNODE_HPP
#define LOGFILEWRITERNODE_HPP


#include <PolySyncNode.hpp>


// Forward declaration
namespace polysync{
    class Logfile;
}


/**
 * @brief The LogfileWriterNode class
 *
 * @li Implement an interface to polysync::Node
 * @li Open PolySync *.plog file using the PolySync C++ Logfile API.
 * @li On each call to @ref okStateEvent() write a
 * polysync::datamodel::ByteArrayMessage to the logfile.
 */
class LogfileWriterNode : public polysync::Node
{

public:

    /**
     * @brief Destructor
     *
     * @li Call polysync::Node::Node() -- Base class constructor
     */
    LogfileWriterNode();


    /**
     * @brief Default Desctructor -- Does nothing
     *
     * @note Dynamically allocated resources are deleted in @ref
     * releaseStateEvent();
     */
    virtual ~LogfileWriterNode() = default;

   /**
    * @note Default use case is on a single machine. Uncomment statements
    * provided in the source to aid with distributed hosts.
    *
    * @li Set path to *.plog file
    * @li Set Logfile mode to read
    * @li Set Logfile state to enabled
    */
    virtual void prepareLogfileToWrite();

    /**
     * @li Create polysync::datamodel::ByteArrayMessage
     * @li Write message to the Logfile queue
     * @li Every 20 messages, notify user via std out.
     */
    virtual void writeMessage();

    /**
     * @li Output number of messages processed
     * @li Notify user that end of file was reached
     */
    virtual void printResults();

protected:

    // allocate Logfile in initstate
    virtual void initStateEvent() override;

    // create message and write to file, disconnect.
    virtual void okStateEvent() override;

    // called once upon disconnect. Validate here with mode.
    virtual void releaseStateEvent() override;

protected:

    // Compile-time constants
    // Default path assumes that the C or C++ Log Writer has been used.
    static constexpr auto DefaultLogfilePath = "/tmp/polysync_logfile.plog";

    static constexpr ps_timestamp FiveMilliSeconds = 5000;

    static constexpr ps_timestamp FiveSeconds = 5000000;

protected:

    polysync::Logfile * _logFile;

    int _numMessagesWritten;

}; // END LogFileTestNode

#endif // LOGFILEWRITERNODE_HPP
