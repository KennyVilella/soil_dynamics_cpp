/*
This file implements the functions used to calculate the bucket position.

Copyright, 2023, Vilella Kenny.
*/
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <vector>
#include "src/bucket_pos.hpp"
#include "src/types.hpp"
#include "src/utils.hpp"

void soil_simulator::CalcBucketPos(
    SimOut* sim_out, std::vector<float> pos, std::vector<float> ori, Grid grid,
    Bucket bucket, SimParam sim_param, float tol
) {
    // Calculating position of the bucker vertices
    auto j_pos = soil_simulator::CalcRotationQuaternion(
        ori, bucket.j_pos_init_);
    auto b_pos = soil_simulator::CalcRotationQuaternion(
        ori, bucket.b_pos_init_);
    auto t_pos = soil_simulator::CalcRotationQuaternion(
        ori, bucket.t_pos_init_);

    // Unit vector normal to the side of the bucket
    auto normal_side = soil_simulator::CalcNormal(j_pos, b_pos, t_pos);

    // Declaring vectors for each vertex of the bucket
    std::vector<float> j_r_pos(3);
    std::vector<float> j_l_pos(3);
    std::vector<float> b_r_pos(3);
    std::vector<float> b_l_pos(3);
    std::vector<float> t_r_pos(3);
    std::vector<float> t_l_pos(3);

    for (auto ii = 0; ii < 3; ii++) {
        // Adding position of the bucket origin
        j_pos[ii] += pos[ii];
        b_pos[ii] += pos[ii];
        t_pos[ii] += pos[ii];

        // Position of each vertex of the bucket
        j_r_pos[ii] = j_pos[ii] + 0.5 * bucket.width_ * normal_side[ii];
        j_l_pos[ii] = j_pos[ii] - 0.5 * bucket.width_ * normal_side[ii];
        b_r_pos[ii] = b_pos[ii] + 0.5 * bucket.width_ * normal_side[ii];
        b_l_pos[ii] = b_pos[ii] - 0.5 * bucket.width_ * normal_side[ii];
        t_r_pos[ii] = t_pos[ii] + 0.5 * bucket.width_ * normal_side[ii];
        t_l_pos[ii] = t_pos[ii] - 0.5 * bucket.width_ * normal_side[ii];

        // Adding a small increment to all vertices
        // This is to account for the edge case where one of the vertex is at
        // cell border. In that case, the increment would remove any ambiguity
        j_r_pos[ii] += tol * (
            (j_l_pos[ii] - j_r_pos[ii]) + (b_r_pos[ii] - j_r_pos[ii]) +
            (t_r_pos[ii] - j_r_pos[ii]));
        j_l_pos[ii] += tol * (
            (j_r_pos[ii] - j_l_pos[ii]) + (b_l_pos[ii] - j_l_pos[ii]) +
            (t_l_pos[ii] - j_l_pos[ii]));
        b_r_pos[ii] += tol * (
            (b_l_pos[ii] - b_r_pos[ii]) + (j_r_pos[ii] - b_r_pos[ii]) +
            (t_r_pos[ii] - b_r_pos[ii]));
        b_l_pos[ii] += tol * (
            (b_r_pos[ii] - b_l_pos[ii]) + (j_l_pos[ii] - b_l_pos[ii]) +
            (t_l_pos[ii] - b_l_pos[ii]));
        t_r_pos[ii] += tol * (
            (t_l_pos[ii] - t_r_pos[ii]) + (j_r_pos[ii] - t_r_pos[ii]) +
            (b_r_pos[ii] - t_r_pos[ii]));
        t_l_pos[ii] += tol * (
            (t_r_pos[ii] - t_l_pos[ii]) + (j_l_pos[ii] - t_l_pos[ii]) +
            (b_l_pos[ii] - t_l_pos[ii]));
    }

    // Calculating the 2D bounding box of the bucket
    float bucket_x_min = std::min({
        j_r_pos[0], j_l_pos[0], b_r_pos[0], b_l_pos[0], t_r_pos[0], t_l_pos[0]
    });
    float bucket_x_max = std::max({
        j_r_pos[0], j_l_pos[0], b_r_pos[0], b_l_pos[0], t_r_pos[0], t_l_pos[0]
    });
    float bucket_y_min = std::min({
        j_r_pos[1], j_l_pos[1], b_r_pos[1], b_l_pos[1], t_r_pos[1], t_l_pos[1]
    });
    float bucket_y_max = std::max({
        j_r_pos[1], j_l_pos[1], b_r_pos[1], b_l_pos[1], t_r_pos[1], t_l_pos[1]
    });

    // Updating bucket_area
    sim_out->bucket_area_[0][0] = static_cast<int>(std::max(
        round(bucket_x_min / grid.cell_size_xy_ +
            grid.half_length_x_ - sim_param.cell_buffer_)
        , 1.0));
    sim_out->bucket_area_[0][1] = static_cast<int>(std::min(
        round(bucket_x_max / grid.cell_size_xy_ +
            grid.half_length_x_ + sim_param.cell_buffer_)
        , 2.0 * grid.half_length_x_));
    sim_out->bucket_area_[1][0] = static_cast<int>(std::max(
        round(bucket_y_min / grid.cell_size_xy_ +
            grid.half_length_y_ - sim_param.cell_buffer_)
        , 1.0));
    sim_out->bucket_area_[1][1] = static_cast<int>(std::min(
        round(bucket_y_max / grid.cell_size_xy_ +
            grid.half_length_y_ + sim_param.cell_buffer_)
        , 2.0 * grid.half_length_y_));

    // Determining where each surface of the bucket is located
    auto base_pos = soil_simulator::CalcRectanglePos(
        b_r_pos, b_l_pos, t_l_pos, t_r_pos, 0.5 * grid.cell_size_z_, grid, tol);
    auto back_pos = soil_simulator::CalcRectanglePos(
        b_r_pos, b_l_pos, j_l_pos, j_r_pos, 0.5 * grid.cell_size_z_, grid, tol);
    auto right_side_pos = soil_simulator::CalcTrianglePos(
        j_r_pos, b_r_pos, t_r_pos, 0.5 * grid.cell_size_z_, grid, tol);
    auto left_side_pos = soil_simulator::CalcTrianglePos(
        j_l_pos, b_l_pos, t_l_pos, 0.5 * grid.cell_size_z_, grid, tol);

    // Sorting all list of cells indices where the bucket is located
    sort(base_pos.begin(), base_pos.end());
    sort(back_pos.begin(), back_pos.end());
    sort(right_side_pos.begin(), right_side_pos.end());
    sort(left_side_pos.begin(), left_side_pos.end());

    // Reinitializing bucket position
    for (auto ii = 0 ; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0 ; jj < sim_out->body_[0].size(); jj++)
            std::fill(
                sim_out->body_[ii][jj].begin(), sim_out->body_[ii][jj].end(),
                0.0);

    // Updating the bucket position
    soil_simulator::UpdateBody(base_pos, sim_out, grid, tol);
    soil_simulator::UpdateBody(back_pos, sim_out, grid, tol);
    soil_simulator::UpdateBody(right_side_pos, sim_out, grid, tol);
    soil_simulator::UpdateBody(left_side_pos, sim_out, grid, tol);
}

