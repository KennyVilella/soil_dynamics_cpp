/*
This file implements the functions used to calculate the bucket position.

Copyright, 2023, Vilella Kenny.
*/
#include <cmath>
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

void soil_simulator::CalcLinePos(
    std::vector<float> a, std::vector<float> b, float delta,
    Grid grid
) {

    // Vector ab
    std::vector<float> ab(3);
    for (auto ii = 0; ii < 3; ii++)
        ab[ii] = b[ii] - a[ii];

    // Creating unit vector
    float norm_ab = std::sqrt(ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2]);
    int nn = std::max(2, static_cast<int>(round(norm_ab / delta + 1)));
    std::vector<float> unit_vect(nn);
    for (auto ii = 0; ii < nn; ii++)
        unit_vect[ii] = ii * delta;

    // Setting constants used for the vectorial decomposition
    float c_x = a[0] / grid.cell_size_xy_ + grid.half_length_x_ + 1;
    float c_y = a[1] / grid.cell_size_xy_ + grid.half_length_y_ + 1;
    float c_z = a[2] / grid.cell_size_z_ + grid.half_length_z_ + 1;
    float d_x = ab[0] / grid.cell_size_xy_;
    float d_y = ab[1] / grid.cell_size_xy_;
    float d_z = ab[2] / grid.cell_size_z_;

    // Determining the cells where the line is located
    std::vector<std::vector<float>> line_pos;
    line_pos.resize(3, std::vector<float>(nn, 0.0));
    for (auto ii = 0; ii < nn; ii++) {
        line_pos[ii][0] = static_cast<int>(
            round(c_x + d_x * unit_vect[ii]));
        line_pos[ii][1] = static_cast<int>(
            round(c_y + d_y * unit_vect[ii]));
        line_pos[ii][2] = static_cast<int>(
            std::ceil(c_z + d_z * unit_vect[ii]));
/*
    return line_pos;
*/
}

void soil_simulator::UpdateBody() {
}

void soil_simulator::IncludeNewBodyPos() {
}
