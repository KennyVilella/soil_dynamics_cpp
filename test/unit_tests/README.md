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
| IC-MBS-1  | &cross;      | &cross; | &cross;      | &cross; | Terrain      | &check;      | &cross; |
| IC-MBS-2  | First layer  | &cross; | &cross;      | &cross; | &cross;      | &cross;      | &check; |
| IC-MBS-3  | First layer  | &cross; | &cross;      | &cross; | Terrain      | &check;      | &cross; |
| IC-MBS-4  | First layer  | &cross; | &cross;      | &cross; | First layer  | &check;      | &cross; |
| IC-MBS-5  | First layer  | &check; | &cross;      | &cross; | First layer  | &check;      | &cross; |
| IC-MBS-6  | Second layer | &check; | &cross;      | &cross; | &cross;      | &cross;      | &check; |
| IC-MBS-7  | Second layer | &cross; | &cross;      | &cross; | Terrain      | &check;      | &cross; |
| IC-MBS-8  | Second layer | &cross; | &cross;      | &cross; | Second layer | &check;      | &cross; |
| IC-MBS-9  | Second layer | &check; | &cross;      | &cross; | Second layer | &check;      | &cross; |
| IC-MBS-10 | First layer  | &check; | Second layer | &cross; | &cross;      | &cross;      | &cross; |
| IC-MBS-11 | First layer  | &cross; | Second layer | &cross; | First layer  | &check;      | &cross; |
| IC-MBS-12 | First layer  | &cross; | Second layer | &cross; | First layer  | &cross;      | &cross; |
| IC-MBS-13 | First layer  | &check; | Second layer | &cross; | First layer  | &check;      | &cross; |
| IC-MBS-14 | First layer  | &check; | Second layer | &cross; | First layer  | &cross;      | &cross; |
| IC-MBS-15 | Second layer | &check; | First layer  | &cross; | &cross;      | &cross;      | &cross; |
| IC-MBS-16 | Second layer | &cross; | First layer  | &cross; | Second layer | &check;      | &cross; |
| IC-MBS-17 | Second layer | &cross; | First layer  | &cross; | Second layer | &cross;      | &cross; |
| IC-MBS-18 | Second layer | &check; | First layer  | &cross; | Second layer | &check;      | &cross; |
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
  <tr><td>Test name </td><td>Bottom layer </td><td>Bottom layer</td><td>Soil </td><td>Top layer     </td><td>Soil  </td><td>Avalanche</td><td>Enough space</td><td>Blocked</td></tr>
  <tr><td>IC-MIBS-1  </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-2  </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-3  </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-4  </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-5  </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-6  </td><td>First layer </td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-7  </td><td>First layer </td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-8  </td><td>First layer </td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-9  </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-10 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-11 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-12 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-13 </td><td>First layer </td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-14 </td><td>First layer </td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-15 </td><td>First layer </td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-16 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-17 </td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-18 </td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-19 </td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-20 </td><td>Second layer</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-21 </td><td>Second layer</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-22 </td><td>Second layer</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-23 </td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-24 </td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-25 </td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-26 </td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-27 </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-28 </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-29 </td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-30 </td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-31 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-32 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-33 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-34 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-35 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-36 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-37 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-38 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-39 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-40 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-41 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-42 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-43 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-44 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-45 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-46 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-47 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-48 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-49 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-50 </td><td>First layer </td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-51 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-52 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-53 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-54 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-55 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-56 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-57 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-58 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-59 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-60 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-61 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-62 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-63 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-64 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-65 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-66 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-67 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-68 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-69 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-70 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-71 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-72 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-73 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-74 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-75 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-76 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-77 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-78 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-79 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-80 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-81 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-82 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-83 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-84 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-85 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-86 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-87 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-88 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-89 </td><td>First layer </td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-90 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-91 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-92 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-93 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-94 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-95 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-96 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-97 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-98 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-99 </td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-100</td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-101</td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-102</td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-103</td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-104</td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-105</td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-106</td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-107</td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-108</td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-109</td><td>First layer </td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-110</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-111</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-112</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-113</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-114</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-115</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-116</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-117</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-118</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-119</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-120</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-121</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-122</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-123</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-124</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-125</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-126</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-127</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-128</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-129</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-130</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-131</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-132</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-133</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-134</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-135</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-136</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-137</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-138</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-139</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-140</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-141</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-142</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-143</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-144</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-145</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-146</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-147</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-148</td><td>First layer </td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-149</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-150</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-151</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-152</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-153</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-154</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-155</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-156</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-157</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-158</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-159</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-160</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-161</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-162</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-163</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-164</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-165</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-166</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-167</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-168</td><td>Second layer</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-169</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-170</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-171</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-172</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-173</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-174</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-175</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-176</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-177</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-178</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-179</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-180</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-181</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-182</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-183</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-184</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-185</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-186</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-187</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-188</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-189</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-190</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-191</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-192</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-193</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-194</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-195</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-196</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-197</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-198</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-199</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-200</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-201</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-202</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-203</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-204</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-205</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-206</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-207</td><td>Second layer</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-208</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-209</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-210</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-211</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-212</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-213</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-214</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-215</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-216</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-217</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-218</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-219</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-220</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-221</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-222</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-223</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-224</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-225</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-226</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-227</td><td>Second layer</td><td>Second layer</td><td>&cross;</td><td>First layer </td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-228</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-229</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-230</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-231</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-232</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-233</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-234</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-235</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-236</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-237</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-238</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-239</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-240</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-241</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-242</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-243</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-244</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-245</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-246</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-247</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>Second layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-248</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-249</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-250</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-251</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-252</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-253</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-254</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-255</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-256</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>&check;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-257</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-258</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-259</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-260</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-261</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>First layer </td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-262</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-263</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&cross;</td><td>First Layer </td><td>&cross;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-264</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-265</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>Second Layer</td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>
  <tr><td>IC-MIBS-266</td><td>Second layer</td><td>Second layer</td><td>&check;</td><td>First layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>Second Layer</td><td>&check;</td><td>First Layer </td><td>&check;</td><td>&cross;     </td><td>&cross;</td><td>&cross;</td></tr>
  <tr><td>           </td><td>Second layer</td><td>&cross;     </td><td>&cross;</td><td>&cross;     </td><td>&cross;</td><td>Terrain     </td><td>&check;</td><td>&cross;</td></tr>







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

