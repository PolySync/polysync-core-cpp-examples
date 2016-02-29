/*
 * Copyright (c) 2015 HARBRICK TECHNOLOGIES, INC
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
 * \example ReplayControl.cpp
 *
 * Demonstrates how to use the Replay API routines to control a replay session.
 * Although this example shows how to start and stop a replay session, it
 * does NOT spawn other nodes for you. If you have no other nodes running
 * in no hardware mode which can replay log data, then the replay session
 * will not replay the log file data.
 */

#include <iostream>

#include "PolySyncRecordReplay.hpp"

using namespace polysync;
using namespace std;

/**
 * @brief ReplayNode class
 *
 * The ReplayNode class exists to override the functions defined in the
 * base Node class.  The functions exist in the base class but
 * must be overloaded to define the node's custom behavior.
 */
class ReplayNode : public Node
{
    /**
     * @brief Timestamp of when the recording began.
     */
    ps_timestamp startTime;

    /**
     * @brief @ref polysync::ReplaySession used to start and stop the
     * replay session.
     */
    ReplaySession *replay;

    /**
     * @brief initStateEvent
     *
     * Override the base class functionality to create and
     * start a @ref polysync::ReplaySession.
     */
    void initStateEvent() override
    {
        // Create a new replay session.
        // Uses the @ref polysync::Node to subscribe and publish
        // Record & Replay messages.
        replay = new ReplaySession{ *this };

        // Set the @ref ps_rnr_session_id of this replay session.
        // Check that the log session id below exists on your system.
        // If it does not, run the RecordControl example and come back to this.
        replay->load( 42 );

        // Optionally set looping to true. If you are using the log session
        // collected in the RecordControl example, wait until the end of the
        // 1 minute log and the replay will loop back to the beginning.
        replay->setLooping( true );

        // Start the replay.
        replay->start();

        // Get time the replay started.
        startTime = getTimestamp();

        cout << "Replay started." << endl;
    };

    /**
     * @brief okStateEvent
     *
     * Override the base class functionality to end the replay session until
     * the user provides some input.
     */
    void okStateEvent() override
    {
        cout << "Press a key to end the replay session: ";

        // Wait until user input.
        cin.ignore();

        // Stop the replay.
        replay->stop();

        // Clean up dynamic memory.
        delete replay;

        // Disconnect this Node.
        disconnectPolySync();
    }
};

/**
 * @brief main
 *
 * Entry point for this tutorial application.
 */
int main()
{
    // Create an instance of the ReplayNode and connect it to PolySync.
    ReplayNode replayNode;

    // When the node has been connected, it will call
    // 'initStateEvent' once, and then call 'okStateEvent' until
    // the user provides some input.
    replayNode.connectPolySync();

    return 0;
}