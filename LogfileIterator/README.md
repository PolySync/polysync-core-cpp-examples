### LogfileIterator
This example is very powerful and very popular. It uses the logfile API.
It allows a user to iterate over a specified logfile manually and read its contents.
A logfile iterator callback is set up, which is called every time a new “record” is popped from the logfile. 
This example then prints some details from the header of each of the records
The logfile iteration happens many times faster than real time.
Be aware lot of people have issues with this example because they don’t realize that just because a message publishes 
a message type doesn’t mean it logs that same message type. 

For example most CAN drivers log with `ps_can_frame_msg` but publish `ps_radar_targets_msg`, `ps_objects_msg` etc. 
This results in them trying to cast new logfile frames to `ps_radar_targets_msg` etc instead of their actual format.

### Hardware requirements

### Dependencies

### Building and Running the Node
```bash
$ cd LogfileIterator
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-logfile-iterator-cpp
```

For more API examples visit the "Turorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/)