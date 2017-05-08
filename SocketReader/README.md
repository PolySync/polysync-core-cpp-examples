### SocketReader

This is a simple example illustrating UDP socket communication in PolySync when using the PolySync socket API.
It reads from the UDP socket using API calls and socket class.

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install libglib2.0-dev
```

### Building and running the node

```bash
$ cd SocketReader 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-socket-reader-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
