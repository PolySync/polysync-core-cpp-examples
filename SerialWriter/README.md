### SerialWriter

This can be used as a reference if a node is required to speak directly to a device over a serial connection.
It pairs nicely with the `SerialReader` example.
This example creates and writes data to a serial connection, to a hardware device, or to another software application.

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install libglib2.0-dev
```

### Building and running the node

```bash
$ cd SerialWriter 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-serial-writer-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