| Test name | Description of the unit test                                                      |
| --------- | --------------------------------------------------------------------------------- |
| IC-LIC-1  | Testing with first bucket layer and no intersecting cell.                         |
| IC-LIC-2  | Testing with first bucket layer and no intersecting cell. (2)                     |
| IC-LIC-3  | Testing with second bucket layer and no intersecting cell.                        |
| IC-LIC-4  | Testing with first and second bucket layer and no intersecting cell.              |
| IC-LIC-5  | Testing with first bucket layer fully intersecting with the terrain.              |
| IC-LIC-6  | Testing with second bucket layer fully intersecting with the terrain.             |
| IC-LIC-7  | Testing with first bucket layer fully intersecting with the terrain and second bucket layer partially intersecting.   |
| IC-LIC-8  | Testing with second bucket layer fully intersecting with the terrain and first bucket layer not intersecting.   |
| IC-LIC-9  | Testing  with first bucket layer fully intersecting with the terrain and second bucket layer not intersecting.  |
| IC-LIC-10 | Testing with first and second bucket layer fully intersecting with the terrain.   |

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

| Test name | Description of the unit test                                                                       |
| --------- | -------------------------------------------------------------------------------------------------- |
| RE-LUT-1  | Testing for all terrain at zero. No cell is detected.                                              |
| RE-LUT-2  | Testing for a positive height that is stable or unstable depending on the minimum height provided. |
| RE-LUT-3  | Same as RE-LUT-2 but for a higher height.                                                          |
| RE-LUT-4  | Testing for a negative height that is stable or unstable depending on the minimum height provided. |
| RE-LUT-5  | Same as RE-LUT-4 but for a lower height.                                                           |
| RE-LUT-6  | Testing with two unstable neighbouring cells.                                                      |
| RE-LUT-7  | Testing with an unstable cell close to the border of the grid.                                     |

### `CheckUnstableTerrainCell`

Unit test for the `CheckUnstableTerrainCell` function.

The tested function checks the configuration in a specified location and return a status code following the situation.
The purpose of these tests is to check all possible configurations.
The description of the unit tests can therefore be done with a simple table describing the configuration at the specified location.

| Test name | Bottom layer | Soil    | Until top | Stable  | Top layer    | Soil    | Stable  | Avalanche    |
| --------- | ------------ | ------- | --------- |-------- | ------------ | ------- | ------- | ------------ |
| RE-CUT-1  | &cross;      | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-CUT-2  | &cross;      | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | Terrain      |
| RE-CUT-3  | First layer  | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | Terrain      |
| RE-CUT-4  | First layer  | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-CUT-5  | First layer  | &cross; | &cross;   | &cross; | &cross;      | &cross; | &check; | First layer  |
| RE-CUT-6  | First layer  | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | Terrain      |
| RE-CUT-7  | First layer  | &check; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-CUT-8  | First layer  | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | First layer  |
| RE-CUT-9  | Second layer | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | Terrain      |
| RE-CUT-10 | Second layer | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-CUT-11 | Second layer | &cross; | &cross;   | &cross; | &cross;      | &cross; | &check; | Second layer |
| RE-CUT-12 | Second layer | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | Terrain      |
| RE-CUT-13 | Second layer | &check; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-CUT-14 | Second layer | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | Second layer |
| RE-CUT-15 | First layer  | &cross; | &cross;   | &cross; | Second layer | &cross; | &cross; | Terrain      |
| RE-CUT-16 | First layer  | &cross; | &cross;   | &cross; | Second layer | &cross; | &check; | First layer  |
| RE-CUT-17 | First layer  | &cross; | &cross;   | &cross; | Second layer | &cross; | &cross; | First layer  |
| RE-CUT-18 | First layer  | &cross; | &cross;   | &cross; | Second layer | &check; | &cross; | Terrain      |
| RE-CUT-19 | First layer  | &cross; | &cross;   | &cross; | Second layer | &check; | &check; | First layer  |
| RE-CUT-20 | First layer  | &cross; | &cross;   | &cross; | Second layer | &check; | &cross; | First layer  |
| RE-CUT-21 | First layer  | &check; | &cross;   | &cross; | Second layer | &cross; | &cross; | Terrain      |
| RE-CUT-22 | First layer  | &check; | &cross;   | &cross; | Second layer | &cross; | &check; | First layer  |
| RE-CUT-23 | First layer  | &check; | &cross;   | &cross; | Second layer | &cross; | &cross; | First layer  |
| RE-CUT-24 | First layer  | &check; | &cross;   | &cross; | Second layer | &check; | &cross; | Terrain      |
| RE-CUT-25 | First layer  | &check; | &cross;   | &cross; | Second layer | &check; | &check; | First layer  |
| RE-CUT-26 | First layer  | &check; | &cross;   | &cross; | Second layer | &check; | &cross; | First layer  |
| RE-CUT-27 | First layer  | &check; | &check;   | &check; | Second layer | &cross; | &cross; | Terrain      |
| RE-CUT-28 | First layer  | &check; | &check;   | &check; | Second layer | &cross; | &check; | &cross;      |
| RE-CUT-29 | First layer  | &check; | &check;   | &check; | Second layer | &cross; | &cross; | Second layer |
| RE-CUT-30 | First layer  | &check; | &check;   | &check; | Second layer | &check; | &cross; | Terrain      |
| RE-CUT-31 | First layer  | &check; | &check;   | &check; | Second layer | &check; | &check; | &cross;      |
| RE-CUT-32 | First layer  | &check; | &check;   | &check; | Second layer | &check; | &cross; | Second layer |
| RE-CUT-33 | Second layer | &cross; | &cross;   | &cross; | First layer  | &cross; | &cross; | Terrain      |
| RE-CUT-34 | Second layer | &cross; | &cross;   | &cross; | First layer  | &cross; | &check; | Second layer |
| RE-CUT-35 | Second layer | &cross; | &cross;   | &cross; | First layer  | &cross; | &cross; | Second layer |
| RE-CUT-36 | Second layer | &cross; | &cross;   | &cross; | First layer  | &check; | &cross; | Terrain      |
| RE-CUT-37 | Second layer | &cross; | &cross;   | &cross; | First layer  | &check; | &check; | Second layer |
| RE-CUT-38 | Second layer | &cross; | &cross;   | &cross; | First layer  | &check; | &cross; | Second layer |
| RE-CUT-39 | Second layer | &check; | &cross;   | &cross; | First layer  | &cross; | &cross; | Terrain      |
| RE-CUT-40 | Second layer | &check; | &cross;   | &cross; | First layer  | &cross; | &check; | Second layer |
| RE-CUT-41 | Second layer | &check; | &cross;   | &cross; | First layer  | &cross; | &cross; | Second layer |
| RE-CUT-42 | Second layer | &check; | &cross;   | &cross; | First layer  | &check; | &cross; | Terrain      |
| RE-CUT-43 | Second layer | &check; | &cross;   | &cross; | First layer  | &check; | &check; | Second layer |
| RE-CUT-44 | Second layer | &check; | &cross;   | &cross; | First layer  | &check; | &cross; | Second layer |
| RE-CUT-45 | Second layer | &check; | &check;   | &check; | First layer  | &cross; | &cross; | Terrain      |
| RE-CUT-46 | Second layer | &check; | &check;   | &check; | First layer  | &cross; | &check; | &cross;      |
| RE-CUT-47 | Second layer | &check; | &check;   | &check; | First layer  | &cross; | &cross; | First layer  |
| RE-CUT-48 | Second layer | &check; | &check;   | &check; | First layer  | &check; | &cross; | Terrain      |
| RE-CUT-49 | Second layer | &check; | &check;   | &check; | First layer  | &check; | &check; | &cross;      |
| RE-CUT-50 | Second layer | &check; | &check;   | &check; | First layer  | &check; | &cross; | First layer  |

