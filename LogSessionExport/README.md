### LogSessionExport

This example demonstrates how to use the PolySync Log Session Transfer API to export a recorded log session, so that it can be imported and replayed on any other PolySync system.

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install <package>
```

### Building and running the node

```bash
$ cd LogSessionExport 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-log-session-export-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
