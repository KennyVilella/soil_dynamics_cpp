/*
This file declares the classes used by the simulator to hold the data.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <vector>

namespace soil_simulator {

/// \brief Store information related to the position of the body soil.
struct body_soil{
    /// Index of the body soil layer.
    int ind;

    /// Index of the body soil position in the X direction.
    int ii;

    /// Index of the body soil position in the Y direction.
    int jj;

    /// Cartesian coordinate in the X direction of the body soil in the
    /// reference body frame. [m]
    float x_b;

    /// Cartesian coordinate in the Y direction of the body soil in the
    /// reference body frame. [m]
    float y_b;

    /// Cartesian coordinate in the Z direction of the body soil in the
    /// reference body frame. [m]
    float z_b;

    /// Vertical extent of the soil column. [m]
    float h_soil;
};

/// \brief Store all parameters related to the simulation grid.
///
/// Convention:
/// - The simulation grid is centred at 0, that is, if the extent of the grid
///   is 10.0, the grid would then extend from -5.0 to 5.0, this applies to
///   all direction.
/// - The grid is composed of regular 3D cells
///
///                H-----------G
///               /           /|
///              /     O     / |
///             /           /  |
///            E-----------F   C
///            |           |  /
///            |           | /
///            |           |/
///            A-----------B
///
/// - The cells have the same size in both lateral direction
///
/// \code
///     AB = BC = CD = DA = EF = FG = GH = HE,
/// \endcode
///
///  while their height can potentially be lower
///
/// \code
///     AE = BF = CG = DH <= AB.
/// \endcode
///
/// - The centre of each cell (O) is considered to be at the centre of the top
///   surface.
/// - The considered reference frame follows the right-hand rule,
///   with the Z direction pointing upward.
///
/// Usage:
/// \code
///     soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
/// \endcode
///
/// This would create a grid of size `[-4, 4]` in the X direction, `[-4, 4]` in
/// the Y direction, `[-3, 3]` in the Z direction, and with cells of size
/// `0.05 x 0.05 x 0.01` in the XYZ direction.
class Grid {
 public:
     /// Number of grid elements in the positive (or negative) X direction.
     int half_length_x_;

     /// Number of grid elements in the positive (or negative) Y direction.
     int half_length_y_;

     /// Number of grid elements in the positive (or negative) Z direction.
     int half_length_z_;

     /// Size of the cells in the X and Y direction. [m]
     float cell_size_xy_;

     /// Height of the cells in the Z direction. [m]
     float cell_size_z_;

     /// Surface area of one cell in the horizontal plane. [m^2]
     float cell_area_;

     /// Volume of one cell. [m^3]
     float cell_volume_;

     /// Vector providing a conversion between cell's index and cell's position
     /// in the X direction.
     std::vector<float> vect_x_;

     /// Vector providing a conversion between cell's index and cell's position
     /// in the Y direction.
     std::vector<float> vect_y_;

     /// Vector providing a conversion between cell's index and cell's position
     /// in the Z direction.
     std::vector<double> vect_z_;

     /// \brief Create a new instance of `Grid` using the grid size in [m].
     ///
     /// The actual size of the grid would be:
     /// - [-grid_size_x, grid_size_x] in the X direction.
     /// - [-grid_size_y, grid_size_y] in the Y direction.
     /// - [-grid_size_z, grid_size_z] in the Z direction.
     ///
     /// Requirements:
     /// - All inputs should be greater than zero.
     /// - cell_size_xy should be lower than or equal to grid_size_x and
     ///   grid_size_y.
     /// - cell_size_z should be lower than or equal to grid_size_z.
     ///
     /// \param grid_size_x: Half size of the grid in the X direction. [m]
     /// \param grid_size_y: Half size of the grid in the Y direction. [m]
     /// \param grid_size_z: Half size of the grid in the Z direction. [m]
     /// \param cell_size_xy: Size of the cells in the X and Y direction. [m]
     /// \param cell_size_z: Height of the cells in the Z direction. [m]
     Grid(
         float grid_size_x = 4.0, float grid_size_y = 4.0,
         float grid_size_z = 4.0, float cell_size_xy = 0.05,
         float cell_size_z = 0.01);

     /// \brief Destructor
     ~Grid() {}
};

/// \brief Base class for objects interacting with soil.
///
/// This class is not meant to be used directly. Derived classes should be
/// used instead.
class Body {
 public:
     /// Cartesian coordinates of the body joint in its reference pose. [m]
     std::vector<float> j_pos_init_;

     /// Cartesian coordinates of the body base in its reference pose. [m]
     std::vector<float> b_pos_init_;

     /// Cartesian coordinates of the body teeth in its reference pose. [m]
     std::vector<float> t_pos_init_;

     /// Width of the body. [m]
     float width_;

     /// Cartesian coordinates of the body origin. [m]
     std::vector<float> pos_;

     /// Orientation of the body. [Quaternion]
     std::vector<float> ori_;

 protected:
     /// \brief Constructor
     Body() {}

     /// \brief Destructor
     ~Body() {}
};

/// \brief Store all parameters related to a bucket object.
///
/// Convention:
/// - The bucket is approximated as a triangular prism
///
///                      A ____________________ D
///                     /.                     /|
///                    / .                    / |
///                   /  .                   /  |
///                  /   .                  /   |
///                 /    .                 /    |
///                /     .                /     |
///               /      .               /      |
///              /       C . . . . . .  / . . . F
///             /      .               /        ̸
///            /     .                /       ̸
///           /    .                 /      ̸
///          /   .                  /     ̸
///         /  .                   /    ̸
///        / .                    /   ̸
///       B ____________________ E
///
/// - The middle of the segment AD is referred to as the bucket joint.
/// - The middle of the segment CF is referred to as the bucket base.
/// - The middle of the segment BE is referred to as the bucket teeth.
/// - The surface ABED is open and referred to as the bucket front.
/// - The surface BCFE is a bucket wall and referred to as the bucket base.
/// - The surface ACFD is a bucket wall and referred to as the bucket back.
/// - The surface ABC is a bucket wall and referred to as the
///   bucket right side.
/// - The surface DEF is a bucket wall and referred to as the
///   bucket left side.
/// - The bucket has a constant width, denoted as
///
/// \code
///    AD = BE = CF = `width_`.
/// \endcode
///
/// - The centre of rotation of the bucket is assumed to be at the bucket
///   origin (not shown in the figure) and the bucket vertices are given
///   relative to this origin.
/// - The provided coordinates are assumed to be the reference pose of the
///   bucket, from which the bucket pose is calculated throughout the code.
///
/// Usage:
/// \code
///    std::vector<float> o_pos = {0.0, 0.0, 0.0};
///    std::vector<float> j_pos = {0.0, 0.0, 0.0};
///    std::vector<float> b_pos = {0.0, 0.0, -0.5};
///    std::vector<float> t_pos = {1.0, 0.0, -0.5};
///
///    soil_simulator::Bucket bucket(o_pos, j_pos, b_pos, t_pos, 0.5);
/// \endcode
///
/// This would create a bucket ABCDEF with its centre of rotation at the
/// bucket joint and with:
/// - `A = [0.0, -0.25, 0.0]`
/// - `B = [1.0, -0.25, -0.5]`
/// - `C = [0.0, -0.25, -0.5]`
/// - `D = [0.0, 0.25, 0.0]`,
/// - `E = [1.0, 0.25, -0.5]`
/// - `F = [0.0, 0.25, -0.5]`
class Bucket : public Body {
 public:
     /// \brief Create a new instance of `Bucket` using the reference positions
     ///        of the bucket origin, joint, base, and teeth as well as the
     ///        bucket width.
     ///
     /// The position of the bucket joint, base, and teeth are given relative
     /// to the position of the bucket origin.
     ///
     /// Requirements:
     /// - All provided Cartesian coordinates should be a vector of size 3.
     /// - The bucket joint, base and teeth should have strictly different
     ///   location.
     /// - The bucket width should be greater than zero.
     ///
     /// \param o_pos_init: Cartesian coordinates of the bucket origin in its
     ///                    reference pose. [m]
     /// \param j_pos_init: Cartesian coordinates of the bucket joint in its
     ///                    reference pose. [m]
     /// \param b_pos_init: Cartesian coordinates of the bucket base in its
     ///                    reference pose. [m]
     /// \param t_pos_init: Cartesian coordinates of the bucket teeth in its
     ///                    reference pose. [m]
     /// \param width: Width of the bucket. [m]
     Bucket(
         std::vector<float> o_pos_init = {0.0, 0.0, 0.0},
         std::vector<float> j_pos_init = {0.0, 0.0, 0.0},
         std::vector<float> b_pos_init = {0.0, 0.0, -0.5},
         std::vector<float> t_pos_init = {0.7, 0.0, -0.5},
         float width = 0.5);

     /// \brief Destructor
     ~Bucket() {}
};

/// \brief Store all parameters related to the simulation.
///
/// Note
/// - The value of `max_iterations_` should be carefully selected. A higher
///   number will result in a `terrain_` closer to equilibrium at the end of
///   each time step, but it may impact significantly the performance of the
///   simulator. A value of 3 is suggested.
///
/// Usage:
/// \code
///    soil_simulator::SimParam sim_param(0.85, 5, 4);
/// \endcode
class SimParam {
 public:
     /// The repose angle of the considered soil. [rad]
     float repose_angle_;

     /// The maximum number of relaxation iterations per step.
     int max_iterations_;

     /// The number of buffer cells surrounding the body and the relaxed
     /// terrain where soil equilibrium is checked.
     int cell_buffer_;

     /// \brief Create a new instance of `SimParam`.
     ///
     /// Requirements:
     /// - The `repose_angle` should be between 0.0 and pi / 2. The upper limit
     ///   may be extended in the future.
     /// - The `max_iterations` should be greater or equal to zero.
     /// - The `cell_buffer` should be greater or equal to 2.
     ///
     /// \param repose_angle: The repose angle of the considered soil. [rad]
     /// \param max_iterations: The maximum number of relaxation iterations
     ///                        per step.
     /// \param cell_buffer: The number of buffer cells surrounding the body
     ///                     and the relaxed terrain where soil equilibrium
     ///                     is checked.
     SimParam(
         float repose_angle = 0.85, int max_iterations = 10,
         int cell_buffer = 4);

     /// \brief Destructor
     ~SimParam() {}
};

/// \brief Store all outputs of the simulation.
///
/// Convention
/// - The `terrain_` Matrix stores the height of the terrain at each XY
///   position, see the `Grid` class for more details on the simulation grid.
/// - The cells where a body wall is located is stored in `body_`. At each
///   XY position, the first layer indicates the lowest height where a body
///   wall is located while the second layer indicates the maximum height of
///   this body wall. If a second body wall is located at the same XY
///   position, its minimum and maximum height are indicated in the third and
///   fourth layers, respectively.
/// - For each body, there can be only two distinct body walls located at
///   the same XY position. As a result, the number of layers in the `body_`
///   vector should be equal to four times the number of body.
/// - Similarly, `body_soil_` stores the location of the soil resting on a
///   body wall. The structure of `body_soil_` is identical to `body_`. An
///   additional restriction is that the minimum height of the soil resting on
///   the body must correspond to the maximum height of a body wall.
/// - The locations where there is soil resting on the body are stored in
///   `body_soil_pos_` as a vector of `body_soil` struct.
/// - The active areas (`body_area_`, `relax_area_` and `impact_area_`) are
///   assumed to be rectangular and to follow the grid geometry. They are thus
///   stored as 2x2 Matrices where:
///   [0, 0] corresponds to the minimum X index.
///   [0, 1] corresponds to the maximum X index.
///   [1, 0] corresponds to the minimum Y index.
///   [1, 1] corresponds to the maximum Y index.
///
/// Note:
/// - Currently, only one body at a time is supported, but this restriction
///   may be removed in the future.
///
/// Usage:
/// \code
///    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
///
///    soil_simulator::SimOut sim_out(grid);
/// \endcode
///
/// This would create a flat terrain located at 0 height.
class SimOut {
 public:
     /// Indicates whether the terrain is at equilibrium.
     bool equilibrium_;

     /// Height of the terrain. [m]
     std::vector<std::vector<float>> terrain_;

     /// Store the vertical extension of all body walls for each XY
     /// position. [m]
     std::vector<std::vector<std::vector<float>>> body_;

     /// Store the vertical extension of all soil resting on a body wall for
     /// each XY position. [m]
     std::vector<std::vector<std::vector<float>>> body_soil_;

     /// Store the information related to the soil resting on the body.
     std::vector<body_soil> body_soil_pos_;

     /// Store the 2D bounding box of the body with a buffer determined
     /// by the parameter `cell_buffer_` of `SimParam`.
     int body_area_[2][2];

     /// Store the 2D bounding box of the area where soil has been relaxed with
     /// a buffer determined by the parameter `cell_buffer_` of `SimParam`.
     int relax_area_[2][2];

     /// Store the union of `body_area_` and `relax_area_`. It corresponds
     /// to the area where the soil equilibrium is checked.
     int impact_area_[2][2];

     /// \brief Create a new instance of `SimOut`.
     ///
     /// \param grid: Class that stores information related to the simulation
     ///              grid.
     explicit SimOut(Grid grid = Grid());

     /// \brief Destructor
    ~SimOut() {}
};

}  // namespace soil_simulator
