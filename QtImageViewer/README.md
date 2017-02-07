### QtImageViewer

This is an example of a threaded C++ PolySync node.
This code is an example application intended to demonstrate an interaction with PolySync image data using the C++ PolySync APIs.
It will visualize LiDAR, RADAR, and object data from multiple sources in a Qt window.
This example has tools to freeze frame and measure distance, color by source or type, and are great for building sensor fusion applications.
The graphics were built with OpenGL and are very extendable.
It has a built-in system/node management tool to dynamically handle nodes coming on/off bus.

### Hardware requirements

Sensors: LiDAR, RADAR
Video Device

### Dependencies

A system installation of Qt is required for this to compile.
Qt operates under [LGPLv3](http://www.gnu.org/licenses/lgpl-3.0.en.html) 
Download and install Qt [here](http://www.qt.io/download/)

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install <package>
```

### Building and running the node

```bash
$ cd QtImageViewer 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-qt-image-viewer-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).




