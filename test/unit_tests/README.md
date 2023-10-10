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
| BP-DVR-2  | Testing hat rounding is done properly with a simple rectangle in the XY plane.      |
| BP-DVR-3  | Testing with a simple rectangle in the XY plane on a cell border.                   |
| BP-DVR-4  | Testing with an arbitrary rectangle. Results were obtained with a drawing.          |
| BP-DVR-5  | Testing the edge case where the rectangle is a line. No decomposition can be made.  |
| BP-DVR-5  | Testing the edge case where the rectangle is a point. No decomposition can be made. |

### `DecomposeVectorTriangle`

Unit tests for the `DecomposeVectorTriangle` function.
Note that the `DecomposeVectorTriangle` function does not account for the case where the triangle follows a cell border. 
It is therefore necessary to solve this potential ambiguity before calling the function.

| Test name | Description of the unit test                                                       |
| --------- | ---------------------------------------------------------------------------------- |
| BP-DVT-1  | Testing with a simple triangle in the XY plane.                                    |
| BP-DVT-2  | Testing hat rounding is done properly with a simple triangle in the XY plane.      |
| BP-DVT-3  | Testing with an arbitrary triangle. Results were obtained with a drawing.          |
| BP-DVT-4  | Testing the edge case where the triangle is a line. No decomposition can be made.  |
| BP-DVT-5  | Testing the edge case where the triangle is a point. No decomposition can be made. |

### `CalcRectanglePos`

Unit tests for the `CalcRectanglePos` function.
Note that the `CalcRectanglePos` function does not account for the case where the rectangle follows a cell border.
It is therefore necessary to solve this potential ambiguity before calling the function.
As a result, a small increment (`1e-5`) is added or removed to the input in order to make sure that the input coordinates do not correspond to a cell border.

For each case (except edge cases), some tests are present to check that the results do not depend on the order where the rectangle vertices are given to the function.
For the first case (unit test 1 to 4), the full results are checked for three different orders and only the size of the outputs are checked for the five remaining possible orders.
For other cases, only the size of the output is checked for the seven remaining possible orders.

| Test name | Description of the unit test                                                    |
| --------- | ------------------------------------------------------------------------------- |
| BP-CR-1   | Testing with a simple rectangle in the XY plane.                                |
| BP-CR-2   | Testing with a simple rectangle in the XY plane in a different input order.     |
| BP-CR-3   | Testing with a simple rectangle in the XY plane in a different input order.     |
| BP-CR-4   | Testing with a simple rectangle in the XY plane for the remaining input orders. |
| BP-CR-5   | Testing with a simple rectangle in the XY plane on a cell border.               |
| BP-CR-6   | Testing with a simple rectangle in the XZ plane.                                |
| BP-CR-7   | Testing with an arbitrary rectangle. Results were obtained with a drawing.     |
| BP-CR-8   | Testing the edge case where the rectangle is a line.                            |
| BP-CR-9   | Testing the edge case where the rectangle is a point.                           |
| BP-CR-10  | Testing the edge case where the rectangle is a point on the edge of a cell.     |

### `CalcTrianglePos`

Unit tests for the `CalcTrianglePos` function.
Note that the `CalcTrianglePos` function does not account for the case where the triangle follows a cell border.
It is therefore necessary to solve this potential ambiguity before calling the function.
As a result, a small increment (`1e-5`) is added or removed to the input in order to make sure that the input coordinates do not correspond to a cell border.

For each case (except edge cases), some tests are present to check that the results do not depend on the order where the triangle vertices are given to the function.
For the first case (unit test 1 to 4), the full results are checked for three different orders and only the size of the outputs are checked for the three remaining possible orders.
For other cases, only the size of the output is checked for the five remaining possible orders.

