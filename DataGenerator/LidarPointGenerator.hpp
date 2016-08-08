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
 * @file LidarPointGenerator.hpp
 * @brief LidarPointGenerator Source
 *
 */

#ifndef LIDARPOINTS_HPP
#define LIDARPOINTS_HPP

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>


class LidarPointGenerator
{
public:
    LidarPointGenerator( polysync::Node & );

    void updatePoints();

    void publishPoints();

    void initializeMessage();

private:

    polysync::datamodel::LidarPointsMessage _message;

    float _relativeTime{ 0.0 };
    const float _gridScale{ 10.0 };
    const ulong _gridSideLength{ 100 };
    const ulong _sensorID{ 11 };
    const ulong _numberOfPoints{ 10000 };
    const float _sineFrequency{ 4.0 };
};

#endif // LIDARPOINTS_HPP
