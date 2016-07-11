/**
 * @file LogFileTestNode.hpp
 * @brief PolySync Node used to test the PolySyncLogFile class functionality.
 *
 * PUBLIC_HEADER
 */

#ifndef LOGFILETESTNODE_HPP
#define LOGFILETESTNODE_HPP

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

#include "PolySyncLogfile.hpp"

class LogfileTestNode : public polysync::Node
{

public:

    /**
     * @brief LogfileTestNode Empty Constructor
     * Initialize private variables and call polysync::Node::Node()
     * (base class constructor)
     */
    LogfileTestNode();

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
     * @brief Called from initStateEvent(), this function sets up all the
     * parameters needed in order to Replay (read) a Logfile.
     *
     * @note Sets filterOutMessages() by type (optional).
     * Sets filepath (optional) and Session Id.
     * Sets mode to write, and enables state.
     * Sets _numMessagesRead to 0; final result printed in releaseStateEvent.
     */
    void prepareLogfileToRead();

    /**
     * @brief called from initStateEvent(), this function sets up Logfile
     * Iterator to iterate over PolySync Logfiles outside of the Replay time
     * domain. That means that Mode should be set to OFF and State to Disabled.
     */
    void prepareLogfileToIterate();

    /**
     * @brief Filtering messages by type in Logfile writer prevents filtered
     * messages from being written to disk. Filtering messages by type in
     * Logfile Reader prevents filtered messages from being published to bus.
     * LogFile Reader Queue always receives messages regardless of filter.

     * @note This function calls the  CPP Logfile Class' filter messages by type
     * function. The first 2 params are Logfile Writer filters; the second 2
     * params are Logfile Reader filters. If you do not want to filter at all,
     * do not call this function. If you call this function, set to 0 / nullptr
     * whatever params you do not want to set.
     */
    void filterOutMessages();

    /**
     * @brief Writes a single message for each okStateEvent() loop until
     * Ctrl + C.
     */
    void writeMessage();

    /**
     * @brief Reads (and dequeues) a single message for each okStateEvent()
     * loop until Ctrl + C.
     */
    void readDequeuedMessage();

    /**
     * @brief logFileIteratorCallback
     * @param file_attributes
     * @param msg_type
     * @param log_record
     * @param user_data
     */
    static void logfileIteratorCallback(
                const ps_logfile_attributes * const file_attributes,
                const ps_msg_type msg_type,
                const ps_rnr_log_record * const log_record,
                void * const user_data );

    /**
     * @brief Iterates over each record in a Logfile, and does so outside of
     * the normal PolySync Replay time domain.
     * Hence, Mode should be set to OFF and State should be disabled prior to
     * invocation (in prepareLogfileToIterate() ).
     */
    void iterateOverLogfiles();

    /**
     * @brief Pauses Replay by toggling State to Disabled and sleeping.
     */
    void pauseReplay();

    /**
     * @brief Resumes Replay by toggling State to Enabled.
     */
    void resumeReplay();

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

    GAsyncQueue * _replayQueue;

    int _numMessagesWritten;

    int _numMessagesRead;

    bool _messagesWereWritten;

    bool _messagesWereRead;

    bool _logFileWasIterated;

}; // END LogFileTestNode


#endif // LOGFILETESTNODE_HPP
