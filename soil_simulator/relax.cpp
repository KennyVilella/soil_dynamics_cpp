/*
This file implements the functions used for the terrain relaxation.

Copyright, 2023, Vilella Kenny.
*/
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include "soil_simulator/relax.hpp"
#include "soil_simulator/types.hpp"
#include "soil_simulator/utils.hpp"

/// The soil stability is determined by the `repose_angle_`. If the slope formed
/// by two neighbouring soil columns exceeds the `repose_angle_`, it is
/// considered unstable, and the soil from the higher column should avalanche to
/// the neighbouring column to reach an equilibrium state.
///
/// By convention, this function only checks the stability of the soil in the
/// four adjacent cells:
///                     ↑
///                   ← O →
///                     ↓
///
/// The diagonal directions are not checked for simplicity and performance
/// reasons.
///
/// This function only moves the soil when the following conditions are met:
///
/// (1) The soil column in the neighbouring cell is low enough.
/// (2) Either:
///     (a) The body is not on the soil, meaning there is space between the
///         `terrain_` and the body, or there is no body.
///     (b) The body is on the `terrain_`, but the combination of the body
///         and body soil is not high enough to prevent soil avalanche.
///
/// In case (2a), the soil will avalanche on the `terrain_`, while in case (2b),
/// the soil will avalanche on the body.
void soil_simulator::RelaxTerrain(
    SimOut* sim_out, Grid grid, Body* body, SimParam sim_param, float tol
) {
    // Assuming that the terrain is at equilibrium
    sim_out->equilibrium_ = true;

    // Calculating the maximum slope allowed by the repose angle
    float slope_max = std::tan(sim_param.repose_angle_);
    // Calculating the maximum height different allowed by the repose angle
    float dh_max = grid.cell_size_xy_ * slope_max;
    dh_max = grid.cell_size_z_ * round(dh_max / grid.cell_size_z_);

    // Locating cells requiring relaxation
    auto unstable_cells = LocateUnstableTerrainCell(sim_out, dh_max, tol);

    if (unstable_cells.size() == 0) {
        // Terrain is already at equilibrium
        return;
    }

    // Randomizing unstable cells to reduce asymmetry
    // random_suffle is not used because it is machine dependent,
    // which makes unit testing difficult
    for (int aa = unstable_cells.size() - 1; aa > 0; aa--) {
        std::uniform_int_distribution<int> dist(0, aa);
        int bb = dist(rng);
        std::swap(unstable_cells[aa], unstable_cells[bb]);
    }

    // Storing all possible directions for relaxation
    std::vector<std::vector<int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    // Initializing the 2D bounding box of the unstable cells
    int relax_min_x = 2 * grid.half_length_x_;
    int relax_max_x = 0;
    int relax_min_y = 2 * grid.half_length_y_;
    int relax_max_y = 0;

    // Iterating over all unstable cells
    for (auto nn = 0; nn < unstable_cells.size(); nn++) {
        int ii = unstable_cells[nn][0];
        int jj = unstable_cells[nn][1];

        // Updating the 2D bounding box of the unstable cells
        relax_min_x = std::min(relax_min_x, ii);
        relax_max_x = std::max(relax_max_x, ii);
        relax_min_y = std::min(relax_min_y, jj);
        relax_max_y = std::max(relax_max_y, jj);

        // Randomizing direction to avoid asymmetry
        // random_suffle is not used because it is machine dependent,
        // which makes unit testing difficult
        for (int aa = directions.size() - 1; aa > 0; aa--) {
            std::uniform_int_distribution<int> dist(0, aa);
            int bb = dist(rng);
            std::swap(directions[aa], directions[bb]);
        }

        // Iterating over the possible directions
        for (auto xy = 0; xy < directions.size(); xy++) {
            int ii_c = ii + directions[xy][0];
            int jj_c = jj + directions[xy][1];

            // Calculating minimum height allowed surrounding the considered
            // soil cell
            float h_min = sim_out->terrain_[ii][jj] - dh_max;

            // Checking if the cell requires relaxation
            auto status = CheckUnstableTerrainCell(
                sim_out, ii_c, jj_c, h_min, tol);

            if (status == 0) {
                // Soil cell already at equilibrium
                continue;
            } else {
                // Soil cell requires relaxation
                sim_out->equilibrium_ = false;
            }

            // Relaxing the soil cell
            RelaxUnstableTerrainCell(
                sim_out, status, dh_max, ii, jj, ii_c, jj_c, grid, body, tol);
        }
    }

    // Updating relax_area
    sim_out->relax_area_[0][0] = std::max(
        relax_min_x - sim_param.cell_buffer_, 2);
    sim_out->relax_area_[0][1] = std::min(
        relax_max_x + sim_param.cell_buffer_, 2 * grid.half_length_x_);
    sim_out->relax_area_[1][0] = std::max(
        relax_min_y - sim_param.cell_buffer_, 2);
    sim_out->relax_area_[1][1] = std::min(
        relax_max_y + sim_param.cell_buffer_, 2 * grid.half_length_y_);
}

/// The soil stability is determined by the `repose_angle_`. If the slope formed
/// by two neighbouring soil columns exceeds the `repose_angle_`, it is
/// considered unstable, and the soil from the higher column should avalanche to
/// the neighbouring column to reach an equilibrium state.
///
/// By convention, this function only checks the stability of the soil in the
/// four adjacent cells:
///                     ↑
///                   ← O →
///                     ↓
///
/// The diagonal directions are not checked for simplicity and performance
/// reasons.
///
/// This function only moves the soil when the following conditions are met:
///
/// (1) The soil column in the neighbouring cell is low enough.
/// (2) There is space on the top of the neighbouring soil column.
void soil_simulator::RelaxBodySoil(
    SimOut* sim_out, Grid grid, Body* body, SimParam sim_param, float tol
) {
    // Calculating the maximum slope allowed by the repose angle
    float slope_max = std::tan(sim_param.repose_angle_);
    // Calculating the maximum height different allowed by the repose angle
    float dh_max = grid.cell_size_xy_ * slope_max;
    dh_max = grid.cell_size_z_ * round(dh_max / grid.cell_size_z_);

    // Storing all possible directions for relaxation
    std::vector<std::vector<int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    // Initializing queue for new body_soil_pos
    std::vector<soil_simulator::body_soil> *new_body_soil_pos = (
        new std::vector<soil_simulator::body_soil>);

    // Iterating over all body_soil cells
    for (auto nn = 0; nn < sim_out->body_soil_pos_.size(); nn++) {
        int ii = sim_out->body_soil_pos_[nn].ii;
        int jj = sim_out->body_soil_pos_[nn].jj;
        int ind = sim_out->body_soil_pos_[nn].ind;
        float h_soil = sim_out->body_soil_pos_[nn].h_soil;

        // Checking if soil is present
        if (h_soil < tol) {
            // No soil to be moved
            continue;
        }

        // Randomizing direction to avoid asymmetry
        // random_suffle is not used because it is machine dependent,
        // which makes unit testing difficult
        for (int aa = directions.size() - 1; aa > 0; aa--) {
            std::uniform_int_distribution<int> dist(0, aa);
            int bb = dist(rng);
            std::swap(directions[aa], directions[bb]);
        }

        // Iterating over the possible directions
        for (auto xy = 0; xy < directions.size(); xy++) {
            int ii_c = ii + directions[xy][0];
            int jj_c = jj + directions[xy][1];

            // Calculating minimum height allowed surrounding the considered
            // soil cell
            float h_min = sim_out->body_soil_[ind+1][ii][jj] - dh_max;

            // Checking if the cell requires relaxation
            auto status = CheckUnstableBodyCell(
                sim_out, ii, jj, ind, ii_c, jj_c, h_min, tol);

            if (status == 0) {
                // Soil cell already at equilibrium
                continue;
            } else {
                // Soil cell requires relaxation
                sim_out->equilibrium_ = false;
            }

            // Relaxing the soil cell
            RelaxUnstableBodyCell(
                sim_out, status, new_body_soil_pos, dh_max, nn, ii, jj, ind,
                ii_c, jj_c, grid, body, tol);
        }
    }

    // Adding new body_soil_pos
    for (auto nn = 0; nn < new_body_soil_pos->size(); nn++) {
        sim_out->body_soil_pos_.push_back((*new_body_soil_pos)[nn]);
    }
    delete new_body_soil_pos;
}

