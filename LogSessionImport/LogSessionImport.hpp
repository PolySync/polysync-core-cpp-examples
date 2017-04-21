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
 * \example LogSessionImport.hpp
 *
 * Log session import example.
 *
 * Demonstrates how to use the PolySync log session transfer API to import a previously
 * exported log session to a new distributed system.
 *
 */

#ifndef LOGSESSIONIMPORT_HPP
#define LOGSESSIONIMPORT_HPP


#include <PolySyncDataSubscriber.hpp>
#include <PolySyncLogSessionTransfer.hpp>

#include "LogSessionImport.hpp"


class SessionImportExample : public polysync::DataSubscriber
{

public:

    SessionImportExample( const std::string & sessionPath );


private:

    void handleEvent( std::shared_ptr< polysync::Message > message );

    void handleTransferStatus(
            const polysync::LogSessionTransferState status,
            const std::shared_ptr<
                polysync::datamodel::FileSyncStatusMessage > & statusMsg);

    const std::string _sessionPath;

    std::unique_ptr< polysync::LogSessionImport > _importer;

};


#endif // LOGSESSIONIMPORT_HPP

