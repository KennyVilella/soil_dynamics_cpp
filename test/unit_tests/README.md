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

This file implements unit tests for the functions in the `utils.cpp` file.
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
| UT-CBM-9  | Testing that a warning is issued for a large movement.                          |

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

This file implements unit tests for the functions in the `bucket_pos.cpp` file.

### `CalcLinePos`

Unit tests for the `CalcLinePos` function.
Note that the `CalcLinePos` function does not account well for the case where the line follows a cell border.
It is therefore necessary to solve this potential ambiguity before calling the function.
As a result, a small increment (`1e-5`) is added or removed to the input in order to make sure that the input coordinates do not correspond to a cell border.

For each case, some tests are present to check that the results do not depend on the order where the line vertices are given to the function.

| Test name | Description of the unit test                                                          |
| --------- | ------------------------------------------------------------------------------------- |
| BP-CL-1   | Testing with a line following the X axis.                                             |
| BP-CL-2   | Testing that rounding is done properly with a line following the X axis.              |
| BP-CL-3   | Testing with a line following the Y axis.                                             |
| BP-CL-4   | Testing with an arbitrary line in the XY plane. Results were obtained with a drawing. |
| BP-CL-5   | Testing with an arbitrary line in the XZ plane. Results were obtained with a drawing. |
| BP-CL-6   | Testing the edge case where the line is a point.                                      |

### `DecomposeVectorRectangle`

Unit tests for the `DecomposeVectorRectangle` function.
Note that the `DecomposeVectorRectangle` function does not account for the case where the rectangle follows a cell border.
It is therefore necessary to solve this potential ambiguity before calling the function.
As a result, a small increment (`1e-12`) is sometimes added or removed to the input in order to make sure that the input coordinates do not correspond to a cell border.

| Test name | Description of the unit test                                                        |
| --------- | ----------------------------------------------------------------------------------- |
| BP-DVR-1  | Testing with a simple rectangle in the XY plane.                                    |
| BP-DVR-2  | Testing that rounding is done properly with a simple rectangle in the XY plane.     |
| BP-DVR-3  | Testing with an arbitrary rectangle. Results were obtained with a drawing.          |
| BP-DVR-4  | Testing the edge case where the rectangle is a line. No decomposition can be made.  |
| BP-DVR-5  | Testing the edge case where the rectangle is a point. No decomposition can be made. |

### `DecomposeVectorTriangle`

Unit tests for the `DecomposeVectorTriangle` function.
Note that the `DecomposeVectorTriangle` function does not account for the case where the triangle follows a cell border. 
It is therefore necessary to solve this potential ambiguity before calling the function.

| Test name | Description of the unit test                                                       |
| --------- | ---------------------------------------------------------------------------------- |
| BP-DVT-1  | Testing with a simple triangle in the XY plane.                                    |
| BP-DVT-2  | Testing that rounding is done properly with a simple triangle in the XY plane.     |
| BP-DVT-3  | Testing with an arbitrary triangle. Results were obtained with a drawing.          |
| BP-DVT-4  | Testing the edge case where the triangle is a line. No decomposition can be made.  |
| BP-DVT-5  | Testing the edge case where the triangle is a point. No decomposition can be made. |

### `CalcRectanglePos`

Unit tests for the `CalcRectanglePos` function.
Note that the `CalcRectanglePos` function does not account for the case where the rectangle follows a cell border.
It is therefore necessary to solve this potential ambiguity before calling the function.
As a result, a small increment (`1e-5`) is added or removed to the input in order to make sure that the input coordinates do not correspond to a cell border.

For each case, some tests are present to check that the results do not depend on the order where the rectangle vertices are given to the function.

| Test name | Description of the unit test                                               |
| --------- | -------------------------------------------------------------------------- |
| BP-CR-1   | Testing with a simple rectangle in the XY plane.                           |
| BP-CR-2   | Testing with a simple rectangle in the XZ plane.                           |
| BP-CR-3   | Testing with an arbitrary rectangle. Results were obtained with a drawing. |
| BP-CR-4   | Testing the edge case where the rectangle is a line.                       |
| BP-CR-5   | Testing the edge case where the rectangle is a point.                      |

### `CalcTrianglePos`

Unit tests for the `CalcTrianglePos` function.
Note that the `CalcTrianglePos` function does not account for the case where the triangle follows a cell border.
It is therefore necessary to solve this potential ambiguity before calling the function.
As a result, a small increment (`1e-5`) is added or removed to the input in order to make sure that the input coordinates do not correspond to a cell border.

For each case, some tests are present to check that the results do not depend on the order where the triangle vertices are given to the function.

| Test name | Description of the unit test                                              |
| --------- | ------------------------------------------------------------------------- |
| BP-CT-1   | Testing with a simple triangle in the XY plane.                           |
| BP-CT-2   | Testing with a simple triangle in the XZ plane.                           |
| BP-CT-3   | Testing with an arbitrary triangle. Results were obtained with a drawing. |
| BP-CT-4   | Testing the edge case where the triangle is a line.                       |
| BP-CT-5   | Testing the edge case where the triangle is a point.                      |

### `IncludeNewBodyPos`

Unit tests for the `IncludeNewBodyPos` function.

| Test name | Description of the unit test                                                                       |
| --------- | -------------------------------------------------------------------------------------------------- |
| BP-INB-1  | Testing to add a new body position where there is no existing position.                            |
| BP-INB-2  | Testing to add a new body position distinct from the existing position on the first bucket layer.  |
| BP-INB-3  | Testing to add a new body position distinct from the existing position on the second bucket layer. |
| BP-INB-4  | Testing to add a new body position overlapping with the top of an existing position on the second bucket layer. |
| BP-INB-5  | Testing to add a new body position overlapping with the bottom of an existing position on the second bucket layer. |
| BP-INB-6  | Testing to add a new body position overlapping with the top of an existing position on the first bucket layer. |
| BP-INB-7  | Testing to add a new body position overlapping with the bottom of an existing position on the first bucket layer. |
| BP-INB-8  | Testing to add a new body position fully overlapping with an existing position on the second bucket layer. |
| BP-INB-9  | Testing to add a new body position overlapping with the two existing positions.            |
| BP-INB-10 | Testing to add a new body position within an existing position on the first bucket layer.  |
| BP-INB-11 | Testing to add a new body position within an existing position on the second bucket layer. |
| BP-INB-12 | Testing to add a new body position distinct from the two existing positions.               |

### `UpdateBody`

Unit tests for the `UpdateBody` function.

| Test name | Description of the unit test                                          |
| --------- | --------------------------------------------------------------------- |
| BP-UB-1   | Testing to add an arbitrary body wall. The case where a new body position is added is tested. |
| BP-UB-2   | Testing to add a second arbitrary body wall. Multiple cases are tested including the addition of a second body position, the addition of a body position overlapping with the top or bottom of an existing position. |
| BP-UB-3   | Testing to add a third arbitrary body wall. The case where two body positions are merged into one is tested. |
| BP-UB-4   | Testing to add a fourth arbitrary body wall. The case where a new body position is added distinct from the two existing positions is tested. |

