### CSVExporter

Subscribes to a limited set of message types (below), and writes the incoming data to a CSV file (one CSV file for each message type)
   `ps_platform_motion_msg`
   `ps_objects_msg`
   `ps_lane_model_msg`
   `ps_lidar_points_msg`
   `ps_radar_targets_msg`
   `ps_traffic_sign_msg`

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu

```bash
sudo apt-get install <package>
```

### Building and Running the Node

```bash
$ cd CSVExporter 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-csv-export
```

For more API examples visit the "Turorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/)