// The rectangle is defined by providing the Cartesian coordinates of its four
// vertices in the proper order.
//
// To optimize performance, the function iterates over a portion of the
// horizontal grid where the rectangle is located. For each cell, the function
// calculates the height of the plane formed by the rectangle at the top right
// corner of the cell. If the cell is within the rectangle area, the calcualted
// height is added to the results for the four neighboring cells.
//
// This method works because when a plane intersects with a rectangular cell,
// the minimum and maximum height of the plane within the cell occurs at one of
// the cell corners. By iterating through all the cells, the function ensures
// that all the corners of each cell are investigated.
//
// However, this approach does not work when the rectangle is perpendicular to
// the XY plane. To handle this case, the function uses the `CalcLinePos`
// function to include the cells that lie on the four edges of the rectangle.
//
// Note:
// - The iteration is performed over the top right corner of each cell,
//   but any other corner could have been chosen without affecting the results.
// - Not all cells are provided, since, at a given XY position, only the cells
//   with the minimum and maximum height are important.
// - When the rectangle follows a cell border, the exact location of the
//   rectangle becomes ambiguous. It is assumed that the caller resolves
//   this ambiguity.
std::vector<std::vector<int>> soil_simulator::CalcRectanglePos(
    std::vector<float> a, std::vector<float> b, std::vector<float> c,
    std::vector<float> d, float delta, Grid grid, float tol
) {
    // Converting the four rectangle vertices from position to indices
    std::vector<float> a_ind(3, 0);
    std::vector<float> b_ind(3, 0);
    std::vector<float> c_ind(3, 0);
    std::vector<float> d_ind(3, 0);
    a_ind[0] = a[0] / grid.cell_size_xy_ + grid.half_length_x_;
    b_ind[0] = b[0] / grid.cell_size_xy_ + grid.half_length_x_;
    c_ind[0] = c[0] / grid.cell_size_xy_ + grid.half_length_x_;
    d_ind[0] = d[0] / grid.cell_size_xy_ + grid.half_length_x_;
    a_ind[1] = a[1] / grid.cell_size_xy_ + grid.half_length_y_;
    b_ind[1] = b[1] / grid.cell_size_xy_ + grid.half_length_y_;
    c_ind[1] = c[1] / grid.cell_size_xy_ + grid.half_length_y_;
    d_ind[1] = d[1] / grid.cell_size_xy_ + grid.half_length_y_;
    a_ind[2] = a[2] / grid.cell_size_z_ + grid.half_length_z_;
    b_ind[2] = b[2] / grid.cell_size_z_ + grid.half_length_z_;
    c_ind[2] = c[2] / grid.cell_size_z_ + grid.half_length_z_;
    d_ind[2] = d[2] / grid.cell_size_z_ + grid.half_length_z_;

    // Calculating the bounding box of the rectangle
    int area_min_x = static_cast<int>(std::floor(
        std::min({a_ind[0], b_ind[0], c_ind[0], d_ind[0]})));
    int area_max_x = static_cast<int>(std::ceil(
        std::max({a_ind[0], b_ind[0], c_ind[0], d_ind[0]})));
    int area_min_y = static_cast<int>(std::floor(
        std::min({a_ind[1], b_ind[1], c_ind[1], d_ind[1]})));
    int area_max_y = static_cast<int>(std::ceil(
        std::max({a_ind[1], b_ind[1], c_ind[1], d_ind[1]})));

    // Calculating the lateral extent of the bounding box
    int area_length_x = area_max_x - area_min_x;
    int area_length_y = area_max_y - area_min_y;

    // Calculating the basis formed by the rectangle
    std::vector<float> ab(3, 0.0);
    std::vector<float> ad(3, 0.0);
    std::vector<float> ab_ind(3, 0);
    std::vector<float> ad_ind(3, 0);
    for (auto ii = 0; ii < 3; ii++) {
       ab[ii] = b[ii] - a[ii];
       ad[ii] = d[ii] - a[ii];
    }
    ab_ind[0] = ab[0] / grid.cell_size_xy_;
    ad_ind[0] = ad[0] / grid.cell_size_xy_;
    ab_ind[1] = ab[1] / grid.cell_size_xy_;
    ad_ind[1] = ad[1] / grid.cell_size_xy_;
    ab_ind[2] = ab[2] / grid.cell_size_z_;
    ad_ind[2] = ad[2] / grid.cell_size_z_;

    // Listing the cells inside the rectangle area
    auto [c_ab, c_ad, in_rectangle, n_cell] =
        soil_simulator::DecomposeVectorRectangle(
            ab_ind, ad_ind, a_ind, area_min_x, area_min_y, area_length_x,
            area_length_y, tol);

    // Determining cells where inner portion of the rectangle area is located
    std::vector<std::vector<int>> rect_pos;
    rect_pos.resize(n_cell, std::vector<int>(3, 0));
    int nn_cell = 0;
    for (auto ii = area_min_x; ii < area_max_x; ii++)
        for (auto jj = area_min_y; jj < area_max_y; jj++) {
            // Calculating corresponding indices
            int ii_s = ii - area_min_x;
            int jj_s = jj - area_min_y;

            if (in_rectangle[ii_s][jj_s] == true) {
                // Cell is inside the rectangle area
                // Calculating the height index of the rectangle at this corner
                int kk = static_cast<int>(std::ceil(
                    a_ind[2] + c_ab[ii_s][jj_s] * ab_ind[2] +
                    c_ad[ii_s][jj_s] * ad_ind[2]));

                // Adding the four neighboring cells with the calculated height
                rect_pos[nn_cell][0] = ii;
                rect_pos[nn_cell][1] = jj;
                rect_pos[nn_cell][2] = kk;
                rect_pos[nn_cell + 1][0] = ii + 1;
                rect_pos[nn_cell + 1][1] = jj;
                rect_pos[nn_cell + 1][2] = kk;
                rect_pos[nn_cell + 2][0] = ii;
                rect_pos[nn_cell + 2][1] = jj + 1;
                rect_pos[nn_cell + 2][2] = kk;
                rect_pos[nn_cell + 3][0] = ii + 1;
                rect_pos[nn_cell + 3][1] = jj + 1;
                rect_pos[nn_cell + 3][2] = kk;

                // Incrementing the index
                nn_cell += 4;
            }
        }

    // Determining the cells where the four edges of the rectangle are located
    auto ab_pos = soil_simulator::CalcLinePos(a, b, delta, grid);
    auto bc_pos = soil_simulator::CalcLinePos(b, c, delta, grid);
    auto cd_pos = soil_simulator::CalcLinePos(c, d, delta, grid);
    auto da_pos = soil_simulator::CalcLinePos(d, a, delta, grid);

    // Concatenating all cells in rect_pos
    rect_pos.insert(rect_pos.end(), ab_pos.begin(), ab_pos.end());
    rect_pos.insert(rect_pos.end(), bc_pos.begin(), bc_pos.end());
    rect_pos.insert(rect_pos.end(), cd_pos.begin(), cd_pos.end());
    rect_pos.insert(rect_pos.end(), da_pos.begin(), da_pos.end());

    return rect_pos;
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

// The triangle is defined by providing the Cartesian coordinates of its three
// vertices in the proper order.
//
// To optimize performance, the function iterates over a portion of the
// horizontal grid where the triangle is located. For each cell, the function
// calculates the height of the plane formed by the triangle at the top right
// corner of the cell. If the cell is within the triangle area, the calcualted
// height is added to the results for the four neighboring cells.
//
// This method works because when a plane intersects with a rectangular cell,
// the minimum and maximum height of the plane within the cell occurs at one of
// the cell corners. By iterating through all the cells, the function ensures
// that all the corners of each cell are investigated.
//
// However, this approach does not work when the triangle is perpendicular to
// the XY plane. To handle this case, the function uses the `CalcLinePos`
// function to include the cells that lie on the three edges of the triangle.
//
// Note:
// - The iteration is performed over the top right corner of each cell,
//   but any other corner could have been chosen without affecting the results.
// - Not all cells are provided, since, at a given XY position, only the cells
//   with the minimum and maximum height are important.
// - When the triangle follows a cell border, the exact location of the
//   triangle becomes ambiguous. It is assumed that the caller resolves
//   this ambiguity.
std::vector<std::vector<int>> soil_simulator::CalcTrianglePos(
    std::vector<float> a, std::vector<float> b, std::vector<float> c,
    float delta, Grid grid, float tol
) {
    // Converting the three triangle vertices from position to indices
    std::vector<float> a_ind(3, 0);
    std::vector<float> b_ind(3, 0);
    std::vector<float> c_ind(3, 0);
    a_ind[0] = a[0] / grid.cell_size_xy_ + grid.half_length_x_;
    b_ind[0] = b[0] / grid.cell_size_xy_ + grid.half_length_x_;
    c_ind[0] = c[0] / grid.cell_size_xy_ + grid.half_length_x_;
    a_ind[1] = a[1] / grid.cell_size_xy_ + grid.half_length_y_;
    b_ind[1] = b[1] / grid.cell_size_xy_ + grid.half_length_y_;
    c_ind[1] = c[1] / grid.cell_size_xy_ + grid.half_length_y_;
    a_ind[2] = a[2] / grid.cell_size_z_ + grid.half_length_z_;
    b_ind[2] = b[2] / grid.cell_size_z_ + grid.half_length_z_;
    c_ind[2] = c[2] / grid.cell_size_z_ + grid.half_length_z_;

    // Calculating the bounding box of the triangle
    int area_min_x = static_cast<int>(std::floor(
        std::min({a_ind[0], b_ind[0], c_ind[0]})));
    int area_max_x = static_cast<int>(std::ceil(
        std::max({a_ind[0], b_ind[0], c_ind[0]})));
    int area_min_y = static_cast<int>(std::floor(
        std::min({a_ind[1], b_ind[1], c_ind[1]})));
    int area_max_y = static_cast<int>(std::ceil(
        std::max({a_ind[1], b_ind[1], c_ind[1]})));

    // Calculating the lateral extent of the bounding box
    int area_length_x = area_max_x - area_min_x;
    int area_length_y = area_max_y - area_min_y;

    // Calculating the basis formed by the triangle
    std::vector<float> ab(3, 0.0);
    std::vector<float> ac(3, 0.0);
    std::vector<float> ab_ind(3, 0);
    std::vector<float> ac_ind(3, 0);
    for (auto ii = 0; ii < 3; ii++) {
       ab[ii] = b[ii] - a[ii];
       ac[ii] = c[ii] - a[ii];
    }
    ab_ind[0] = ab[0] / grid.cell_size_xy_;
    ac_ind[0] = ac[0] / grid.cell_size_xy_;
    ab_ind[1] = ab[1] / grid.cell_size_xy_;
    ac_ind[1] = ac[1] / grid.cell_size_xy_;
    ab_ind[2] = ab[2] / grid.cell_size_z_;
    ac_ind[2] = ac[2] / grid.cell_size_z_;

    // Listing the cells inside the triangle area
    auto [c_ab, c_ac, in_triangle, n_cell] =
        soil_simulator::DecomposeVectorTriangle(
            ab_ind, ac_ind, a_ind, area_min_x, area_min_y, area_length_x,
            area_length_y, tol);

    // Determining cells where inner portion of the triangle area is located
    std::vector<std::vector<int>> tri_pos;
    tri_pos.resize(n_cell, std::vector<int>(3, 0));
    int nn_cell = 0;
    for (auto ii = area_min_x; ii < area_max_x; ii++)
        for (auto jj = area_min_y; jj < area_max_y; jj++) {
            // Calculating corresponding indices
            int ii_s = ii - area_min_x;
            int jj_s = jj - area_min_y;

            if (in_triangle[ii_s][jj_s] == true) {
                // Cell is inside the triangle area
                // Calculating the height index of the triangle at this corner
                int kk = static_cast<int>(std::ceil(
                    a_ind[2] + c_ab[ii_s][jj_s] * ab_ind[2] +
                    c_ac[ii_s][jj_s] * ac_ind[2]));

                // Adding the four neighboring cells with the calculated height
                tri_pos[nn_cell][0] = ii;
                tri_pos[nn_cell][1] = jj;
                tri_pos[nn_cell][2] = kk;
                tri_pos[nn_cell + 1][0] = ii + 1;
                tri_pos[nn_cell + 1][1] = jj;
                tri_pos[nn_cell + 1][2] = kk;
                tri_pos[nn_cell + 2][0] = ii;
                tri_pos[nn_cell + 2][1] = jj + 1;
                tri_pos[nn_cell + 2][2] = kk;
                tri_pos[nn_cell + 3][0] = ii + 1;
                tri_pos[nn_cell + 3][1] = jj + 1;
                tri_pos[nn_cell + 3][2] = kk;

                // Incrementing the index
                nn_cell += 4;
            }
        }

    // Determining the cells where the three edges of the triangle are located
    auto ab_pos = soil_simulator::CalcLinePos(a, b, delta, grid);
    auto bc_pos = soil_simulator::CalcLinePos(b, c, delta, grid);
    auto ca_pos = soil_simulator::CalcLinePos(c, a, delta, grid);

    // Concatenating all cells in tri_pos
    tri_pos.insert(tri_pos.end(), ab_pos.begin(), ab_pos.end());
    tri_pos.insert(tri_pos.end(), bc_pos.begin(), bc_pos.end());
    tri_pos.insert(tri_pos.end(), ca_pos.begin(), ca_pos.end());

    return tri_pos;
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

// For each XY position, the first cell found in `area_pos` corresponds to
// the minimum height of the bucket, while the last one provides the maximum
// height. As a result, this function must be called separately for each bucket
// wall and `area_pos` must be sorted.
void soil_simulator::UpdateBody(
    std::vector<std::vector<int>> area_pos, SimOut* sim_out, Grid grid,
    float tol
) {
    // Initializing cell position and height
    int ii = area_pos[0][0];
    int jj = area_pos[0][1];
    float min_h = grid.vect_z_[area_pos[0][2]] - grid.cell_size_z_;
    float max_h = grid.vect_z_[area_pos[0][2]];

    // Iterating over all cells in area_pos
    for (auto nn = 0; nn < area_pos.size(); nn++) {
        if ((ii != area_pos[nn][0]) || (jj != area_pos[nn][1])) {
            // New XY position ###
            // Updating bucket position for the previous XY position
            soil_simulator::IncludeNewBodyPos(
                sim_out, ii, jj, min_h, max_h, tol);

            // Initializing new cell position and height
            min_h = grid.vect_z_[area_pos[nn][2]] - grid.cell_size_z_;
            max_h = grid.vect_z_[area_pos[nn][2]];
            ii = area_pos[nn][0];
            jj = area_pos[nn][1];
        } else {
            // New height for the XY position
            // Updating maximum height
            max_h = grid.vect_z_[area_pos[nn][2]];
        }
    }

    // Updating bucket position for the last XY position
    soil_simulator::IncludeNewBodyPos(sim_out, ii, jj, min_h, max_h, tol);
}

// The minimum and maximum heights of the bucket at that position are given by
// `min_h` and `max_h`, respectively.
// If the given position overlaps with an existing position, then the existing
// position is updated as the union of the two positions. Otherwise, a new
// position is added to `body_`.
void soil_simulator::IncludeNewBodyPos(
    SimOut* sim_out, int ii, int jj, float min_h, float max_h, float tol
) {
    std::vector<int> status(2);
    // Iterating over the two bucket layers and storing their status
    for (auto nn = 0; nn < 2; nn++) {
        int ind = 2 * nn;
        if (
            (sim_out->body_[ind][ii][jj] == 0.0) &&
            (sim_out->body_[ind+1][ii][jj] == 0.0)) {
            // No existing position
            status[nn] = 0;
        } else if (
            (min_h - tol < sim_out->body_[ind][ii][jj]) &&
            (max_h + tol > sim_out->body_[ind][ii][jj])) {
            // New position is overlapping with an existing position
            status[nn] = 1;
        } else if (
            (min_h - tol < sim_out->body_[ind+1][ii][jj]) &&
            (max_h + tol > sim_out->body_[ind+1][ii][jj])) {
            // New position is overlapping with an existing position
            status[nn] = 1;
        } else if (
            (min_h + tol > sim_out->body_[ind][ii][jj]) &&
            (max_h - tol < sim_out->body_[ind+1][ii][jj])
        ) {
            // New position is within an existing position
            return;
        } else {
            // New position is not overlapping with the two existing positions
            status[nn] = -1;
        }
    }

    // Updating the bucket position
    if ((status[0] == 1) && (status[1] == 1)) {
        // New position is overlapping with the two existing positions
        sim_out->body_[0][ii][jj] = std::min(
            {sim_out->body_[0][ii][jj], sim_out->body_[2][ii][jj], min_h});
        sim_out->body_[1][ii][jj] = std::max(
            {sim_out->body_[1][ii][jj], sim_out->body_[3][ii][jj], max_h});

        // Resetting obsolete bucket position
        sim_out->body_[2][ii][jj] = 0.0;
        sim_out->body_[3][ii][jj] = 0.0;
    } else if (status[0] == 1) {
        // New position is overlapping with an existing position
        sim_out->body_[0][ii][jj] = std::min(sim_out->body_[0][ii][jj], min_h);
        sim_out->body_[1][ii][jj] = std::max(sim_out->body_[1][ii][jj], max_h);
    } else if (status[1] == 1) {
        // New position is overlapping with an existing position
        sim_out->body_[2][ii][jj] = std::min(sim_out->body_[2][ii][jj], min_h);
        sim_out->body_[3][ii][jj] = std::max(sim_out->body_[3][ii][jj], max_h);
    } else if (status[0] == 0) {
        // No existing position
        sim_out->body_[0][ii][jj] = min_h;
        sim_out->body_[1][ii][jj] = max_h;
    } else if (status[1] == 0) {
        // No existing position
        sim_out->body_[2][ii][jj] = min_h;
        sim_out->body_[3][ii][jj] = max_h;
    } else {
        // New position is not overlapping with the two existing positions
        // This should not happen and indicates a problem in the workflow
        throw std::runtime_error("Try to update body, but given position does"
            "not overlap with two existing ones");
    }
}
