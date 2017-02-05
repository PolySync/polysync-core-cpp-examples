### VideoDeviceViewer
This example demonstrates how to use the Video API routines to communicate with a video device, and encode/decode the data.

You would use this example if you needed a node to talk directly to a video device, and didn’t want to use the `generic-video-device` driver, which supports all V4L USB video devices.
It connects to a USB webcam (that must be Video4Linux (V4L) compatible), encodes the sensor data, and then decodes the same image stream to show the image processing pipeline.
This DOES NOT connect to a node defined in the SDF, it connects directly to the hardware device using the Video API.
It expects XXXXX pixel format

### Hardware requirements

### Dependencies

### Building and Running the Node
```bash
$ cd VideoDeviceViewer 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-video-encode-decode-cpp
```

For more API examples visit the "Turorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/)