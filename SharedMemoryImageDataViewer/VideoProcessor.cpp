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
#include "VideoProcessor.hpp"

#include <PolySyncDTCException.hpp>
#include <PolySyncSharedMemory.hpp>
#include <PolySyncVideo.hpp>

#include <QImage>
#include <QPixmap>

#include <iostream>

using namespace std;

void VideoProcessor::slotRun()
{
    // Set the desired format for video device.
    polysync::VideoFormat deviceFormat{
        PIXEL_FORMAT_YUYV,
        640, 480,
        PSYNC_VIDEO_DEFAULT_FRAMES_PER_SECOND };

    // Set format for encoder output
    polysync::VideoFormat encodedFormat{
        PIXEL_FORMAT_H264,
        640, 480,
        PSYNC_VIDEO_DEFAULT_FRAMES_PER_SECOND };

    // Set format for decoder output
    polysync::VideoFormat decodedFormat{
        PIXEL_FORMAT_BGR24,
        640,
        480,
        PSYNC_VIDEO_DEFAULT_FRAMES_PER_SECOND };

    try
    {
        polysync::SharedMemoryImageData sharedMemory( _sharedMemoryKey );

        // Attach to existing shared memory segment at given key
        sharedMemory.attach();

        // Set size of input data buffer to arbitrarily large size
        sharedMemory.setBufferSize( 1280*1080*3 );

        // Create encoder/decoder
        polysync::VideoEncoder encoder{ deviceFormat, encodedFormat };
        polysync::VideoDecoder decoder{ encodedFormat, decodedFormat };

        while( 1 )
        {
            // Block and wait for data
            // Shared memory behaves as a FIFO (first in, first out ) queue.
            sharedMemory.pop();

            if( sharedMemory.getMessageType() == PSYNC_SHDMEM_MSG_TYPE_IMAGE_DATA )
            {
                // Encode data received
                encoder.encode( sharedMemory.getData() );

                auto encodedBuffer = encoder.getCopyOfEncodedBuffer();
                auto encodedSize = encodedBuffer.size();

                if( encodedSize > 0  )
                {
                    // Decode encoded data
                    decoder.decode( encodedBuffer );
                    auto decodedBuffer = decoder.getCopyOfDecodedBuffer();

                    auto decodedSize = decodedBuffer.size();
                    if( decodedSize > 0 )
                    {
                        QImage img( decodedBuffer.data(),
                                    640, 480,
                                    QImage::Format_RGB888 );

                        // Output data to VideoViewer for rendering
                        emit signalPixmap( QPixmap::fromImage( img ) );
                    }
                }
            }
        }
    }
    catch( polysync::DTCException & e )
    {
        cout << e.what() << endl;
    }
}
