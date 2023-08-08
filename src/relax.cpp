/*
This file implements the functions used for the terrain relaxation.

Copyright, 2023, Vilella Kenny.
*/
#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>
#include "src/relax.hpp"
#include "src/types.hpp"

void soil_simulator::RelaxTerrain(
    SimOut* sim_out, Grid grid, SimParam sim_param, float tol
) {
    // Assuming that the terrain is at equilibrium
    sim_out->equilibrium_ = true;

    // Calculating the maximum slope allowed by the repose angle
    float slope_max = std::tan(sim.repose_angle_);
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
                sim_out, status, dh_max, ii, jj, ii_c, jj_c, grid, tol);
        }
    }

    // Updating relax_area
    sim_out->relax_area_[0][0] = std::max(relax_min_x - sim->cell_buffer_, 2);
    sim_out->relax_area_[0][1] = std::min(
        relax_max_x + sim.cell_buffer_, 2 * grid.half_length_x_);
    sim_out->relax_area_[1][0] = std::max(relax_min_y - sim.cell_buffer_, 2);
    sim_out->relax_area_[1][1] = std::min(
        relax_max_y + sim.cell_buffer_, 2 * grid.half_length_y_);
}

void soil_simulator::RelaxBodySoil(
    SimOut* sim_out, Grid grid, SimParam sim_param, float tol
) {
}

/// It is important to note that the cells selected by this function are not
/// necessarily unstable, as a bucket or the soil resting on it could be
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

/// In case of instability, this function returns a three-digit number
/// (`status`) that provides information on how the soil should avalanche.
/// The interpretation of the three-digit number is described below.
///
/// The first digit indicates the potential presence of the bucket:
/// - 1 when the first bucket layer is present.
/// - 2 when the second bucket layer is present.
/// - 3 when the two bucket layers are present.
/// - 4 when no bucket layer is present.
///
/// The second digit indicates the layer at the top where the soil should
/// avalanche:
/// - 0 when it is the `terrain` (no bucket is present).
/// - 1 when it is the second bucket soil layer.
/// - 2 when it is the second bucket layer.
/// - 3 when it is the first bucket soil layer.
/// - 4 when it is the first bucket layer.
///
/// The third digit indicates whether the soil should avalanche below or above
/// the bucket:
/// - 0 when there is no bucket.
/// - 1 when the soil should avalanche below the bucket.
/// - 2 when the soil should avalanche on the top of the bucket.
///
/// The combination of these three digits provides a comprehensive description
/// of how the soil should avalanche in different scenarios.
///
/// Note that not all combinations for `status` are possible. Some combinations,
/// such as `401`, `231` and `220`, are impossible.
int soil_simulator::CheckUnstableTerrainCell(
    SimOut* sim_out, int ii_c, int jj_c, float h_min, float tol
) {
    // Checking whether soil is low enough, because unstability may have
    // already been solved
    if (sim_out->terrain_[ii_c][jj_c] + tol < h_min) {
        // Adjacent terrain is low enough
        bool bucket_presence_1 = (
            (sim_out->body_[0][ii_c][jj_c] != 0.0) ||
            (sim_out->body_[1][ii_c][jj_c] != 0.0));
        bool bucket_presence_3 = (
            (sim_out->body_[2][ii_c][jj_c] != 0.0) ||
            (sim_out->body_[3][ii_c][jj_c] != 0.0));
        int status;
        float column_top;
        float bucket_bot;

        if (bucket_presence_1 || bucket_presence_3) {
            // Bucket is present
            // Calculating extension of bucket and soil
            if (!bucket_presence_1) {
                // Only the second bucket layer is present
                status = 200;
                bucket_bot = sim_out->body_[2][ii_c][jj_c];
                if (
                    (sim_out->body_soil_[2][ii_c][jj_c] != 0.0) ||
                    (sim_out->body_soil_[3][ii_c][jj_c] != 0.0)
                ) {
                    // Bucket soil is present
                    status += 10;
                    column_top = sim_out->body_soil_[3][ii_c][jj_c];
                } else {
                    // Bucket soil is not present
                    status += 20;
                    column_top = sim_out->body_[3][ii_c][jj_c];
                }
            } else if (!bucket_presence_3) {
                // Only the first bucket layer is present
                status = 100;
                bucket_bot = sim_out->body_[0][ii_c][jj_c];
                if (
                    (sim_out->body_soil_[0][ii_c][jj_c] != 0.0) ||
                    (sim_out->body_soil_[1][ii_c][jj_c] != 0.0)
                ) {
                    // Bucket soil is present
                    status += 30;
                    column_top = sim_out->body_soil_[1][ii_c][jj_c];
                } else {
                    // Bucket soil is not present
                    status += 40;
                    column_top = sim_out->body_[1][ii_c][jj_c];
                }
            } else {
                // Two bucket layers are present
                status = 300;

                // Checking which layer is lower
                if (
                    sim_out->body_[0][ii_c][jj_c] <
                    sim_out->body_[2][ii_c][jj_c]
                ) {
                    // First bucket layer is lower
                    bucket_bot = sim_out->body_[0][ii_c][jj_c];

                    if (
                        (sim_out->body_soil_[2][ii_c][jj_c] != 0.0) ||
                        (sim_out->body_soil_[3][ii_c][jj_c] != 0.0)
                    ) {
                        // Bucket soil is present
                        status += 10;
                        column_top = sim_out->body_soil_[3][ii_c][jj_c];
                    } else {
                        // Bucket soil is not present
                        status += 20;
                        column_top = sim_out->body_[3][ii_c][jj_c];
                    }
                } else {
                    // Second bucket layer is lower
                    bucket_bot = sim_out->body_[2][ii_c][jj_c];

                    if (
                        (sim_out->body_soil_[0][ii_c][jj_c] != 0.0) ||
                        (sim_out->body_soil_[1][ii_c][jj_c] != 0.0)
                    ) {
                        // Bucket soil is present
                        status += 30;
                        column_top = sim_out->body_soil_[1][ii_c][jj_c];
                    } else {
                        // Bucket soil is not present
                        status += 40;
                        column_top = sim_out->body_[1][ii_c][jj_c];
                    }
                }
            }

            if (sim_out->terrain_[ii_c][jj_c] + tol < bucket_bot) {
                // Space under the bucket
                return status + 1;
            }

            if (column_top + tol < h_min) {
                // Column is low enough
                return status + 2;
            }
        } else {
            // No bucket
            return 400;
        }
    }

    return 0;
}

