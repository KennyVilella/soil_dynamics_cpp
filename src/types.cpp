/*
Copyright, 2023,  Vilella Kenny.
*/
#include <math.h>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "src/soil_dynamics.hpp"
#include "src/types.hpp"

soil_simulator::grid::grid(
    float grid_size_x, float grid_size_y, float grid_size_z, float size_xy,
    float size_z
) {
    if (size_z <= 0.0)
        throw std::invalid_argument("cell_size_z should be greater than zero");

    if (size_xy <= 0.0)
        throw std::invalid_argument("cell_size_xy should be greater than zero");

    if (grid_size_x <= 0.0)
        throw std::invalid_argument("grid_size_x should be greater than zero");

    if (grid_size_y <= 0.0)
        throw std::invalid_argument("grid_size_y should be greater than zero");

    if (grid_size_z <= 0.0)
        throw std::invalid_argument("grid_size_z should be greater than zero");

    if (size_z > size_xy)
        throw std::invalid_argument("cell_size_z should be lower than or equal"
            "to cell_size_xy");

    if (grid_size_x < size_xy)
        throw std::invalid_argument("cell_size_xy should be lower than or equal"
            " to grid_size_x");

    if (grid_size_y < size_xy)
        throw std::invalid_argument("cell_size_xy should be lower than or equal"
            " to grid_size_y");

    if (grid_size_z < size_z)
        throw std::invalid_argument("cell_size_z should be lower than or equal"
            " to grid_size_z");

    cell_size_xy = size_xy;
    cell_size_z = size_z;

    half_length_x = static_cast<int>(round(grid_size_x / cell_size_xy));
    half_length_y = static_cast<int>(round(grid_size_y / cell_size_xy));
    half_length_z = static_cast<int>(round(grid_size_z / cell_size_z));

    cell_area = cell_size_xy * cell_size_xy;
    cell_volume = cell_area * cell_size_z;

    vect_x.resize(2 * half_length_x + 1);
    vect_y.resize(2 * half_length_y + 1);
    vect_z.resize(2 * half_length_z + 1);

    vect_x[0] = -grid_size_x;
    for (auto ii = 1 ; ii < vect_x.size() ; ii++)
        vect_x[ii] = vect_x[ii-1] + cell_size_xy;

    vect_y[0] = -grid_size_y;
    for (auto ii = 1 ; ii < vect_y.size() ; ii++)
        vect_y[ii] = vect_y[ii-1] + cell_size_xy;

    vect_z[0] = -grid_size_z;
    for (auto ii = 1 ; ii < vect_z.size() ; ii++)
        vect_z[ii] = vect_z[ii-1] + cell_size_z;
}

soil_simulator::bucket::bucket(
    std::vector<float> o_pos, std::vector<float> j_pos, std::vector<float> b_pos,
    std::vector<float> t_pos, float bucket_width
) {
    if (o_pos.size() != 3)
        throw std::invalid_argument("o_pos should be a vector of size 3");

    if (j_pos.size() != 3)
        throw std::invalid_argument("j_pos should be a vector of size 3");

    if (b_pos.size() != 3)
        throw std::invalid_argument("b_pos should be a vector of size 3");

    if (t_pos.size() != 3)
        throw std::invalid_argument("t_pos should be a vector of size 3");

    if (j_pos == b_pos)
        throw std::invalid_argument("j_pos should not be equal to b_pos");

    if (j_pos == t_pos)
        throw std::invalid_argument("j_pos should not be equal to  t_pos");

    if (b_pos == t_pos)
        throw std::invalid_argument("b_pos should not be equal to t_pos");

    if (bucket_width <= 0.0)
        throw std::invalid_argument("bucket_width should be greater than zero");

    width = bucket_width;

    pos.resize(3);
    for (auto ii = 0 ; ii < 3 ; ii++)
        pos[ii] = 0.0;

    ori.resize(4);
    for (auto ii = 0 ; ii < 4 ; ii++)
        ori[ii] = 0.0;

    j_pos_init.resize(3);
    b_pos_init.resize(3);
    t_pos_init.resize(3);

    for (auto ii = 0 ; ii < 3 ; ii++) {
        j_pos_init[ii] = j_pos[ii] - o_pos[ii];
        b_pos_init[ii] = b_pos[ii] - o_pos[ii];
        t_pos_init[ii] = t_pos[ii] - o_pos[ii];
    }
}

soil_simulator::sim_param::sim_param(
    float in_repose_angle, int in_max_iterations, int in_cell_buffer
) {
    if ((in_repose_angle > std::numbers::pi / 2) || (in_repose_angle < 0.0))
        throw std::invalid_argument("repose_angle should be betweem 0.0 and"
            " pi/2");

    if (in_max_iterations < 0.0)
        throw std::invalid_argument("max_iterations should be greater or equal"
            " to zero");

    if (in_cell_buffer < 2.0)
        throw std::invalid_argument("cell_buffer should be greater or equal to 2");

    repose_angle = in_repose_angle;
    max_iterations = in_max_iterations;
    cell_buffer = in_cell_buffer;
}

soil_simulator::sim_out::sim_out(
    grid grid
) {
    equilibrium = false;

    terrain.resize(
        2*grid.half_length_x+1, std::vector<float>(2*grid.half_length_y+1, 0.0));
    body.resize(
        4, std::vector<std::vector<float>>(2*grid.half_length_x+1,
        std::vector<float>(2*grid.half_length_y+1, 0.0)));
    body_soil.resize(
        4, std::vector<std::vector<float>>(2*grid.half_length_x+1,
        std::vector<float>(2*grid.half_length_y+1, 0.0)));

    body_soil_pos.resize(1);

    for (auto ii = 0 ; ii < 2 ; ii++)
        for (auto jj = 0 ; jj < 2 ; jj++) {
            bucket_area[ii][jj] = 0.0;
            relax_area[ii][jj] = 0.0;
            impact_area[ii][jj] = 0.0;
        }
}
