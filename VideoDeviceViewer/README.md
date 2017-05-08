### VideoDeviceViewer

This code is an example application to demonstrate interaction with a video device using the PolySync Video API in C++.
It will visualize LiDAR, RADAR, and object data from multiple sources.
This example has tools to freeze frame and measure distance, color by source or type, and is great for building sensor fusion applications.
The graphics were built with OpenGL and are very extendable.
It has a built-in system/node management tool to dynamically handle nodes coming on/off bus.

### Hardware requirements

Sensors: LiDAR, RADAR
Video Device

### Dependencies

A system installation of Qt is required for this to compile.
Qt operates under [LGPLv3](http://www.gnu.org/licenses/lgpl-3.0.en.html). 
Download and install Qt [here](http://www.qt.io/download/).

Packages: libglib2.0-dev libgstreamer1.0-0

To install on Ubuntu:

```bash
sudo apt-get install libglib2.0-dev libgstreamer1.0-0
```

### Building and running the node

```bash
$ cd VideoDeviceViewer 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-video-device-viewer-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
