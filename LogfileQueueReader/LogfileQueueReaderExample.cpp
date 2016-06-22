/*
 * Copyright (c) 2016 HARBRICK TECHNOLOGIES, INC
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


#include "LogFileTestNode.hpp"          // For all CPP Logfile API examples.

using namespace std;

using namespace polysync::datamodel;


void LogFileTestNode::prepareLogfileToRead()
{
// 1. filter out certain message types from being published (optional).
//    filterOutMessages();

// 2. Choose Read method: either filepath (simplest use case), or session ID.
    // Option 1: set filepath. If set by writer in same file, no need to reset.
    _logFile->setFilePath( "/tmp/polysync_logfile.plog"  );

    _logFile->setSessionId( 1 );

    // Option 2: Using setSessionId() instead of setFilePath():
    //_logFile->setSessionId( 1234 );

// 3. Enable the Replay Message Queue + get a reference to the Queue.
    _logFile->enableOutputQueue(1);

    _replayQueue = _logFile->getReplayMessageQueue();

// 4. Begin replay. Enqueue messages onto Replay Queue if queue valid.
    _logFile->setModeRead();

    _logFile->setStateEnabled( 0 );

    cout<<"Logfile Replay started. Ctrl + C to stop, or wait until EOF." << endl;

// 5. Set flags / counters.
    _numMessagesRead = 0;

    _messagesWereRead = true;
}


void LogFileTestNode::filterOutMessages()
{
    sleepMicro( 1000000 ); //always sleep before modeoff so queue doesn't flush.

    _logFile->setModeOff();

    // get message type:
    ps_msg_type byteArrayMessageType =
        getMessageTypeByName("ps_byte_array_msg");

    ps_msg_type readerFilterList[1] = { byteArrayMessageType };

//Filter. First 2 params: writer filters. Second 2 params: reader filters.
// Filtering here will prevent selected message types from being published;
// the reader queue always receives all messages regardless of filtering.
    _logFile->setMessageTypeFilters( nullptr, 0, readerFilterList, 1 );
}


void LogFileTestNode::readDequeuedMessage()
{
// Replay: dequeue a single message for each okStateEvent() loop:
    ps_msg_ref dequeuedMessage =
            g_async_queue_try_pop( _replayQueue );

    if( dequeuedMessage != PSYNC_MSG_REF_INVALID )
    {
        auto messageToPrint = buildMessage( *this, dequeuedMessage );

        ++_numMessagesRead;

        cout<<"\nReplaying message # " <<_numMessagesRead <<" in queue\n";

        messageToPrint->printHeader();
    }
}


void LogFileTestNode::pauseReplay()
{
    _logFile->setStateDisabled();     // Pause Replay.
}


void LogFileTestNode::resumeReplay()
{
    _logFile->setStateEnabled( 0 );   // Resume Replay.
}


void LogFileTestNode::printResults()
{
    if( _messagesWereRead )
    {
        cout<<"\n\nRead " << _numMessagesRead <<" total messages.\n";

        if( _logFile->readerGetEofStatus() )
        {
            cout<<"End of file was reached.\n";
        }
        else
        {
            cout<<"End of file was not reached.\n";
        }
    }

    cout<<"\n***  End PolySync LogFile C++ Reader Example  ***\n"
           "*************************************************\n"

          "\nReleasing logfile resources. If all messages did not print "
          "to Terminal \nin either read, write, or iterator, that is due to "
          "i/o (printf / cout) \nbeing slower than CPU.\n\n";
}


void LogFileTestNode::initStateEvent()
{
// 1. Init LogFile API resources:
    _logFile = new Logfile{ *this };

// 2. Set up parameters and validate before reading in okStateEvent() loop.
    prepareLogfileToRead();

    if( _logFile->readerGetEofStatus() || !_replayQueue )
    {
        cout<<"\nDisconnecting: either Replay Queue invalid, or Replay EOF. \n";
        disconnectPolySync();
    }

// 3. Replay (read): Read messages in okStateEvent() until EOF or Ctrl + C.
}


void LogFileTestNode::okStateEvent()
{
// For each okStateEvent() loop, dequeue and read a single message.

    readDequeuedMessage();

    if( _logFile->readerGetEofStatus() )
    {
        disconnectPolySync();
    }

/**** Logfile Reader Control: to pause/resume Replay, toggle State.
      Timing issues may occur; use with discretion. ****/

//    if( _numMessagesRead == 100 ) // example
//    {
//        pauseReplay();

//        resumeReplay();
//    }
}


void LogFileTestNode::releaseStateEvent()
{
    printResults();

// Turn off mode. Turning off the mode automatically disables state.
// Sleep before turning mode off after last write to avoid flushing of queue.

    sleep( 5 );

    _logFile->setModeOff();

    delete _logFile;
}


int main()
{
    cout<<"\n\n************************************************\n"
    "*** PolySync LogFile C++ API: Reader Example ***\n\n";

    try
    {
        LogFileTestNode aNode;

        sleep( 1 );

// Regarding setting node name using Logfile Reader and how that relates to
// setFilePath(), setSessionId(), and setNodeName():

// Example: in directory 1234, let a logfile be name.5678.plog,
// where 1234 = session ID, 5678 = GUID, and name = node name.
// To read from this logfile, you can use its filepath or its session Id.

// Session ID logic is default write logic. If you do not set explicit filepath,
// then directory will be timestamp of write time, and name will be
// whatever is set in setNodeName().

// For Logfile Reader, using a Logfile's explicit filepath is strongly recommended.

// Option 1: Using explicit filepath to read logfiles (recommended/simpler):
// Any file specified in the path can be read from; node name is irrelevant.
// How: use any setNodeName(), and use setFilePath() in prepareLogfileToRead().

// Option 2: Using Session ID to read logfiles:
// Since reader's SDF key must match written SDF key, it's required to use the
// same node reference/context to write to, and then read from, a logfile,
// if reading from a Logfile using its Session ID.
// This means that node name must match the written file, re: setNodeName(),
// and it requires combining the Logfile Writer and Logfile Reader into
// the same "example" file to guarantee the same node reference and SDF Key.
// How: not covered in this example. All C/C++ Logfile examples use filepath.

        aNode.setNodeName("custom-nodename"); // Read logfiles.

        aNode.connectPolySync();
    }
    catch( std::exception & e )
    {
        cout<< e.what() << std::endl;
    }

    return 0;
}

