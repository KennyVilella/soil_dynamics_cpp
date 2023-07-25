/*
This file implements the functions used to calculate the bucket position.

Copyright, 2023, Vilella Kenny.
*/
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include "src/bucket_pos.hpp"
#include "src/types.hpp"

void soil_simulator::CalcBucketPos() {
}

void soil_simulator::CalcRectanglePos() {
}

void soil_simulator::DecomposeVectorRectangle() {
}

void soil_simulator::CalcTrianglePos() {
}

void soil_simulator::DecomposeVectorTriangle() {
}

// For the sake of accuracy, the line is divided into smaller segments using a
// spatial increment `delta`.
//
// The coordinates of each sub-point (ab_i) along the line can then be
// calculated as
//
//    ab_i = a + ab * i * delta / norm(ab)
//
// where i is the increment number and ab = b - a.
// The Cartesian coordinates can then be converted into indices
//
//    ab_i_ind = ab_i / cell_size + grid_half_length + 1
//
// Finally, the floating-point values are rounded to obtain the cell indices in
// the X, Y, Z directions.
// As the center of each cell is considered to be on the center of the top
// surface, `round` should be used for getting the cell indices in the X and Y
// direction, while `ceil` should be used for the Z direction.
//
// Note:
// When the line follows a cell border, the exact location of the line becomes
// ambiguous. It is assumed that the caller resolves this ambiguity.
std::vector<std::vector<int>> soil_simulator::CalcLinePos(
    std::vector<float> a, std::vector<float> b, float delta,
    Grid grid
) {
    // Vector ab
    std::vector<float> ab(3);
    for (auto ii = 0; ii < 3; ii++)
        ab[ii] = b[ii] - a[ii];

    // Creating unit vector
    float norm_ab = std::sqrt(ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2]);
    int nn = std::max(2, static_cast<int>(round(norm_ab / delta) + 1));
    std::vector<float> unit_vect(nn);
    for (auto ii = 0; ii < nn; ii++)
        unit_vect[ii] = ii * 1.0 / (nn - 1);

    // Setting constants used for the vectorial decomposition
    float c_x = a[0] / grid.cell_size_xy_ + grid.half_length_x_ + 1;
    float c_y = a[1] / grid.cell_size_xy_ + grid.half_length_y_ + 1;
    float c_z = a[2] / grid.cell_size_z_ + grid.half_length_z_ + 1;
    float d_x = ab[0] / grid.cell_size_xy_;
    float d_y = ab[1] / grid.cell_size_xy_;
    float d_z = ab[2] / grid.cell_size_z_;

    // Determining the cells where the line is located
    std::vector<std::vector<int>> line_pos;
    line_pos.resize(nn, std::vector<int>(3, 0));
    for (auto ii = 0; ii < nn; ii++) {
        line_pos[ii][0] = static_cast<int>(
            round(c_x + d_x * unit_vect[ii]));
        line_pos[ii][1] = static_cast<int>(
            round(c_y + d_y * unit_vect[ii]));
        line_pos[ii][2] = static_cast<int>(
            std::ceil(c_z + d_z * unit_vect[ii]));
    }

    return line_pos;
}

void soil_simulator::UpdateBody() {
}

void soil_simulator::IncludeNewBodyPos() {
}
