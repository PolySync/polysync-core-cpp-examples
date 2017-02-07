### SharedMemoryImageDataViewer

This code is an example application that demonstrates using the PolySync Shared Memory C++ API. This can be used to read, encode, decode, and view data placed in shared memory by a video device driver writing to shared memory using the PolySync Shared Memory API in C++.

You must set up the shared memory queue buffer size prior to starting the application.
It requires RGB24 pixel format.

### Hardware requirements

Sensors: LiDAR, RADAR
Video Device

### Dependencies

A system installation of Qt is required for this to compile.
Qt operates under [LGPLv3](http://www.gnu.org/licenses/lgpl-3.0.en.html). 
Download and install Qt [here](http://www.qt.io/download/).

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install <package>
````

### Building and running the node

```bash
$ cd SharedMemoryImageDataViewer 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-sharedmem-image-data-viewer-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
