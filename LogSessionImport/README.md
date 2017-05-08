### LogSessionImport

This example demonstrates how to use the PolySync Log Session Transfer API to import a previously exported log session to a new distributed system.

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install libglib2.0-dev
```

### Building and running the node

```bash
$ cd LogSessionImport 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-log-session-import-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
