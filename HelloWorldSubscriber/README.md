### HelloWorldSubscriber

This example is the most basic C++ application you can have that subscribes to data through high-level messages from the PolySync bus.

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install libglib2.0-dev
```

### Building and running the node

```bash
$ cd HelloWorldSubscriber 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-helloworld-subscriber-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
