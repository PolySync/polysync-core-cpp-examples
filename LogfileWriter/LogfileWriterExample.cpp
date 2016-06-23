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
 *
 * Regarding setting node name using Logfile Writer and how that relates
 * to setFilePath(), setSessionId(), and setNodeName()
 * Example: in directory 1234, let a logfile be name.5678.plog,
 * where 1234 = session ID, 5678 = GUID, and name = node name.
 *
 * For Logfile Writer: In prepareLogfileToWrite(), set sessionId;
 * optionally, set custom filepath. If writer uses explicit
 * setFilePath(), then setNodeName is irrelevant.
 *
 * If writer uses setSessionId() and not setFilePath(), then name of
 * logfile will be set with setNodeName(), located in dir specified by
 * Session Id.
 */

/**
 * \example LogfileWriterExample.cpp
 *
 * PolySync Logfile CPP API Writer Example.
 *
 * Shows how to use the Logfile API to write logfiles.
 *
 * @file LogfileWriterExample.cpp
 * @brief LogfileWriter Source
 *
 */


#include "LogfileTestNode.hpp"          // For all CPP Logfile API examples.

using namespace std;

using namespace polysync::datamodel;


void LogFileTestNode::prepareLogfileToWrite()
{
    // 1. Filter out certain message types from being written to disk (optional).
    //filterOutMessages();

    // 2. Set logfile file path (optional). Overrides default logic for Session ID.
    _logFile->setFilePath( "/tmp/polysync_logfile.plog"  );

    // 3. Set session ID. You can use timestamp or arbitrary name such as 1234.
    _logFile->setSessionId( getTimestamp() );

    // 4. Begin Replay: Enable state in single node context.
    _logFile->setModeWrite();

    _logFile->setStateEnabled( 0 );

    cout << "Logfile Record started. Ctrl + C to stop." << endl;

    /* Note: Enable state in  *distributed system context* uses timing now
     *      + offset:
     * ps_timestamp stateTimingOffsetDistributedContext =
     *      getTimestamp() + 2000000;
     *
     * _logFile->setStateEnabled( stateTimingOffsetDistributedContext );
     */

    // 5. Set counter.
    _numMessagesWritten = 0;
}


void LogFileTestNode::filterOutMessages()
{
    // 1. Sleep before modeoff so queue doesn't flush.
    sleepMicro( 1000000 );

    _logFile->setModeOff();

    // 2. get message type:
    ps_msg_type byteArrayMessageType =
        getMessageTypeByName("ps_byte_array_msg");

    ps_msg_type readerFilterList[1] = { byteArrayMessageType };

    /* 3. Filter.
     * First 2 params: writer filters. Second 2 params: reader filters.
     * Using this function will prevent selected message types from being
     * written to disk.
     */
    _logFile->setMessageTypeFilters( readerFilterList, 1, nullptr, 0 );
}


void LogFileTestNode::writeMessage()
{
    // Record: Write a single message for each okStateEvent() loop.

    // 1. Allocate message / fill out its parameters.
    ByteArrayMessage byteArrayMessage { *this };

    byteArrayMessage.setBytes( { 'P','o','l','y','S','y','n','c' } );

    byteArrayMessage.setDataType( 0 ); // index to print only every 20 messages.

    // 2. Tell user how many messages have been written so far.
     ++_numMessagesWritten;

    byteArrayMessage.setDataType( _numMessagesWritten );

    // 3. Each message written receives a unique timestamp (Replay time slicing).
    byteArrayMessage.setHeaderTimestamp( getTimestamp() );

    // 4. Write a message. Tell user how many messages have been written so far.
    _logFile->writeMessage( byteArrayMessage );

    if( _numMessagesWritten % 20 == 0 )
    {
       cout << "\nLogged Message ID:" << byteArrayMessage.getDataType();
    }

    // 5. Set write frequency for "real time 1x Replay" in okState w/ sleepMicro.
}


void LogFileTestNode::printResults()
{
    cout << "\n\nWrote " << _numMessagesWritten <<" total messages.\n"

          "\n***  End PolySync LogFile C++ Writer Example  ***\n"
          "*************************************************\n";

    cout << "\nReleasing logfile resources. If all messages did not print "
          "to Terminal \nin either read, write, or iterator, that is due to "
          "i/o (printf / cout) \nbeing slower than CPU.\n\n";
}


void LogFileTestNode::initStateEvent()
{
    // 1. Init LogFile API resources:
    _logFile = new Logfile{ *this };

    // 2. Set up parameters before writing in okStateEvent() loop.
    prepareLogfileToWrite();

    // 3. Record (write): Write Messages until Ctrl + C in okStateEvent() loop.
}


void LogFileTestNode::okStateEvent()
{
    /* For each okStateEvent() loop, process a single write + sleep (required).
     * For larger messages such as Lidar, set a larger (higher) write frequency.
     * Terminate with Ctrl + C.
     */
    // 1. Write a message.
    writeMessage();

    // 2. Set write frequency for 1x Replay.
    sleepMicro( 5000 );
}


void LogFileTestNode::releaseStateEvent()
{
    printResults();

    /* Turn off mode. Turning off the mode automatically disables state.
     * Sleep before turning mode off after last write to avoid flushing of queue.
     *
     */
    sleepMicro( 5000000 );

    _logFile->setModeOff();

    delete _logFile;
}


int main()
{
    cout << "\n\n************************************************\n"
    "*** PolySync LogFile C++ API: Writer Example ***\n\n";

    try
    {
        LogFileTestNode aNode;

        sleep( 1 );

        aNode.setNodeName("custom-nodename");

        aNode.connectPolySync();
    }
    catch( std::exception & e )
    {
        cout<< e.what() << std::endl;
    }

    return 0;
}
