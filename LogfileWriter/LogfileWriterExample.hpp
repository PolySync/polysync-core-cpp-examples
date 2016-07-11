/**
 * @file LogFileTestNode.hpp
 * @brief PolySync Node used to test the PolySyncLogFile class functionality.
 *
 * PUBLIC_HEADER
 */

#ifndef LOGFILEWRITERNODE_HPP
#define LOGFILEWRITERNODE_HPP

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>
#include <PolySyncLogfile.hpp>

class LogfileWriterNode : public polysync::Node
{

public:

    /**
     * @brief LogfileTestNode Empty Constructor
     * Initialize private variables and call polysync::Node::Node()
     * (base class constructor)
     */
    LogfileWriterNode();

    /**
     * @brief Called from initStateEvent(), this function sets up all the
     * parameters needed in order to Record (write) a Logfile.
     *
     * @note Sets filterOutMessages() by type (optional).
     * Sets filepath (optional) and Session Id.
     * Sets mode to write, and enables state.
     * Sets _numMessagesWritten to 0; final result printed in releaseStateEvent.
     *
     * @note After this function is called in initStateEvent(), fall through
     * to okStateEvent() to write a single message for each okStateEvent()
     * loop.
     */
    void prepareLogfileToWrite();

    /**
     * @brief Writes a single message for each okStateEvent() loop until
     * Ctrl + C.
     */
    void writeMessage();

    /**
     * @brief Prints results: if _messagesWereWritten, _numMessagesWritten.
     * If _messagesWereRead, _numMessagesRead.
     * If _logFileWasIterated, iterator completed.
     */
    void printResults();

protected:

    // allocate Logfile in initstate
    void initStateEvent() override;

    // create message and write to file, disconnect.
    void okStateEvent() override;

    // called once upon disconnect. Validate here with mode.
    void releaseStateEvent() override;

private:

    polysync::Logfile * _logFile;

    int _numMessagesWritten;

    int _numMessagesRead;

    bool _messagesWereWritten;

    bool _messagesWereRead;

    bool _logFileWasIterated;

}; // END LogFileTestNode

#endif // LOGFILEWRITERNODE_HPP