In this table,
- `Soil` indicates whether soil is present on the corresponding layer.
- `Until top` indicates whether the soil fully fill the gap between the two layers.
- `Stable` indicates whether the corresponding soil layer is unstable considering the configuration.
- `Avalanche` indicates where the soil should avalanche.

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
| RE-RUT-1  | &cross;      | &cross; | &cross;      | &cross; | Terrain      | &check;      |
| RE-RUT-2  | First layer  | &cross; | &cross;      | &cross; | Terrain      | &check;      |
| RE-RUT-3  | First layer  | &cross; | &cross;      | &cross; | Terrain      | &cross;      |
| RE-RUT-4  | First layer  | &cross; | &cross;      | &cross; | First layer  | &check;      |
| RE-RUT-5  | First layer  | &check; | &cross;      | &cross; | Terrain      | &check;      |
| RE-RUT-6  | First layer  | &check; | &cross;      | &cross; | Terrain      | &cross;      |
| RE-RUT-7  | First layer  | &check; | &cross;      | &cross; | First layer  | &check;      |
| RE-RUT-8  | Second layer | &cross; | &cross;      | &cross; | Terrain      | &check;      |
| RE-RUT-9  | Second layer | &cross; | &cross;      | &cross; | Terrain      | &cross;      |
| RE-RUT-10 | Second layer | &cross; | &cross;      | &cross; | Second layer | &check;      |
| RE-RUT-11 | Second layer | &check; | &cross;      | &cross; | Terrain      | &check;      |
| RE-RUT-12 | Second layer | &check; | &cross;      | &cross; | Terrain      | &cross;      |
| RE-RUT-13 | Second layer | &check; | &cross;      | &cross; | Second layer | &check;      |
| RE-RUT-14 | First layer  | &cross; | Second layer | &cross; | Terrain      | &check;      |
| RE-RUT-15 | First layer  | &cross; | Second layer | &cross; | Terrain      | &cross;      |
| RE-RUT-16 | First layer  | &cross; | Second layer | &cross; | First layer  | &check;      |
| RE-RUT-17 | First layer  | &cross; | Second layer | &cross; | First layer  | &cross;      |
| RE-RUT-18 | First layer  | &check; | Second layer | &cross; | Second layer | &check;      |
| RE-RUT-19 | First layer  | &check; | Second layer | &check; | First layer  | &check;      |
| RE-RUT-20 | First layer  | &check; | Second layer | &check; | First layer  | &cross;      |
| RE-RUT-21 | First layer  | &check; | Second layer | &check; | Second layer | &check;      |
| RE-RUT-22 | Second layer | &cross; | First layer  | &cross; | Terrain      | &check;      |
| RE-RUT-23 | Second layer | &cross; | First layer  | &cross; | Terrain      | &cross;      |
| RE-RUT-24 | Second layer | &cross; | First layer  | &cross; | Second layer | &check;      |
| RE-RUT-25 | Second layer | &cross; | First layer  | &cross; | Second layer | &cross;      |
| RE-RUT-26 | Second layer | &check; | First layer  | &cross; | First layer  | &check;      |
| RE-RUT-27 | Second layer | &check; | First layer  | &check; | Second layer | &check;      |
| RE-RUT-28 | Second layer | &check; | First layer  | &check; | Second layer | &cross;      |
| RE-RUT-29 | Second layer | &check; | First layer  | &check; | First layer  | &check;      |

