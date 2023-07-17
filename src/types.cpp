/*

*/
#include "soil_dynamics.hpp"
#include "types.hpp"
#include <math.h>
#include <vector>
#include <stdexcept>

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
}
