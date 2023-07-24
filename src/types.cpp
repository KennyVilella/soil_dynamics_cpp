/*
This file implements the constructor for the classes used by the simulator to
hold the data.
Checks are conducted to verify the integrity and validity of the data.

Copyright, 2023,  Vilella Kenny.
*/
#include <math.h>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "src/soil_dynamics.hpp"
#include "src/types.hpp"

// Create a new instance of `Grid` using the grid size in [m].
// The actual size of the grid would be:
// - [-grid_size_x, grid_size_x] in the X direction.
// - [-grid_size_y, grid_size_y] in the Y direction.
// - [-grid_size_z, grid_size_z] in the Z direction.
//
// Requirements:
// - All inputs should be greater than zero.
// - cell_size_xy should be lower than or equal to grid_size_x and grid_size_y.
// - cell_size_z should be lower than or equal to grid_size_z.
//
// Example:
//     soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
//
// This would create a grid of size [-4, 4] in the X direction, [-4, 4] in
// the Y direction, [-3, 3] in the Z direction, and with cells of size
// 0.05 x 0.05 x 0.01 in the XYZ direction.
soil_simulator::Grid::Grid(
    float grid_size_x, float grid_size_y, float grid_size_z, float cell_size_xy,
    float cell_size_z
) {
    if (cell_size_z <= 0.0)
        throw std::invalid_argument("cell_size_z should be greater than zero");

    if (cell_size_xy <= 0.0)
        throw std::invalid_argument("cell_size_xy should be greater than zero");

    if (grid_size_x <= 0.0)
        throw std::invalid_argument("grid_size_x should be greater than zero");

    if (grid_size_y <= 0.0)
        throw std::invalid_argument("grid_size_y should be greater than zero");

    if (grid_size_z <= 0.0)
        throw std::invalid_argument("grid_size_z should be greater than zero");

    if (cell_size_z > cell_size_xy)
        throw std::invalid_argument("cell_size_z should be lower than or equal"
            "to cell_size_xy");

    if (grid_size_x < cell_size_xy)
        throw std::invalid_argument("cell_size_xy should be lower than or equal"
            " to grid_size_x");

    if (grid_size_y < cell_size_xy)
        throw std::invalid_argument("cell_size_xy should be lower than or equal"
            " to grid_size_y");

    if (grid_size_z < cell_size_z)
        throw std::invalid_argument("cell_size_z should be lower than or equal"
            " to grid_size_z");

    cell_size_xy_ = cell_size_xy;
    cell_size_z_ = cell_size_z;

    half_length_x_ = static_cast<int>(round(grid_size_x / cell_size_xy));
    half_length_y_ = static_cast<int>(round(grid_size_y / cell_size_xy));
    half_length_z_ = static_cast<int>(round(grid_size_z / cell_size_z));

    cell_area_ = cell_size_xy * cell_size_xy;
    cell_volume_ = cell_area_ * cell_size_z;

    vect_x_.resize(2 * half_length_x_ + 1);
    vect_y_.resize(2 * half_length_y_ + 1);
    vect_z_.resize(2 * half_length_z_ + 1);

    vect_x_[0] = -grid_size_x;
    for (auto ii = 1 ; ii < vect_x_.size() ; ii++)
        vect_x_[ii] = vect_x_[ii-1] + cell_size_xy;

    vect_y_[0] = -grid_size_y;
    for (auto ii = 1 ; ii < vect_y_.size() ; ii++)
        vect_y_[ii] = vect_y_[ii-1] + cell_size_xy;

    vect_z_[0] = -grid_size_z;
    for (auto ii = 1 ; ii < vect_z_.size() ; ii++)
        vect_z_[ii] = vect_z_[ii-1] + cell_size_z;
}