In this table,
- `Soil` indicates whether soil is present on the corresponding layer.
- `Avalanche` indicates where the soil should avalanche.
- `Enough space` indicates whether there is enough space on the layer where the soil should avalanche to accommodate all the avalanching soil.

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
| RE-RT-2   | &cross;      | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | Terrain      |
| RE-RT-3   | First layer  | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | Terrain      |
| RE-RT-4   | First layer  | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-RT-5   | First layer  | &cross; | &cross;   | &cross; | &cross;      | &cross; | &check; | First layer  |
| RE-RT-6   | First layer  | &check; | &cross;   | &check; | &cross;      | &cross; | &check; | Terrain      |
| RE-RT-7   | First layer  | &check; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-RT-8   | First layer  | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | First layer  |
| RE-RT-9   | Second layer | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | Terrain      |
| RE-RT-10  | Second layer | &cross; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-RT-11  | Second layer | &cross; | &cross;   | &cross; | &cross;      | &cross; | &check; | Second layer |
| RE-RT-12  | Second layer | &check; | &cross;   | &check; | &cross;      | &cross; | &check; | Terrain      |
| RE-RT-13  | Second layer | &check; | &cross;   | &check; | &cross;      | &cross; | &check; | &cross;      |
| RE-RT-14  | Second layer | &check; | &cross;   | &cross; | &cross;      | &cross; | &check; | Second layer |
| RE-RT-15  | First layer  | &cross; | &cross;   | &check; | Second layer | &cross; | &check; | Terrain      |
| RE-RT-16  | First layer  | &cross; | &cross;   | &cross; | Second layer | &cross; | &check; | First layer  |
| RE-RT-17  | First layer  | &cross; | &cross;   | &cross; | Second layer | &cross; | &cross; | First layer  |
|           | First layer  | &check; | &check;   | &check; | Second layer | &cross; | &cross; | Second layer |
| RE-RT-18  | First layer  | &cross; | &cross;   | &check; | Second layer | &check; | &check; | Terrain      |
| RE-RT-19  | First layer  | &cross; | &cross;   | &cross; | Second layer | &check; | &check; | First layer  |
| RE-RT-20  | First layer  | &cross; | &cross;   | &cross; | Second layer | &check; | &cross; | First layer  |
|           | First layer  | &check; | &check;   | &check; | Second layer | &check; | &cross; | Second layer |
| RE-RT-21  | First layer  | &check; | &cross;   | &check; | Second layer | &cross; | &check; | Terrain      |
| RE-RT-22  | First layer  | &check; | &cross;   | &cross; | Second layer | &cross; | &check; | First layer  |
| RE-RT-23  | First layer  | &check; | &cross;   | &cross; | Second layer | &cross; | &cross; | First layer  |
|           | First layer  | &check; | &check;   | &check; | Second layer | &cross; | &cross; | Second layer |
| RE-RT-24  | First layer  | &check; | &check;   | &check; | Second layer | &check; | &check; | Terrain      |
| RE-RT-25  | First layer  | &check; | &cross;   | &cross; | Second layer | &check; | &check; | First layer  |
| RE-RT-26  | First layer  | &check; | &cross;   | &cross; | Second layer | &check; | &cross; | First layer  |
|           | First layer  | &check; | &check;   | &check; | Second layer | &check; | &cross; | Second layer |
| RE-RT-27  | Second layer | &cross; | &cross;   | &check; | First layer  | &cross; | &check; | Terrain      |
| RE-RT-28  | Second layer | &cross; | &cross;   | &cross; | First layer  | &cross; | &check; | Second layer |
| RE-RT-29  | Second layer | &cross; | &cross;   | &cross; | First layer  | &cross; | &cross; | Second layer |
|           | Second layer | &check; | &check;   | &check; | First layer  | &cross; | &cross; | First layer  |
| RE-RT-30  | Second layer | &cross; | &cross;   | &check; | First layer  | &check; | &check; | Terrain      |
| RE-RT-31  | Second layer | &cross; | &cross;   | &cross; | First layer  | &check; | &check; | Second layer |
| RE-RT-32  | Second layer | &cross; | &cross;   | &cross; | First layer  | &check; | &cross; | Second layer |
|           | Second layer | &check; | &check;   | &check; | First layer  | &check; | &cross; | First layer  |
| RE-RT-33  | Second layer | &check; | &check;   | &check; | First layer  | &cross; | &check; | Terrain      |
| RE-RT-34  | Second layer | &check; | &cross;   | &cross; | First layer  | &cross; | &check; | Second layer |
| RE-RT-35  | Second layer | &check; | &cross;   | &cross; | First layer  | &cross; | &cross; | Second layer |
|           | Second layer | &check; | &check;   | &check; | First layer  | &cross; | &cross; | First layer  |
| RE-RT-36  | Second layer | &check; | &check;   | &check; | First layer  | &check; | &check; | Terrain      |
| RE-RT-37  | Second layer | &check; | &cross;   | &cross; | First layer  | &check; | &check; | Second layer |
| RE-RT-38  | Second layer | &check; | &cross;   | &cross; | First layer  | &check; | &cross; | Second layer |
|           | Second layer | &check; | &check;   | &check; | First layer  | &check; | &cross; | First layer  |