/// It is important to note that the cells selected by this function are not
/// necessarily unstable, as a body or the soil resting on it could be
/// supporting the soil column. This is only a first-order selection of
/// cells potentially unstable.
std::vector<std::vector<int>> soil_simulator::LocateUnstableTerrainCell(
    SimOut* sim_out, float dh_max, float tol
) {
    // Initializing
    std::vector<std::vector<int>> unstable_cells;

    // Iterating over the terrain
    int ii_min = sim_out->impact_area_[0][0];
    int ii_max = sim_out->impact_area_[0][1];
    int jj_min = sim_out->impact_area_[1][0];
    int jj_max = sim_out->impact_area_[1][1];
    for (auto ii = ii_min; ii < ii_max; ii++)
        for (auto jj = jj_min; jj < jj_max; jj++) {
            // Calculating the minimum height allowed surrounding
            // the considered soil cell
            float h_min = sim_out->terrain_[ii][jj] - dh_max - tol;

            if (
                (sim_out->terrain_[ii-1][jj] < h_min) ||
                (sim_out->terrain_[ii+1][jj] < h_min) ||
                (sim_out->terrain_[ii][jj-1] < h_min) ||
                (sim_out->terrain_[ii][jj+1] < h_min)
            ) {
                // Soil cell is requiring relaxation
                unstable_cells.push_back(std::vector<int> {ii, jj});
            }
        }

    return unstable_cells;
}

/// In case of instability, this function returns a two-digit number (`status`)
/// that provides information on how the soil should avalanche. The
/// interpretation of the two-digit number is described below.
///
/// The first digit indicates the potential presence of the body:
/// - 1 when the first body layer is present.
/// - 2 when the second body layer is present.
/// - 3 when the two body layers are present.
/// - 4 when no body layer is present.
///
/// The second digit indicates the layer where the soil should avalanche:
/// - 0 when it is the terrain` (no body is present).
/// - 1 when it is the second body soil layer.
/// - 2 when it is the second body layer.
/// - 3 when it is the first body soil layer.
/// - 4 when it is the first body layer.
///
/// The combination of these two digits provides a comprehensive description of
/// how the soil should avalanche in different scenarios.
///
/// Note that not all combinations for `status` are possible.
/// Some combinations, such as `41` or `23` are impossible.
int soil_simulator::CheckUnstableTerrainCell(
    SimOut* sim_out, int ii_c, int jj_c, float h_min, float tol
) {
    // Checking whether soil is low enough, because unstability may have
    // already been solved
    if (sim_out->terrain_[ii_c][jj_c] + tol < h_min) {
        // Adjacent terrain is low enough
        // Determining presence of body
        bool body_absence_1 = (
            (sim_out->body_[0][ii_c][jj_c] == 0.0) &&
            (sim_out->body_[1][ii_c][jj_c] == 0.0));
        bool body_absence_3 = (
            (sim_out->body_[2][ii_c][jj_c] == 0.0) &&
            (sim_out->body_[3][ii_c][jj_c] == 0.0));
        int status;
        float column_top;
        float body_bot;
        if (body_absence_1 && body_absence_3) {
            // No body
            return 40;
        } else if (body_absence_1) {
            // Only the second body layer
            status = 20;
            body_bot = sim_out->body_[2][ii_c][jj_c];

            if (sim_out->terrain_[ii_c][jj_c] + tol < body_bot) {
                // Space under the body
                return status;
            } else {
                // Body is on the terrain
                if (
                    (sim_out->body_soil_[2][ii_c][jj_c] != 0.0) ||
                    (sim_out->body_soil_[3][ii_c][jj_c] != 0.0)
                ) {
                    // Body soil is present
                    status += 1;
                    column_top = sim_out->body_soil_[3][ii_c][jj_c];
                } else {
                    // Body soil is not present
                    status += 2;
                    column_top = sim_out->body_[3][ii_c][jj_c];
                }
            }
        } else if (body_absence_3) {
            // Only the first body layer
            status = 10;
            body_bot = sim_out->body_[0][ii_c][jj_c];

            if (sim_out->terrain_[ii_c][jj_c] + tol < body_bot) {
                // Space under the body
                return status;
            } else {
                // Body is on the terrain
                if (
                    (sim_out->body_soil_[0][ii_c][jj_c] != 0.0) ||
                    (sim_out->body_soil_[1][ii_c][jj_c] != 0.0)
                ) {
                    // Body soil is present
                    status += 3;
                    column_top = sim_out->body_soil_[1][ii_c][jj_c];
                } else {
                    // Body soil is not present
                    status += 4;
                    column_top = sim_out->body_[1][ii_c][jj_c];
                }
            }
        } else {
            // Two body layers are present
            status = 30;
            int ind_bot;
            int ind_top;

            // Checking which layer is lower
            if (sim_out->body_[0][ii_c][jj_c] < sim_out->body_[2][ii_c][jj_c]) {
                // First body layer is lower
                body_bot = sim_out->body_[0][ii_c][jj_c];
                ind_bot = 0;
                ind_top = 2;
            } else {
                // Second body layer is lower
                body_bot = sim_out->body_[2][ii_c][jj_c];
                ind_bot = 2;
                ind_top = 0;
            }

            if (sim_out->terrain_[ii_c][jj_c] + tol < body_bot) {
                // Space under the body
                return status;
            } else {
                // Body is on the terrain
                if (
                    (sim_out->body_soil_[ind_bot][ii_c][jj_c] != 0.0) ||
                    (sim_out->body_soil_[ind_bot+1][ii_c][jj_c] != 0.0)) {
                    // Body soil is present on the bottom body layer
                    if (
                        sim_out->body_soil_[ind_bot+1][ii_c][jj_c] + tol >
                        sim_out->body_[ind_top][ii_c][jj_c]) {
                        // Soil is filling the space between the body layers
                        // Soil may avalanche on the body
                        if (
                            (sim_out->body_soil_[ind_top][ii_c][jj_c] != 0.0) ||
                            (sim_out->body_soil_[ind_top+1][ii_c][jj_c] != 0.0)
                        ) {
                            // Body soil is present on the top body layer
                            column_top = (
                                sim_out->body_soil_[ind_top+1][ii_c][jj_c]);
                            status += ind_bot + 1;
                        } else {
                            // Body soil is not present on top body layer
                            column_top = sim_out->body_[ind_top+1][ii_c][jj_c];
                            status += ind_bot + 2;
                        }
                    } else {
                        // Soil may relax between the two body layers
                        column_top = sim_out->body_soil_[ind_bot+1][ii_c][jj_c];
                        status += ind_top + 1;
                    }
                } else {
                    // Body soil is not present on the bottom body layer
                    column_top = sim_out->body_[ind_bot+1][ii_c][jj_c];
                    status += ind_top + 2;
                }
            }
        }
        if (column_top + tol < h_min) {
            // Column is low enough
            return status;
        }
    }
    return 0;
}