### `CalcBucketPos`

Unit tests for the `CalcBucketPos` function.

| Test name | Description of the unit test                                           |
| --------- | ---------------------------------------------------------------------- |
| BP-CB-1   | Testing for a simple flat bucket in the XZ plane.                      |
| BP-CB-2   | Testing for a simple flat bucket in the XY plane.                      |
| BP-CB-3   | Testing for an arbitrary bucket. Results were obtained with a drawing. |

## `test_body_soil.cpp`

This file implements unit tests for the function in the `body_soil.cpp` file.

### `UpdateBodySoil`

Unit tests for the `UpdateBodySoil` function.
The tests are separated into two categories:

* Unit tests 1 - 11 are base tests.
* Unit tests 12 - 17 are implementation specific and may need modification when changing the algorithm.

| Test name | Description of the unit test                                           |
| --------- | ---------------------------------------------------------------------- |
| BS-UBS-1  | Testing for a one cell translation following the X axis when `body_` and `body_soil_` are on the first body layer. |
| BS-UBS-2  | Testing for a one cell translation following the X axis when `body_` and `body_soil_` are on the first and second body layer, respectively. |
| BS-UBS-3  | Testing for a one cell translation following the X axis when `body_` and `body_soil_` are on the second and first body layer, respectively. |
| BS-UBS-4  | Testing for a one cell translation following the X axis when `body_` and `body_soil_` are on the second body layer. |
| BS-UBS-5  | Testing for a pi/2 rotation around the Z axis when `body_` and `body_soil_` are on the first body layer. |
| BS-UBS-6  | Testing for a pi/4 rotation around the Z axis when `body_` and `body_soil_` are on the first body layer. |
| BS-UBS-7  | Testing for a one cell translation following the X axis combined to a pi/4 rotation around the Z axis when `body_` and `body_soil_` are on the first body layer. |
| BS-UBS-8  | Testing for a pi rotation around the X and Z axis when `body_soil_` is on the first body layer. Soil is avalanching to the `terrain_`. Checking that a warning is issued. |
| BS-UBS-9  | Testing for a pi/2 rotation around the Y axis when `body_` and two `body_soil_` are on the first body layer. The two `body_soil_` are avalanching to the same position. |
| BS-UBS-10 | Testing for a pi/2 rotation around the Y axis when `body_` and one `body_soil_` are on the first body layer, while a second `body_soil_` is on the second body layer. The two `body_soil_` are avalanching to the same position. |
| BS-UBS-11 | Testing for a pi/2 rotation around the Y axis when `body_` and one `body_soil_` are on the second body layer, while a second `body_soil_` is on the first body layer. The two `body_soil_` are avalanching to the same position. |
| BS-UBS-12 | Testing that soil column shorter than `cell_size_z_` is not moved. `body_` and `body_soil_` are on the first body layer. No movement is applied. |
| BS-UBS-13 | Testing that `h_soil` is properly rounded. `body_` and `body_soil_` are on the first body layer. No movement is applied. |
| BS-UBS-14 | Testing that the direction in which the neighboring cells are investigated is correct. To do so, the neighboring cells are blocked one by one until all the neighboring cells are investigated. `body_` and `body_soil_` are on the first body layer. A one cell translation following the X axis is applied. |
| BS-UBS-15 | Testing that the direction in which the neighboring cells are investigated is correct. To do so, the neighboring cells are blocked one by one until all the neighboring cells are investigated. `body_` and `body_soil_` are on the first body layer. A one cell translation following the -Y axis is applied. |
| BS-UBS-16 | Testing that soil is moved to a neighboring cell if vertical distance is low enough. `body_` and `body_soil_` are on the first body layer. A one cell translation following the X axis is applied. |
| BS-UBS-17 | The same as BS-UBS-16 except that the new body location is lower than the previous location. |

## `test_intersecting_cells.cpp`

This file implements unit tests for the function in the `intersecting_cells.cpp` file.

### `MoveBodySoil`

Unit tests for the `MoveBodySoil` function.
For all the unit tests, the same initial state is considered.
This initial state corresponds to two body layers with soil, where the soil resting on the second body layer (bottom) intersects with the first (top) body layer.

The tested function moves the intersecting soil to a different location.
The purpose of these tests is to check all possible movements depending on the configuration where the soil should be moved.
The description of the unit tests can therefore be done with a simple table describing the configuration at the new soil location.

| Test name | Bottom layer | Soil    | Top layer    | Soil    | Avalanche    | Enough space | Blocked |
| --------- | ------------ | ------- | ------------ | ------- | ------------ | ------------ | ------- |
| IC-MBS-1  | &cross;      | &cross; | &cross;      | &cross; | terrain      | &check;      | &cross; |
| IC-MBS-2  | First layer  | &cross; | &cross;      | &cross; | terrain      | &check;      | &cross; |
| IC-MBS-3  | First layer  | &cross; | &cross;      | &cross; | &cross;      | &cross;      | &check; |
| IC-MBS-4  | First layer  | &cross; | &cross;      | &cross; | First layer  | &check;      | &cross; |
| IC-MBS-5  | First layer  | &check; | &cross;      | &cross; | First layer  | &check;      | &cross; |
| IC-MBS-6  | Second layer | &cross; | &cross;      | &cross; | terrain      | &check;      | &cross; |
| IC-MBS-7  | Second layer | &check; | &cross;      | &cross; | &cross;      | &cross;      | &check; |
| IC-MBS-8  | Second layer | &cross; | &cross;      | &cross; | Second layer | &check;      | &cross; |
| IC-MBS-9  | Second layer | &check; | &cross;      | &cross; | Second layer | &check;      | &cross; |
| IC-MBS-10 | First layer  | &check; | Second layer | &cross; | &cross;      | &cross;      | &cross; |
| IC-MBS-11 | Second layer | &check; | First layer  | &cross; | &cross;      | &cross;      | &cross; |
| IC-MBS-12 | First layer  | &cross; | Second layer | &cross; | First layer  | &check;      | &cross; |
| IC-MBS-13 | Second layer | &cross; | First layer  | &cross; | Second layer | &check;      | &cross; |
| IC-MBS-14 | First layer  | &check; | Second layer | &cross; | First layer  | &check;      | &cross; |
| IC-MBS-15 | Second layer | &check; | First layer  | &cross; | Second layer | &check;      | &cross; |
| IC-MBS-16 | First layer  | &cross; | Second layer | &cross; | First layer  | &cross;      | &cross; |
| IC-MBS-17 | Second layer | &cross; | First layer  | &cross; | Second layer | &cross;      | &cross; |
| IC-MBS-18 | First layer  | &check; | Second layer | &cross; | First layer  | &cross;      | &cross; |
| IC-MBS-19 | Second layer | &check; | First layer  | &cross; | Second layer | &cross;      | &cross; |