In this table,
- `Soil` indicates whether soil is present on the corresponding layer.
- `Until top` indicates whether the soil fully fill the gap between the two layers.
- `Stable` indicates whether the corresponding soil layer is unstable considering the configuration.
- `Avalanche` indicates where the soil should avalanche.

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
| RE-CUB-1  | &cross;      | &cross; | &cross;   | &cross;    | &cross;      | &cross; | &cross;    | &cross;      |
| RE-CUB-2  | &cross;      | &cross; | &cross;   | &cross;    | &cross;      | &cross; | &cross;    | Terrain      |
| RE-CUB-3  | First layer  | &cross; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | &cross;      |
| RE-CUB-4  | First layer  | &cross; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | Terrain      |
| RE-CUB-5  | First layer  | &cross; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | First layer  |
| RE-CUB-6  | First layer  | &check; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | &cross;      |
| RE-CUB-7  | First layer  | &check; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | Terrain      |
| RE-CUB-8  | First layer  | &check; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | First layer  |
| RE-CUB-9  | Second layer | &cross; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | &cross;      |
| RE-CUB-10 | Second layer | &cross; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | Terrain      |
| RE-CUB-11 | Second layer | &cross; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | Second layer |
| RE-CUB-12 | Second layer | &check; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | &cross;      |
| RE-CUB-13 | Second layer | &check; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | Terrain      |
| RE-CUB-14 | Second layer | &check; | &cross;   | &check;    | &cross;      | &cross; | &cross;    | Second layer |
| RE-CUB-15 | First layer  | &cross; | &cross;   | &check;    | Second layer | &cross; | &cross;    | &cross;      |
| RE-CUB-16 | First layer  | &cross; | &cross;   | &check;    | Second layer | &cross; | &cross;    | First layer  |
| RE-CUB-17 | First layer  | &cross; | &cross;   | &check;    | Second layer | &cross; | &check;    | &cross;      |
| RE-CUB-18 | First layer  | &cross; | &cross;   | &check;    | Second layer | &cross; | &check;    | First layer  |
| RE-CUB-19 | First layer  | &cross; | &cross;   | &check;    | Second layer | &check; | &cross;    | &cross;      |
| RE-CUB-20 | First layer  | &cross; | &cross;   | &check;    | Second layer | &check; | &cross;    | First layer  |
| RE-CUB-21 | First layer  | &cross; | &cross;   | &check;    | Second layer | &check; | &check;    | &cross;      |
| RE-CUB-22 | First layer  | &cross; | &cross;   | &check;    | Second layer | &check; | &check;    | First layer  |
| RE-CUB-23 | First layer  | &cross; | &cross;   | &cross;    | Second layer | &cross; | &cross;    | &cross;      |
| RE-CUB-24 | First layer  | &cross; | &cross;   | &cross;    | Second layer | &cross; | &check;    | &cross;      |
| RE-CUB-25 | First layer  | &cross; | &cross;   | &cross;    | Second layer | &cross; | &check;    | Second layer |
| RE-CUB-26 | First layer  | &cross; | &cross;   | &cross;    | Second layer | &check; | &cross;    | &cross;      |
| RE-CUB-27 | First layer  | &cross; | &cross;   | &cross;    | Second layer | &check; | &check;    | &cross;      |
| RE-CUB-28 | First layer  | &cross; | &cross;   | &cross;    | Second layer | &check; | &check;    | Second layer |
| RE-CUB-29 | First layer  | &check; | &cross;   | &check;    | Second layer | &cross; | &cross;    | &cross;      |
| RE-CUB-30 | First layer  | &check; | &cross;   | &check;    | Second layer | &cross; | &cross;    | First layer  |
| RE-CUB-31 | First layer  | &check; | &cross;   | &check;    | Second layer | &cross; | &check;    | &cross;      |
| RE-CUB-32 | First layer  | &check; | &cross;   | &check;    | Second layer | &cross; | &check;    | First layer  |
| RE-CUB-33 | First layer  | &check; | &cross;   | &check;    | Second layer | &check; | &cross;    | &cross;      |
| RE-CUB-34 | First layer  | &check; | &cross;   | &check;    | Second layer | &check; | &cross;    | First layer  |
| RE-CUB-35 | First layer  | &check; | &cross;   | &check;    | Second layer | &check; | &check;    | &cross;      |
| RE-CUB-36 | First layer  | &check; | &cross;   | &check;    | Second layer | &check; | &check;    | First layer  |
| RE-CUB-37 | First layer  | &check; | &cross;   | &cross;    | Second layer | &cross; | &cross;    | &cross;      |
| RE-CUB-38 | First layer  | &check; | &cross;   | &cross;    | Second layer | &cross; | &check;    | &cross;      |
| RE-CUB-39 | First layer  | &check; | &cross;   | &cross;    | Second layer | &cross; | &check;    | Second layer |
| RE-CUB-40 | First layer  | &check; | &cross;   | &cross;    | Second layer | &check; | &cross;    | &cross;      |
| RE-CUB-41 | First layer  | &check; | &cross;   | &cross;    | Second layer | &check; | &check;    | &cross;      |
| RE-CUB-42 | First layer  | &check; | &cross;   | &cross;    | Second layer | &check; | &check;    | Second layer |
| RE-CUB-43 | First layer  | &check; | &check;   | &check;    | Second layer | &cross; | &cross;    | &cross;      |
| RE-CUB-44 | First layer  | &check; | &check;   | &check;    | Second layer | &cross; | &check;    | &cross;      |
| RE-CUB-45 | First layer  | &check; | &check;   | &check;    | Second layer | &cross; | &check;    | Second layer |
| RE-CUB-46 | First layer  | &check; | &check;   | &check;    | Second layer | &check; | &cross;    | &cross;      |
| RE-CUB-47 | First layer  | &check; | &check;   | &check;    | Second layer | &check; | &check;    | &cross;      |
| RE-CUB-48 | First layer  | &check; | &check;   | &check;    | Second layer | &check; | &check;    | Second layer |
| RE-CUB-49 | First layer  | &check; | &check;   | &cross;    | Second layer | &cross; | &cross;    | &cross;      |
| RE-CUB-50 | First layer  | &check; | &check;   | &cross;    | Second layer | &cross; | &check;    | &cross;      |
| RE-CUB-51 | First layer  | &check; | &check;   | &cross;    | Second layer | &cross; | &check;    | Second layer |
| RE-CUB-52 | First layer  | &check; | &check;   | &cross;    | Second layer | &check; | &cross;    | &cross;      |
| RE-CUB-53 | First layer  | &check; | &check;   | &cross;    | Second layer | &check; | &check;    | &cross;      |
| RE-CUB-54 | First layer  | &check; | &check;   | &cross;    | Second layer | &check; | &check;    | Second layer |
| RE-CUB-55 | Second layer | &cross; | &cross;   | &check;    | First layer  | &cross; | &cross;    | &cross;      |
| RE-CUB-56 | Second layer | &cross; | &cross;   | &check;    | First layer  | &cross; | &cross;    | Second layer |
| RE-CUB-57 | Second layer | &cross; | &cross;   | &check;    | First layer  | &cross; | &check;    | &cross;      |
| RE-CUB-58 | Second layer | &cross; | &cross;   | &check;    | First layer  | &cross; | &check;    | Second layer |
| RE-CUB-59 | Second layer | &cross; | &cross;   | &check;    | First layer  | &check; | &cross;    | &cross;      |
| RE-CUB-60 | Second layer | &cross; | &cross;   | &check;    | First layer  | &check; | &cross;    | Second layer |
| RE-CUB-61 | Second layer | &cross; | &cross;   | &check;    | First layer  | &check; | &check;    | &cross;      |
| RE-CUB-62 | Second layer | &cross; | &cross;   | &check;    | First layer  | &check; | &check;    | Second layer |
| RE-CUB-63 | Second layer | &cross; | &cross;   | &cross;    | First layer  | &cross; | &cross;    | &cross;      |
| RE-CUB-64 | Second layer | &cross; | &cross;   | &cross;    | First layer  | &cross; | &check;    | &cross;      |
| RE-CUB-65 | Second layer | &cross; | &cross;   | &cross;    | First layer  | &cross; | &check;    | First layer  |
| RE-CUB-66 | Second layer | &cross; | &cross;   | &cross;    | First layer  | &check; | &cross;    | &cross;      |
| RE-CUB-67 | Second layer | &cross; | &cross;   | &cross;    | First layer  | &check; | &check;    | &cross;      |
| RE-CUB-68 | Second layer | &cross; | &cross;   | &cross;    | First layer  | &check; | &check;    | First layer  |
| RE-CUB-69 | Second layer | &check; | &cross;   | &check;    | First layer  | &cross; | &cross;    | &cross;      |
| RE-CUB-70 | Second layer | &check; | &cross;   | &check;    | First layer  | &cross; | &cross;    | Second layer |
| RE-CUB-71 | Second layer | &check; | &cross;   | &check;    | First layer  | &cross; | &check;    | &cross;      |
| RE-CUB-72 | Second layer | &check; | &cross;   | &check;    | First layer  | &cross; | &check;    | Second layer |
| RE-CUB-73 | Second layer | &check; | &cross;   | &check;    | First layer  | &check; | &cross;    | &cross;      |
| RE-CUB-74 | Second layer | &check; | &cross;   | &check;    | First layer  | &check; | &cross;    | Second layer |
| RE-CUB-75 | Second layer | &check; | &cross;   | &check;    | First layer  | &check; | &check;    | &cross;      |
| RE-CUB-76 | Second layer | &check; | &cross;   | &check;    | First layer  | &check; | &check;    | Second layer |
| RE-CUB-77 | Second layer | &check; | &cross;   | &cross;    | First layer  | &cross; | &cross;    | &cross;      |
| RE-CUB-78 | Second layer | &check; | &cross;   | &cross;    | First layer  | &cross; | &check;    | &cross;      |
| RE-CUB-79 | Second layer | &check; | &cross;   | &cross;    | First layer  | &cross; | &check;    | First layer  |
| RE-CUB-80 | Second layer | &check; | &cross;   | &cross;    | First layer  | &check; | &cross;    | &cross;      |
| RE-CUB-81 | Second layer | &check; | &cross;   | &cross;    | First layer  | &check; | &check;    | &cross;      |
| RE-CUB-82 | Second layer | &check; | &cross;   | &cross;    | First layer  | &check; | &check;    | First layer  |
| RE-CUB-83 | Second layer | &check; | &check;   | &check;    | First layer  | &cross; | &cross;    | &cross;      |
| RE-CUB-84 | Second layer | &check; | &check;   | &check;    | First layer  | &cross; | &check;    | &cross;      |
| RE-CUB-85 | Second layer | &check; | &check;   | &check;    | First layer  | &cross; | &check;    | First layer  |
| RE-CUB-86 | Second layer | &check; | &check;   | &check;    | First layer  | &check; | &cross;    | &cross;      |
| RE-CUB-87 | Second layer | &check; | &check;   | &check;    | First layer  | &check; | &check;    | &cross;      |
| RE-CUB-88 | Second layer | &check; | &check;   | &check;    | First layer  | &check; | &check;    | First layer  |
| RE-CUB-89 | Second layer | &check; | &check;   | &cross;    | First layer  | &cross; | &cross;    | &cross;      |
| RE-CUB-90 | Second layer | &check; | &check;   | &cross;    | First layer  | &cross; | &check;    | &cross;      |
| RE-CUB-91 | Second layer | &check; | &check;   | &cross;    | First layer  | &cross; | &check;    | First layer  |
| RE-CUB-92 | Second layer | &check; | &check;   | &cross;    | First layer  | &check; | &cross;    | &cross;      |
| RE-CUB-93 | Second layer | &check; | &check;   | &cross;    | First layer  | &check; | &check;    | &cross;      |
| RE-CUB-94 | Second layer | &check; | &check;   | &cross;    | First layer  | &check; | &check;    | First layer  |

