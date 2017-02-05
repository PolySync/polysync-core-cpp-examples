### PublishSubscribe
This example shows simple publish and subscribe functionality in PolySync.
It publishes an empty `ps_event_msg` to show how the publish functionality works in PolySync.
It subscribes to all diagnostic messages on the PolySync bus and then prints the info each diagnostic message contains, as they are received in real time.

### Hardware requirements

### Dependencies

### Building and Running the Node
```bash
$ cd PublishSubscribe 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-publisher-subscriber-cpp
```

For more API examples visit the "Turorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/)