Below a short description for some cases:

* When "Blocked" is &check;, a wall is blocking the movement.
* When "Avalanche", "Enough space" and "Blocked" are &cross;, the soil is fully filling the space between the two body layers. Soil cannot avalanche but the investigation in this direction can pursue.
* When "Avalanche" is &check; and "Enough space" is &cross;, the soil can partially avalanche.

### `MoveIntersectingBodySoil`

Unit tests for the `MoveIntersectingBodySoil` function.

The tested function moves all the intersecting soil following a set of rules.
The purpose of these tests is to check all possible movements depending on the configuration of the intersecting soil and its surrounding cells.
As for the `MoveBodySoil` function, a simple table is used to describe the configuration investigated.
However, for the `MoveIntersectingBodySoil` function, several movements can be investigated within a single unit test.
By convention, multiples rows are present within a unit test when several movements in the same direction are made, while when a new direction is investigated, the test name of the unit test is repeated in the first column.

By construction, in the previous location, there are necessarily two body layers and soil must be present on the bottom layer, while the presence of soil on the top layer has no impact on the algorithm.
As a result, for the previous location, only the identity of the bottom layer is provided.

<table>
  <tr>
    <td> </td><td>Previous location</td><td colspan="4">New location</td><td colspan="3">Configuration</td>
  </tr>
  <tr><td>Test name</td><td>Bottom layer</td><td>Bottom layer</td><td>Soil</td><td>Top layer</td><td>Soil</td><td>Avalanche</td><td>Enough space</td><td>Blocked</td></tr>
  <tr><td>IC-MIBS-1</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-2</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-3</td><td>First layer</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-4</td><td>Second layer</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-5</td><td>First layer</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-6</td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-7</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-8</td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-9</td><td>First layer</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-10</td><td>Second layer</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-11</td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-12</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-13</td><td>First layer</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-14</td><td>Second layer</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-15</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-16</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-17</td><td>First layer</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-18</td><td>Second layer</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-19</td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-20</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-21</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-22</td><td>Second layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-23</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-24</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-25</td><td>First layer</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-26</td><td>Second layer</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-27</td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-28</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-29</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-30</td><td>Second layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-31</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-32</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-33</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-34</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-35</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-36</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-37</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-38</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-39</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-40</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>terrain</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-41</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-42</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-43</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-44</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-45</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-46</td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-47</td><td>First layer</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-48</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-49</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>IC-MIBS-49</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>IC-MIBS-49</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>IC-MIBS-49</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>IC-MIBS-49</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-50</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-51</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>&cross;</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-52</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>IC-MIBS-52</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>IC-MIBS-52</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>IC-MIBS-52</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>IC-MIBS-52</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-53</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-54</td><td>First layer</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-55</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-56</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-57</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-58</td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>&cross;</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>First layer</td><td>First layer</td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-59</td><td>First layer</td><td>Second layer</td><td>&check;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>          </td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
</table>

In addition to these basic unit tests, a few extra edge cases are checked.

| Test name  | Description of the unit test                                                                     |
| ---------- | ------------------------------------------------------------------------------------------------ |
| IC-MIBS-60 | Testing when a lot of soil is present in the first bucket layer but soil is still avalanching.   |
| IC-MIBS-61 | Testing when a lot of soil is present in the second bucket layer but soil is still avalanching.  |
| IC-MIBS-62 | Testing when the bucket is totally underground but the soil is still avalanching on the terrain. |
| IC-MIBS-63 | Testing when the soil column is composed of various layers in `body_soil_pos_`.                  |
| IC-MIBS-64 | Testing when there is no intersecting cell                                                       |
| IC-MIBS-65 | Testing the randomness of the investigated direction for the soil movement.                      |
| IC-MIBS-66 | Testing that a warning is issued if all soil cannot be moved.                                    |

### `LocateIntersectingCells`

Unit test for the `LocateIntersectingCells` function.

Currently, all the tests are done simultaneously.
In the future, the tests will be separated and described in this section.

### `MoveIntersectingBody`

Unit tests for the `MoveIntersectingBody` function.

| Test name | Description of the unit test                                                             |
| --------- | ---------------------------------------------------------------------------------------- |
| IC-MIB-1  | Testing for a single intersecting cell in the -X direction.                              |
| IC-MIB-2  | Testing for a single intersecting cell in the +X direction.                              |
| IC-MIB-3  | Testing for a single intersecting cell in the -Y direction.                              |
| IC-MIB-4  | Testing for a single intersecting cell in the +Y direction.                              |
| IC-MIB-5  | Testing for a single intersecting cell in the -X-Y direction.                            |
| IC-MIB-6  | Testing for a single intersecting cell in the +X-Y direction.                            |
| IC-MIB-7  | Testing for a single intersecting cell in the -X+Y direction.                            |
| IC-MIB-8  | Testing for a single intersecting cell in the +X+Y direction.                            |
| IC-MIB-9  | Testing for a single intersecting cell with the second bucket layer.                     |
| IC-MIB-10 | Testing for a single intersecting cells with various bucket layers.                      |
| IC-MIB-11 | Testing for a single intersecting cell with all the bucket under the terrain.            |
| IC-MIB-12 | Testing for a single intersecting cell under a large bucket.                             |
| IC-MIB-13 | Testing when soil is moved in several steps. All the soil is fitting under the bucket.   |
| IC-MIB-14 | Testing when soil is moved in several steps. Some soil going outside the bucket.         |
| IC-MIB-15 | Testing when soil is moved in several steps. Soil is perfectly fitting under the bucket. |
| IC-MIB-16 | Testing when there is no intersecting cell.                                              |
| IC-MIB-17 | Testing the randomness of the investigated direction for the soil movement.              |

## `test_relax.cpp`

This file implements unit tests for the function in the `relax.cpp` file.

### `LocateUnstableTerrainCell`

Unit test for the `LocateUnstableTerrainCell` function.

Currently, all the tests are done simultaneously.
In the future, the tests will be separated and described in this section.

### `CheckUnstableTerrainCell`

Unit test for the `CheckUnstableTerrainCell` function.

The tested function checks the configuration in a specified location and return a status code following the situation.
The purpose of these tests is to check all possible configurations.
The description of the unit tests can therefore be done with a simple table describing the configuration at the specified location.

