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

#include <PolySyncLogfile.hpp>

#include "LogfileWriterExample.hpp"


using namespace std;


LogfileWriterNode::LogfileWriterNode()
    :
      Node(),
    _logFile( nullptr ),
    _numMessagesWritten( 0 )
{
    // empty
}


void LogfileWriterNode::prepareLogfileToWrite()
{
    // 1. Set logfile file path (optional). Overrides default logic for Session ID.
    _logFile->setFilePath( DefaultLogfilePath  );

    // 2. Set session ID. You can use timestamp or arbitrary name such as 1234.
    _logFile->setSessionId( polysync::getTimestamp() );

    // 3. Begin Replay: Enable state in single node context.
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

    // 4. Set counter.
    _numMessagesWritten = 0;
}


void LogfileWriterNode::writeMessage()
{
    // Record: Write a single message for each okStateEvent() loop.

    // 1. Allocate message / fill out its parameters.
    polysync::datamodel::ByteArrayMessage byteArrayMessage { *this };

    byteArrayMessage.setBytes( { 'P','o','l','y','S','y','n','c' } );

    byteArrayMessage.setDataType( 0 ); // index to print only every 20 messages.

    // 2. Tell user how many messages have been written so far.
     ++_numMessagesWritten;

    byteArrayMessage.setDataType( _numMessagesWritten );

    // 3. Each message written receives a unique timestamp (Replay time slicing).
    byteArrayMessage.setHeaderTimestamp( polysync::getTimestamp() );

    // 4. Write a message. Tell user how many messages have been written so far.
    _logFile->writeMessage( byteArrayMessage );

    if( _numMessagesWritten % 20 == 0 )
    {
       cout << "\nLogged Message ID:" << byteArrayMessage.getDataType();
    }

    // 5. Set write frequency for "real time 1x Replay" in okState w/ sleepMicro.
}


void LogfileWriterNode::printResults()
{
    cout << endl << endl
         << "Wrote " << _numMessagesWritten <<" total messages."
         << endl
         << "***  End PolySync LogFile C++ Writer Example  ***"
         << endl
         << "*************************************************"
         << endl << endl;
}


void LogfileWriterNode::initStateEvent()
{
    // 1. Init LogFile API resources:
    _logFile = new polysync::Logfile{ *this };

    // 2. Set up parameters before writing in okStateEvent() loop.
    prepareLogfileToWrite();

    // 3. Record (write): Write Messages until Ctrl + C in okStateEvent() loop.
}


void LogfileWriterNode::okStateEvent()
{
    /* For each okStateEvent() loop, process a single write + sleep (required).
     * For larger messages such as Lidar, set a larger (higher) write frequency.
     * Terminate with Ctrl + C.
     */
    // 1. Write a message.
    writeMessage();

    // 2. Set write frequency for 1x Replay.
    polysync::sleepMicro( FiveMilliSeconds );
}


void LogfileWriterNode::releaseStateEvent()
{
    printResults();

    /* Turn off mode. Turning off the mode automatically disables state.
     * Sleep before turning mode off after last write to avoid flushing of queue.
     *
     */
    polysync::sleepMicro( FiveSeconds );

    _logFile->setModeOff();

    delete _logFile;
}


int main()
{
    cout << endl << endl
         << "************************************************"
         << endl
         << "*** PolySync C++ LogFile API: Writer Example ***"
         << endl << endl;

    try
    {
        LogfileWriterNode logfileWriter;

        sleep( 1 );

        logfileWriter.setNodeName( "logfile-writer" );

        logfileWriter.connectPolySync();
    }
    catch( std::exception & e )
    {
        cout << e.what() << endl;
    }

    return 0;
}
