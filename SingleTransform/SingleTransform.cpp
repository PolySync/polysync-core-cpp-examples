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
 * \example SingleTransform.cpp
 *
 * Transform Example in C++ to demonstrate a single level transform.
 *
 * Shows how to use the transform API to perform a single operation on the
 * transform stack.
 *
 *
 * Transform Class definition
 *      file:///usr/local/polysync/doc/cpp_api_docs/html/d9/d32/classpolysync_1_1_transform.html
 */

#include <iostream>

#include <PolySyncTransform.hpp>
#include <PolySyncDTCException.hpp>

using namespace polysync;

int main()
{
    // The transform object can throw one of several DTC errors for a given function call.
    // This is a large try-catch block to describe where the error occurred.
    try
    {
        // Create a Transform object, an origin, and an orientation
        Transform transform;

        // Set the origin using an array of doubles that contain the X,Y,Z
        // coordinates of the origin
        transform.setOrigin( { 0.0, 0.0, 0.0 } );

        // Set the orientation using an array of doubles that contain the
        // roll, pitch, yaw, of the transform
        transform.setOrientation( { 1.0, 2.0, 3.0 } );

        // Push the transform on to the transform stack using the ps_identifier
        // PSYNC_COORDINATE_FRAME_PLATFORM
        transform.push( PSYNC_COORDINATE_FRAME_PLATFORM );

        // Apply the entire transformation from the start to the end point
        auto endPoint = transform.apply( { 3.3, 2.2, 1.1 } );

        // Display the results of the transform
        std::cout << "Starting position: "
                  << 3.3
                  << ", "
                  << 2.2
                  << ", "
                  << 1.1
                  << std::endl;
        std::cout << "Ending position: " << endPoint[P_X] << ", ";
        std::cout << endPoint[P_Y] << ", ";
        std::cout << endPoint[P_Z] << std::endl;

    }
    catch ( polysync::DTCException& d )
    {
        std::cout << "The example encountered the following error: "
                  << d.what()
                  << std::endl;
    }

    return 0;
}