/// In case of instability, this function returns a two-digit number (`status`)
/// that provides information on how the soil should avalanche. The
/// interpretation of the two-digit number is described below.
///
/// The first digit indicates the potential presence of the body:
/// - 1 when the first body layer is present.
/// - 2 when the second body layer is present.
/// - 3 when the two body layers are present.
/// - 4 when no body layer is present.
///
/// The second digit indicates the layer where the soil should avalanche:
/// - 0 when it is the terrain (no body is present).
/// - 1 when it is the second body soil layer.
/// - 2 when it is the second body layer.
/// - 3 when it is the first body soil layer.
/// - 4 when it is the first body layer.
///
/// The combination of these two digits provides a comprehensive description of
/// how the soil should avalanche in different scenarios.
///
/// Note that not all combinations for `status` are possible.
/// Some combinations, such as `41` or `23` are impossible.
int soil_simulator::CheckUnstableBodyCell(
    SimOut* sim_out, int ii, int jj, int ind, int ii_c, int jj_c, float h_min,
    float tol
) {
    // Determining presence of body
    bool body_absence_1 = (
        (sim_out->body_[0][ii_c][jj_c] == 0.0) &&
        (sim_out->body_[1][ii_c][jj_c] == 0.0));
    bool body_absence_3 = (
        (sim_out->body_[2][ii_c][jj_c] == 0.0) &&
        (sim_out->body_[3][ii_c][jj_c] == 0.0));
    int status;
    float column_top;

    if (body_absence_1 && body_absence_3) {
        // No body
        if (sim_out->terrain_[ii_c][jj_c] + tol < h_min) {
            return 40;
        }
    } else if (body_absence_1) {
        // Only the second body layer
        status = 20;

        if (
            sim_out->body_[ind+1][ii][jj] + tol < sim_out->body_[2][ii_c][jj_c]
        ) {
            // Soil should avalanche to the terrain
            column_top = sim_out->terrain_[ii_c][jj_c];
        } else if (
            (sim_out->body_soil_[2][ii_c][jj_c] != 0.0) ||
            (sim_out->body_soil_[3][ii_c][jj_c] != 0.0)) {
            // Body soil is present
            status += 1;
            column_top = sim_out->body_soil_[3][ii_c][jj_c];
        } else {
            // Body soil is not present
            status += 2;
            column_top = sim_out->body_[3][ii_c][jj_c];
        }

        if (column_top + tol < h_min) {
            // Column is low enough
            return status;
        }
    } else if (body_absence_3) {
        // Only the first body layer
        status = 10;

        if (
            sim_out->body_[ind+1][ii][jj] + tol < sim_out->body_[0][ii_c][jj_c]
        ) {
            // Soil should avalanche to the terrain
            column_top = sim_out->terrain_[ii_c][jj_c];
        } else if (
            (sim_out->body_soil_[0][ii_c][jj_c] != 0.0) ||
            (sim_out->body_soil_[1][ii_c][jj_c] != 0.0)
        ) {
            // Body soil is present
            status += 3;
            column_top = sim_out->body_soil_[1][ii_c][jj_c];
        } else {
            // Body soil is not present
            status += 4;
            column_top = sim_out->body_[1][ii_c][jj_c];
        }

        if (column_top + tol < h_min) {
            // Column is low enough
            return status;
        }
    } else {
        // Both body layers are present
        status = 30;
        int ind_n_bot;
        int ind_n_top;

        if (sim_out->body_[0][ii_c][jj_c] < sim_out->body_[2][ii_c][jj_c]) {
            // First layer at bottom
            ind_n_bot = 0;
            ind_n_top = 2;
        } else {
            // Second layer at bottom
            ind_n_bot = 2;
            ind_n_top = 0;
        }

        if (
            sim_out->body_[ind+1][ii][jj] + tol <
            sim_out->body_[ind_n_top][ii_c][jj_c]) {
            // Soil may avalanche on the bottom layer
            if (
                (sim_out->body_soil_[ind_n_bot][ii_c][jj_c] != 0.0) ||
                (sim_out->body_soil_[ind_n_bot+1][ii_c][jj_c] != 0.0)) {
                // Body soil is present
                if (
                    sim_out->body_soil_[ind_n_bot+1][ii_c][jj_c] + tol <
                    sim_out->body_[ind_n_top][ii_c][jj_c]) {
                    // Some space is avilable
                    status += ind_n_top + 1;
                    column_top = sim_out->body_soil_[ind_n_bot+1][ii_c][jj_c];
                }
            } else {
                // Body soil is not present
                status += ind_n_top + 2;
                column_top = sim_out->body_[ind_n_bot+1][ii_c][jj_c];
            }
        }

        if (
            (sim_out->body_[ind+1][ii][jj] + tol >
            sim_out->body_[ind_n_top][ii_c][jj_c]) ||
            (status == 30)) {
            // Soil may avalanche on the top layer
            if (
                (sim_out->body_soil_[ind_n_top][ii_c][jj_c] != 0.0) ||
                (sim_out->body_soil_[ind_n_top+1][ii_c][jj_c] != 0.0)) {
                // Body soil is present
                status += ind_n_bot + 1;
                column_top = sim_out->body_soil_[ind_n_top+1][ii_c][jj_c];
            } else {
                // Body soil is not present
                status += ind_n_bot + 2;
                column_top = sim_out->body_[ind_n_top+1][ii_c][jj_c];
            }
        }

        if (column_top + tol < h_min) {
            // Column is low enough
            return status;
        }
    }

    return 0;
}

