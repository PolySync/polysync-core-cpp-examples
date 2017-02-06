### Record

Record is more of a tool than an example; it exposes much of the functionality of the RecordSession API as a command line tool.
This is an interactive application which expects you to enter the record session ID.  It allows you to stop the recording.
As an example this code exposes to the user how to properly access the RecordSession APIs functionality.

### Hardware requirements

A sensor or video device

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu

```bash
sudo apt-get install <package>
```

### Building and Running the Node

```bash
$ cd Record 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-record-cpp
```

For more API examples visit the "Turorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/)