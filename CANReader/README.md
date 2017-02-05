### CANReader
Using the PolySync state machine this example does a simple CAN read in OK the state.
This example will give you experience for the times you need a node to listen to a CAN channel.

### Hardware requirements
- linuxcan-compatible hardware is connected, i.e. a Kvaser Leaf Light

### Dependencies
You need to have an CAN channel to run this example see:  [Connecting To A CAN Sensor](https://help.polysync.io/articles/configuration/runtime-node-configuration/connecting-to-a-can-radar-sensor/)

### Building and Running the Node
```bash
$ cd CANReader 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-can-reader-cpp 1
```

For more API examples visit the "Turorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/)
