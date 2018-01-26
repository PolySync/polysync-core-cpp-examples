/*
 * Copyright (c) 2015 PolySync
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
#include <iostream>

#include <PolySyncApplication.hpp>

#include "LogSessionExport.hpp"


int main( int argc, char * argv[] )
{
    if( argc < 2 )
    {
        std::cerr << "Usage: " << argv[0] << " [sessionId]" << std::endl;
        return -1;
    }

    auto sessionId = std::stoull( argv[1] );

    std::unique_ptr< SessionExportExample > exportExample;

    if( sessionId != 0 )
    {
        if( argc == 3 )
        {
            exportExample = std::unique_ptr< SessionExportExample >{
                        new SessionExportExample{ sessionId, argv[2] } };
        }
        else
        {
            exportExample = std::unique_ptr< SessionExportExample >{
                    new SessionExportExample{ sessionId } };
        }
    }

    auto application = polysync::Application::getInstance();

    application->setNodeName( "polysync-log-session-export-cpp" );

    application->connectPolySync();
}
