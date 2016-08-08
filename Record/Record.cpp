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
 */

/**
 * \example Record.cpp
 *
 * Demonstrates how to use the Record API routines to control a recording session.
 * Although this example shows how to start and stop a recording session, it
 * does NOT spawn other nodes for you. If you have no other nodes running
 * which can log data, then the resulting recording will be empty.
 *
 */

#include <iostream>

#include "PolySyncNode.hpp"
#include "PolySyncRecordReplay.hpp"

using namespace polysync;
using namespace std;

/**
 * @brief RecordNode class
 *
 * The RecordNode class exists to override the functions defined in the
 * base Node class.  The functions exist in the base class but
 * must be overloaded to define the node's custom behavior.
 */
class RecordNode : public Node
{

    /**
     * @brief @ref polysync::RecordSession used to start and stop the
     * recording session.
     */
    RecordSession *recording;

    /**
     * @brief initStateEvent
     *
     * Override the base class functionality to create and
     * start a @ref polysync::RecordSession.
     */
    void initStateEvent() override
    {
        // Create a new record session.
        // Uses the @ref polysync::Node to subscribe and publish
        // Record & Replay messages.
        recording = new RecordSession{ *this };

        // Set the @ref ps_rnr_session_id of this recording.
        ps_rnr_session_id sessionId;
        cout << "Enter recording session id: ";
        cin >> sessionId;
        recording->setId( sessionId );

        // Activate the recording session.
        recording->activate();

        // Start the recording.
        recording->start();

        cout << "Recording started." << endl;
    };

    /**
     * @brief okStateEvent
     *
     * Override the base class functionality to end the recording after
     * receiving user input.
     */
    void okStateEvent() override
    {
        string input;
        do
        {
            cout << endl << "Enter 'q' to end recording: ";
            cin >> input;
        }
        while( input != "q" );

        // Stop the recording session.
        recording->stop();

        // Clean up dynamic memory.
        delete recording;

        // Disconnect this Node.
        disconnectPolySync();

        cout << "Recording stopped." << endl;
    }
};

/**
 * @brief main
 *
 * Entry point for this tutorial application.
 */
int main()
{
    // Create an instance of the RecordNode and connect it to PolySync.
    RecordNode recordingNode;

    // When the node has been connected, it will call
    // 'initStateEvent' once, and then continue to loop
    // using 'okStateEvent' until user provides input.
    recordingNode.connectPolySync();

    return 0;
}
