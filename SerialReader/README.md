### SerialReader

This can be used as a reference if a node is required to listen to a device over a serial connection.
This example creates and reads data through a serial connection to a hardware device, or another software application.
It exercises the Serial API and pairs nicely with the `SerialWriter` example.

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install <package>
```

### Building and running the node

```bash
$ cd SerialReader 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-serial-reader-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
