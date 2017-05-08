### PublishSubscribe

This example illustrates the simple publish and subscribe functionality found in PolySync.
It publishes an empty `ps_event_msg` to demonstrate how the publish functionality works in PolySync.
It subscribes to all diagnostic messages on the PolySync bus, and then prints the info each diagnostic message contains as they are received in real time.

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install libglib2.0-dev
```

### Building and running the node

```bash
$ cd PublishSubscribe 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-publisher-subscriber-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
