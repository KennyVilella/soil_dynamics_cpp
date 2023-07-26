/*
This file declares the classes used by the simulator to hold the data.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <vector>
#include "src/soil_dynamics.hpp"

namespace soil_simulator {

// Store all parameters related to the simulation grid.
//
// Convention:
// - The simulation grid is centred at 0, that is, if the extent of the grid
//   is 10.0, the grid would then extend from -5.0 to 5.0, this applies to
//   all direction.
// - The grid is composed of regular 3D cells
//
//                H-----------G
//               /           /|
//              /     O     / |
//             /           /  |
//            E-----------F   C
//            |           |  /
//            |           | /
//            |           |/
//            A-----------B
//
// - The cells have the same size in both lateral direction
//
//     AB = BC = CD = DA = EF = FG = GH = HE,
//
//  while their height can potentially be lower
//
//     AE = BF = CG = DH <= AB.
//
// - The center of each cell (O) is considered to be at the center of the top
//   surface.
// - The considered reference frame follows the right-hand rule,
//   with the Z direction pointing upward.
//
// Fields:
// - `half_length_x_`: Number of grid elements in the positive (or negative)
//                     X direction.
// - `half_length_y_`: Number of grid elements in the positive (or negative)
//                     Y direction.
// - `half_length_z_`: Number of grid elements in the positive (or negative)
//                     Z direction.
// - `cell_size_xy_`: Size of the cells in the X and Y direction. [m]
// - `cell_size_z_`: Height of the cells in the Z direction. [m]
// - `cell_area_`: Surface area of one cell in the horizontal plane. [m^2]
// - `cell_volume_`: Volume of one cell. [m^3]
// - `vect_x_`: Vector providing a conversion between cell's index and
//              cell's position in the X direction.
// - `vect_y_`: Vector providing a conversion between cell's index and
//              cell's position in the Y direction.
// - `vect_z_`: Vector providing a conversion between cell's index and
//              cell's position in the Z direction.
class Grid {
 public:
     // Class member
     int half_length_x_;
     int half_length_y_;
     int half_length_z_;
     float cell_size_xy_;
     float cell_size_z_;
     float cell_area_;
     float cell_volume_;
     std::vector<float> vect_x_;
     std::vector<float> vect_y_;
     std::vector<float> vect_z_;

     // Constructor
     Grid(
         float grid_size_x = 4.0, float grid_size_y = 4.0,
         float grid_size_z = 4.0, float cell_size_xy = 0.05,
         float cell_size_z = 0.01);

     // Destructor
     ~Grid() {}
};

// Store all parameters related to a bucket object.
//
// Convention:
// - The bucket is approximated as a triangular prism
//
//                     A ____________________ D
//                    /.                     /|
//                   / .                    / |
//                  /  .                   /  |
//                 /   .                  /   |
//                /    .                 /    |
//               /     .                /     |
//              /      .               /      |
//             /       C . . . . . .  / . . . F
//            /      .               /        ̸
//           /     .                /       ̸
//          /    .                 /      ̸
//         /   .                  /     ̸
//        /  .                   /    ̸
//       / .                    /   ̸
//      B ____________________ E
//
// - The middle of the segment AD is referred to as the bucket joint.
// - The middle of the segment CF is referred to as the bucket base.
// - The middle of the segment BE is referred to as the bucket teeth.
// - The surface ABED is open and referred to as the bucket front.
// - The surface BCFE is a bucket wall and referred to as the bucket base.
// - The surface ACFD is a bucket wall and referred to as the bucket back.
// - The surface ABC is a bucket wall and referred to as the
//   bucket right side.
// - The surface DEF is a bucket wall and referred to as the
//   bucket left side.
// - The bucket has a constant width, denoted as
//
//    AD = BE = CF = `width_`.
//
// - The center of rotation of the bucket is assumed to be at the bucket
//   origin (not shown in the figure) and the bucket vertices are given
//   relative to this origin.
// - The provided coordinates are assumed to be the reference pose of the
//   bucket, from which the bucket pose is calculated throughout the code.
//
// Fields:
// - `j_pos_init_`: Cartesian coordinates of the bucket joint in its
//                  reference pose. [m]
// - `b_pos_init_`: Cartesian coordinates of the bucket base in its
//                  reference pose. [m]
// - `t_pos_init_`: Cartesian coordinates of the bucket teeth in its
//                  reference pose. [m]
// - `width_`: Width of the bucket. [m]
// - `pos_`: Cartesian coordinates of the bucket origin. [m]
// - `ori_`: Orientation of the bucket. [Quaternion]
class Bucket {
 public:
     // Class member
     std::vector<float> j_pos_init_;
     std::vector<float> b_pos_init_;
     std::vector<float> t_pos_init_;
     float width_;
     std::vector<float> pos_;
     std::vector<float> ori_;

     // Constructor
     Bucket(
         std::vector<float> o_pos_init = {0.0, 0.0, 0.0},
         std::vector<float> j_pos_init = {0.0, 0.0, 0.0},
         std::vector<float> b_pos_init = {0.0, 0.0, -0.5},
         std::vector<float> t_pos_init = {0.7, 0.0, -0.5},
         float width = 0.5);

     // Destructor
     ~Bucket() {}
};

// Store all parameters related to the simulation.
//
// Note
// - The value of `max_iterations_` should be carefully selected. A higher
//   number will result in a `terrain_` closer to equilibrium at the end of
//   each time step, but it may impact significantly the performance of the
//   simulator. A value of 3 is suggested.
//
// Fields
// - `repose_angle_`: The repose angle of the considered soil. [rad]
// - `max_iterations_`: The maximum number of relaxation iterations per step.
// - `cell_buffer_`: The number of buffer cells surrounding the bucket and the
//                   relaxed terrain where soil equilibrium is checked.
class SimParam {
 public:
     // Class member
     float repose_angle_;
     int max_iterations_;
     int cell_buffer_;

     // Constructor
     SimParam(
         float repose_angle = 0.85, int max_iterations = 10,
         int cell_buffer = 4);

     // Destructor
     ~SimParam() {}
};

// Store all outputs of the simulation.
//
// Convention
// - The `terrain_` Matrix stores the height of the terrain at each XY
//   position, see the `Grid` class for more details on the simulation grid.
// - The cells where a bucket wall is located is stored in `body_`. At each
//   XY position, the first layer indicates the lowest height where a bucket
//   wall is located while the second layer indicates the maximum height of
//   this bucket wall. If a second bucket wall is located at the same XY
//   position, its minimum and maximum height are indicated in the third and
//   fourth layers, respectively.
// - For each bucket, there can be only two distinct bucket walls located at
//   the same XY position. As a result, the number of layers in the `body_`
//   vector should be equal to four times the number of bucket.
// - Similarly, `body_soil_` stores the location of the soil resting on a bucket
//   wall. The structure of `body_soil_` is identical to `body_`. An additionnal
//   restriction is that the minimum height of the soil resting on the bucket
//   must correspond to the maximum height of a bucket wall.
// - The locations where there is soil resting on the bucket are stored in
//   `body_soil_pos_` as 3-elements vectors. The first element corresponds to
//   the index where the minimum height of the soil is stored, while the second
//   and third element correspond to the index of the X and Y position,
//   respectively.
// - The active areas (`bucket_area_`, `relax_area_` and `impact_area_`) are
//   assumed to be rectangular and to follow the grid geometry. They are thus
//   stored as 2x2 Matrices where:
//   [1, 1] corresponds to the minimum X index.
//   [1, 2] corresponds to the maximum X index.
//   [2, 1] corresponds to the minimum Y index.
//   [2, 2] corresponds to the maximum Y index.
//
// Note:
// - Currently, only one bucket at a time is supported, but this restriction
//   may be removed in the future.
//
// Fields:
// - `equilibrium_`: Indicates whether the terrain is at equilibrium.
// - `terrain_`: Height of the terrain. [m]
// - `body_`: Store the vertical extension of all bucket walls for each
//            XY position. [m]
// - `body_soil_`: Store the vertical extension of all soil resting on a bucket
//                 wall for each XY position. [m]
// - `body_soil_pos_`: Store the indices of locations where there is soil
//                     resting on the bucket.
// - `bucket_area_`: Store the 2D bounding box of the bucket with a buffer
//                   determined by the parameter `cell_buffer_` of `SimParam`.
// - `relax_area_`: Store the 2D bounding box of the area where soil has been
//                  relaxed with a buffer determined by the parameter
//                  `cell_buffer_` of `SimParam`.
// - `impact_area_`: Store the union of `bucket_area_` and `relax_area_`. It
//                   corresponds to the area where the soil equilibrium is
//                   checked.
class SimOut {
 public:
     // Class member
     bool equilibrium_;
     std::vector<std::vector<float>> terrain_;
     std::vector<std::vector<std::vector<float>>> body_;
     std::vector<std::vector<std::vector<float>>> body_soil_;
     std::vector<std::vector<int>> body_soil_pos_;
     int bucket_area_[2][2];
     int relax_area_[2][2];
     int impact_area_[2][2];

     // Constructor
     explicit SimOut(Grid grid);

     // Destructor
    ~SimOut() {}
};
}  // namespace soil_simulator
