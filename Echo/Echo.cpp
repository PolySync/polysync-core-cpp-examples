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
 * \example Echo.cpp
 *
 * PolySync Echo C++ API example application
 *
 * Demonstrates a command line utility for echoing PolySync messges to stdout
 *
 * @file Echo.cpp
 * @brief Echo Source
 *
 */

#include "EchoNode.hpp"


/**
 * Entry point for this tutorial application
 * The "connectPolySync" begins this node's PolySync execution loop.
 *
 * @param argc - int, the number of parameters on the command-line
 * @param argv - char* [], the parsed command-line arguments
 *
 * @return int - exit code
 */
int main( int argc, char *argv[] )
{
    // Create an instance of the PolySyncEcho and connect it to PolySync.
    PolySyncEcho echo;


    // Nodes will only connect if help option -h not used,
    // and, if all arguments are valid.
    if ( echo.optionsParse( argc, argv ) )
    {
        if ( echo.helpWasRequested() )
        {
            echo.printEchoHelpPage();
        }
        else
        {
            // Begin the PolySync event loop.
            echo.connectPolySync();
        }
    }

    // For a brief tutorial on the Echo tool, run help: $ polysync-echo -h

    return 0;
}