| Test name | Bottom layer | Soil    | Until top | Stable  | Top layer    | Soil    | Stable  | Avalanche    |
| --------- | ------------ | ------- | --------- |-------- | ------------ | ------- | ------- | ------------ |
| RE-CUT-1  | &cross;      | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-CUT-2  | &cross;      | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | terrain      |
| RE-CUT-3  | First layer  | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | terrain      |
| RE-CUT-4  | First layer  | &cross; | &cross;   | &cross; | &cross;      | &cross; | &check; | First layer  |
| RE-CUT-5  | First layer  | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      | 
| RE-CUT-6  | First layer  | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | terrain      |
| RE-CUT-7  | First layer  | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | First layer  |
| RE-CUT-8  | First layer  | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | &cross;      | 
| RE-CUT-9  | Second layer | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | terrain      |
| RE-CUT-10 | Second layer | &cross; | &cross;   | &cross; | &cross;      | &cross; | &check; | Second layer | 
| RE-CUT-11 | Second layer | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-CUT-12 | Second layer | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | terrain      |
| RE-CUT-13 | Second layer | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | Second layer |
| RE-CUT-14 | Second layer | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | &cross;      |
| RE-CUT-15 | First layer  | &cross; | &cross;   | &cross; | Second layer | &cross; | &cross; | terrain      |
| RE-CUT-16 | First layer  | &cross; | &cross;   | &cross; | Second layer | &cross; | &cross; | First layer  |
| RE-CUT-17 | First layer  | &cross; | &cross;   | &cross; | Second layer | &cross; | &check; | First layer  | 
| RE-CUT-18 | First layer  | &cross; | &cross;   | &cross; | Second layer | &check; | &cross; | terrain      |
| RE-CUT-19 | First layer  | &cross; | &cross;   | &cross; | Second layer | &check; | &cross; | First layer  |
| RE-CUT-20 | First layer  | &cross; | &cross;   | &cross; | Second layer | &check; | &cross; | First layer  |
| RE-CUT-21 | First layer  | &check; | &cross;   | &cross; | Second layer | &cross; | &cross; | terrain      |
| RE-CUT-22 | First layer  | &check; | &cross;   | &cross; | Second layer | &cross; | &cross; | First layer  |
| RE-CUT-23 | First layer  | &check; | &cross;   | &cross; | Second layer | &cross; | &check; | First layer  |
| RE-CUT-24 | First layer  | &check; | &check;   | &cross; | Second layer | &cross; | &cross; | terrain      |
| RE-CUT-25 | First layer  | &check; | &check;   | &cross; | Second layer | &cross; | &cross; | Second layer |
| RE-CUT-26 | First layer  | &check; | &check;   | &cross; | Second layer | &cross; | &check; | &cross;      |
| RE-CUT-27 | First layer  | &check; | &cross;   | &cross; | Second layer | &check; | &cross; | terrain      |
| RE-CUT-28 | First layer  | &check; | &cross;   | &cross; | Second layer | &check; | &cross; | First layer  |
| RE-CUT-29 | First layer  | &check; | &cross;   | &cross; | Second layer | &check; | &cross; | First layer  |
| RE-CUT-30 | First layer  | &check; | &check;   | &cross; | Second layer | &check; | &cross; | terrain      |
| RE-CUT-31 | First layer  | &check; | &check;   | &cross; | Second layer | &check; | &cross; | Second layer |
| RE-CUT-32 | First layer  | &check; | &check;   | &cross; | Second layer | &check; | &cross; | &cross;      |
| RE-CUT-33 | Second layer | &cross; | &cross;   | &cross; | First layer  | &cross; | &cross; | terrain      |
| RE-CUT-34 | Second layer | &cross; | &cross;   | &cross; | First layer  | &cross; | &cross; | Second layer |
| RE-CUT-35 | Second layer | &cross; | &cross;   | &cross; | First layer  | &cross; | &check; | Second layer |
| RE-CUT-36 | Second layer | &cross; | &cross;   | &cross; | First layer  | &check; | &cross; | terrain      | 
| RE-CUT-37 | Second layer | &cross; | &cross;   | &cross; | First layer  | &check; | &cross; | Second layer |
| RE-CUT-38 | Second layer | &cross; | &cross;   | &cross; | First layer  | &check; | &cross; | Second layer |
| RE-CUT-39 | Second layer | &check; | &cross;   | &cross; | First layer  | &cross; | &cross; | terrain      |
| RE-CUT-40 | Second layer | &check; | &cross;   | &cross; | First layer  | &cross; | &cross; | Second layer |
| RE-CUT-41 | Second layer | &check; | &cross;   | &cross; | First layer  | &cross; | &check; | Second layer |
| RE-CUT-42 | Second layer | &check; | &check;   | &cross; | First layer  | &cross; | &cross; | terrain      |
| RE-CUT-43 | Second layer | &check; | &check;   | &cross; | First layer  | &cross; | &cross; | First layer  |
| RE-CUT-44 | Second layer | &check; | &check;   | &cross; | First layer  | &cross; | &check; | &cross;      |
| RE-CUT-45 | Second layer | &check; | &cross;   | &cross; | First layer  | &check; | &cross; | terrain      |
| RE-CUT-46 | Second layer | &check; | &cross;   | &cross; | First layer  | &check; | &cross; | Second layer |
| RE-CUT-47 | Second layer | &check; | &cross;   | &cross; | First layer  | &check; | &cross; | Second layer |
| RE-CUT-48 | Second layer | &check; | &check;   | &cross; | First layer  | &check; | &cross; | terrain      |
| RE-CUT-49 | Second layer | &check; | &check;   | &cross; | First layer  | &check; | &cross; | First layer  |
| RE-CUT-50 | Second layer | &check; | &check;   | &cross; | First layer  | &check; | &cross; | &cross;      |

In addition to these basic unit tests, a few extra edge cases are checked.

| Test name | Description of the unit test                                               |
| --------- | -------------------------------------------------------------------------- |
| RE-CUT-51 | Testing edge case where a lot of space under the bucket is present.        |
| RE-CUT-52 | Testing edge case where bucket height is equal to minimum allowed height.  |
| RE-CUT-53 | Testing edge case where terrain height is equal to minimum allowed height. |

### `RelaxUnstableTerrainCell`

Unit test for the `RelaxUnstableTerrainCell` function.

The tested function moves the soil following the status code provided assuming that it corresponds to the actual configuration.
The purpose of these tests is to check all possible configurations.
The description of the unit tests can therefore be done with a simple table describing the configuration.

