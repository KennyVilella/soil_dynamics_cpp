# Description of unit tests

## Introduction

The description for all unit tests are given below separated by files and functions.
The order in which the unit tests appear in this file is consistent with the order in which they are run, and this order is selected such as the fucntionalities of the simulator are tested step by step.
This helps to identify more precisely the location of potential bugs.

Note that a custom name is associated with each unit test in order to make their identification easier.

## `test_types.cpp`

This file implements unit tests for the classes in the `types.cpp` file.
It should be tested first as mostly all functions in the simulator rely on these classes.

### `Grid`

Unit tests for the `Grid` class and its constructor.

| Test name | Description of the unit test                                                       |
| --------- | ---------------------------------------------------------------------------------- |
| TY-G-1    | Testing that all members of the `Grid` class are present and properly initialized. |
| TY-G-2    | Testing that an exception is raised when `size_z <= 0.0`.                          |
| TY-G-3    | Testing that an exception is raised when `size_xy <= 0.0`.                         |
| TY-G-4    | Testing that an exception is raised when `grid_size_x <= 0.0`.                     |
| TY-G-5    | Testing that an exception is raised when `grid_size_y <= 0.0`.                     |
| TY-G-6    | Testing that an exception is raised when `grid_size_z <= 0.0`.                     |
| TY-G-7    | Testing that an exception is raised when `size_z > size_xy`.                       |
| TY-G-8    | Testing that an exception is raised when `grid_size_x < size_xy`.                  |
| TY-G-9    | Testing that an exception is raised when `grid_size_y < size_xy`.                  |
| TY-G-10   | Testing that an exception is raised when `grid_size_z < size_z`.                   |

### `Bucket`

Unit tests for the `Bucket` class and its constructor.

| Test name | Description of the unit test                                                         |
| --------- | ------------------------------------------------------------------------------------ |
| TY-B-1    | Testing that all members of the `Bucket` class are present and properly initialized. |
| TY-B-2    | Testing that an exception is raised when size of `o_pos` is not 3.                   |
| TY-B-3    | Testing that an exception is raised when size of `j_pos` is not 3.                   |
| TY-B-4    | Testing that an exception is raised when size of `b_pos` is not 3.                   |
| TY-B-5    | Testing that an exception is raised when size of `t_pos` is not 3.                   |
| TY-B-6    | Testing that an exception is raised when `j_pos` and `b_pos` are the same.           |
| TY-B-7    | Testing that an exception is raised when `j_pos` and `t_pos` are the same.           |
| TY-B-8    | Testing that an exception is raised when `b_pos` and `t_pos` are the same.           |
| TY-B-9    | Testing that an exception is raised when `bucket_width <= 0.0`.                      |

### `SimParam`

Unit tests for the `SimParam` class and its constructor.

| Test name | Description of the unit test                                                            |
| --------- | --------------------------------------------------------------------------------------- |
| TY-SP-1   | Testing that all members of the `SimParam` class are present and properly initialized.  |
| TY-SP-2   | Testing that an exception is raised when `repose_angle > pi/2` or `repose_angle < 0.0`. |
| TY-SP-3   | Testing that an exception is raised when `max_iterations <= 0.0`.                       |
| TY-SP-4   | Testing that an exception is raised when `cell_buffer < 2`.                             |

### `SimOut`

Unit tests for the `SimOut` class and its constructor.

| Test name | Description of the unit test                                                         |
| --------- | ------------------------------------------------------------------------------------ |
| TY-SO-1   | Testing that all members of the `SimOut` class are present and properly initialized. |

## `test_utils.cpp`

This file implements unit tests for the classes in the `utils.cpp` file.
It should be tested before the main functionalities of the simulator since the utility functions are used throughout the simulator.

### `CalcBucketCornerPos`

Unit tests for the `CalcBucketCornerPos` function.

| Test name | Description of the unit test                                              |
| --------- | ------------------------------------------------------------------------- |
| UT-CBC-1  | Testing for a bucket in its reference pose.                               |
| UT-CBC-2  | Testing for a bucket with a simple translation applied.                   |
| UT-CBC-3  | Testing for a bucket with a simple rotation applied.                      |
| UT-CBC-4  | Testing for a bucket with both a simple rotation and translation applied. |

### `CheckBucketMovement`

Unit tests for the `CheckBucketMovement` function.

| Test name | Description of the unit test                                                    |
| --------- | ------------------------------------------------------------------------------- |
| UT-CBM-1  | Testing for a one cell translation following the X axis.                        |
| UT-CBM-2  | Testing for an arbitrary translation.                                           |
| UT-CBM-3  | Testing for a 8 degrees rotation around the Y axis.                             |
| UT-CBM-4  | Testing for a 8 degrees rotation around the Y axis combined with a half cell translation following the X axis. |
| UT-CBM-5  | Testing for a translation much shorter than the cell size following the X axis. |
| UT-CBM-6  | Testing for an arbitrary translation much shorter than cell size.               |
| UT-CBM-7  | Testing for a 0.33 degree rotation around the Y axis.                           |
| UT-CBM-8  | Testing for a 0.33 degree rotation around the Y axis combined with a translation much shorter than the cell size following the X axis. |
| UT-CBM-9  | Testing for a large translation following the X axis.                           |

### `CalcNormal`

Unit tests for the `CalcNormal` function.
Note that for each unit test, two different input orders are investigated in order to ensure that the direction of the normal is properly calculated.

