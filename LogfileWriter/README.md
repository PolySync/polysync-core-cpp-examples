### LogfileWriter

This is an example that uses the Logfile API to replay a PolySync logfile `plog` file
Shows how to use the Logfile API routines to write a PolySync byte array message to a PolySync logfile `plog`

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu

```bash
sudo apt-get install <package>
```

### Building and Running the Node

```bash
$ cd LogfileWriter 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-logfile-writer-cpp
```

For more API examples visit the "Turorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/)