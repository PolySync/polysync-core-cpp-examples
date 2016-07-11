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
 * \example LogfileIteratorExample.cpp
 *
 * PolySync Logfile CPP API Iterator Example.
 *
 * Shows how to use the Logfile API to iterate over records in a Logfile
 * outside of the normal PolySync Replay time domain. This means that the
 * Logfile iterator runs as fast as the CPU will allow.
 *
 * @file LogfileIteratorExample.cpp
 * @brief LogfileIterator Source
 *
 * Regarding setting node name using Logfile Iterator and how that
 * relates to setFilePath(), setSessionId(), and setNodeName():
 *
 * Example: in directory 1234, let a logfile be name.5678.plog,
 * where 1234 = session ID, 5678 = GUID, and name = node name.
 *
 * To iterate over records in a logfile, use its explicit filepath.
 *
 * Any file specified in the path can be read from; node name is
 * irrelevant when using explicit setFilePath().
 *
 * How: use any setNodeName(), and use setFilePath() in
 * prepareLogfileToIterate().
 */


#include "LogfileIteratorExample.hpp"

using namespace std;


LogfileIteratorNode::LogfileIteratorNode()
    :
      Node(),
    _logFile( nullptr ),
    _numMessagesWritten( 0 ),
    _numMessagesRead( 0 ),
    _messagesWereWritten( false ),
    _messagesWereRead( false ),
    _logFileWasIterated( false )
{
    // empty
}


void LogfileIteratorNode::prepareLogfileToIterate()
{
    polysync::sleepMicro ( 1000000 ); // Sleep before mode off so queue doesn't flush.

    _logFile->setModeOff(); // Automatically disables state.

    _logFile->setFilePath( "/tmp/polysync_logfile.plog"  );

    cout << "Logfile Iterator started." << endl;
}


void LogfileIteratorNode::printResults()
{   
    cout << "\nIterator complete.\n"
             "\nReleasing logfile resources. If all messages did not print "
            "to Terminal \nin either read, write, or iterator, that is due to "
            "i/o (printf / cout) \nbeing slower than CPU.\n\n";
}


void LogfileIteratorNode::initStateEvent()
{
    // 1. Init LogFile API resources:
    _logFile = new ExampleLogfile{ *this };

    // 2. Set up parameters.
    prepareLogfileToIterate();

    /* 3. Iterate over Logfiles in initStateEvent(), not okStateEvent(), as
     * Logfile iterator operates outside of the normal Replay time domain.
     */
    _logFile->readLogfile( _logFile->getFilePath() );

    // 4. Disconnect after Iterator is done iterating over a Logfile.
    disconnectPolySync();
}


void LogfileIteratorNode::releaseStateEvent()
{
    printResults();

    /* Turn off mode. Turning off the mode automatically disables state.
     * Sleep before turning mode off after last write to avoid flushing of queue.
     */
    polysync::sleepMicro( 5000000 );

    _logFile->setModeOff();

    delete _logFile;
}


int main()
{
    cout << "\n\n************************************************\n"
    "*** PolySync LogFile API: C++ Iterator Example ***\n\n";

    try
    {
        LogfileIteratorNode logfileIteratorNode;

        sleep( 2 );

        logfileIteratorNode.setNodeName( "logfile-iterator" );

        logfileIteratorNode.connectPolySync();
    }
    catch( std::exception & e )
    {
        cout<< e.what() << std::endl;
    }

    return 0;
}


