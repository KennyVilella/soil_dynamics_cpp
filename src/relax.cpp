/*
This file implements the functions used for the terrain relaxation.

Copyright, 2023, Vilella Kenny.
*/
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
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

int soil_simulator::CheckUnstableTerrainCell(
    SimOut* sim_out, int ii_c, int jj_c, float h_min, float tol
) {
    return 0;
}

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
    // Converting status into a char for convenience
    std::string st = std::to_string(status);

    // Calculating new height values
    float h_new = 0.5 * (
        dh_max + sim_out->terrain_[ii][jj] + sim_out->terrain_[ii_c][jj_c]);
    h_new = grid.cell_size_z_ * std::floor((h_new + tol) / grid.cell_size_z_);

    float h_new_c = (
        sim_out->terrain_[ii][jj] + sim_out->terrain_[ii_c][jj_c] - h_new);

    if (status == 400) {
        // No Bucket
        // Updating terrain
        sim_out->terrain_[ii][jj] = h_new;
        sim_out->terrain_[ii_c][jj_c] = h_new_c;
    } else if (st[2] == '1') {
        // Space under the bucket
        float bucket_bot;
        if (st[0] == '1') {
            // Under the first bucket layer
            bucket_bot = sim_out->body_[0][ii_c][jj_c];
        } else if (st[0] == '2') {
            // Under the second bucket layer
            bucket_bot = sim_out->body_[2][ii_c][jj_c];
        } else if (st[0] == '3') {
            // Two bucket layers present
            bucket_bot = std::min(
                {sim_out->body_[0][ii_c][jj_c], sim_out->body_[2][ii_c][jj_c]});
        }

        if (h_new_c < bucket_bot) {
            // Full avalanche
            sim_out->terrain_[ii][jj] = h_new;
            sim_out->terrain_[ii_c][jj_c] = h_new_c;
        } else {
            // Partial avalanche
            sim_out->terrain_[ii][jj] = (
                sim_out->terrain_[ii][jj] + sim_out->terrain_[ii_c][jj_c] -
                bucket_bot);
            sim_out->terrain_[ii_c][jj_c] = bucket_bot;
        }
    } else if (st[2] == '2') {
        // Soil should avalanche on the bucket
        if (st[1] == '1') {
            // Soil avalanche on the second bucket soil layer
            h_new = 0.5 * (
                dh_max + sim_out->terrain_[ii][jj] +
                sim_out->body_soil_[3][ii_c][jj_c]);
            h_new = grid.cell_size_z_ * std::floor(
                (h_new + tol) / grid.cell_size_z_);
            h_new_c = (
                sim_out->terrain_[ii][jj] + sim_out->body_soil_[3][ii_c][jj_c] -
                h_new);

            // Updating terrain
            sim_out->terrain_[ii][jj] = h_new;
            sim_out->body_soil_[3][ii_c][jj_c] = h_new_c;
        } else if (st[1] == '2') {
            // Soil avalanche on the second bucket layer
            h_new = 0.5 * (
                dh_max + sim_out->terrain_[ii][jj] +
                sim_out->body_[3][ii_c][jj_c]);
            h_new = grid.cell_size_z_ * std::floor(
                (h_new + tol) / grid.cell_size_z_);
            h_new_c = (
                sim_out->terrain_[ii][jj] + sim_out->body_[3][ii_c][jj_c] -
                h_new);

            // Updating terrain
            sim_out->terrain_[ii][jj] = h_new;
            sim_out->body_soil_[2][ii_c][jj_c] = sim_out->body_[3][ii_c][jj_c];
            sim_out->body_soil_[3][ii_c][jj_c] = h_new_c;

            // Adding new bucket soil position to body_soil_pos
            sim_out->body_soil_pos_.push_back(std::vector<int> {2, ii_c, jj_c});
        } else if (st[1] == '3') {
            // Soil avalanche on the first bucket soil layer
            h_new = 0.5 * (
                dh_max + sim_out->terrain_[ii][jj] +
                sim_out->body_soil_[1][ii_c][jj_c]);
            h_new = grid.cell_size_z_ * std::floor(
                (h_new + tol) / grid.cell_size_z_);
            h_new_c = (
                sim_out->terrain_[ii][jj] + sim_out->body_soil_[1][ii_c][jj_c] -
                h_new);

            // Updating terrain
            sim_out->terrain_[ii][jj] = h_new;
            sim_out->body_soil_[1][ii_c][jj_c] = h_new_c;
        } else if (st[1] == '4') {
            // Soil avalanche on the first bucket layer
            h_new = 0.5 * (
                dh_max + sim_out->terrain_[ii][jj] +
                sim_out->body_[1][ii_c][jj_c]);
            h_new = grid.cell_size_z_ * std::floor(
                (h_new + tol) / grid.cell_size_z_);
            h_new_c = (
                sim_out->terrain_[ii][jj] + sim_out->body_[1][ii_c][jj_c] -
                h_new);

            // Updating terrain
            sim_out->terrain_[ii][jj] = h_new;
            sim_out->body_soil_[0][ii_c][jj_c] = sim_out->body_[1][ii_c][jj_c];
            sim_out->body_soil_[1][ii_c][jj_c] = h_new_c;

            // Adding new bucket soil position to body_soil_pos
            sim_out->body_soil_pos_.push_back(std::vector<int> {0, ii_c, jj_c});
        }
    }
}

void soil_simulator::RelaxUnstableBodyCell(
    SimOut* sim_out, int status, std::vector<std::vector<int>> body_soil_pos,
    float dh_max, int ii, int jj, int ind, int ii_c, int jj_c, Grid grid,
    float tol
) {
}