In this table,
- `Soil` indicates whether soil is present on the corresponding layer.
- `Until top` indicates whether the soil fully fill the gap between the two layers.
- `Accessible` indicates whether the soil could potentially avalanche on this layer.
  The layer is not accessible if a wall blocked the movement or if the soil column in the corresponding layer is higher than the considered soil column.
- `Avalanche` indicates where the soil should avalanche.

### `RelaxUnstableBodyCell`

Unit test for the `RelaxUnstableBodyCell` function.

The tested function moves the soil following the status code provided assuming that it corresponds to the actual configuration.
It is thus required to have avalanching soil.
The purpose of these tests is to check all possible configurations.
The description of the unit tests can therefore be done with a simple table describing the configuration.

For all the unit tests, the initial position has soil on the first bucket layer.
The configuration of the inital position should not impact the result of this function.

| Test name | Bottom layer | Soil    | Until top | Top layer    | Soil    | Avalanche    | Enough soil | Enough space | Status       |
| --------- | ------------ | ------- | --------- | ------------ | ------- | ------------ | ----------- | ------------ | ------------ |
| RE-RUB-1  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Partial      |
| RE-RUB-2  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Full         |
| RE-RUB-3  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &cross;     | &check;      | Partial      |
| RE-RUB-4  | First layer  | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Partial      |
| RE-RUB-5  | First layer  | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Full         |
| RE-RUB-6  | First layer  | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &cross;      | Partial      |
| RE-RUB-7  | First layer  | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &cross;     | &check;      | Partial      |
| RE-RUB-8  | First layer  | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &cross;     | &cross;      | Partial      |
| RE-RUB-9  | First layer  | &cross; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-10 | First layer  | &cross; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RUB-11 | First layer  | &cross; | &cross;   | &cross;      | &cross; | First layer  | &cross;     | &check;      | Partial      |
| RE-RUB-12 | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-13 | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RUB-14 | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &cross;     | &check;      | Partial      |
| RE-RUB-15 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Partial      |
| RE-RUB-16 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Full         |
| RE-RUB-17 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &cross;      | Partial      |
| RE-RUB-18 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &cross;     | &check;      | Partial      |
| RE-RUB-19 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &cross;     | &cross;      | Partial      |
| RE-RUB-20 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-21 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RUB-22 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Second layer | &cross;     | &check;      | Partial      |
| RE-RUB-23 | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-24 | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RUB-25 | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &cross;     | &check;      | Partial      |
| RE-RUB-26 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-27 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RUB-28 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &cross;     | &check;      | Partial      |
| RE-RUB-29 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &check;     | &cross;      | Partial      |
| RE-RUB-30 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &cross;     | &cross;      | Partial      |
| RE-RUB-31 | First layer  | &cross; | &cross;   | Second layer | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-32 | First layer  | &cross; | &cross;   | Second layer | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RUB-33 | First layer  | &cross; | &cross;   | Second layer | &cross; | Second layer | &cross;     | &check;      | Partial      |
| RE-RUB-34 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-35 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;     | &check;      | Full         |
| RE-RUB-36 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &cross;     | &check;      | Partial      |
| RE-RUB-37 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;     | &cross;      | Partial      |
| RE-RUB-38 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &cross;     | &cross;      | Partial      |
| RE-RUB-39 | First layer  | &check; | &cross;   | Second layer | &check; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-40 | First layer  | &check; | &cross;   | Second layer | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RUB-41 | First layer  | &check; | &cross;   | Second layer | &check; | Second layer | &cross;     | &check;      | Partial      |
| RE-RUB-42 | First layer  | &check; | &check;   | Second layer | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-43 | First layer  | &check; | &check;   | Second layer | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RUB-44 | First layer  | &check; | &check;   | Second layer | &cross; | Second layer | &cross;     | &check;      | Partial      |
| RE-RUB-45 | First layer  | &check; | &check;   | Second layer | &check; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-46 | First layer  | &check; | &check;   | Second layer | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RUB-47 | First layer  | &check; | &check;   | Second layer | &check; | Second layer | &cross;     | &check;      | Partial      |
| RE-RUB-48 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-49 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RUB-50 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &cross;     | &check;      | Partial      |
| RE-RUB-51 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &check;     | &cross;      | Partial      |
| RE-RUB-52 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &cross;     | &cross;      | Partial      |
| RE-RUB-53 | Second layer | &cross; | &cross;   | First layer  | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-54 | Second layer | &cross; | &cross;   | First layer  | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RUB-55 | Second layer | &cross; | &cross;   | First layer  | &cross; | First layer  | &cross;     | &check;      | Partial      |
| RE-RUB-56 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;     | &check;      | Partial      |
| RE-RUB-57 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RUB-58 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &cross;     | &check;      | Partial      |
| RE-RUB-59 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;     | &cross;      | Partial      |
| RE-RUB-60 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &cross;     | &cross;      | Partial      |
| RE-RUB-61 | Second layer | &check; | &cross;   | First layer  | &check; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-62 | Second layer | &check; | &cross;   | First layer  | &check; | First layer  | &check;     | &check;      | Full         |
| RE-RUB-63 | Second layer | &check; | &cross;   | First layer  | &check; | First layer  | &cross;     | &check;      | Partial      |
| RE-RUB-64 | Second layer | &check; | &check;   | First layer  | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-65 | Second layer | &check; | &check;   | First layer  | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RUB-66 | Second layer | &check; | &check;   | First layer  | &cross; | First layer  | &cross;     | &check;      | Partial      |
| RE-RUB-67 | Second layer | &check; | &check;   | First layer  | &check; | First layer  | &check;     | &check;      | Partial      |
| RE-RUB-68 | Second layer | &check; | &check;   | First layer  | &check; | First layer  | &check;     | &check;      | Full         |
| RE-RUB-69 | Second layer | &check; | &check;   | First layer  | &check; | First layer  | &cross;     | &check;      | Partial      |

