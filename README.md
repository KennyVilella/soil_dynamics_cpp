# Soil dynamics simulator

[![Build status](https://github.com/KennyVilella/soil_dynamics_cpp/workflows/CI/badge.svg)](https://github.com/KennyVilella/soil_dynamics_cpp/actions)
[![](https://img.shields.io/badge/docs-main-blue.svg)][docs-main]

<code><b>Note:
This repository is a very preliminary conversion into c++ of this [repository](https://github.com/KennyVilella/soil_dynamics_julia).</b></code>

This soil dynamics simulator is a fast first-order model designed to simulate soil displacement caused by the movement of an excavator bucket.
It employs a cellular automata approach to model the behavior of the soil.
The bucket geometry is assumed to be a simple triangular prism, and the simulator operates on a grid composed of rectangular cells with equal length in the X and Y directions, and a height that is less than or equal to the cell's length.
A crucial requirement of the simulator is that the terrain must be updated every time the bucket moves by more than one cell.

The primary objective of the simulator is to provide terrain updates in less than 1 ms, making it suitable for real-time applications.

## To-do list

There are several important features that are yet to be implemented.
These include, in order of priority:

- Documentation: Provide comprehensive and user-friendly documentation for the simulator.
- Integration testing: Conduct thorough integration tests to ensure the functionality of the simulator.
- Multiple digging buckets: Add support for simulating the behavior of multiple digging buckets simultaneously.
- Force calculation: Incorporate force calculation methods for better integration with rigid body engines.
- Heterogeneous soil properties: Extend the simulator to handle soil properties that vary across the terrain.

[docs-main]: https://kennyvilella.github.io/soil_dynamics_cpp/