/// In case of instability, this function returns a two-digit number (`status`)
/// that provides information on how the soil should avalanche. The
/// interpretation of the two-digit number is described below.
///
/// The first digit indicates the potential presence of the bucket:
/// - 1 when the first bucket layer is present.
/// - 2 when the second bucket layer is present.
/// - 3 when the two bucket layers are present.
/// - 4 when no bucket layer is present.
///
/// The second digit indicates the layer where the soil should avalanche:
/// - 0 when it is the `terrain` (no bucket is present).
/// - 1 when it is the second bucket soil layer.
/// - 2 when it is the second bucket layer.
/// - 3 when it is the first bucket soil layer.
/// - 4 when it is the first bucket layer.
///
/// The combination of these two digits provides a comprehensive description of
/// how the soil should avalanche in different scenarios.
///
/// Note that not all combinations for `status` are possible.
/// Some combinations, such as `41` and `23` are impossible.
int soil_simulator::CheckUnstableBodyCell(
    SimOut* sim_out, int ii, int jj, int ind, int ii_c, int jj_c, float h_min,
    float tol
) {
    return 0;
}

void soil_simulator::RelaxUnstableTerrainCell(
    SimOut* sim_out, int status, float dh_max, int ii, int jj, int ii_c,
    int jj_c, Grid grid, float tol
) {
}

void soil_simulator::RelaxUnstableBodyCell(
    SimOut* sim_out, int status, std::vector<std::vector<int>> body_soil_pos,
    float dh_max, int ii, int jj, int ind, int ii_c, int jj_c, Grid grid,
    float tol
) {
}
