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
 * \example LogSessionExport.hpp
 *
 * Log session export example.
 *
 * Demonstrates how to use the PolySync log session transfer API to export a previously
 * recorded log session from a new distributed system.
 *
 */

#ifndef LOGSESSIONEXPORT_HPP
#define LOGSESSIONEXPORT_HPP


#include <PolySyncLogSessionTransfer.hpp>
#include <PolySyncDataSubscriber.hpp>


class SessionExportExample : public polysync::DataSubscriber
{

public:

    SessionExportExample(
            int sessionId, const std::string & sessionPath = {} );

    ~SessionExportExample(){ std::cout << "dtor" << std::endl; }
private:

    void handleEvent( std::shared_ptr< polysync::Message > message );

    void handleTransferStatus(
            const polysync::LogSessionTransferState state,
            const std::shared_ptr< polysync::datamodel::FileSyncStatusMessage > & );

    const ps_rnr_session_id _sessionId;

    const std::string & _sessionPath;

    std::unique_ptr< polysync::LogSessionExport > _exporter;

};


#endif // LOGSESSIONEXPORT_HPP