| Test name | Bottom layer | Soil    | Top layer    | Soil    | Avalanche    | Enough space |
| --------- | ------------ | ------- | ------------ | ------- | ------------ | ------------ |
| RE-RUT-1  | &cross;      | &cross; | &cross;      | &cross; | terrain      | &check;      |
| RE-RUT-2  | First layer  | &cross; | &cross;      | &cross; | terrain      | &cross;      |
| RE-RUT-3  | First layer  | &cross; | &cross;      | &cross; | terrain      | &check;      |
| RE-RUT-4  | First layer  | &cross; | &cross;      | &cross; | First layer  | &check;      |
| RE-RUT-5  | First layer  | &check; | &cross;      | &cross; | terrain      | &cross;      |
| RE-RUT-6  | First layer  | &check; | &cross;      | &cross; | terrain      | &check;      |
| RE-RUT-7  | First layer  | &check; | &cross;      | &cross; | First layer  | &check;      |  
| RE-RUT-8  | Second layer | &cross; | &cross;      | &cross; | terrain      | &cross;      |
| RE-RUT-9  | Second layer | &cross; | &cross;      | &cross; | terrain      | &check;      |
| RE-RUT-10 | Second layer | &cross; | &cross;      | &cross; | Second layer | &check;      |  
| RE-RUT-11 | Second layer | &check; | &cross;      | &cross; | terrain      | &cross;      |
| RE-RUT-12 | Second layer | &check; | &cross;      | &cross; | terrain      | &check;      |
| RE-RUT-13 | Second layer | &check; | &cross;      | &cross; | Second layer | &check;      | 
| RE-RUT-14 | First layer  | &cross; | Second layer | &cross; | terrain      | &cross;      |
| RE-RUT-15 | First layer  | &cross; | Second layer | &cross; | terrain      | &check;      |
| RE-RUT-16 | First layer  | &cross; | Second layer | &cross; | First layer  | &check;      |
| RE-RUT-17 | First layer  | &cross; | Second layer | &cross; | First layer  | &cross;      |   
| RE-RUT-18 | First layer  | &check; | Second layer | &cross; | Second layer | &check;      |
| RE-RUT-19 | First layer  | &check; | Second layer | &check; | First layer  | &check;      |   
| RE-RUT-20 | First layer  | &check; | Second layer | &check; | First layer  | &cross;      | 
| RE-RUT-21 | First layer  | &check; | Second layer | &check; | Second layer | &check;      |
| RE-RUT-22 | Second layer | &cross; | First layer  | &cross; | terrain      | &cross;      |
| RE-RUT-23 | Second layer | &cross; | First layer  | &cross; | terrain      | &check;      |
| RE-RUT-24 | Second layer | &cross; | First layer  | &cross; | Second layer | &check;      |
| RE-RUT-25 | Second layer | &cross; | First layer  | &cross; | Second layer | &cross;      | 
| RE-RUT-26 | Second layer | &check; | First layer  | &cross; | First layer  | &check;      |
| RE-RUT-27 | Second layer | &check; | First layer  | &check; | Second layer | &check;      | 
| RE-RUT-28 | Second layer | &check; | First layer  | &check; | Second layer | &cross;      |
| RE-RUT-29 | Second layer | &check; | First layer  | &check; | First layer  | &check;      |

### `RelaxTerrain`

Unit test for the `RelaxTerrain` function.

The tested function moves the terrain soil cell that are unstablle following the configuration.
The purpose of these tests is to check all possible configurations.
The description of the unit tests can therefore be done with a simple table describing the configuration.
However, several movements can be investigated within a single unit test.
By convention, multiples rows are present within a unit test when several movements are made.
Each unit test is constructed such that soil is only avalanching to a single position (`10`, `15`).

| Test name | Bottom layer | Soil    | Until top | Stable  | Top layer    | Soil    | Stable  | Avalanche    |
| --------- | ------------ | ------- | --------- |-------- | ------------ | ------- | ------- | ------------ |
| RE-RT-1   | &cross;      | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-RT-2   | &cross;      | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | terrain      |
| RE-RT-3   | First layer  | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | terrain      |
| RE-RT-4   | First layer  | &cross; | &cross;   | &cross; | &cross;      | &cross; | &check; | First layer  |
| RE-RT-5   | First layer  | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-RT-6   | First layer  | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | terrain      |
| RE-RT-7   | First layer  | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | First layer  |
| RE-RT-8   | First layer  | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | &cross;      |
| RE-RT-9   | Second layer | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | terrain      |
| RE-RT-10  | Second layer | &cross; | &cross;   | &cross; | &cross;      | &cross; | &check; | Second layer |
| RE-RT-11  | Second layer | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-RT-12  | Second layer | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | terrain      |
| RE-RT-13  | Second layer | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | Second layer |
| RE-RT-14  | Second layer | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | &cross;      |
| RE-RT-15  | First layer  | &cross; | &cross;   | &check; | Second layer | &cross; | &check; | terrain      |
| RE-RT-16  | First layer  | &cross; | &cross;   | &cross; | Second layer | &cross; | &check; | First layer  |
| RE-RT-17  | First layer  | &cross; | &cross;   | &cross; | Second layer | &cross; | &cross; | First layer  |
|           | First layer  | &check; | &check;   | &cross; | Second layer | &cross; | &cross; | Second layer |
| RE-RT-18  | First layer  | &cross; | &cross;   | &check; | Second layer | &check; | &check; | terrain      |
| RE-RT-19  | First layer  | &cross; | &cross;   | &cross; | Second layer | &check; | &cross; | First layer  |
| RE-RT-20  | First layer  | &cross; | &cross;   | &cross; | Second layer | &check; | &cross; | First layer  |
|           | First layer  | &check; | &check;   | &cross; | Second layer | &check; | &cross; | Second layer |
| RE-RT-21  | First layer  | &check; | &cross;   | &check; | Second layer | &cross; | &check; | terrain      |
| RE-RT-22  | First layer  | &check; | &cross;   | &cross; | Second layer | &cross; | &check; | First layer  |   
| RE-RT-23  | First layer  | &check; | &cross;   | &cross; | Second layer | &cross; | &cross; | First layer  |
|           | First layer  | &check; | &check;   | &cross; | Second layer | &cross; | &cross; | Second layer |
| RE-RT-24  | First layer  | &check; | &check;   | &cross; | Second layer | &check; | &check; | terrain      |
| RE-RT-25  | First layer  | &check; | &cross;   | &cross; | Second layer | &check; | &cross; | First layer  | 
| RE-RT-26  | First layer  | &check; | &cross;   | &cross; | Second layer | &check; | &cross; | First layer  |
|           | First layer  | &check; | &check;   | &cross; | Second layer | &check; | &cross; | Second layer |
| RE-RT-27  | Second layer | &cross; | &cross;   | &check; | First layer  | &cross; | &check; | terrain      |
| RE-RT-28  | Second layer | &cross; | &cross;   | &cross; | First layer  | &cross; | &check; | Second layer |
| RE-RT-29  | Second layer | &cross; | &cross;   | &cross; | First layer  | &cross; | &cross; | Second layer |
|           | Second layer | &check; | &check;   | &cross; | First layer  | &cross; | &cross; | First layer  |
| RE-RT-30  | Second layer | &cross; | &cross;   | &check; | First layer  | &check; | &check; | terrain      |
| RE-RT-31  | Second layer | &cross; | &cross;   | &cross; | First layer  | &check; | &check; | Second layer |
| RE-RT-32  | Second layer | &cross; | &cross;   | &cross; | First layer  | &check; | &cross; | Second layer |
|           | Second layer | &check; | &check;   | &cross; | First layer  | &check; | &cross; | First layer  |
| RE-RT-33  | Second layer | &check; | &check;   | &cross; | First layer  | &cross; | &check; | terrain      |
| RE-RT-34  | Second layer | &check; | &cross;   | &cross; | First layer  | &cross; | &check; | Second layer |
| RE-RT-35  | Second layer | &check; | &cross;   | &cross; | First layer  | &cross; | &cross; | Second layer |
|           | Second layer | &check; | &check;   | &cross; | First layer  | &cross; | &cross; | First layer  |
| RE-RT-36  | Second layer | &check; | &check;   | &cross; | First layer  | &check; | &cross; | terrain      |
| RE-RT-37  | Second layer | &check; | &cross;   | &cross; | First layer  | &check; | &cross; | Second layer |
| RE-RT-38  | Second layer | &check; | &cross;   | &cross; | First layer  | &check; | &cross; | Second layer |
|           | Second layer | &check; | &check;   | &cross; | First layer  | &check; | &cross; | First layer  |

