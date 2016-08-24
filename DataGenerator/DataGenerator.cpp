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
 * \example DataGenerator.cpp
 *
 * PolySync CAN API Reader Example.
 *
 * Shows how to use the CAN API to read CAN frames.
 *
 * @file DataGenerator.cpp
 * @brief DataGenerator Source
 *
 */

#include <iostream>

#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

#include "LidarPointGenerator.hpp"
#include "RadarTargetGenerator.hpp"
#include "ObjectGenerator.hpp"


using namespace polysync::datamodel;


class DataGenerator : public polysync::Node
{

protected:

    /**
     * Override the base class functionality to send messages when the node
     * reaches the "ok" state. This method is triggered continuously while
     * this node is in an operational state.
     */
    virtual void okStateEvent()
    {
        _lidarPointGenerator->updatePoints();

        _lidarPointGenerator->publishPoints();

        _radarTargetGenerator->updateTargets();

        _radarTargetGenerator->publishTargets();

        _objectGenerator->updateObjects();

        _objectGenerator->publishObjects();

        polysync::sleepMicro( _updateInterval );
    }

    /**
     * The initStateEvent is triggered once when this node is initialized in
     * PolySync. This is a good place to initialize variables dependant on a
     * polysync::Node reference.
     */
    virtual void initStateEvent()
    {
        _lidarPointGenerator =
                std::unique_ptr< LidarPointGenerator >{
                    new LidarPointGenerator( *this ) };

        _radarTargetGenerator =
                std::unique_ptr< RadarTargetGenerator >{
                    new RadarTargetGenerator( *this ) };

        _objectGenerator =
                std::unique_ptr< ObjectGenerator >{
                    new ObjectGenerator( *this ) };
    }

    /**
     * The release state is called once when polysync::Node::disconnectPolySync()
     * is called ( ctrl-c in this case ). In this event, the node is still
     * valid in PolySync. We need to release the data generators to allow them
     * to free their polysync::datamodel messages while the node is still valid.
     */
    virtual void releaseStateEvent()
    {
        _lidarPointGenerator.release();

        _radarTargetGenerator.release();

        _objectGenerator.release();
    }

private:

    ps_timestamp _updateInterval{ 50000 };

    std::unique_ptr< LidarPointGenerator > _lidarPointGenerator;

    std::unique_ptr< RadarTargetGenerator > _radarTargetGenerator;

    std::unique_ptr< ObjectGenerator > _objectGenerator;

};

/**
 * Entry point for the publisher side of this tutorial application
 * The "connectPolySync" is a blocking call, users must use Ctrl-C to exit
 * this function.
 */
int main()
{
    DataGenerator dataGenerator;

    dataGenerator.connectPolySync();

    return 0;
}