| Test name | Description of the unit test                                                                   |
| --------- | ---------------------------------------------------------------------------------------------- |
| UT-CN-1   | Testing for a triangle in the XY plane resulting in a unit normal vector following the Z axis. |
| UT-CN-2   | Testing for a triangle in the XZ plane resulting in a unit normal vector following the Y axis. |
| UT-CN-3   | Testing for a triangle in the YZ plane resulting in a unit normal vector following the X axis. |
| UT-CN-4   | Testing for a triangle in a 45 degrees inclined plane resulting in unit normal vector with a 45 degrees slope in the three axis. |

### `MultiplyQuaternion`

Unit tests for the `MultiplyQuaternion` function.

| Test name | Description of the unit test                                         |
| --------- | -------------------------------------------------------------------- |
| UT-MQ-1   | Testing the multiplication of two pi/2 rotations around the Z axis.  |
| UT-MQ-2   | Testing the multiplication of two pi/2 rotations around the Y axis.  |
| UT-MQ-3   | Testing the multiplication of two pi/2 rotations around the X axis.  |
| UT-MQ-4   | Testing the multiplication of two arbitrary rotations. The result has been checked with the `ReferenceFrameRotations` library in Julia. |

### `CalcRotationQuaternion`

Unit tests for the `CalcRotationQuaternion` function.

| Test name | Description of the unit test                                      |
| --------- | ----------------------------------------------------------------- |
| UT-CRQ-1  | Testing the application of a pi/2 rotation around the Z axis.     |
| UT-CRQ-2  | Testing the application of a pi/2 rotation around the Y axis.     |
| UT-CRQ-3  | Testing the application of a pi/2 rotation around the X axis.     |
| UT-CRQ-4  | Testing the application of an arbitrary rotation. The result has been checked with the `ReferenceFrameRotations` library in Julia. |

### `AngleToQuat`

Unit tests for the `AngleToQuat` function.

| Test name | Description of the unit test                                      |
| --------- | ----------------------------------------------------------------- |
| UT-AQ-1   | Testing the conversion of a pi/2 rotation around the Z axis.      |
| UT-AQ-2   | Testing the conversion of a pi/2 rotation around the Y axis.      |
| UT-AQ-3   | Testing the conversion of a pi/2 rotation around the X axis.      |
| UT-AQ-4   | Testing the conversion of an arbitrary rotation. The result has been checked with the `ReferenceFrameRotations` library in Julia. |

### `CalcBucketFramePos`

Unit tests for the `CalcBucketFramePos` function.

| Test name | Description of the unit test                                       |
| --------- | ------------------------------------------------------------------ |
| UT-CBF-1  | Testing for a bucket in its reference position and orientation. Input cell has an arbitrary position. |
| UT-CBF-2  | Testing for a bucket in its reference orientation and an arbitrary position. Input cell has an arbitrary position. |
| UT-CBF-3  | Testing for a bucket in its reference position and rotated by pi/2 around the Z axis. Input cell has an arbitrary position. |
| UT-CBF-4  | Testing for a bucket in its reference position and rotated by pi/2 around the Y axis. Input cell has an arbitrary position. |
| UT-CBF-5  | Testing for a bucket in its reference position and rotated by pi/2 around the X axis. Input cell has an arbitrary position. |
| UT-CBF-6  | Testing for a bucket rotated by pi/2 around the Z axis and an arbitrary position. Input cell has an arbitrary position. |

### `CheckVolume`

Unit tests for the `CheckVolume` function.
Note that for unit tests 1 to 3, results with correct and incorrect initial volumes are investigated to ensure that warning is sent only when an inconsistent initial volume is given.

| Test name | Description of the unit test                                       |
| --------- | ------------------------------------------------------------------ |
| UT-CV-1   | Testing with a `terrain_` everywhere at 0.0 and no `body_soil_`.   |
| UT-CV-2   | Testing with a `terrain_` everywhere at 0.0 except at one location and no `body_soil_`. |
| UT-CV-3   | Testing with a `terrain_` everywhere at 0.0 and some `body_soil_` present at various locations. |
| UT-CV-4   | Testing with the setup of UT-CV-3 that inconsistent amount of soil in `body_soil_pos_` results into a warning. It can be either not enough or too much soil. |

### `CheckSoil`

Unit tests for the `CheckSoil` function.
The unit tests 1 to 4 correspond to the building of the environment setup.
The subsequent unit tests check that improper environment setup yields a warning.
At the end of each unit test, the environment is set to a proper setup and it is checked that no warning is sent.

| Test name | Description of the unit test                                          |
| --------- | --------------------------------------------------------------------- |
| UT-CS-1   | Testing when everything is at zero.                                   |
| UT-CS-2   | Testing for an arbitrary `terrain_` setup.                            |
| UT-CS-3   | Testing for an arbitrary `terrain_` and `body_` setup.                |
| UT-CS-4   | Testing for an arbitrary `terrain_`, `body_`, and `body_soil_` setup. |
| UT-CS-5   | Testing when the `terrain_` is above the `body_`.                     |
| UT-CS-6   | Testing when `body_` is not set properly, that is the maximum height of a body layer is not strictly higher than its minimum height. |
| UT-CS-7   | Testing when `body_soil_` is not set properly, that is the maximum height of a body soil layer is not strictly higher than its minimum height. |
| UT-CS-8   | Testing when `body_` is intersecting with its `body_soil_`.           |
| UT-CS-9   | Testing when there is a gap between `body_` and `body_soil_`.         |
| UT-CS-10  | Testing when `body_soil_` is present but `body_` is not present.      |
| UT-CS-11  | Testing when two `body_` layers are intersecting.                     |
| UT-CS-12  | Testing when the `body_soil_` on the bottom layer is intersecting with the top `body_` layer. |

## `test_bucket_pos.cpp`

## `test_body_soil.cpp`

## `test_intersecting_cells.cpp`

## `test_relax.cpp`