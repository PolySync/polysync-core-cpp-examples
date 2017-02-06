### PathPlanner

This is a C++ algorithm example.
This example will build a basic path planner algorithm in PolySync. We will apply a basic A\* planner to a simulated search space, generate the optimal path to the goal state, and then move a simulated robot along that path to the goal. The optimal path is sent one waypoint at a time as a `PlatformMotionMessage`, which is defined in the [C++ Sensor Data Model](http://polysync.github.io/Docs-2.0/#platformmotionmessage) Module.

For a detailed description, see:  [Path Planner Tutorial](https://help.polysync.io/articles/tutorials-and-examples/tutorials/path-planner-tutorial/).

### Dependencies

[OpenCV for Linux/Mac - Version 2.4.13](https://github.com/Itseez/opencv/archive/2.4.13.zip)

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install <package>
```

### Building and running the node

```bash
$ cd PathPlanner 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-path-planner-robot-cpp
# In another terminal window, run the planner node
$ ./polysync-path-planner-algorithm-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
