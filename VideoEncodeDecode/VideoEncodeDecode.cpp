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
 * \example VideoEncodeDecode.cpp
 *
 * Shows how to use the Video API routines to communicate with a video device,
 * and encode/decode the data.
 *
 */

#include <PolySyncVideo.hpp>

#include <iostream>

using namespace std;

int main( int argc, char *argv[] )
{
    polysync::VideoFormat deviceFormat{
        PIXEL_FORMAT_YUYV,
        640, 480,
        PSYNC_VIDEO_DEFAULT_FRAMES_PER_SECOND };

    polysync::VideoFormat encodedFormat{
        PIXEL_FORMAT_H264,
        640, 480,
        PSYNC_VIDEO_DEFAULT_FRAMES_PER_SECOND };

    polysync::VideoEncoder encoder{ deviceFormat, encodedFormat };

    polysync::VideoFormat decodedFormat{
        PIXEL_FORMAT_RGB24,
        640,
        480,
        PSYNC_VIDEO_DEFAULT_FRAMES_PER_SECOND };

    polysync::VideoDecoder decoder{ encodedFormat, decodedFormat };

    try
    {
        auto videoDeviceList = polysync::getAvailableVideoDevices();

        if( videoDeviceList.size() > 0 )
        {
            auto videoDevice = videoDeviceList[ 0 ];

            if( ! videoDevice.formatIsSupported( deviceFormat ) )
            {
                cout << "VideoFormat not supported."
                     << endl;

                return 0;
            }

            videoDevice.setFormat( deviceFormat );

            videoDevice.enableStreaming();

            ulong frameIndex{ 0 };
            while( videoDevice.poll() )
            {
                cout << "Frame[ " << frameIndex << " ]: "
                     << endl;

                cout << "    Raw Size: "
                     << videoDevice.getBufferLength()
                     << endl;

                encoder.encode( videoDevice.getBuffer() );

                std::vector< uchar > eBuffer;
                do
                {
                    eBuffer = encoder.getCopyOfEncodedBuffer();
                }
                while( eBuffer.size() == 0 );

                cout << "    Encoded size: "
                     << eBuffer.size()
                     << endl;

                std::vector< uchar > dBuffer;
                do
                {
                    decoder.decode( eBuffer );
                    dBuffer = decoder.getCopyOfDecodedBuffer();
                }
                while( dBuffer.size() == 0 );

                cout << "    Decoded size: "
                     << dBuffer.size()
                     << endl;

                ++frameIndex;
            }
        }
    }
    catch( std::exception & e )
    {
        cout << e.what() << endl;
    }
}