// Create a new instance of `Bucket` using the reference positions of the
// bucket origin, joint, base, and teeth as well as the bucket width.
// The position of the bucket joint, base, and teeth are given relative to the
// position of the bucket origin.
//
// Requirements:
// - All provided Cartesian coordinates should be a vector of size 3.
// - The bucket joint, base and teeth should have strictly different location.
// - The bucket width should be greater than zero.
//
// Example:
//    std::vector<float> o_pos = {0.0, 0.0, 0.0};
//    std::vector<float> j_pos = {0.0, 0.0, 0.0};
//    std::vector<float> b_pos = {0.0, 0.0, -0.5};
//    std::vector<float> t_pos = {1.0, 0.0, -0.5};
//
//    soil_simulator::Bucket bucket(o_pos, j_pos, b_pos, t_pos, 0.5);
//
// This would create a bucket ABCDEF with its center of rotation at the
// bucket joint and with
// A = [0.0, -0.25, 0.0], B = [1.0, -0.25, -0.5], C = [0.0, -0.25, -0.5]
// D = [0.0, 0.25, 0.0], E = [1.0, 0.25, -0.5], F = [0.0, 0.25, -0.5].
soil_simulator::Bucket::Bucket(
    std::vector<float> o_pos_init, std::vector<float> j_pos_init,
    std::vector<float> b_pos_init, std::vector<float> t_pos_init,
    float width
) {
    if (o_pos_init.size() != 3)
        throw std::invalid_argument("o_pos_init should be a vector of size 3");

    if (j_pos_init.size() != 3)
        throw std::invalid_argument("j_pos_init should be a vector of size 3");

    if (b_pos_init.size() != 3)
        throw std::invalid_argument("b_pos_init should be a vector of size 3");

    if (t_pos_init.size() != 3)
        throw std::invalid_argument("t_pos_init should be a vector of size 3");

    if (j_pos_init == b_pos_init)
        throw std::invalid_argument("j_pos_init should not be equal to "
            "b_pos_init");

    if (j_pos_init == t_pos_init)
        throw std::invalid_argument("j_pos_init should not be equal to "
            "t_pos_init");

    if (b_pos_init == t_pos_init)
        throw std::invalid_argument("b_pos_init should not be equal to "
            "t_pos_init");

    if (width <= 0.0)
        throw std::invalid_argument("width should be greater than zero");

    width_ = width;

    pos_.resize(3);
    for (auto ii = 0 ; ii < 3 ; ii++)
        pos_[ii] = 0.0;

    ori_.resize(4);
    for (auto ii = 0 ; ii < 4 ; ii++)
        ori_[ii] = 0.0;

    j_pos_init_.resize(3);
    b_pos_init_.resize(3);
    t_pos_init_.resize(3);

    for (auto ii = 0 ; ii < 3 ; ii++) {
        j_pos_init_[ii] = j_pos_init[ii] - o_pos_init[ii];
        b_pos_init_[ii] = b_pos_init[ii] - o_pos_init[ii];
        t_pos_init_[ii] = t_pos_init[ii] - o_pos_init[ii];
    }
}

// Create a new instance of `SimParam`.
//
// Requirements:
// - The `repose_angle` should be between 0.0 and pi / 2. The upper limit may
//   be extended in the future.
// - The `max_iterations` should be greater or equal to zero.
// - The `cell_buffer` should be greater or equal to 2.
//
// Example:
//    soil_simulator::SimParam sim_param(0.85, 5, 4);
soil_simulator::SimParam::SimParam(
    float repose_angle, int max_iterations, int cell_buffer
) {
    if ((repose_angle > std::numbers::pi / 2) || (repose_angle < 0.0))
        throw std::invalid_argument("repose_angle should be betweem 0.0 and"
            " pi/2");

    if (max_iterations < 0.0)
        throw std::invalid_argument("max_iterations should be greater or equal"
            " to zero");

    if (cell_buffer < 2.0)
        throw std::invalid_argument("cell_buffer should be greater or equal to "
            "2");

    repose_angle_ = repose_angle;
    max_iterations_ = max_iterations;
    cell_buffer_ = cell_buffer;
}

// Create a new instance of `SimOut`.
//
// Example:
//    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
//
//    soil_simulator::SimOut sim_out(grid);
//
// This would create a flat terrain located at 0 height.
soil_simulator::SimOut::SimOut(
    Grid grid
) {
    equilibrium_ = false;

    terrain_.resize(
        2*grid.half_length_x_+1,
        std::vector<float>(2*grid.half_length_y_+1, 0.0));
    body_.resize(
        4, std::vector<std::vector<float>>(2*grid.half_length_x_+1,
        std::vector<float>(2*grid.half_length_y_+1, 0.0)));
    body_soil_.resize(
        4, std::vector<std::vector<float>>(2*grid.half_length_x_+1,
        std::vector<float>(2*grid.half_length_y_+1, 0.0)));

    body_soil_pos_.resize(1);

    for (auto ii = 0 ; ii < 2 ; ii++)
        for (auto jj = 0 ; jj < 2 ; jj++) {
            bucket_area_[ii][jj] = 0.0;
            relax_area_[ii][jj] = 0.0;
            impact_area_[ii][jj] = 0.0;
        }
}
