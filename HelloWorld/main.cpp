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

/*
 * PolySync Hello World C++ API example application
 *      Demonstrate the creation of a node and how to process node-level events
 *
 * Node Class definition and how to create your own Nodes
 */

#include <iostream>
#include <PolySyncNode.hpp>

using namespace std;

/**
 * @brief HellowWorldNode class
 *
 * The HelloWorldNode class exists to override the functions defined in the
 * base class.  The functions exist in the base class but are stubbed out
 * and must be overloaded in order for them to do something.  In this instance
 * only the initStateEvent function is overloaded to demonstrate how to
 * handle node events.
 */
class HelloWorldNode : public polysync::Node
{
    /**
     * @brief initStateEvent
     *
     * Override the base class functionality to give the user an indication
     * that the initialization state has been called.
     *
     * @param void
     * @return void
     */
    void initStateEvent() override
    {
        cout << "Hello world!" << endl;
    }
};

/**
 * @brief main
 *
 * Entry point for this tutorial application
 * The "connectToPolySync" is a blocking call, users must use Ctrl-C to exit
 * this function.
 *
 * @param argc - int, the number of parameters on the command-line
 * @param argv - char* [], the parsed command-line arguments
 * 
 * @return int - exit code
 */
int main(int argc, char *argv[])
{
    // Create an instance of the HelloWorldNode and connect it to PolySync
    HelloWorldNode helloNode;

    // When the node has been created, it will cause an initStateEvent to
    // to be sent.
    helloNode.connectToPolySync();

    return 0;
}
