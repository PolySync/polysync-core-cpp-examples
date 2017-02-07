### SerialConfig

This is an example of the Serial API used to configure a serial port.
It requires two serial ports, as the example prints strings between the two ports.

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install <package>
```

### Building and running the node

```bash
$ cd SerialConfig 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-serial-config-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
