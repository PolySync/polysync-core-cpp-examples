### CANWriter
Using the PolySync state machine this example does a simple CAN read in OK the state.
This example will give you experience for the times you need a node to publish data to a CAN channel.

### Hardware requirements
- linuxcan-compatible hardware is connected, i.e. a Kvaser Leaf Light

### Dependencies

### Building and Running the Node
```bash
$ cd CANWriter 
$ mkdir build && cd build
$ cmake ..
$ make
# You need to have an CAN channel to run this example see:  [Connecting To A CAN Sensor](https://help.polysync.io/articles/configuration/runtime-node-configuration/connecting-to-a-can-radar-sensor/)
$ ./polysync-can-writer-cpp 1
```

For more API examples visit the "Turorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/)
