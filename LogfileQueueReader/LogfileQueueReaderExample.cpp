/*
 * Copyright (c) 2016 PolySync
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *
 * Regarding setting node name using Logfile Reader and how that relates
 * to setFilePath(), setSessionId(), and setNodeName():
 *
 * Example: in directory 1234, let a logfile be name.5678.plog,
 * where 1234 = session ID, 5678 = GUID, and name = node name.

 * To read from this logfile, you can use its filepath or its session ID.
 *
 * Session ID logic is default write logic.
 * If you do not set explicit filepath, then directory will be timestamp
 * of write time, and name will be whatever is set in setNodeName().
 *
 * setFilePath() logic overrides default Session ID logic.
 * For Logfile Reader, using a Logfile's explicit filepath is strongly
 * recommended.
 *
 * Option 1: Using explicit filepath to read logfiles (recommended):
 * Any file specified in the path can be read from;
 * node name is irrelevant.
 * How:
 * 1. use any setNodeName(), then
 * 2. use setFilePath() in prepareLogfileToRead().
 *
 * Option 2: Using Session ID to read logfiles:
 * Since reader's SDF key must match written SDF key, it's required to
 * use the same node reference/context to write to, and then read from,
 * a Logfile using its SessionID. Generally, that means writing to a
 * Logfile and then reading from that Logfile in the same "example"
 * file (same Node called from main() with connectPolySync() ).
 * Doing this guarantees the same node reference, and hence, the same SDF Key.
 * How:
 * Not covered in this example.
 * All C/C++ Logfile examples use explicit filepath, not Session ID.
 */

/**
 * \example LogfileQueueReaderExample.cpp
 *
 * PolySync Logfile CPP API Queue Reader Example.
 *
 * Shows how to use the Logfile API to read logfiles from the async queue.
 *
 * @file LogfileQueueReaderExample.cpp
 * @brief LogfileReader Source
 *
 */

#include "LogfileQueueReaderExample.hpp"

using namespace std;

LogfileQueueReaderNode::LogfileQueueReaderNode()
    :
      Node(),
    _logFile( nullptr ),
    _replayQueue( nullptr ),
    _numMessagesWritten( 0 ),
    _numMessagesRead( 0 ),
    _messagesWereWritten( false ),
    _messagesWereRead( false ),
    _logFileWasIterated( false )
{
    // empty
}

void LogfileQueueReaderNode::prepareLogfileToRead()
{
    // 1. filter out certain message types from being published (optional).
    //    filterOutMessages();

    // 2. Choose Read method: either filepath (simplest use case), or session ID.
    // Option 1: set filepath. If set by writer in same file, no need to reset.
    _logFile->setFilePath( "/tmp/polysync_logfile.plog"  );

    _logFile->setSessionId( 1 );

    // 3. Enable the Replay Message Queue + get a reference to the Queue.
    _logFile->enableOutputQueue( 1 );

    // 4. Begin replay. Enqueue messages onto Replay Queue if queue valid.
    _logFile->setModeRead();

    _logFile->setStateEnabled( 0 );

    cout << "Logfile Replay started. Ctrl + C to stop, or wait until EOF." << endl;

    // 5. Set counter.
    _numMessagesRead = 0;
}


void LogfileQueueReaderNode::filterOutMessages()
{
    // 1. Sleep before setModeOff() so queue doesn't flush.
    polysync::sleepMicro( 1000000 );

    _logFile->setModeOff();

    // Disable ps_byte_array_msg for the writer.
    _logFile->setMessageTypeFilters(
        { getMessageTypeByName( "ps_byte_array_msg" ) }, {} );
}


void LogfileQueueReaderNode::readDequeuedMessage()
{
    // Replay: dequeue a single message for each okStateEvent() loop:
    ps_msg_ref dequeuedMessage =
            g_async_queue_try_pop( _logFile->getReplayMessageQueue() );

    if( dequeuedMessage != PSYNC_MSG_REF_INVALID )
    {
        auto messageToPrint =
                polysync::datamodel::buildMessage( *this, dequeuedMessage );

        ++_numMessagesRead;

        cout << "\nReplaying message # " <<_numMessagesRead <<" in queue\n";

        messageToPrint->printHeader();
    }
}


void LogfileQueueReaderNode::pauseReplay()
{
    _logFile->setStateDisabled();
}


void LogfileQueueReaderNode::resumeReplay()
{
    _logFile->setStateEnabled( 0 );
}


void LogfileQueueReaderNode::printResults()
{
    cout << "\n\nRead " << _numMessagesRead <<" total messages.\n";

    if( _logFile->eofHasBeenReached() )
    {
        cout << "End of file was reached.\n";
    }
    else
    {
        cout << "End of file was not reached.\n";
    }

    cout << "\n***  End PolySync LogFile C++ Reader Example  ***\n"
           "*************************************************\n"

          "\nReleasing logfile resources. If all messages did not print "
          "to Terminal \nin either read, write, or iterator, that is due to "
          "i/o (printf / cout) \nbeing slower than CPU.\n\n";
}


void LogfileQueueReaderNode::initStateEvent()
{
    // 1. Init LogFile API resources:
    _logFile = new polysync::Logfile{ *this };

    // 2. Set up parameters and validate before reading in okStateEvent() loop.
    prepareLogfileToRead();

    if( _logFile->eofHasBeenReached() || !_replayQueue )
    {
        cout << "\nDisconnecting: either Replay Queue invalid, or Replay EOF.\n";

        disconnectPolySync();
    }

    // 3. Replay (read): Read messages in okStateEvent() until EOF or Ctrl + C.
}


void LogfileQueueReaderNode::okStateEvent()
{
    // For each okStateEvent(), dequeue and read a single message.

    readDequeuedMessage();

    if( _logFile->eofHasBeenReached() )
    {
        disconnectPolySync();
    }

    /* Logfile Reader Control: to pause/resume Replay, toggle State.
     * Timing issues may occur; use with discretion. */
    /*
    if( _numMessagesRead == 100 ) // example
    {
        pauseReplay();

        resumeReplay();
    }
    */
}


void LogfileQueueReaderNode::releaseStateEvent()
{
    printResults();

    // Turn off mode. Turning off the mode automatically disables state.
    // Sleep before turning mode off after last write to avoid flushing of queue.

    polysync::sleepMicro( 5000000 );

    _logFile->setModeOff();

    delete _logFile;
}


int main()
{
    cout << "\n\n************************************************\n"
    "*** PolySync LogFile C++ API: Reader Example ***\n\n";

    try
    {
        LogfileQueueReaderNode logfileReader;

        sleep( 1 );

        logfileReader.setNodeName( "logfile-queue-reader" );

        logfileReader.connectPolySync();
    }
    catch( std::exception & e )
    {
        cout<< e.what() << std::endl;
    }

    return 0;
}