| Test name | Description of the unit test                                                   |
| --------- | ------------------------------------------------------------------------------ |
| BP-CT-1   | Testing with a simple triangle in the XY plane.                                |
| BP-CT-2   | Testing with a simple triangle in the XY plane in a different input order.     |
| BP-CT-3   | Testing with a simple triangle in the XY plane in a different input order.     |
| BP-CT-4   | Testing with a simple triangle in the XY plane for the remaining input orders. |
| BP-CT-5   | Testing with a simple triangle in the XZ plane.                                |
| BP-CT-6   | Testing with an arbitrary triangle. Results were obtained with a drawing.      |
| BP-CT-7   | Testing the edge case where the triangle is a line.                            |
| BP-CT-8   | Testing the edge case where the triangle is a point.                           |

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
| BS-UBS-8  | Testing for a pi rotation around the X and Z axis when `body_soil_` is on the first body layer. Soil is avalanching to the `terrain_`. |
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

| Test name | Layer at bottom | Soil    | Layer at top | Soil    | Avalanche    | Enough space | Blocked |
| --------- | --------------- | ------- | ------------ | ------- | ------------ | ------------ | ------- |
| IC-MBS-1  | &cross;         | &cross; | &cross;      | &cross; | terrain      | &check;      | &cross; |
| IC-MBS-2  | First layer     | &cross; | &cross;      | &cross; | terrain      | &check;      | &cross; |
| IC-MBS-3  | First layer     | &cross; | &cross;      | &cross; | &cross;      | &cross;      | &check; |
| IC-MBS-4  | First layer     | &cross; | &cross;      | &cross; | First layer  | &check;      | &cross; |
| IC-MBS-5  | First layer     | &check; | &cross;      | &cross; | First layer  | &check;      | &cross; |
| IC-MBS-6  | Second layer    | &cross; | &cross;      | &cross; | terrain      | &check;      | &cross; |
| IC-MBS-7  | Second layer    | &check; | &cross;      | &cross; | &cross;      | &cross;      | &check; |
| IC-MBS-8  | Second layer    | &cross; | &cross;      | &cross; | Second layer | &check;      | &cross; |
| IC-MBS-9  | Second layer    | &check; | &cross;      | &cross; | Second layer | &check;      | &cross; |
| IC-MBS-10 | First layer     | &check; | Second layer | &cross; | &cross;      | &cross;      | &cross; |
| IC-MBS-11 | Second layer    | &check; | First layer  | &cross; | &cross;      | &cross;      | &cross; |
| IC-MBS-12 | First layer     | &cross; | Second layer | &cross; | First layer  | &check;      | &cross; |
| IC-MBS-13 | Second layer    | &cross; | First layer  | &cross; | Second layer | &check;      | &cross; |
| IC-MBS-14 | First layer     | &check; | Second layer | &cross; | First layer  | &check;      | &cross; |
| IC-MBS-15 | Second layer    | &check; | First layer  | &cross; | Second layer | &check;      | &cross; |
| IC-MBS-16 | First layer     | &cross; | Second layer | &cross; | First layer  | &cross;      | &cross; |
| IC-MBS-17 | Second layer    | &cross; | First layer  | &cross; | Second layer | &cross;      | &cross; |
| IC-MBS-18 | First layer     | &check; | Second layer | &cross; | First layer  | &cross;      | &cross; |
| IC-MBS-19 | Second layer    | &check; | First layer  | &cross; | Second layer | &cross;      | &cross; |

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
As a result, the same unit test can appear in several rows.

<table>
  <tr>
    <td> </td><td colspan="2">Body layer(s) at previous location</td><td colspan="4">Body layer(s) at new location</td><td colspan="3">Configuration</td>
  </tr>
  <tr><td>Test name</td><td>Bottom layer</td><td>Soil</td><td>Bottom layer</td><td>Soil</td><td>Top layer</td><td>Soil</td><td>Avalanche</td><td>Enough space</td><td>Blocked</td></tr>
  <tr><td>IC-MIBS-1</td><td>Second layer</td><td>&cross;</td><td>Second layer</td><td>&cross;</td><td>Second layer</td><td>&check;</td><td>Second layer</td><td>&check;</td><td> &check;</td></tr>

</table>

### `LocateIntersectingCells`

### `MoveIntersectingBody`

## `test_relax.cpp`
