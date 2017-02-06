### LogfileQueueReader

This is example is useful when you want to access the raw sensor data, just as it was sent on the wire, before the dynamic driver processes and abstracts the data

{{% note %}} This does not effect the functionality of the dynamic driver, it will still process and publish the high-level message types, as defined in the SDF Configurator node entry. {{% /note %}}

You would use this node if you wanted to filter or pre-process the sensor data before it’s published to the PolySync bus.

This is an example of a node that uses the Logfile API routines to replay a PolySync logfile using the message queue instead of subscribing to a high-level PolySync message type.
This node accesses the raw, non-abstracted `plog` data, and leverging the dynamic driver Hardware Abstraction Layer (HAL) header file to cast the raw data to low-level OEM defined C structs.

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu

```bash
sudo apt-get install <package>
```

### Building and Running the Node

```bash
$ cd LogfileQueueReader 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-logfile-queue-reader-cpp
```

For more API examples visit the "Turorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/)