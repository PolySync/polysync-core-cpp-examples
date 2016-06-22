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
 */


#include "LogfileTestNode.hpp"             // For all CPP Logfile API examples.

using namespace std;

using namespace polysync::datamodel;


void LogFileTestNode::prepareLogfileToIterate()
{
    sleepMicro ( 1000000 ); //always sleep before modeoff so queue doesn't flush.

    _logFile->setModeOff(); // automatically disables state.

    _logFile->setFilePath( "/tmp/polysync_logfile.plog"  );

    cout<<"Logfile Iterator started." << endl;

    _logFileWasIterated = true;
}


void LogFileTestNode::logfileIteratorCallback(
        const ps_logfile_attributes * const fileAttributes,
        const ps_msg_type msg_type,
        const ps_rnr_log_record * const logRecord,
        void * const userData )
{
    cout<<"\nLogfile iterator callback\n";

    if( logRecord )
    {
        cout<<"Log Record: Index: " << logRecord->index <<" - Size: "
           << logRecord->size <<" - Previous Size: "
           << logRecord->prev_size <<" - RnR Timestamp ( \nheader.timestamp):"
           << logRecord->timestamp;
    }

    cout<<endl;

    // get ps_msg_ref, and do something with it.
}



void LogFileTestNode::iterateOverLogfiles()
{
    _logFile->forEachIterator(
                _logFile->getFilePath().c_str(),
                &logfileIteratorCallback,
                nullptr );
}


void LogFileTestNode::printResults()
{
    if( _logFileWasIterated )
    {
        cout<<"\nIterator complete.\n";
    }

    cout<<"\nReleasing logfile resources. If all messages did not print "
          "to Terminal \nin either read, write, or iterator, that is due to "
          "i/o (printf / cout) \nbeing slower than CPU.\n\n";
}


void LogFileTestNode::initStateEvent()
{
// 1. Init LogFile API resources:
    _logFile = new Logfile{ *this };

// 2. Set up parameters.
    prepareLogfileToIterate();

// 3. Iterate over Logfiles in initStateEvent(), not okStateEvent(), as
//    Logfile iterator operates outside of the normal Replay time domain.
    iterateOverLogfiles();

// 4. Disconnect after Iterator is done iterating over a Logfile.
    disconnectPolySync();
}


void LogFileTestNode::okStateEvent()
{
/**** Logfile Iterator. No code should be in in okStateEvent(). ****/
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
    "*** PolySync LogFile API: C++ Iterator Example ***\n\n";

    try
    {
        LogFileTestNode aNode;

        sleep( 2 );

// Regarding setting node name using Logfile Iterator and how that relates to
// setFilePath(), setSessionId(), and setNodeName():

// Example: in directory 1234, let a logfile be name.5678.plog,
// where 1234 = session ID, 5678 = GUID, and name = node name.

// To iterate over records in a logfile, use its explicit filepath.

// Any file specified in the path can be read from; node name is irrelevant.
// How: use any setNodeName(), and use setFilePath() in prepareLogfileToIterate().

        aNode.setNodeName("custom-nodename"); // Read logfiles.

        aNode.connectPolySync();
    }
    catch( std::exception & e )
    {
        cout<< e.what() << std::endl;
    }

    return 0;
}


