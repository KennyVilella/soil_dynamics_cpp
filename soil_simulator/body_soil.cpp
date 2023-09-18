/*
This file implements the functions used to move the soil resting on the bucket
following its movement.

Copyright, 2023, Vilella Kenny.
*/
#include <glog/logging.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include "soil_simulator/body_soil.hpp"
#include "soil_simulator/types.hpp"
#include "soil_simulator/utils.hpp"

/// In this function, the movement applied to the base of the soil column is
/// calculated and the soil is moved to this new location. The original
/// position of the soil column in the bucket frame is stored in the
/// `body_soil_pos_` member of the `SimOut` class.
///
/// It is difficult to track accurately each bucket wall. This is currently done
/// by choosing the bucket layer whose height difference is lower than a cell
/// height. If no bucket layer satisfies this condition, then it is moved to the
/// location with the lowest height difference. Note that tt may still
/// potentially lead to an incorrect choice.
///
/// If no bucket wall is present, the soil is moved down to the terrain and a
/// warning is issued as it should normally not happen.
///
/// The new positions of the soil resting on the bucket are collected into
/// `sim_out.body_soil_pos_` along with the required information using the
/// `body_soil` struct.
void soil_simulator::UpdateBodySoil(
    SimOut* sim_out, std::vector<float> pos, std::vector<float> ori, Grid grid,
    Bucket* bucket, float tol
) {
    // Copying previous body_soil locations
    auto old_body_soil = sim_out->body_soil_;
    auto old_body_soil_pos = sim_out->body_soil_pos_;

    // Resetting body_soil
    for (auto ii = 0 ; ii < sim_out->body_soil_.size(); ii++)
        for (auto jj = 0 ; jj < sim_out->body_soil_[0].size(); jj++)
            std::fill(
                sim_out->body_soil_[ii][jj].begin(),
                sim_out->body_soil_[ii][jj].end(), 0.0);

    // Resetting body_soil_pos
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // Iterating over all XY positions where body_soil is present
    float min_cell_height_diff = grid.cell_size_z_;
    for (auto nn = 0; nn < old_body_soil_pos.size(); nn++) {
        int ind = old_body_soil_pos[nn].ind;
        int ii = old_body_soil_pos[nn].ii;
        int jj = old_body_soil_pos[nn].jj;
        float x_b = old_body_soil_pos[nn].x_b;
        float y_b = old_body_soil_pos[nn].y_b;
        float z_b = old_body_soil_pos[nn].z_b;
        float h_soil = old_body_soil_pos[nn].h_soil;

        if (h_soil < tol) {
            // No soil to be moved
            continue;
        }

        // Calculating new cell position
        std::vector<float> cell_pos = {x_b, y_b, z_b};
        auto new_cell_pos = soil_simulator::CalcRotationQuaternion(
            ori, cell_pos);

        // Calculating new cell position in global frame
        new_cell_pos[0] += pos[0];
        new_cell_pos[1] += pos[1];
        new_cell_pos[2] += pos[2];

        // Calculating new cell indices
        int ii_n = static_cast<int>(round(
            new_cell_pos[0] / grid.cell_size_xy_ + grid.half_length_x_));
        int jj_n = static_cast<int>(round(
            new_cell_pos[1] / grid.cell_size_xy_ + grid.half_length_y_));

        // Establishing order of exploration
        std::vector<std::vector<int>> directions;
        int sx;
        int sy;
        (ii_n > ii) ? sx = 1 : sx = -1;
        (jj_n > jj) ? sy = 1 : sy = -1;
        if (std::abs(sx) > std::abs(sy)) {
            // Main direction follows X
            directions = {
                {0, 0}, {sx, 0}, {sx, sy}, {0, sy}, {sx, -sy},
                {0, -sy}, {-sx, sy}, {-sx, 0}, {-sx, -sy}};
        } else {
            // Main direction follows Y
            directions = {
                {0, 0}, {0, sy}, {sx, sy}, {sx, 0}, {-sx, sy},
                {-sx, 0}, {sx, -sy}, {0, -sy}, {-sx, -sy}};
        }

        // Declaring variables used to store the closest location
        bool soil_moved = false;
        int ii_s;
        int jj_s;
        int ind_s;
        float dist_s = 2 * grid.half_length_z_;

        // Starting loop over neighbours
        for (auto xy = 0; xy < directions.size(); xy++) {
            // Determining cell to investigate
            int ii_t = ii_n + directions[xy][0];
            int jj_t = jj_n + directions[xy][1];

            // Detecting presence of bucket
            bool bucket_presence_1 = (
                (sim_out->body_[0][ii_t][jj_t] != 0.0) ||
                (sim_out->body_[1][ii_t][jj_t] != 0.0));
            bool bucket_presence_3 = (
                (sim_out->body_[2][ii_t][jj_t] != 0.0) ||
                (sim_out->body_[3][ii_t][jj_t] != 0.0));

            if (bucket_presence_1) {
                // First bucket layer is present
                float dist = (
                    std::abs(new_cell_pos[2] - sim_out->body_[1][ii_t][jj_t]));
                if (dist < min_cell_height_diff - tol) {
                    // Moving body_soil to new location, this implementation
                    // works regardless of the presence of body_soil
                    sim_out->body_soil_[1][ii_t][jj_t] += (
                        sim_out->body_[1][ii_t][jj_t] -
                        sim_out->body_soil_[0][ii_t][jj_t] + h_soil);
                    sim_out->body_soil_[0][ii_t][jj_t] = (
                        sim_out->body_[1][ii_t][jj_t]);

                    // Adding position to body_soil_pos
                    sim_out->body_soil_pos_.push_back(soil_simulator::body_soil
                        {0, ii_t, jj_t, x_b, y_b, z_b, h_soil});
                    soil_moved = true;
                    break;
                } else if (dist < dist_s) {
                    // Updating new default location
                    dist_s = dist;
                    ii_s = ii_t;
                    jj_s = jj_t;
                    ind_s = 1;
                }
            }
            if (bucket_presence_3) {
                // Second bucket layer is present
                float dist = (
                    std::abs(new_cell_pos[2] - sim_out->body_[3][ii_t][jj_t]));
                if (dist < min_cell_height_diff - tol) {
                    // Moving body_soil to new location, this implementation
                    // works regardless of the presence of body_soil
                    sim_out->body_soil_[3][ii_t][jj_t] += (
                        sim_out->body_[3][ii_t][jj_t] -
                        sim_out->body_soil_[2][ii_t][jj_t] + h_soil);
                    sim_out->body_soil_[2][ii_t][jj_t] = (
                        sim_out->body_[3][ii_t][jj_t]);

                    // Adding position to body_soil_pos
                    sim_out->body_soil_pos_.push_back(soil_simulator::body_soil
                         {2, ii_t, jj_t, x_b, y_b, z_b, h_soil});
                    soil_moved = true;
                    break;
                } else if (dist < dist_s) {
                    // Updating new default location
                    dist_s = dist;
                    ii_s = ii_t;
                    jj_s = jj_t;
                    ind_s = 3;
                }
            }
        }

        if (!soil_moved) {
            if (dist_s != 2 * grid.half_length_z_) {
                // Moving body_soil to closest location, this implementation
                // works regardless of the presence of body_soil
                sim_out->body_soil_[ind_s][ii_s][jj_s] += (
                    sim_out->body_[ind_s][ii_s][jj_s] -
                    sim_out->body_soil_[ind_s-1][ii_s][jj_s] + h_soil);
                sim_out->body_soil_[ind_s-1][ii_s][jj_s] = (
                    sim_out->body_[ind_s][ii_s][jj_s]);

                // Adding position to body_soil_pos
                sim_out->body_soil_pos_.push_back(soil_simulator::body_soil
                     {ind_s-1, ii_s, jj_s, x_b, y_b, z_b, h_soil});
            } else {
                // This should normally not happen, it is only for safety
                // Moving body_soil to terrain
                sim_out->terrain_[ii_n][jj_n] += h_soil;
                LOG(WARNING) << "WARNING\nBucket soil could not be updated.\n "
                   "Soil is moved to the terrain to maintain mass conservation";
            }
        }
    }

    // Updating new bucket position
    bucket->pos_ = pos;
    bucket->ori_ = ori;
}
