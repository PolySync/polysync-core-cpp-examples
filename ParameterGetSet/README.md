### ParameterGetSet

This example demonstrates the logic used to filter for a specific node on the bus, and uses the ParameterGetSet API to update a specific parameter ID at runtime.
It publishes and subscribes the `ps_parameters_msg`, and checks and then updates the coordinate frame identifier, specifically for a video device node.

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu:

```bash
sudo apt-get install <package>
```

### Building and running the node

```bash
$ cd ParameterGetSet 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-parameter-get-set-cpp
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