In this table,
- `Soil` indicates whether soil is present on the corresponding layer.
- `Until top` indicates whether the soil fully fill the gap between the two layers.
- `Avalanche` indicates where the soil should avalanche.
- `Enough soil` indicates whether all the soil is located in one `body_soil` or several has to be moved.
- `Enough space` indicates whether there is enough space on the layer where the soil should avalanche to accommodate all the avalanching soil.
- `Status` indicates the type of soil avalanche. `Partial` when some soil remained from the original location, `Full` when all soil avalanche, and `&cross;` when there is no avalanche.

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

| Test name | Bottom layer | Soil    | Until top | Top layer    | Soil    | Avalanche    | Enough soil | Enough space | Status       |
| --------- | ------------ | ------- | --------- | ------------ | ------- | ------------ | ----------- | ------------ | ------------ |
| RE-RBS-1  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | &cross;      | &check;     | &check;      | &cross;      |
| RE-RBS-2  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Partial      |
| RE-RBS-3  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Full         |
| RE-RBS-4  | &cross;      | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &cross;     | &check;      | Partial      |
|           | &cross;      | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Full         |
| RE-RBS-5  | First layer  | &cross; | &cross;   | &cross;      | &cross; | &cross;      | &check;     | &check;      | &cross;      |
| RE-RBS-6  | First layer  | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Partial      |
| RE-RBS-7  | First layer  | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Full         |
| RE-RBS-8  | First layer  | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &cross;      | Partial      |
| RE-RBS-9  | First layer  | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &cross;     | &check;      | Partial      |
|           | First layer  | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Full         |
| RE-RBS-10 | First layer  | &cross; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-11 | First layer  | &cross; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-12 | First layer  | &cross; | &cross;   | &cross;      | &cross; | First layer  | &cross;     | &check;      | Partial      |
|           | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-13 | First layer  | &check; | &cross;   | &cross;      | &cross; | &cross;      | &check;     | &check;      | &cross;      |
| RE-RBS-14 | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-15 | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-16 | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &cross;     | &check;      | Partial      |
|           | First layer  | &check; | &cross;   | &cross;      | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-17 | Second layer | &cross; | &cross;   | &cross;      | &cross; | &cross;      | &check;     | &check;      | &cross;      |
| RE-RBS-18 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Partial      |
| RE-RBS-19 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Full         |
| RE-RBS-20 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &cross;      | Partial      |
| RE-RBS-21 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &cross;     | &check;      | Partial      |
|           | Second layer | &cross; | &cross;   | &cross;      | &cross; | Terrain      | &check;     | &check;      | Full         |
| RE-RBS-22 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-23 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-24 | Second layer | &cross; | &cross;   | &cross;      | &cross; | Second layer | &cross;     | &check;      | Partial      |
|           | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-25 | Second layer | &check; | &cross;   | &cross;      | &cross; | &cross;      | &check;     | &check;      | &cross;      |
| RE-RBS-26 | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-27 | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-28 | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &cross;     | &check;      | Partial      |
|           | Second layer | &check; | &cross;   | &cross;      | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-29 | First layer  | &cross; | &cross;   | Second layer | &cross; | &cross;      | &check;     | &check;      | &cross;      |
| RE-RBS-30 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-31 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-32 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &check;     | &cross;      | Partial      |
| RE-RBS-33 | First layer  | &cross; | &cross;   | Second layer | &cross; | First layer  | &cross;     | &check;      | Partial      |
|           | First layer  | &check; | &cross;   | Second layer | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-34 | First layer  | &cross; | &cross;   | Second layer | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-35 | First layer  | &cross; | &cross;   | Second layer | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-36 | First layer  | &cross; | &cross;   | Second layer | &cross; | Second layer | &cross;     | &check;      | Partial      |
|           | First layer  | &cross; | &cross;   | Second layer | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-37 | First layer  | &cross; | &cross;   | Second layer | &check; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-38 | First layer  | &cross; | &cross;   | Second layer | &check; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-39 | First layer  | &cross; | &cross;   | Second layer | &check; | First layer  | &check;     | &cross;      | Partial      |
| RE-RBS-40 | First layer  | &cross; | &cross;   | Second layer | &check; | First layer  | &cross;     | &check;      | Partial      |
|           | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-41 | First layer  | &cross; | &cross;   | Second layer | &check; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-42 | First layer  | &cross; | &cross;   | Second layer | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-43 | First layer  | &cross; | &cross;   | Second layer | &check; | Second layer | &cross;     | &check;      | Partial      |
|           | First layer  | &cross; | &cross;   | Second layer | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-44 | First layer  | &check; | &cross;   | Second layer | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-45 | First layer  | &check; | &cross;   | Second layer | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-46 | First layer  | &check; | &cross;   | Second layer | &cross; | First layer  | &check;     | &cross;      | Partial      |
| RE-RBS-47 | First layer  | &check; | &cross;   | Second layer | &cross; | First layer  | &cross;     | &check;      | Partial      |
|           | First layer  | &check; | &cross;   | Second layer | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-48 | First layer  | &check; | &cross;   | Second layer | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-49 | First layer  | &check; | &cross;   | Second layer | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-50 | First layer  | &check; | &cross;   | Second layer | &cross; | Second layer | &cross;     | &check;      | Partial      |
|           | First layer  | &check; | &cross;   | Second layer | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-51 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-52 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-53 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;     | &cross;      | Partial      |
| RE-RBS-54 | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &cross;     | &check;      | Partial      |
|           | First layer  | &check; | &cross;   | Second layer | &check; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-55 | First layer  | &check; | &cross;   | Second layer | &check; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-56 | First layer  | &check; | &cross;   | Second layer | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-57 | First layer  | &check; | &cross;   | Second layer | &check; | Second layer | &cross;     | &check;      | Partial      |
|           | First layer  | &check; | &cross;   | Second layer | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-58 | First layer  | &check; | &check;   | Second layer | &cross; | &cross;      | &check;     | &check;      | &cross;      |
| RE-RBS-59 | First layer  | &check; | &check;   | Second layer | &check; | &cross;      | &check;     | &check;      | &cross;      |
| RE-RBS-60 | First layer  | &check; | &check;   | Second layer | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-61 | First layer  | &check; | &check;   | Second layer | &cross; | Second layer | &cross;     | &check;      | Partial      |
|           | First layer  | &check; | &check;   | Second layer | &check; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-62 | First layer  | &check; | &check;   | Second layer | &check; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-63 | First layer  | &check; | &check;   | Second layer | &check; | Second layer | &cross;     | &check;      | Partial      |
|           | First layer  | &check; | &check;   | Second layer | &check; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-64 | Second layer | &cross; | &cross;   | First layer  | &cross; | &cross;      | &check;     | &check;      | &cross;      |
| RE-RBS-65 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-66 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-67 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &check;     | &cross;      | Partial      |
| RE-RBS-68 | Second layer | &cross; | &cross;   | First layer  | &cross; | Second layer | &cross;     | &check;      | Partial      |
|           | Second layer | &check; | &cross;   | First layer  | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-69 | Second layer | &cross; | &cross;   | First layer  | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-70 | Second layer | &cross; | &cross;   | First layer  | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-71 | Second layer | &cross; | &cross;   | First layer  | &cross; | First layer  | &cross;     | &check;      | Partial      |
|           | Second layer | &cross; | &cross;   | First layer  | &check; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-72 | Second layer | &cross; | &cross;   | First layer  | &check; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-73 | Second layer | &cross; | &cross;   | First layer  | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-74 | Second layer | &cross; | &cross;   | First layer  | &check; | Second layer | &check;     | &cross;      | Partial      |
| RE-RBS-75 | Second layer | &cross; | &cross;   | First layer  | &check; | Second layer | &cross;     | &check;      | Partial      |
|           | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-76 | Second layer | &cross; | &cross;   | First layer  | &check; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-77 | Second layer | &cross; | &cross;   | First layer  | &check; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-78 | Second layer | &cross; | &cross;   | First layer  | &check; | First layer  | &cross;     | &check;      | Partial      |
|           | Second layer | &cross; | &cross;   | First layer  | &check; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-79 | Second layer | &check; | &cross;   | First layer  | &cross; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-80 | Second layer | &check; | &cross;   | First layer  | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-81 | Second layer | &check; | &cross;   | First layer  | &cross; | Second layer | &check;     | &cross;      | Partial      |
| RE-RBS-82 | Second layer | &check; | &cross;   | First layer  | &cross; | Second layer | &cross;     | &check;      | Partial      |
|           | Second layer | &check; | &cross;   | First layer  | &cross; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-83 | Second layer | &check; | &cross;   | First layer  | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-84 | Second layer | &check; | &cross;   | First layer  | &cross; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-85 | Second layer | &check; | &cross;   | First layer  | &cross; | First layer  | &cross;     | &check;      | Partial      |
|           | Second layer | &check; | &cross;   | First layer  | &check; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-86 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;     | &check;      | Partial      |
| RE-RBS-87 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-88 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;     | &cross;      | Partial      |
| RE-RBS-89 | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &cross;     | &check;      | Partial      |
|           | Second layer | &check; | &cross;   | First layer  | &check; | Second layer | &check;     | &check;      | Full         |
| RE-RBS-90 | Second layer | &check; | &cross;   | First layer  | &check; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-91 | Second layer | &check; | &cross;   | First layer  | &check; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-92 | Second layer | &check; | &cross;   | First layer  | &check; | First layer  | &cross;     | &check;      | Partial      |
|           | Second layer | &check; | &cross;   | First layer  | &check; | First layer  | &check;     | &check;      | Full         |
| RE-RBS-93 | Second layer | &check; | &check;   | First layer  | &cross; | &cross;      | &check;     | &check;      | &cross;      |
| RE-RBS-94 | Second layer | &check; | &check;   | First layer  | &check; | &cross;      | &check;     | &check;      | &cross;      |
| RE-RBS-95 | Second layer | &check; | &check;   | First layer  | &cross; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-96 | Second layer | &check; | &check;   | First layer  | &cross; | First layer  | &cross;     | &check;      | Partial      |
|           | Second layer | &check; | &check;   | First layer  | &check; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-97 | Second layer | &check; | &check;   | First layer  | &check; | First layer  | &check;     | &check;      | Partial      |
| RE-RBS-98 | Second layer | &check; | &check;   | First layer  | &check; | First layer  | &cross;     | &check;      | Partial      |
|           | Second layer | &check; | &check;   | First layer  | &check; | First layer  | &check;     | &check;      | Partial      |

In this table,
- `Soil` indicates whether soil is present on the corresponding layer.
- `Until top` indicates whether the soil fully fill the gap between the two layers.
- `Avalanche` indicates where the soil should avalanche.
- `Enough soil` indicates whether all the soil is located in one `body_soil` or several has to be moved.
- `Enough space` indicates whether there is enough space on the layer where the soil should avalanche to accommodate all the avalanching soil.
- `Status` indicates the type of soil avalanche.

In addition to these basic unit tests, a few extra edge cases are checked.

| Test name | Description of the unit test                                                |
| --------- | --------------------------------------------------------------------------- |
| RE-RBS-99 | Testing the randomness of the investigated direction for the soil movement. |