In addition to these basic unit tests, a few extra edge cases are checked.

| Test name | Description of the unit test                                                |
| --------- | --------------------------------------------------------------------------- |
| RE-RT-39  | Testing edge case where a lot of space under the bucket is present.         |
| RE-RT-40  | Testing edge case where multiple avalanches are required.                   |
| RE-RT-41  | Testing the randomness of the investigated direction for the soil movement. |

### `CheckUnstableBodyCell`

Unit test for the `CheckUnstableBodyCell` function.

The tested function checks the configuration in a specified location and return a status code following the situation.
The purpose of these tests is to check all possible configurations.
The description of the unit tests can therefore be done with a simple table describing the configuration at the specified location.

For all the unit tests, the initial position has soil on the first bucket layer.
The configuration of the inital position should not impact the result of this function.

| Test name | Bottom layer | Soil    | Until top | Accessible | Top layer    | Soil    | Accessible | Avalanche    |
| --------- | ------------ | ------- | --------- |----------- | ------------ | ------- | ---------- | ------------ |
| RE-CUB-1  | &cross;      | &cross; | &cross;   | &cross;    | &cross;      | &cross; | &cross;    | terrain      |
| RE-CUB-2  | First layer  | &cross; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | terrain      |
| RE-CUB-3  | First layer  | &check; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | terrain      |
| RE-CUB-4  | First layer  | &cross; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | First layer  |
| RE-CUB-5  | First layer  | &check; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | First layer  |
| RE-CUB-6  | Second layer | &cross; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | terrain      |
| RE-CUB-7  | Second layer | &check; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | terrain      |
| RE-CUB-8  | Second layer | &cross; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | Second layer |
| RE-CUB-9  | Second layer | &check; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | Second layer |
| RE-CUB-10 | First layer  | &cross; | &cross;   | &check;    | Second layer | &cross; | &cross;    | First layer  |
| RE-CUB-11 | First layer  | &check; | &cross;   | &check;    | Second layer | &cross; | &cross;    | First layer  | 
| RE-CUB-12 | First layer  | &cross; | &cross;   | &check;    | Second layer | &check; | &cross;    | First layer  |
| RE-CUB-13 | First layer  | &check; | &cross;   | &check;    | Second layer | &check; | &cross;    | First layer  |
| RE-CUB-14 | First layer  | &cross; | &cross;   | &cross;    | Second layer | &cross; | &check;    | Second layer |
| RE-CUB-15 | First layer  | &check; | &check;   | &cross;    | Second layer | &cross; | &check;    | Second layer |
| RE-CUB-16 | First layer  | &cross; | &cross;   | &cross;    | Second layer | &check; | &check;    | Second layer |
| RE-CUB-17 | First layer  | &check; | &check;   | &cross;    | Second layer | &check; | &check;    | Second layer |
| RE-CUB-18 | Second layer | &cross; | &cross;   | &check;    | First layer  | &cross; | &cross;    | Second layer |
| RE-CUB-19 | Second layer | &check; | &cross;   | &check;    | First layer  | &cross; | &cross;    | Second layer | 
| RE-CUB-20 | Second layer | &cross; | &cross;   | &check;    | First layer  | &check; | &cross;    | Second layer |
| RE-CUB-21 | Second layer | &check; | &cross;   | &check;    | First layer  | &check; | &cross;    | Second layer |
| RE-CUB-22 | Second layer | &cross; | &cross;   | &cross;    | First layer  | &cross; | &check;    | First layer  |
| RE-CUB-23 | Second layer | &check; | &check;   | &cross;    | First layer  | &cross; | &check;    | First layer  |
| RE-CUB-24 | Second layer | &cross; | &cross;   | &cross;    | First layer  | &check; | &check;    | First layer  |
| RE-CUB-25 | Second layer | &check; | &check;   | &cross;    | First layer  | &check; | &check;    | First layer  |
| RE-CUB-26 | Second layer | &check; | &check;   | &check;    | First layer  | &cross; | &cross;    | &cross;      |
| RE-CUB-27 | Second layer | &check; | &check;   | &check;    | First layer  | &check; | &check;    | &cross;      |
| RE-CUB-28 | First layer  | &check; | &check;   | &check;    | Second layer | &cross; | &cross;    | &cross;      |
| RE-CUB-29 | First layer  | &check; | &check;   | &check;    | Second layer | &check; | &check;    | &cross;      |
| RE-CUB-30 | Second layer | &check; | &check;   | &check;    | First layer  | &cross; | &check;    | First layer  |
| RE-CUB-31 | Second layer | &check; | &check;   | &check;    | First layer  | &check; | &check;    | First layer  |
| RE-CUB-32 | First layer  | &check; | &check;   | &check;    | Second layer | &cross; | &check;    | Second layer |
| RE-CUB-33 | First layer  | &check; | &check;   | &check;    | Second layer | &check; | &check;    | Second layer |
| RE-CUB-34 | &cross;      | &cross; | &cross;   | &cross;    | &cross;      | &cross; | &cross;    | &cross;      |
| RE-CUB-35 | First layer  | &cross; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | &cross;      |
| RE-CUB-36 | First layer  | &check; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | &cross;      |
| RE-CUB-37 | Second layer | &cross; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | &cross;      |
| RE-CUB-38 | Second layer | &check; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | &cross;      |
| RE-CUB-39 | Second layer | &check; | &cross;   | &cross;    | First layer  | &cross; | &check;    | &cross;      |
| RE-CUB-40 | Second layer | &check; | &check;   | &check;    | First layer  | &cross; | &cross;    | &cross;      |
| RE-CUB-41 | First layer  | &cross; | &cross;   | &cross;    | Second layer | &check; | &check;    | &cross;      |
| RE-CUB-42 | First layer  | &cross; | &cross;   | &check;    | Second layer | &cross; | &check;    | &cross;      |

### `RelaxUnstableBodyCell`

