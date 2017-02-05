### SharedMemoryImageDataViewer
This code is an example application that demonstrates using the PolySync Shared Memory C++ API to read, encode, decode, and view data placed in shared memory by a video device driver writing to shared memory using the PolySync Shared Memory API in C++.

You must set up the shared memory queue buffer size prior to starting the application.
It expects RGB24 pixel format.

### Hardware requirements

### Dependencies
A system installation of Qt is required for this to compile.
Qt operates under [LGPLv3](http://www.gnu.org/licenses/lgpl-3.0.en.html) 
Download and install Qt [here](http://www.qt.io/download/)

### Building and Running the Node
```bash
$ cd SharedMemoryImageDataViewer 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-sharedmem-image-data-viewer-cpp
```

For more API examples visit the "Turorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/)