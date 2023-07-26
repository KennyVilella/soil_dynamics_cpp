/*
This file implements the functions used to calculate the bucket position.

Copyright, 2023, Vilella Kenny.
*/
#include <algorithm>
#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>
#include "src/bucket_pos.hpp"
#include "src/types.hpp"

void soil_simulator::CalcBucketPos() {
}

void soil_simulator::CalcRectanglePos() {
}

// The position of the rectangle is defined by its edges AB and AD, while the
// specified area extends over [`area_min_x`, `area_min_x + area_length_x`]
// on the X direction and [`area_min_y`, `area_min_y + area_length_y`] on
// the Y direction.
//
// For each cell in the specified area, the function decomposes it into the
// basis formed by the vectors AB and AD. Let O be the name of a cell, it can
// then be decomposed as
//
//   AO = c_ab * AB + c_ad * AD.
//
// This decomposition leads to a system of 2 equations with
// 2 unknowns (c_ab and c_ad)
//
//   AO[1] = c_ab * AB[1] + c_ad * AD[1] {1},
//   AO[2] = c_ab * AB[2] + c_ad * AD[2] {2}.
//
// One may note that AB[1] * {2} - AB[2] * {1} implies that
//
//   AB[1] * AO[2] - AB[2] * AO[1] = c_ad * AD[2] * AB[1] - c_ad * AD[1] * AB[2]
//
// that can be further rewritten as
//
//   c_ad = (AB[1] * AO[2] - AB[2] * AO[1]) / (AD[2] * AB[1] - AD[1] * AB[2]).
//
// Similarly, AD[1] * {2} - AD[2] * {1} implies that
//
//   c_ab = -(AD[1] * AO[2] - AD[2] * AO[1]) / (AD[2] * AB[1] - AD[1] * AB[2]).
//
// This decomposition allows us to determine whether the cell O is inside the
// rectangle area, since this requires c_ab and c_ad to be between 0 and 1.
//
// Note:
// By convention, the decomposition is done at the top right corner of
// each cell.
std::tuple<
    std::vector<std::vector<float>>, std::vector<std::vector<float>>,
    std::vector<std::vector<bool>>, int>
soil_simulator::DecomposeVectorRectangle(
    std::vector<float> ab_ind, std::vector<float> ad_ind,
    std::vector<float> a_ind, int area_min_x, int area_min_y,
    int area_length_x, int area_length_y, float tol
) {
    // Setiing up the outputs
    std::vector<std::vector<float>> c_ab;
    std::vector<std::vector<float>> c_ad;
    std::vector<std::vector<bool>> in_rectangle;
    c_ab.resize(area_length_x, std::vector<float>(area_length_y, 0.0));
    c_ad.resize(area_length_x, std::vector<float>(area_length_y, 0.0));
    in_rectangle.resize(area_length_x, std::vector<bool>(area_length_y, false));

    // Setting constants for decomposing the cell position into the reference
    // basis of the rectangle
    float c_ab_x = ad_ind[1] / (ab_ind[0] * ad_ind[1] - ab_ind[1] * ad_ind[0]);
    float c_ab_y = ad_ind[0] / (ab_ind[0] * ad_ind[1] - ab_ind[1] * ad_ind[0]);
    float c_ad_x = ab_ind[1] / (ab_ind[0] * ad_ind[1] - ab_ind[1] * ad_ind[0]);
    float c_ad_y = ab_ind[0] / (ab_ind[0] * ad_ind[1] - ab_ind[1] * ad_ind[0]);

    // Preparation for the determination of the rectangle position
    // Iterating over the top right corner of all cells in the specified area
    int n_cell = 0;
    for (auto ii_s = 0; ii_s < area_length_x; ii_s++)
        for (auto jj_s = 0; jj_s < area_length_y; jj_s++) {
            // Calculating the indices corresponding to the simulation grid
            float ii = area_min_x + 0.5 + ii_s;
            float jj = area_min_y + 0.5 + jj_s;

            // Decomposing the cell corner position into the basis formed
            // by the rectangle
            c_ab[ii_s][jj_s] = (
                c_ab_x * (ii - a_ind[0]) - c_ab_y * (jj - a_ind[1]));
            c_ad[ii_s][jj_s] = (
                -c_ad_x * (ii - a_ind[0]) + c_ad_y * (jj - a_ind[1]));

            if ((c_ab[ii_s][jj_s] > tol) && (c_ab[ii_s][jj_s] < 1 - tol) &&
                (c_ad[ii_s][jj_s] > tol) && (c_ad[ii_s][jj_s] < 1 - tol)) {
                // Cell is inside the rectangle area
                in_rectangle[ii_s][jj_s] = true;
                n_cell += 4;
            } else {
                // Cell is outside the rectangle area
                in_rectangle[ii_s][jj_s] = false;
            }
        }

    return {c_ab, c_ad, in_rectangle, n_cell};
}