/// The precise movement applied to the soil cell depends on the `status` number
/// provided by the `CheckUnstableTerrainCell` function.
///
/// The soil is moved such that the slope formed by the two neighbouring soil
/// columns is equal to the `repose_angle_`. When the body is preventing this
/// configuration, the soil avalanche below the body to fill the space under
/// it.
///
/// Note that it is assumed that the given `status` is accurate, so no extra
/// checks are present.
void soil_simulator::RelaxUnstableTerrainCell(
    SimOut* sim_out, int status, float dh_max, int ii, int jj, int ii_c,
    int jj_c, Grid grid, Body* body, float tol
) {
    // Converting status into a string for convenience
    std::string st = std::to_string(status);

    float h_new;
    float h_new_c;
    float h_soil;
    if (st[1] == '0') {
        // Soil should avalanche on the terrain
        // Calculating new height values
        h_new = 0.5 * (
            dh_max + sim_out->terrain_[ii][jj] + sim_out->terrain_[ii_c][jj_c]);
        h_new = grid.cell_size_z_ * std::floor(
            (h_new + tol) / grid.cell_size_z_);
        h_new_c = (
            sim_out->terrain_[ii][jj] + sim_out->terrain_[ii_c][jj_c] - h_new);
        float body_bot;

        if (st[0] == '4') {
            // No body
            sim_out->terrain_[ii][jj] = h_new;
            sim_out->terrain_[ii_c][jj_c] = h_new_c;
            return;
        } else if (st[0] == '1') {
            // Under the first body layer
            body_bot = sim_out->body_[0][ii_c][jj_c];
        } else if (st[0] == '2') {
            // Under the second body layer
            body_bot = sim_out->body_[2][ii_c][jj_c];
        } else if (st[0] == '3') {
            // Two body layers present
            body_bot = std::min(
                {sim_out->body_[0][ii_c][jj_c], sim_out->body_[2][ii_c][jj_c]});
        }

        if (h_new_c < body_bot) {
            // Full avalanche
            sim_out->terrain_[ii][jj] = h_new;
            sim_out->terrain_[ii_c][jj_c] = h_new_c;
        } else {
            // Partial avalanche
            sim_out->terrain_[ii][jj] = (
                sim_out->terrain_[ii][jj] + sim_out->terrain_[ii_c][jj_c] -
                body_bot);
            sim_out->terrain_[ii_c][jj_c] = body_bot;
        }
    } else if (st[1] == '1') {
        // Soil avalanche on the second body soil layer
        h_new = 0.5 * (
            dh_max + sim_out->terrain_[ii][jj] +
            sim_out->body_soil_[3][ii_c][jj_c]);
        h_new = grid.cell_size_z_ * std::floor(
            (h_new + tol) / grid.cell_size_z_);
        h_soil = sim_out->terrain_[ii][jj] - h_new;
        h_new_c = sim_out->body_soil_[3][ii_c][jj_c] + h_soil;

        if (st[0] == '3') {
            // Two body layers are present
            if (sim_out->body_[3][ii_c][jj_c] < sim_out->body_[0][ii_c][jj_c]) {
                // Soil should avalanche between the two body layer
                if (h_new_c - tol > sim_out->body_[0][ii_c][jj_c]) {
                    // Not enough space for all the soil
                    h_soil = (
                        sim_out->body_[0][ii_c][jj_c] -
                        sim_out->body_soil_[3][ii_c][jj_c]);
                    h_new_c = sim_out->body_[0][ii_c][jj_c];
                    h_new = sim_out->terrain_[ii][jj] - h_soil;
                }
            }
        }

        // Updating terrain
        sim_out->terrain_[ii][jj] = h_new;
        sim_out->body_soil_[3][ii_c][jj_c] = h_new_c;

        // Calculating pos of cell in body frame
        auto pos = soil_simulator::CalcBodyFramePos(
            ii_c, jj_c, sim_out->body_[3][ii_c][jj_c], grid, body);

        // Adding new body soil position to body_soil_pos
        sim_out->body_soil_pos_.push_back(
            soil_simulator::body_soil
            {2, ii_c, jj_c, pos[0], pos[1], pos[2], h_soil});
    } else if (st[1] == '2') {
        // Soil avalanche on the second body layer
        h_new = 0.5 * (
            dh_max + sim_out->terrain_[ii][jj] +
            sim_out->body_[3][ii_c][jj_c]);
        h_new = grid.cell_size_z_ * std::floor(
            (h_new + tol) / grid.cell_size_z_);
        h_new_c = (
            sim_out->terrain_[ii][jj] + sim_out->body_[3][ii_c][jj_c] - h_new);

        if (st[0] == '3') {
            // Two body layers are present
            if (sim_out->body_[3][ii_c][jj_c] < sim_out->body_[0][ii_c][jj_c]) {
                // Soil should avalanche between the two body layer
                if (h_new_c - tol > sim_out->body_[0][ii_c][jj_c]) {
                    // Not enough space for all the soil
                    h_new_c = sim_out->body_[0][ii_c][jj_c];
                    h_new = (
                        sim_out->terrain_[ii][jj] -
                        sim_out->body_[0][ii_c][jj_c] +
                        sim_out->body_[3][ii_c][jj_c]);
                }
            }
        }

        // Updating terrain
        sim_out->terrain_[ii][jj] = h_new;
        sim_out->body_soil_[2][ii_c][jj_c] = sim_out->body_[3][ii_c][jj_c];
        sim_out->body_soil_[3][ii_c][jj_c] = h_new_c;

        // Calculating pos of cell in body frame
        auto pos = soil_simulator::CalcBodyFramePos(
            ii_c, jj_c, sim_out->body_[3][ii_c][jj_c], grid, body);

        // Adding new body soil position to body_soil_pos
        h_soil = h_new_c - sim_out->body_[3][ii_c][jj_c];
        sim_out->body_soil_pos_.push_back(
            soil_simulator::body_soil
            {2, ii_c, jj_c, pos[0], pos[1], pos[2], h_soil});
    } else if (st[1] == '3') {
        // Soil avalanche on the first body soil layer
        h_new = 0.5 * (
            dh_max + sim_out->terrain_[ii][jj] +
            sim_out->body_soil_[1][ii_c][jj_c]);
        h_new = grid.cell_size_z_ * std::floor(
            (h_new + tol) / grid.cell_size_z_);
        h_soil = sim_out->terrain_[ii][jj] - h_new;
        h_new_c = sim_out->body_soil_[1][ii_c][jj_c] + h_soil;

        if (st[0] == '3') {
            // Two body layers are present
            if (sim_out->body_[1][ii_c][jj_c] < sim_out->body_[2][ii_c][jj_c]) {
                // Soil should avalanche between the two body layer
                if (h_new_c - tol > sim_out->body_[2][ii_c][jj_c]) {
                    // Not enough space for all the soil
                    h_soil = (
                        sim_out->body_[2][ii_c][jj_c] -
                        sim_out->body_soil_[1][ii_c][jj_c]);
                    h_new_c = sim_out->body_[2][ii_c][jj_c];
                    h_new = sim_out->terrain_[ii][jj] - h_soil;
                }
            }
        }

        // Updating terrain
        sim_out->terrain_[ii][jj] = h_new;
        sim_out->body_soil_[1][ii_c][jj_c] = h_new_c;

        // Calculating pos of cell in body frame
        auto pos = soil_simulator::CalcBodyFramePos(
            ii_c, jj_c, sim_out->body_[1][ii_c][jj_c], grid, body);

        // Adding new body soil position to body_soil_pos
        sim_out->body_soil_pos_.push_back(
            soil_simulator::body_soil
            {0, ii_c, jj_c, pos[0], pos[1], pos[2], h_soil});
    } else if (st[1] == '4') {
        // Soil avalanche on the first body layer
        h_new = 0.5 * (
            dh_max + sim_out->terrain_[ii][jj] +
            sim_out->body_[1][ii_c][jj_c]);
        h_new = grid.cell_size_z_ * std::floor(
            (h_new + tol) / grid.cell_size_z_);
        h_new_c = (
            sim_out->terrain_[ii][jj] + sim_out->body_[1][ii_c][jj_c] - h_new);

        if (st[0] == '3') {
            // Two body layers are present
            if (sim_out->body_[1][ii_c][jj_c] < sim_out->body_[2][ii_c][jj_c]) {
                // Soil should avalanche between the two body layer
                if (h_new_c - tol > sim_out->body_[2][ii_c][jj_c]) {
                    // Not enough space for all the soil
                    h_new_c = sim_out->body_[2][ii_c][jj_c];
                    h_new = (
                        sim_out->terrain_[ii][jj] -
                        sim_out->body_[2][ii_c][jj_c] +
                        sim_out->body_[1][ii_c][jj_c]);
                }
            }
        }

        // Updating terrain
        sim_out->terrain_[ii][jj] = h_new;
        sim_out->body_soil_[0][ii_c][jj_c] = sim_out->body_[1][ii_c][jj_c];
        sim_out->body_soil_[1][ii_c][jj_c] = h_new_c;

        // Calculating pos of cell in body frame
        auto pos = soil_simulator::CalcBodyFramePos(
            ii_c, jj_c, sim_out->body_[1][ii_c][jj_c], grid, body);

        // Adding new body soil position to body_soil_pos
        h_soil = h_new_c - sim_out->body_[1][ii_c][jj_c];
        sim_out->body_soil_pos_.push_back(
            soil_simulator::body_soil
            {0, ii_c, jj_c, pos[0], pos[1], pos[2], h_soil});
    }
}

