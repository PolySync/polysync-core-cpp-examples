### LogfileReader

This example uses the Logfile API to replay a PolySync logfile `plog` file.
Even though it depends on a file in the `/tmp/` directory, it can still be easily updated to point to any existing `plog` file (the intention of the example).
   /tmp/polysync_logfile.plog

Steps to run: 

   1. Update the file `src/logfile_reader.c` on line 74 to point to an existing `plog` file, (point to new rnr_logs location)
   2. Compile
   3. Run

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install libglib2.0-dev
```

### Building and running the node

```bash
$ cd LogfileReader 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-logfile-reader-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
