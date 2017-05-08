### DataGenerator

This example covers how to populate and publish data from a single node into a PolySync message on the bus, 
enable multiple nodes to subscribe to messages to access data asynchronously, and visualize all data being 
published to the PolySync bus.

This example generates data and publishes it on the PolySync bus by generating fake radar, lidar, and object data.
It periodically publishes `ps_lidar_points_msg`, `ps_radar_targets_msg`, and `ps_objects_msg` to the bus in the OK state.
It uses a lightweight diagnostic tool to tell if a system is properly up and running. 

There is an article in the PolySync Help Center that describes the C++ version of this example:
[Data Generation Tutorial](https://help.polysync.io/articles/tutorials-and-examples/tutorials/data-generation-tutorial/)

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install libglib2.0-dev
```

### Building and running the node

```bash
$ cd DataGenerator 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-data-generator-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
