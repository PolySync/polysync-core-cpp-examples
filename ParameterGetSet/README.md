### ParameterGetSet
This example demonstrates the logic used to filter for a specific node on the bus, and use the Parameter get/set API to update a specific parameter ID at runtime
It publishes and subscribes the `ps_parameters_msg`
It checks and then updates the coordinate frame identifier, specifically for a video device node


### Hardware requirements

### Dependencies

### Building and Running the Node
```bash
$ cd ParameterGetSet 
$ mkdir build && cd build
$ cmake ..
$ make
$ ./polysync-parameter-get-set-cpp
```

For more API examples visit the "Turorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/)