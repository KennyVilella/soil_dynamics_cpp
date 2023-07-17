/*

*/
#include "soil_dynamics.hpp"
#include "types.hpp"
#include <math.h>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace soil_simulator;

grid::grid(
    float grid_size_x, float grid_size_y, float grid_size_z, float cell_size_xy, float cell_size_z
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
        throw std::invalid_argument("cell_size_z should be lower than or equal to cell_size_xy");

    if (grid_size_x < cell_size_xy)
        throw std::invalid_argument("cell_size_xy should be lower than or equal to grid_size_x");

    if (grid_size_y < cell_size_xy)
        throw std::invalid_argument("cell_size_xy should be lower than or equal to grid_size_y");

    if (grid_size_z < cell_size_z)
        throw std::invalid_argument("cell_size_z should be lower than or equal to grid_size_z");

    half_length_x = (int)round(grid_size_x / cell_size_xy);
    half_length_y = (int)round(grid_size_y / cell_size_xy);
    half_length_z = (int)round(grid_size_z / cell_size_z);

    cell_area = cell_size_xy * cell_size_xy;
    cell_volume = cell_area * cell_size_z;

    std::vector<float> vect_x(2 * half_length_x + 1);
    std::vector<float> vect_y(2 * half_length_y + 1);
    std::vector<float> vect_z(2 * half_length_z + 1);

    vect_x[0] = -half_length_x;
    for (auto ii = 1 ; ii <  vect_x.size() ; ii++)
        vect_x[ii] = vect_x[ii-1] + 1;

    vect_y[0] = -half_length_y;
    for (auto ii = 1 ; ii <  vect_y.size() ; ii++)
        vect_y[ii] = vect_y[ii-1] + 1;

    vect_z[0] = -half_length_z;
    for (auto ii = 1 ; ii <  vect_z.size() ; ii++)
        vect_z[ii] = vect_z[ii-1] + 1;
};

bucket::bucket(
    std::vector<float> o_pos_init, std::vector<float> j_pos_init, std::vector<float> b_pos_init,
    std::vector<float> t_pos_init, float width
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
        throw std::invalid_argument("j_pos_init should not be equal to b_pos_init");

    if (j_pos_init == t_pos_init)
        throw std::invalid_argument("j_pos_init should not be equal to t_pos_init");

    if (b_pos_init == t_pos_init)
        throw std::invalid_argument("b_pos_init should not be equal to t_pos_init");

    if (width <= 0.0)
        throw std::invalid_argument("width should be greater than zero");

    std::vector<float> pos(3, 0.0);
    std::vector<float> ori(4, 0.0);

    for (auto ii = 0 ; ii <  3 ; ii++)
    {
        j_pos_init[ii] -= o_pos_init[ii];
        b_pos_init[ii] -= o_pos_init[ii];
        t_pos_init[ii] -= o_pos_init[ii];
    }
};

sim_param::sim_param(
    float repose_angle, int max_iterations, int cell_buffer
) {

    if ((repose_angle > std::numbers::pi / 2) || (repose_angle < 0.0))
        throw std::invalid_argument("repose_angle should be betweem 0.0 and pi/2");

    if (max_iterations < 0.0)
        throw std::invalid_argument("max_iterations should be greater or equal to zero");

    if (cell_buffer < 2.0)
        std::cout << "cell_buffer too low, setting to 2";
        cell_buffer = 2;
};

sim_out::sim_out(
    int terrain, grid grid
) {

    if (std::extent_v<decltype(terrain), 0> != 2 * grid.half_length_x + 1)
        throw std::invalid_argument("Dimension of terrain in X does not match with the grid size");

    if (std::extent_v<decltype(terrain), 1> != 2 * grid.half_length_y + 1)
        throw std::invalid_argument("Dimension of terrain in Y does not match with the grid size");

    bool equilibrium = false;

    float body[4][2*grid.half_length_x+1][2*grid.half_length_x+1] = {0.0};
    float body_soil[4][2*grid.half_length_x+1][2*grid.half_length_x+1] = {0.0};

    std::vector<std::vector<int>> body_soil_pos[1];

    int bucket_area[2][2] = {0};
    int relax_area[2][2] = {0};
    int impact_area[2][2] = {0};
};
