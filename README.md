# Soil dynamics simulator

[![Build status](https://github.com/KennyVilella/soil_dynamics_cpp/workflows/CI/badge.svg)](https://github.com/KennyVilella/soil_dynamics_cpp/actions)
[![](https://img.shields.io/badge/docs-main-blue.svg)][docs-main]

This soil dynamics simulator is a fast first-order model designed to simulate soil displacement caused by the movement of an excavator bucket.
It employs a cellular automata approach to model the behaviour of the soil.
The bucket geometry is assumed to be a simple triangular prism, and the simulator operates on a grid composed of rectangular cells with equal length in the X and Y directions, and a height that is less than or equal to the cell's length.
A crucial requirement of the simulator is that the terrain must be updated every time the bucket moves by more than one cell.

The primary objective of the simulator is to provide terrain updates in less than 1 ms, making it suitable for real-time applications.

<code><b>Note:
A Julia version of this simulator is available [here](https://github.com/KennyVilella/soil_dynamics_julia).</b></code>

## To-do list

There are several important features that are yet to be implemented.
These include, in order of priority:

- Integration testing: Conduct thorough integration tests to ensure the functionality of the simulator.
- Multiple digging buckets: Add support for simulating the behaviour of multiple digging buckets simultaneously.
- Force calculation: Incorporate force calculation methods for better integration with rigid body engines.
- Heterogeneous soil properties: Extend the simulator to handle soil properties that vary across the terrain.

## Installation

To install the simulator, first create the build folder
```
cmake -S <path_to_repository> -B <path_to_repository>/build
```

and then execute the following command to build an executable
```
cmake --build <path_to_repository>/build --target soil_dynamics
```

and the following command to build a static library
```
cmake --build <path_to_repository>/build --target soil_simulator
```

## Running the simulator

An example script for using the simulator can be found in the `test/example` folder.
The `soil_evolution.cpp` file contains the implementation.
To run the example script, build the simulator with the following command
```
cmake --build <path_to_repository>/build --target soil_evolution
```

and then execute the following file
```
./<path_to_repository>/build/test/example/soil_evolution
```

Options of the simulator can be changed in `test/example/main.cpp`.

To run the unit tests, build and execute with the following commands
```
cmake --build <path_to_repository>/build --target unit_tests
./<path_to_repository>/build/test/unit_tests/unit_tests
```

To run the benchmark functions, build and execute with the following commands
```
cmake --build <path_to_repository>/build --target benchmarks
./<path_to_repository>/build/test/benchmarks/benchmarks
```

## Visualizing results

The example script provides options to write the results into CSV files.
The following options can be enabled to write the results at each time-step:
- `WriteBucket`: Write the bucket corners.
- `WriteSoil`: Write the terrain and the soil resting on the bucket.

[ParaView][] can be used to visualize the results.
To do so, follow these steps in Paraview
- File -> Load state -> select the file `paraview_state.pvsm` in the `test/example` folder.
- In the pop-up window, select appropiately the three group of files to visualize.
Note that some warnings/errors may appear when visualizing.


[docs-main]: https://kennyvilella.github.io/soil_dynamics_cpp/
[ParaView]: https://www.paraview.org
