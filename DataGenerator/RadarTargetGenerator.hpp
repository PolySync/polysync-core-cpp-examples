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
 * @file RadarTargetGenerator.hpp
 * @brief RadarTargetGenerator Header.
 *
 */

#ifndef RADARTARGETGENERATOR_HPP
#define RADARTARGETGENERATOR_HPP

#include <PolySyncDataModel.hpp>
#include <PolySyncNode.hpp>

class RadarTargetGenerator
{
public:
    RadarTargetGenerator( polysync::Node & );

    void initializeMessage();
    void updateTargets();
    void publishTargets();
    void resetTargets();

private:
    polysync::datamodel::RadarTargetsMessage _message;

    std::vector< polysync::datamodel::RadarTarget > _targetVector;

    const ps_identifier _sensorID { 11 };
    const int _targetsCount{ 2 };
    const double _amplitude{ 2.0 };
    const double _velocityX{ 3.5 };
    const double _velocityY{ 3.5 };
    bool _reset{ false };
};

#endif // RADARTARGETGENERATOR_HPP
