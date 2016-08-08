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
 * \example TransformStack.cpp
 *
 * Transform Example to demonstrate a transform stack.
 *
 * Shows how to use the transform API to perform multiple transforms in the
 * same stack.  The transforms can incorporate changes in orientation and
 * spatial location e.g. the object is spinning while moving forward.
 *
 * Transform Class definition
 */

#include <iostream>

#include <PolySyncTransform.hpp>
#include <PolySyncDTCException.hpp>

using namespace polysync;

int main()
{
    // The transform object can throw one of several DTC errors for a given
    // function call. This is a large try-catch block to describe where the
    // error occurred.
    try
    {
        // Create a Transform object, an origin, and an orientation
        Transform transform;

        // Variable to store identifiers in the transform stack
        ps_identifier id = PSYNC_COORDINATE_FRAME_PLATFORM;

        // The first transform is strictly a scalar in space.
        // There is no change in orientation.

        // Set the origin (10,0,0)
        transform.setOrigin( { 10.0, 0.0, 0.0 } );

        // Set the orientation
        transform.setOrientation( { 0.0, 0.0, 0.0 } );

        // Push the transform on to the transform stack using the ps_identifier
        // PSYNC_COORDINATE_FRAME_PLATFORM
        transform.push( id );

        // The second stage of the transform involves both a change in the
        // origin and a change in orientation.
        // Transform moves to the right and rotates around the z-axis
        transform.setOrigin( { 0.0, -10, 0.0 } );
        transform.setOrientation( { 0.0, 0.0, 2.0 } );

        // Increment ID so that the next push has a unique ID
        id++;
        transform.push( id );

        // Apply the entire transformation from the start to the end point
        std::array< double, 3 > startPt = { 1.0, 1.0, 1.0 };

        auto endPt = transform.apply( startPt );

        // Display the results of the transform
        std::cout << "Starting position: " << startPt[P_X] << ", "
                  << startPt[P_Y] << ", " << startPt[P_Z] << std::endl;

        std::cout << "Ending position: " << endPt[P_X] << ", " << endPt[P_Y];
        std::cout << ", " << endPt[P_Z] << std::endl;

    }
    catch ( polysync::DTCException& d )
    {
        std::cout << "polysync-transform-stack-cpp encountered the following error: "
                  << d.what() << std::endl;
    }

    return 0;
}