Unit test for the `RelaxUnstableBodyCell` function.

The tested function moves the soil following the status code provided assuming that it corresponds to the actual configuration.
The purpose of these tests is to check all possible configurations.
The description of the unit tests can therefore be done with a simple table describing the configuration.

For all the unit tests, the initial position has soil on the first bucket layer.
The configuration of the inital position should not impact the result of this function.

| Test name | Bottom layer | Soil    | Until top | Top layer    | Soil    | Avalanche    | Enough soil | Enough space | Status       |
| --------- | ------------ | ------- | --------- | ------------ | ------- | ------------ | ----------- | ------------ | ------------ |
| RE-RUB-1  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;     | &check;      | Partial      |
| RE-RUB-2  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;     | &check;      | Full         |
| RE-RUB-3  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | terrain      | &cross;     | &check;      | Full         |
| RE-RUB-4  | First layer  | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;     | &check;      | Partial      |
| RE-RUB-5  | First layer  | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;     | &check;      | Full         |
| RE-RUB-6  | First layer  | &cross; | &cross;   | &cross;      | &cross; | terrain      | &cross;     | &check;      | Partial      |
| RE-RUB-7  | First layer  | &cross; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-8  | First layer  | &cross; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RUB-9  | First layer  | &cross; | &cross;   | &cross;      | &cross; | First layer  | &cross;     | &check;      | Full         |
| RE-RUB-10 | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-11 | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RUB-12 | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &cross;     | &check;      | Full         |
| RE-RUB-13 | Second layer | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;     | &check;      | Partial      |
| RE-RUB-14 | Second layer | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;     | &check;      | Full         |
| RE-RUB-15 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-16 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RUB-17 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Second layer | &cross;     | &check;      | Full         |
| RE-RUB-18 | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-19 | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RUB-20 | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &cross;     | &check;      | Full         |
| RE-RUB-21 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-22 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RUB-23 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &cross;     | &check;      | Full         |
| RE-RUB-24 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &check;     | &cross;      | Partial      |
| RE-RUB-25 | First layer  | &cross; | &cross;   | Second layer | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-26 | First layer  | &cross; | &cross;   | Second layer | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RUB-27 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-28 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;     | &check;      | Full         |  
| RE-RUB-29 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &cross;     | &check;      | Full         |
| RE-RUB-30 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;     | &cross;      | Partial      |
| RE-RUB-31 | First layer  | &check; | &cross;   | Second layer | &check; | Second layer | &check;     | &check;      | Partial      |  
| RE-RUB-32 | First layer  | &check; | &cross;   | Second layer | &check; | Second layer | &check;     | &check;      | Fulll        | 
| RE-RUB-33 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-34 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RUB-35 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &cross;     | &check;      | Full         |
| RE-RUB-36 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &check;     | &cross;      | Partial      |
| RE-RUB-37 | Second layer | &cross; | &cross;   | First layer  | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-38 | Second layer | &cross; | &cross;   | First layer  | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RUB-39 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-40 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;     | &check;      | Full         | 
| RE-RUB-41 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &cross;     | &check;      | Full         |
| RE-RUB-42 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;     | &cross;      | Partial      |
| RE-RUB-43 | Second layer | &check; | &cross;   | First layer  | &check; | First layer  | &check;     | &check;      | Partial      | 
| RE-RUB-44 | Second layer | &check; | &cross;   | First layer  | &check; | First layer  | &check;     | &check;      | Fulll        | 
| RE-RUB-45 | First layer  | &check; | &check;   | Second layer | &check; | &cross;      | &check;     | &cross;      | &cross;      |
| RE-RUB-46 | Second layer | &check; | &check;   | First layer  | &check; | &cross;      | &check;     | &cross;      | &cross;      | 
| RE-RUB-47 | First layer  | &check; | &check;   | Second layer | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-48 | First layer  | &check; | &check;   | Second layer | &check; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-49 | Second layer | &check; | &check;   | First layer  | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-50 | Second layer | &check; | &check;   | First layer  | &check; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-51 | Second layer | &check; | &cross;   | &cross;      | &cross; | terrain      | &check;     | &cross;      | Partial      |

### `RelaxBodySoil`

Unit test for the `RelaxBodySoil` function.

The tested function moves the body soil cell that are unstablle following the configuration.
The purpose of these tests is to check all possible configurations.
The description of the unit tests can therefore be done with a simple table describing the configuration.
However, several movements can be investigated within a single unit test.
By convention, multiples rows are present within a unit test when the avalanche occurs in several steps.
Each unit test is constructed such that soil is only avalanching to a single position (`10`, `15`).

For all the unit tests, the initial position has soil on the first bucket layer.
The configuration of the inital position should not impact the result of this function.

