### QtImageViewer

This example creates a GUI application using Qt to visualize PolySync image data.
It contains a threaded VideoProcessor (polysync::Node) that receives image data from the PolySync
bus and packages it for rendering in a QWidget.

### Hardware requirements

Sensors: Video Device

* Note: Sensor must be configured to publish MJPEG.
 
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




