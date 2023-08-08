/*
This file implements the functions used for the terrain relaxation.

Copyright, 2023, Vilella Kenny.
*/
#include <vector>
#include "src/relax.hpp"
#include "src/types.hpp"

void soil_simulator::RelaxTerrain(
    SimOut* sim_out, Grid grid, SimParam sim_param, float tol
) {
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
    // Determining presence of bucket
    bool bucket_absence_1 = (
        (sim_out->body_[0][ii_c][jj_c] == 0.0) &&
        (sim_out->body_[1][ii_c][jj_c] == 0.0));
    bool bucket_absence_3 = (
        (sim_out->body_[2][ii_c][jj_c] == 0.0) &&
        (sim_out->body_[3][ii_c][jj_c] == 0.0));
    int status;
    float column_top;

    if (bucket_absence_1 && bucket_absence_3) {
        // No bucket
        if (sim_out->terrain_[ii_c][jj_c] + tol < h_min) {
            return 40;
        }
    } else if (bucket_absence_1) {
        // Only the second bucket layer
        status = 20;

        if (
            (sim_out->body_soil_[2][ii_c][jj_c] != 0.0) ||
            (sim_out->body_soil_[3][ii_c][jj_c] != 0.0)) {
            // Bucket soil is present
            status += 1;
            column_top = sim_out->body_soil_[3][ii_c][jj_c];
        } else {
            // Bucket soil is not present
            status += 2;
            column_top = sim_out->body_[3][ii_c][jj_c];
        }

        if (column_top + tol < h_min) {
            // Column is low enough
            return status;
        }
    } else if (bucket_absence_3) {
        // Only the first bucket layer
        status = 10;

        if (
            (sim_out->body_soil_[0][ii_c][jj_c] != 0.0) ||
            (sim_out->body_soil_[1][ii_c][jj_c] != 0.0)
        ) {
            // Bucket soil is present
            status += 3;
            column_top = sim_out->body_soil_[1][ii_c][jj_c];
        } else {
            // Bucket soil is not present
            status += 4;
            column_top = sim_out->body_[1][ii_c][jj_c];
        }

        if (column_top + tol < h_min){
            // Column is low enough
            return status;
        }
    } else {
        // Both bucket layers are present
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
                // Bucket soil is present
                if (
                    sim_out->body_soil_[ind_n_bot+1][ii_c][jj_c] + tol <
                    sim_out->body_[ind_n_top][ii_c][jj_c]) {
                    // Some space is avilable
                    status += ind_n_top + 1;
                    column_top = sim_out->body_soil_[ind_n_bot+1][ii_c][jj_c];
                }
            } else {
                // Bucket soil is not present
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
                // Bucket soil is present
                status += ind_n_bot + 1;
                column_top = sim_out->body_soil_[ind_n_top+1][ii_c][jj_c];
            } else {
                // Bucket soil is not present
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