| Test name | Bottom layer | Soil    | Until top | Top layer    | Soil    | Avalanche    | Enough space | Status       |
| --------- | ------------ | ------- | --------- | ------------ | ------- | ------------ | ------------ | ------------ |
| RE-RBS-1  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;      | Partial      |
| RE-RBS-2  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;      | Full         |
| RE-RBS-3  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;      | Partial      |
|           | &cross;      | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;      | Full         |
| RE-RBS-4  | First layer  | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;      | Partial      |
| RE-RBS-5  | First layer  | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;      | Full         |
| RE-RBS-6  | First layer  | &cross; | &cross;   | &cross;      | &cross; | terrain      | &cross;      | Partial      |
| RE-RBS-7  | First layer  | &cross; | &cross;   | &cross;      | &cross; | First layer  | &check;      | Partial      |
| RE-RBS-8  | First layer  | &cross; | &cross;   | &cross;      | &cross; | First layer  | &check;      | Full         |
| RE-RBS-9  | First layer  | &cross; | &cross;   | &cross;      | &cross; | First layer  | &check;      | Partial      |
|           | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;      | Full         |
| RE-RBS-10 | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;      | Partial      |
| RE-RBS-11 | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;      | Full         |
| RE-RBS-12 | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;      | Partial      |
|           | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;      | Full         |
| RE-RBS-13 | Second layer | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;      | Partial      |
| RE-RBS-14 | Second layer | &cross; | &cross;   | &cross;      | &cross; | terrain      | &check;      | Full         |
| RE-RBS-15 | Second layer | &cross; | &cross;   | &cross;      | &cross; | terrain      | &cross;      | Partial      |
| RE-RBS-16 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Second layer | &check;      | Partial      |
| RE-RBS-17 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Second layer | &check;      | Full         |
| RE-RBS-18 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Second layer | &check;      | Partial      |
|           | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;      | Full         |
| RE-RBS-19 | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;      | Partial      |
| RE-RBS-20 | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;      | Full         |
| RE-RBS-21 | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;      | Partial      |
|           | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;      | Full         |
| RE-RBS-22 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &check;      | Partial      |
| RE-RBS-23 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &check;      | Full         |
| RE-RBS-24 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &check;      | Partial      |
|           | First layer  | &check; | &cross;   | Second layer | &cross; | First layer  | &check;      | Full         |
| RE-RBS-25 | First layer  | &cross; | &cross;   | Second layer | &cross; | Second layer | &check;      | Partial      |
| RE-RBS-26 | First layer  | &cross; | &cross;   | Second layer | &cross; | Second layer | &check;      | Full         |
| RE-RBS-27 | First layer  | &check; | &cross;   | Second layer | &cross; | First layer  | &check;      | Partial      |
| RE-RBS-28 | First layer  | &check; | &cross;   | Second layer | &cross; | First layer  | &check;      | Full         |
| RE-RBS-29 | First layer  | &check; | &cross;   | Second layer | &cross; | First layer  | &check;      | Partial      |
|           | First layer  | &check; | &cross;   | Second layer | &cross; | First layer  | &check;      | Full         |
| RE-RBS-30 | First layer  | &check; | &cross;   | Second layer | &cross; | Second layer | &check;      | Partial      |
| RE-RBS-31 | First layer  | &check; | &cross;   | Second layer | &cross; | Second layer | &check;      | Full         |
| RE-RBS-32 | First layer  | &cross; | &cross;   | Second layer | &check; | First layer  | &check;      | Partial      |
| RE-RBS-33 | First layer  | &cross; | &cross;   | Second layer | &check; | First layer  | &check;      | Full         |
| RE-RBS-34 | First layer  | &cross; | &cross;   | Second layer | &check; | Second layer | &check;      | Partial      |
| RE-RBS-35 | First layer  | &cross; | &cross;   | Second layer | &check; | Second layer | &check;      | Full         |
| RE-RBS-36 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;      | Partial      |
| RE-RBS-37 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;      | Full         |
| RE-RBS-38 | First layer  | &check; | &cross;   | Second layer | &check; | Second layer | &check;      | Partial      |
| RE-RBS-39 | First layer  | &check; | &cross;   | Second layer | &check; | Second layer | &check;      | Full         |
| RE-RBS-40 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &cross;      | Partial      |
| RE-RBS-41 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &cross;      | Partial      |
| RE-RBS-42 | First layer  | &check; | &check;   | Second layer | &check; | First layer  | &cross;      | &cross;      |
| RE-RBS-43 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &check;      | Partial      |
| RE-RBS-44 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &check;      | Full         |
| RE-RBS-45 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &check;      | Partial      |
|           | Second layer | &check; | &cross;   | First layer  | &cross; | Second layer | &check;      | Full         |
| RE-RBS-46 | Second layer | &cross; | &cross;   | First layer  | &cross; | First layer  | &check;      | Partial      |
| RE-RBS-47 | Second layer | &cross; | &cross;   | First layer  | &cross; | First layer  | &check;      | Full         |
| RE-RBS-48 | Second layer | &check; | &cross;   | First layer  | &cross; | Second layer | &check;      | Partial      |
| RE-RBS-49 | Second layer | &check; | &cross;   | First layer  | &cross; | Second layer | &check;      | Full         |
| RE-RBS-50 | Second layer | &check; | &cross;   | First layer  | &cross; | Second layer | &check;      | Partial      |
|           | Second layer | &check; | &cross;   | First layer  | &cross; | Second layer | &check;      | Full         |
| RE-RBS-51 | Second layer | &check; | &cross;   | First layer  | &cross; | First layer  | &check;      | Partial      |
| RE-RBS-52 | Second layer | &check; | &cross;   | First layer  | &cross; | First layer  | &check;      | Full         |
| RE-RBS-53 | Second layer | &cross; | &cross;   | First layer  | &check; | Second layer | &check;      | Partial      |
| RE-RBS-54 | Second layer | &cross; | &cross;   | First layer  | &check; | Second layer | &check;      | Full         |
| RE-RBS-55 | Second layer | &cross; | &cross;   | First layer  | &check; | First layer  | &check;      | Partial      |
| RE-RBS-56 | Second layer | &cross; | &cross;   | First layer  | &check; | First layer  | &check;      | Full         |
| RE-RBS-57 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;      | Partial      |
| RE-RBS-58 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;      | Full         |
| RE-RBS-59 | Second layer | &check; | &cross;   | First layer  | &check; | First layer  | &check;      | Partial      |
| RE-RBS-60 | Second layer | &check; | &cross;   | First layer  | &check; | First layer  | &check;      | Full         |
| RE-RBS-61 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &cross;      | Partial      |
| RE-RBS-62 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &cross;      | Partial      |
| RE-RBS-63 | Second layer | &check; | &check;   | First layer  | &check; | &cross;      | &cross;      | &cross;      |
| RE-RBS-64 | First layer  | &check; | &check;   | Second layer | &cross; | Second layer | &check;      | Partial      |
| RE-RBS-65 | First layer  | &check; | &check;   | Second layer | &check; | Second layer | &check;      | Partial      |
| RE-RBS-66 | Second layer | &check; | &check;   | First layer  | &cross; | First layer  | &check;      | Partial      |
| RE-RBS-67 | Second layer | &check; | &check;   | First layer  | &check; | First layer  | &check;      | Partial      |
| RE-RBS-68 | &cross;      | &cross; | &cross;   | &cross;      | &cross; | &cross;      | &check;      | &cross;      |
| RE-RBS-69 | First layer  | &cross; | &cross;   | &cross;      | &cross; | &cross;      | &check;      | &cross;      |
| RE-RBS-70 | First layer  | &check; | &cross;   | &cross;      | &cross; | &cross;      | &check;      | &cross;      | 
| RE-RBS-71 | Second layer | &cross; | &cross;   | &cross;      | &cross; | &cross;      | &check;      | &cross;      | 
| RE-RBS-72 | Second layer | &check; | &cross;   | &cross;      | &cross; | &cross;      | &check;      | &cross;      |
| RE-RBS-73 | First layer  | &cross; | &cross;   | Second layer | &cross; | &cross;      | &check;      | &cross;      | 
| RE-RBS-74 | First layer  | &check; | &check;   | Second layer | &cross; | &cross;      | &check;      | &cross;      |    
| RE-RBS-75 | First layer  | &check; | &check;   | Second layer | &check; | &cross;      | &check;      | &cross;      |    
| RE-RBS-76 | Second layer | &cross; | &cross;   | First layer  | &cross; | &cross;      | &check;      | &cross;      |    
| RE-RBS-77 | Second layer | &check; | &check;   | First layer  | &cross; | &cross;      | &check;      | &cross;      |
| RE-RBS-78 | Second layer | &check; | &check;   | First layer  | &check; | &cross;      | &check;      | &cross;      |

In addition to these basic unit tests, a few extra edge cases are checked.

| Test name | Description of the unit test                                                |
| --------- | --------------------------------------------------------------------------- |
| RE-RBS-79 | Testing the randomness of the investigated direction for the soil movement. |
