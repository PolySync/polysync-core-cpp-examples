### CANReaderAndPublisher

This example also acts as a guide for when you need a node to read and publish data on a CAN channel.
Using the PolySync state machine, this example does a simple CAN read and publishes each CAN frame received to the PolySync bus as a `ps_can_frame_msg`.

### Hardware requirements

- linuxcan-compatible hardware is connected, i.e. a Kvaser Leaf Light

### Dependencies

You need to have a CAN channel to run this example, see:  [Connecting To A CAN Sensor](https://help.polysync.io/articles/configuration/runtime-node-configuration/connecting-to-a-can-radar-sensor/)

Packages: libglib2.0-dev

To install on Ubuntu

```bash
sudo apt-get install libglib2.0-dev
```

### Building and running the node

```bash
$ cd CANReaderAndPublisher 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-can-reader-and-publisher-cpp 1
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
