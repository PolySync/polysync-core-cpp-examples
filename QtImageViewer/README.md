### QtImageViewer

This example creates a GUI application using Qt to visualize PolySync image data.
It contains a threaded VideoProcessor (polysync::Node) that receives image data from the PolySync
bus and packages it for rendering in a QWidget.

### Hardware requirements

Sensors: Video Device

* Note: Sensor must be configured to publish MJPEG.
 
### Install Dependencies

A Qt install and mesa-common-dev Debian package are required to run this example.

1. [Download Qt installer here](http://www.qt.io/download/)

2. Navigate to Downloads directory

    `cd ~/Downloads`

3. Make the installer executable **NOTE**: make sure the name of the *.run file matches

    `chmod +x qt-unified-online-installer.run`

4. Run the installer, this can take up to 30 minutes **NOTE**: Only the desktop libraries are required, you can uncheck other elements (such as QtCreator) in the installer to speed up the process.

    `./qt-unified-online-installer.run`

5. Install OpenGL

    `sudo apt install mesa-common-dev`

6. Follow the build instructions.

7. After building the QtImageViewer, move a qt.conf file into the build file for the QtImageViewer. 
 - The qt.conf file overrides the hard coded paths compiled into the QT library

8. Run the node.


### Building and running the node

#### Build

```bash
# Tell cmake where to find Qt, note that the path listed below may be different (your version may not be 5.8)
$ export CMAKE_PREFIX_PATH=/path/to/Qt/5.8/gcc_64/lib/cmake
$ cd QtImageViewer 
$ mkdir build && cd build
$ cmake ..
$ make
$ mv ../res/qt.conf qt.conf
```

#### Run

`$ ./polysync-qt-image-viewer-cpp`


For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).