void soil_simulator::CalcTrianglePos() {
}

// The position of the triangle is defined by its edges AB and AC, while the
// specified area extends over [`area_min_x`, `area_min_x + area_length_x`]
// on the X direction and [`area_min_y`, `area_min_y + area_length_y`] on the
// Y direction.
//
// For each cell in the specified area, the function decomposes it into the
// basis formed by the vectors AB and AC. Let O be the name of a cell, it can
// then be decomposed as
//
//   AO = c_ab * AB + c_ac * AC.
//
// This decomposition leads to a system of 2 equations with
// 2 unknowns (c_ab and c_ac)
//
//   AO[1] = c_ab * AB[1] + c_ac * AC[1] {1},
//   AO[2] = c_ab * AB[2] + c_ac * AC[2] {2}.
//
// One may note that AB[1] * {2} - AB[2] * {1} implies that
//
//   AB[1] * AO[2] - AB[2] * AO[1] = c_ac * AC[2] * AB[1] - c_ac * AC[1] * AB[2]
//
// that can be further rewritten as
//
//   c_ac = (AB[1] * AO[2] - AB[2] * AO[1]) / (AC[2] * AB[1] - AC[1] * AB[2]).
//
// Similarly, AC[1] * {2} - AC[2] * {1} implies that
//
//   c_ab = -(AC[1] * AO[2] - AC[2] * AO[1]) / (AC[2] * AB[1] - AC[1] * AB[2]).
//
// This decomposition allows us to determine whether the cell O is inside the
// triangle area, since this requires c_ab and c_ac to be between 0 and 1,
// and the sum of c_ab and c_ac to be lower than 1.
//
// Note:
// By convention, the decomposition is done at the top right corner of
// each cell.
std::tuple<
    std::vector<std::vector<float>>, std::vector<std::vector<float>>,
    std::vector<std::vector<bool>>, int>
soil_simulator::DecomposeVectorTriangle(
    std::vector<float> ab_ind, std::vector<float> ac_ind,
    std::vector<float> a_ind, int area_min_x, int area_min_y,
    int area_length_x, int area_length_y, float tol
) {
    // Setiing up the outputs
    std::vector<std::vector<float>> c_ab;
    std::vector<std::vector<float>> c_ac;
    std::vector<std::vector<bool>> in_triangle;
    c_ab.resize(area_length_x, std::vector<float>(area_length_y, 0.0));
    c_ac.resize(area_length_x, std::vector<float>(area_length_y, 0.0));
    in_triangle.resize(area_length_x, std::vector<bool>(area_length_y, false));

    // Setting constants for decomposing the cell position into the reference
    // basis of the triangle
    float c_ab_x = ac_ind[1] / (ab_ind[0] * ac_ind[1] - ab_ind[1] * ac_ind[0]);
    float c_ab_y = ac_ind[0] / (ab_ind[0] * ac_ind[1] - ab_ind[1] * ac_ind[0]);
    float c_ac_x = ab_ind[1] / (ab_ind[0] * ac_ind[1] - ab_ind[1] * ac_ind[0]);
    float c_ac_y = ab_ind[0] / (ab_ind[0] * ac_ind[1] - ab_ind[1] * ac_ind[0]);

    // Preparation for the determination of the triangle position
    // Iterating over the top right corner of all cells in the specified area
    int n_cell = 0;
    for (auto ii_s = 0; ii_s < area_length_x; ii_s++)
        for (auto jj_s = 0; jj_s < area_length_y; jj_s++) {
            // Calculating the indices corresponding to the simulation grid
            float ii = area_min_x + 0.5 + ii_s;
            float jj = area_min_y + 0.5 + jj_s;

            // Decomposing the cell corner position into the basis formed
            // by the triangle
            c_ab[ii_s][jj_s] = (
                c_ab_x * (ii - a_ind[0]) - c_ab_y * (jj - a_ind[1]));
            c_ac[ii_s][jj_s] = (
                -c_ac_x * (ii - a_ind[0]) + c_ac_y * (jj - a_ind[1]));

            if ((c_ab[ii_s][jj_s] > tol) && (c_ac[ii_s][jj_s] > tol) &&
                (c_ab[ii_s][jj_s] + c_ac[ii_s][jj_s] < 1.0 - tol)) {
                // Cell is inside the triangle area
                in_triangle[ii_s][jj_s] = true;
                n_cell += 4;
            } else {
                // Cell is outside the triangle area
                in_triangle[ii_s][jj_s] = false;
            }
        }

    return {c_ab, c_ac, in_triangle, n_cell};
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
    float c_x = a[0] / grid.cell_size_xy_ + grid.half_length_x_;
    float c_y = a[1] / grid.cell_size_xy_ + grid.half_length_y_;
    float c_z = a[2] / grid.cell_size_z_ + grid.half_length_z_;
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