/// The precise movement applied to the soil cell depends on the `status` number
/// provided by the `CheckUnstableBodyCell` function.
///
/// The soil is moved such that the slope formed by the two neighbouring soil
/// columns is equal to the `repose_angle_`, provided that the body is not
/// preventing this configuration.
///
/// Note that it is assumed that the given `status` is accurate, so no extra
/// checks are present.
void soil_simulator::RelaxUnstableBodyCell(
    SimOut* sim_out, int status, std::vector<body_soil>* body_soil_pos,
    float dh_max, int nn, int ii, int jj, int ind, int ii_c, int jj_c,
    Grid grid, Body* body, float tol
) {
    // Converting status into a string for convenience
    std::string st = std::to_string(status);
    float h_new;
    float h_new_c;
    float h_soil;
    if (st[1] == '0') {
        // No body
        // Calculating new height values
        h_new = 0.5 * (
            dh_max + sim_out->body_soil_[ind+1][ii][jj] +
            sim_out->terrain_[ii_c][jj_c]);
        h_new = grid.cell_size_z_ * std::floor(
            (h_new + tol) / grid.cell_size_z_);
        h_soil = sim_out->body_soil_[ind+1][ii][jj] - h_new;

        // Checking amount of soil in body_soil_pos_
        if (h_soil > sim_out->body_soil_pos_[nn].h_soil) {
            // Not enough soil in body_soil_pos_
            h_soil = sim_out->body_soil_pos_[nn].h_soil;
        }
        h_new_c = sim_out->terrain_[ii_c][jj_c] + h_soil;

        if (st[0] == '1') {
            // First body layer is present
            if (h_new_c - tol > sim_out->body_[0][ii_c][jj_c]) {
                // Not enough space for all the soil
                h_soil = (
                    sim_out->body_[0][ii_c][jj_c] -
                    sim_out->terrain_[ii_c][jj_c]);
                h_new_c = sim_out->body_[0][ii_c][jj_c];
            }
        } else if (st[0] == '2') {
            // Second body layer is present
            if (h_new_c - tol > sim_out->body_[2][ii_c][jj_c]) {
                // Not enough space for all the soil
                h_soil = (
                    sim_out->body_[2][ii_c][jj_c] -
                    sim_out->terrain_[ii_c][jj_c]);
                h_new_c = sim_out->body_[2][ii_c][jj_c];
            }
        }
        h_new = sim_out->body_soil_[ind+1][ii][jj] - h_soil;

        if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
            // Soil on the body should partially avalanche
            sim_out->terrain_[ii_c][jj_c] = h_new_c;
            sim_out->body_soil_[ind+1][ii][jj] = h_new;
            sim_out->body_soil_pos_[nn].h_soil -= h_soil;
        } else {
            // All soil on the body should avalanche
            sim_out->terrain_[ii_c][jj_c] += h_soil;
            sim_out->body_soil_[ind][ii][jj] = 0.0;
            sim_out->body_soil_[ind+1][ii][jj] = 0.0;
            sim_out->body_soil_pos_[nn].h_soil = 0.0;
        }
    } else if (st[0] == '1') {
        // Only the first body layer
        if (st[1] == '3') {
            // Body soil is present
            h_new = 0.5 * (
                dh_max + sim_out->body_soil_[ind+1][ii][jj] +
                sim_out->body_soil_[1][ii_c][jj_c]);
            h_new = grid.cell_size_z_ * std::floor(
                (h_new + tol) / grid.cell_size_z_);
            h_soil = sim_out->body_soil_[ind+1][ii][jj] - h_new;

            // Checking amount of soil in body_soil_pos_
            if (h_soil > sim_out->body_soil_pos_[nn].h_soil) {
                // Not enough soil in body_soil_pos_
                h_soil = sim_out->body_soil_pos_[nn].h_soil;
                h_new = sim_out->body_soil_[ind+1][ii][jj] - h_soil;
            }

            if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
                // Soil on the body should partially avalanche
                sim_out->body_soil_[ind+1][ii][jj] = h_new;
                sim_out->body_soil_pos_[nn].h_soil -= h_soil;
            } else {
                // All soil on the body should avalanche
                sim_out->body_soil_[ind][ii][jj] = 0.0;
                sim_out->body_soil_[ind+1][ii][jj] = 0.0;
                sim_out->body_soil_pos_[nn].h_soil = 0.0;
            }
            sim_out->body_soil_[1][ii_c][jj_c] += h_soil;

            // Calculating pos of cell in body frame
            auto pos = soil_simulator::CalcBodyFramePos(
                ii_c, jj_c, sim_out->body_[1][ii_c][jj_c], grid, body);

            // Adding new body soil position to body_soil_pos
            body_soil_pos->push_back(soil_simulator::body_soil
                {0, ii_c, jj_c, pos[0], pos[1], pos[2], h_soil});
        } else if (st[1] == '4') {
            // Body soil is not present
            h_new = 0.5 * (
                dh_max + sim_out->body_soil_[ind+1][ii][jj] +
                sim_out->body_[1][ii_c][jj_c]);
            h_new = grid.cell_size_z_ * std::floor(
                (h_new + tol) / grid.cell_size_z_);
            h_soil = sim_out->body_soil_[ind+1][ii][jj] - h_new;

            // Checking amount of soil in body_soil_pos_
            if (h_soil > sim_out->body_soil_pos_[nn].h_soil) {
                // Not enough soil in body_soil_pos_
                h_soil = sim_out->body_soil_pos_[nn].h_soil;
                h_new = sim_out->body_soil_[ind+1][ii][jj] - h_soil;
            }

            sim_out->body_soil_[0][ii_c][jj_c] = sim_out->body_[1][ii_c][jj_c];
            if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
                // Soil on the body should partially avalanche
                sim_out->body_soil_[ind+1][ii][jj] = h_new;
                sim_out->body_soil_pos_[nn].h_soil -= h_soil;
            } else {
                // All soil on the body should avalanche
                sim_out->body_soil_[ind][ii][jj] = 0.0;
                sim_out->body_soil_[ind+1][ii][jj] = 0.0;
                sim_out->body_soil_pos_[nn].h_soil = 0.0;
            }
            sim_out->body_soil_[1][ii_c][jj_c] = (
                    sim_out->body_[1][ii_c][jj_c] + h_soil);

            // Calculating pos of cell in body frame
            auto pos = soil_simulator::CalcBodyFramePos(
                ii_c, jj_c, sim_out->body_[1][ii_c][jj_c], grid, body);

            // Adding new body soil position to body_soil_pos
            body_soil_pos->push_back(soil_simulator::body_soil
                {0, ii_c, jj_c, pos[0], pos[1], pos[2], h_soil});
        }
    } else if (st[0] == '2') {
        // Only the second body layer
        if (st[1] == '1') {
            // Body soil is present
            h_new = 0.5 * (
                dh_max + sim_out->body_soil_[ind+1][ii][jj] +
                sim_out->body_soil_[3][ii_c][jj_c]);
            h_new = grid.cell_size_z_ * std::floor(
                (h_new + tol) / grid.cell_size_z_);
            h_soil = sim_out->body_soil_[ind+1][ii][jj] - h_new;

            // Checking amount of soil in body_soil_pos_
            if (h_soil > sim_out->body_soil_pos_[nn].h_soil) {
                // Not enough soil in body_soil_pos_
                h_soil = sim_out->body_soil_pos_[nn].h_soil;
                h_new = sim_out->body_soil_[ind+1][ii][jj] - h_soil;
            }

            if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
                // Soil on the body should partially avalanche
                sim_out->body_soil_[ind+1][ii][jj] = h_new;
                sim_out->body_soil_pos_[nn].h_soil -= h_soil;
            } else {
                // All soil on the body should avalanche
                sim_out->body_soil_[ind][ii][jj] = 0.0;
                sim_out->body_soil_[ind+1][ii][jj] = 0.0;
                sim_out->body_soil_pos_[nn].h_soil = 0.0;
            }
            sim_out->body_soil_[3][ii_c][jj_c] += h_soil;

            // Calculating pos of cell in body frame
            auto pos = soil_simulator::CalcBodyFramePos(
                ii_c, jj_c, sim_out->body_[3][ii_c][jj_c], grid, body);

            // Adding new body soil position to body_soil_pos
            body_soil_pos->push_back(soil_simulator::body_soil
                {2, ii_c, jj_c, pos[0], pos[1], pos[2], h_soil});
        } else if (st[1] == '2') {
            // Body soil is not present
            h_new = 0.5 * (
                dh_max + sim_out->body_soil_[ind+1][ii][jj] +
                sim_out->body_[3][ii_c][jj_c]);
            h_new = grid.cell_size_z_ * std::floor(
                (h_new + tol) / grid.cell_size_z_);
            h_soil = sim_out->body_soil_[ind+1][ii][jj] - h_new;

            // Checking amount of soil in body_soil_pos_
            if (h_soil > sim_out->body_soil_pos_[nn].h_soil) {
                // Not enough soil in body_soil_pos_
                h_soil = sim_out->body_soil_pos_[nn].h_soil;
                h_new = sim_out->body_soil_[ind+1][ii][jj] - h_soil;
            }

            sim_out->body_soil_[2][ii_c][jj_c] = sim_out->body_[3][ii_c][jj_c];
            if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
                // Soil on the body should partially avalanche
                sim_out->body_soil_[ind+1][ii][jj] = h_new;
                sim_out->body_soil_pos_[nn].h_soil -= h_soil;
            } else {
                // All soil on the body should avalanche
                sim_out->body_soil_[ind][ii][jj] = 0.0;
                sim_out->body_soil_[ind+1][ii][jj] = 0.0;
                sim_out->body_soil_pos_[nn].h_soil = 0.0;
            }
            sim_out->body_soil_[3][ii_c][jj_c] = (
                sim_out->body_[3][ii_c][jj_c] + h_soil);

            // Calculating pos of cell in body frame
            auto pos = soil_simulator::CalcBodyFramePos(
                ii_c, jj_c, sim_out->body_[3][ii_c][jj_c], grid, body);

            // Adding new body soil position to body_soil_pos
            body_soil_pos->push_back(soil_simulator::body_soil
                {2, ii_c, jj_c, pos[0], pos[1], pos[2], h_soil});
        }
    } else if (st[0] == '3') {
        // Both body layer
        if (st[1] == '1') {
            // Soil should avalanche on the second body soil layer
            h_new = 0.5 * (
                dh_max + sim_out->body_soil_[ind+1][ii][jj] +
                sim_out->body_soil_[3][ii_c][jj_c]);
            h_new = grid.cell_size_z_ * std::floor(
                (h_new + tol) / grid.cell_size_z_);
            h_soil = sim_out->body_soil_[ind+1][ii][jj] - h_new;

            // Checking amount of soil in body_soil_pos_
            if (h_soil > sim_out->body_soil_pos_[nn].h_soil) {
                // Not enough soil in body_soil_pos_
                h_soil = sim_out->body_soil_pos_[nn].h_soil;
                h_new = sim_out->body_soil_[ind+1][ii][jj] - h_soil;
            }
            h_new_c = sim_out->body_soil_[3][ii_c][jj_c] + h_soil;

            if (sim_out->body_[0][ii_c][jj_c] > sim_out->body_[2][ii_c][jj_c]) {
                // Soil should avalanche on the bottom layer
                if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
                    // Soil on the body should partially avalanche
                    if (h_new_c - tol > sim_out->body_[0][ii_c][jj_c]) {
                        // Not enough space available
                        h_soil = (
                            sim_out->body_[0][ii_c][jj_c] -
                            sim_out->body_soil_[3][ii_c][jj_c]);
                        sim_out->body_soil_[ind+1][ii][jj] -= h_soil;
                        sim_out->body_soil_[3][ii_c][jj_c] = (
                            sim_out->body_[0][ii_c][jj_c]);
                    } else {
                        // Enough space for the partial avalanche
                        sim_out->body_soil_[3][ii_c][jj_c] = h_new_c;
                        sim_out->body_soil_[ind+1][ii][jj] = h_new;
                    }
                    sim_out->body_soil_pos_[nn].h_soil -= h_soil;
                } else {
                    // All soil on the body may avalanche
                    // By construction, it must have enough space for
                    // the full avalanche
                    sim_out->body_soil_[3][ii_c][jj_c] += h_soil;
                    sim_out->body_soil_[ind][ii][jj] = 0.0;
                    sim_out->body_soil_[ind+1][ii][jj] = 0.0;
                    sim_out->body_soil_pos_[nn].h_soil = 0.0;
                }
            } else {
                // Soil should avalanche on the top layer
                if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
                    // Soil on the body should partially avalanche
                    sim_out->body_soil_[3][ii_c][jj_c] = h_new_c;
                    sim_out->body_soil_[ind+1][ii][jj] = h_new;
                    sim_out->body_soil_pos_[nn].h_soil -= h_soil;
                } else {
                    // All soil on the body should avalanche
                    sim_out->body_soil_[3][ii_c][jj_c] += h_soil;
                    sim_out->body_soil_[ind][ii][jj] = 0.0;
                    sim_out->body_soil_[ind+1][ii][jj] = 0.0;
                    sim_out->body_soil_pos_[nn].h_soil = 0.0;
                }
            }
            if (h_soil > tol) {
                // Calculating pos of cell in body frame
                auto pos = soil_simulator::CalcBodyFramePos(
                    ii_c, jj_c, sim_out->body_[3][ii_c][jj_c], grid, body);

                // Adding new body soil position to body_soil_pos
                body_soil_pos->push_back(soil_simulator::body_soil
                    {2, ii_c, jj_c, pos[0], pos[1], pos[2], h_soil});
            }
        } else if (st[1] == '2') {
            // Soil should avalanche on the second body layer
            h_new = 0.5 * (
                dh_max + sim_out->body_soil_[ind+1][ii][jj] +
                sim_out->body_[3][ii_c][jj_c]);
            h_new = grid.cell_size_z_ * std::floor(
                (h_new + tol) / grid.cell_size_z_);
            h_soil = sim_out->body_soil_[ind+1][ii][jj] - h_new;

            // Checking amount of soil in body_soil_pos_
            if (h_soil > sim_out->body_soil_pos_[nn].h_soil) {
                // Not enough soil in body_soil_pos_
                h_soil = sim_out->body_soil_pos_[nn].h_soil;
                h_new = sim_out->body_soil_[ind+1][ii][jj] - h_soil;
            }
            h_new_c = sim_out->body_[3][ii_c][jj_c] + h_soil;


            sim_out->body_soil_[2][ii_c][jj_c] = sim_out->body_[3][ii_c][jj_c];
            if (sim_out->body_[0][ii_c][jj_c] > sim_out->body_[2][ii_c][jj_c]) {
                // Soil should avalanche on the bottom layer
                if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
                    // Soil on the body should partially avalanche
                    if (h_new_c - tol > sim_out->body_[0][ii_c][jj_c]) {
                        // Not enough space available
                        h_soil = (
                            sim_out->body_[0][ii_c][jj_c] -
                            sim_out->body_[3][ii_c][jj_c]);
                        sim_out->body_soil_[ind+1][ii][jj] -= h_soil;
                        sim_out->body_soil_[3][ii_c][jj_c] = (
                            sim_out->body_[0][ii_c][jj_c]);
                    } else {
                        // Enough space for the partial avalanche
                        sim_out->body_soil_[3][ii_c][jj_c] = h_new_c;
                        sim_out->body_soil_[ind+1][ii][jj] = h_new;
                    }
                    sim_out->body_soil_pos_[nn].h_soil -= h_soil;
                } else {
                    // All soil on the body may avalanche
                    // By construction, it must have enough space for
                    // the full avalanche
                    sim_out->body_soil_[3][ii_c][jj_c] = h_new_c;
                    sim_out->body_soil_[ind][ii][jj] = 0.0;
                    sim_out->body_soil_[ind+1][ii][jj] = 0.0;
                    sim_out->body_soil_pos_[nn].h_soil = 0.0;
                }
            } else {
                // Soil should avalanche on the top layer
                sim_out->body_soil_[3][ii_c][jj_c] = h_new_c;
                if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
                    // Soil on the body should partially avalanche
                    sim_out->body_soil_[ind+1][ii][jj] = h_new;
                    sim_out->body_soil_pos_[nn].h_soil -= h_soil;
                } else {
                    // All soil on the body should avalanche
                    sim_out->body_soil_[ind][ii][jj] = 0.0;
                    sim_out->body_soil_[ind+1][ii][jj] = 0.0;
                    sim_out->body_soil_pos_[nn].h_soil = 0.0;
                }
            }

            // Calculating pos of cell in body frame
            auto pos = soil_simulator::CalcBodyFramePos(
                ii_c, jj_c, sim_out->body_[3][ii_c][jj_c], grid, body);

            // Adding new body soil position to body_soil_pos
            body_soil_pos->push_back(soil_simulator::body_soil
                {2, ii_c, jj_c, pos[0], pos[1], pos[2], h_soil});
        } else if (st[1] == '3') {
            // Soil should avalanche on the first body soil layer
            h_new = 0.5 * (
                dh_max + sim_out->body_soil_[ind+1][ii][jj] +
                sim_out->body_soil_[1][ii_c][jj_c]);
            h_new = grid.cell_size_z_ * std::floor(
                (h_new + tol) / grid.cell_size_z_);
            h_soil = sim_out->body_soil_[ind+1][ii][jj] - h_new;

            // Checking amount of soil in body_soil_pos_
            if (h_soil > sim_out->body_soil_pos_[nn].h_soil) {
                // Not enough soil in body_soil_pos_
                h_soil = sim_out->body_soil_pos_[nn].h_soil;
                h_new = sim_out->body_soil_[ind+1][ii][jj] - h_soil;
            }
            h_new_c = sim_out->body_soil_[1][ii_c][jj_c] + h_soil;

            if (sim_out->body_[0][ii_c][jj_c] > sim_out->body_[2][ii_c][jj_c]) {
                // Soil should avalanche on the top layer
                if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
                    // Soil on the body should partially avalanche
                    sim_out->body_soil_[1][ii_c][jj_c] = h_new_c;
                    sim_out->body_soil_[ind+1][ii][jj] = h_new;
                    sim_out->body_soil_pos_[nn].h_soil -= h_soil;
                } else {
                    // All soil on the body should avalanche
                    sim_out->body_soil_[1][ii_c][jj_c] += h_soil;
                    sim_out->body_soil_[ind][ii][jj] = 0.0;
                    sim_out->body_soil_[ind+1][ii][jj] = 0.0;
                    sim_out->body_soil_pos_[nn].h_soil = 0.0;
                }
            } else {
                // Soil should avalanche on the bottom layer
                if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
                    // Soil on the body should partially avalanche
                    if (h_new_c - tol > sim_out->body_[2][ii_c][jj_c]) {
                        // Not enough space available
                        h_soil = (
                            sim_out->body_[2][ii_c][jj_c] -
                            sim_out->body_soil_[1][ii_c][jj_c]);
                        sim_out->body_soil_[ind+1][ii][jj] -= h_soil;
                        sim_out->body_soil_[1][ii_c][jj_c] = (
                            sim_out->body_[2][ii_c][jj_c]);
                    } else {
                        // Enough space for the partial avalanche
                        sim_out->body_soil_[1][ii_c][jj_c] = h_new_c;
                        sim_out->body_soil_[ind+1][ii][jj] = h_new;
                    }
                    sim_out->body_soil_pos_[nn].h_soil -= h_soil;
                } else {
                    // All soil on the body may avalanche
                    // By construction, it must have enough space for
                    // the full avalanche
                    sim_out->body_soil_[1][ii_c][jj_c] += h_soil;
                    sim_out->body_soil_[ind][ii][jj] = 0.0;
                    sim_out->body_soil_[ind+1][ii][jj] = 0.0;
                    sim_out->body_soil_pos_[nn].h_soil = 0.0;
                }
            }
            if (h_soil > tol) {
                // Calculating pos of cell in body frame
                auto pos = soil_simulator::CalcBodyFramePos(
                    ii_c, jj_c, sim_out->body_[1][ii_c][jj_c], grid, body);

                // Adding new body soil position to body_soil_pos
                body_soil_pos->push_back(soil_simulator::body_soil
                    {0, ii_c, jj_c, pos[0], pos[1], pos[2], h_soil});
            }
        } else if (st[1] == '4') {
            // Soil should avalanche on the first body layer
            h_new = 0.5 * (
                dh_max + sim_out->body_soil_[ind+1][ii][jj] +
                sim_out->body_[1][ii_c][jj_c]);
            h_new = grid.cell_size_z_ * std::floor(
                (h_new + tol) / grid.cell_size_z_);
            h_soil = sim_out->body_soil_[ind+1][ii][jj] - h_new;

            // Checking amount of soil in body_soil_pos_
            if (h_soil > sim_out->body_soil_pos_[nn].h_soil) {
                // Not enough soil in body_soil_pos_
                h_soil = sim_out->body_soil_pos_[nn].h_soil;
                h_new = sim_out->body_soil_[ind+1][ii][jj] - h_soil;
            }
            h_new_c = sim_out->body_[1][ii_c][jj_c] + h_soil;

            sim_out->body_soil_[0][ii_c][jj_c] = sim_out->body_[1][ii_c][jj_c];
            if (sim_out->body_[0][ii_c][jj_c] > sim_out->body_[2][ii_c][jj_c]) {
                // Soil should avalanche on the top layer
                sim_out->body_soil_[1][ii_c][jj_c] = h_new_c;
                if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
                    // Soil on the body should partially avalanche
                    sim_out->body_soil_[ind+1][ii][jj] = h_new;
                    sim_out->body_soil_pos_[nn].h_soil -= h_soil;
                } else {
                    // All soil on the body should avalanche
                    sim_out->body_soil_[ind][ii][jj] = 0.0;
                    sim_out->body_soil_[ind+1][ii][jj] = 0.0;
                    sim_out->body_soil_pos_[nn].h_soil = 0.0;
                }
            } else {
                // Soil should avalanche on the bottom layer
                if (h_new - tol > sim_out->body_soil_[ind][ii][jj]) {
                    // Soil on the body should partially avalanche
                    if (h_new_c - tol > sim_out->body_[2][ii_c][jj_c]) {
                        // Not enough space available
                        h_soil = (
                            sim_out->body_[2][ii_c][jj_c] -
                            sim_out->body_[1][ii_c][jj_c]);
                        sim_out->body_soil_[ind+1][ii][jj] -= h_soil;
                        sim_out->body_soil_[1][ii_c][jj_c] = (
                            sim_out->body_[2][ii_c][jj_c]);
                    } else {
                        // Enough space for the partial avalanche
                        sim_out->body_soil_[1][ii_c][jj_c] = h_new_c;
                        sim_out->body_soil_[ind+1][ii][jj] = h_new;
                    }
                    sim_out->body_soil_pos_[nn].h_soil -= h_soil;
                } else {
                    // All soil on the body may avalanche
                    // By construction, it must have enough space for
                    // the full avalanche
                    h_new_c = (
                        sim_out->body_[1][ii_c][jj_c] +
                        sim_out->body_soil_[ind+1][ii][jj] -
                        sim_out->body_soil_[ind][ii][jj]);

                    sim_out->body_soil_[1][ii_c][jj_c] = h_new_c;
                    sim_out->body_soil_[ind][ii][jj] = 0.0;
                    sim_out->body_soil_[ind+1][ii][jj] = 0.0;
                    sim_out->body_soil_pos_[nn].h_soil = 0.0;
                }
            }

            // Calculating pos of cell in body frame
            auto pos = soil_simulator::CalcBodyFramePos(
                ii_c, jj_c, sim_out->body_[1][ii_c][jj_c], grid, body);

            // Adding new body soil position to body_soil_pos
            body_soil_pos->push_back(soil_simulator::body_soil
                {0, ii_c, jj_c, pos[0], pos[1], pos[2], h_soil});
        }
    }
}
