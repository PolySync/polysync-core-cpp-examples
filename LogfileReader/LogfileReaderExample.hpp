/**
 * @file LogfileReaderExample.hpp
 *
 * PUBLIC_HEADER
 */

#ifndef LOGFILEQUEUEREADERNODE_HPP
#define LOGFILEQUEUEREADERNODE_HPP


#include <PolySyncNode.hpp>


// Forward declaration
namespace polysync{
    class Logfile;
}


/**
 * @brief The LogfileReaderNode class
 *
 * @li Implement an interface to polysync::Node
 * @li Open PolySync *.plog file using the PolySync C++ Logfile API.
 * @li Override polysync::Node::messageEvent() to receive data that has been
 * extracted from the logfile and published to the PolySync bus via the Logfile
 * API.
 */
class LogfileReaderNode : public polysync::Node
{

public:

    /**
     * @brief Destructor
     *
     * @li Call polysync::Node::Node() -- Base class constructor
     */
    LogfileReaderNode();

    /**
     * @brief Default Desctructor -- Does nothing
     *
     * @note Dynamically allocated resources are deleted in @ref
     * releaseStateEvent();
     */
    virtual ~LogfileReaderNode() = default;


protected:

    /**
     * @note Uncomment the first statement to demonstrate message filtering.
     *
     * @li Set path to *.plog file
     * @li Set Logfile mode to read
     * @li Set Logfile state to enabled
     */
    virtual void prepareLogfileToRead();

    /**
     * @li Output number of messages processed
     * @li Notify user that end of file was reached
     */
    virtual void printResults();


    /**
     * @note Called once after polysync::Node::connectPolySync() is called
     *
     * @li Allocate Logfile object
     * @li Call @ref prepareLogfileToRead()
     * @li If something went wrong, disconnectPolySync();
     */
    virtual void initStateEvent() override;

    /**
     * @note Called repeatedly during runtime
     *
     * @li Print message data to std out
     * @li @ref disconnectPolySync() if end of logfile has been reached
     */
    virtual void okStateEvent() override;

    /**
     * @note Called once, after polysync::Node::disconnectPolySync()
     *
     * @li call @ref printResults()
     * @li Set Logfile mode to off
     * @li delete dynamically allocated memory
     */
    virtual void releaseStateEvent() override;

    /**
     * @note Called repeatedly, any time data is available from the PolySync
     * bus.
     *
     * @param std::shared_ptr< PolySyncMessage >
     */
    virtual void messageEvent(
            std::shared_ptr< polysync::Message > message ) override;


protected:

    // Compile-time constants
    // Default path assumes that the C or C++ Log Writer has been used.
    static constexpr auto DefaultLogfilePath = "/tmp/polysync_logfile.plog";

    static constexpr ps_timestamp FiveSeconds = 5000000;

    static constexpr ps_timestamp DefaultSessionId = 1;

    static constexpr ps_timestamp DefaultTimingOffset = 1;

    static constexpr uint Enable = 1;

protected:

    polysync::Logfile * _logFile;

    int _numMessagesRead;

};


#endif // LOGFILEQUEUEREADERNODE_HPP
