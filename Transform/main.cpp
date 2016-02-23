/*
 * PolySync Single Transform C++ API example application
 *      Transform from one coordinate system to another using the Transform API
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
        Point origin { 0.0, 0.0, 0.0 };
        Orientation orientation { 1.0, 2.0, 3.0 };

        // Use the PSYNC_COORDINATE_FRAME_PLATFORM as the base of the transform
        ps_identifier id = PSYNC_COORDINATE_FRAME_PLATFORM;

        // Set the origin
        transform.setOrigin( origin );

        // Set the orientation
        transform.setOrientation( orientation );

        // Push the transform on to the transform stack using the current id
        transform.push( id );

        // Create a start and end point
        struct Point startPoint { 3.3, 2.2, 1.1 };
        struct Point endPoint { 0.0, 0.0, 0.0 };

        // Apply the entire transformation from the start to the end point
        transform.apply( startPoint, endPoint );

        // Display the results of the transform
        std::cout << "Starting position: " << startPoint.x << ", " << startPoint.y << ", " << startPoint.z << std::endl;
        std::cout << "  Ending position: " << endPoint.x << ", " << endPoint.y << ", " << endPoint.z << std::endl;

    }
    catch ( DTCException& d )
    {
        std::cout << "The example encountered the following error: " << d.what() << std::endl;
    }

    return 0;
}

