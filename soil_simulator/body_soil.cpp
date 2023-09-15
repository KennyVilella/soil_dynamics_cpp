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
/// by looking at the height difference between the previous and new soil
/// locations, if this height difference is lower than twice the minimum cell
/// size, it is assumed to be the same bucket wall.
///
/// If no bucket wall is present, the soil is moved down to the terrain and a
/// warning is issued as it should normally not happen.
///
/// The new positions of the soil resting on the bucket are collected into
/// `sim_out.body_soil_pos_` along with the required information.
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
    float min_cell_height_diff = 2 * std::min(
        grid.cell_size_z_, grid.cell_size_xy_);
    for (auto nn = 0; nn < old_body_soil_pos.size(); nn++) {
        int ind = old_body_soil_pos[nn].ind;
        int ii = old_body_soil_pos[nn].ii;
        int jj = old_body_soil_pos[nn].jj;
        float x_b = old_body_soil_pos[nn].x_b;
        float y_b = old_body_soil_pos[nn].y_b;
        float z_b = old_body_soil_pos[nn].z_b;
        float h_soil = old_body_soil_pos[nn].h_soil;

        if (h_soil < tol) {
            // No soil tto be moved
            continue;
        }

        // Calculating new cell position
        std::vector<float> cell_pos = {x_b, y_b, z_b};
        auto new_cell_pos = soil_simulator::CalcRotationQuaternion(
            ori, cell_pos);

        // Calculating movement made by the bucket
        float dx = new_cell_pos[0] - cell_pos[0];
        float dy = new_cell_pos[1] - cell_pos[1];

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
        int sx = copysign(1, dx);
        int sy = copysign(1, dy);
        if (std::abs(dx) > std::abs(dy)) {
            directions = {
                {0, 0}, {sx, 0}, {sx, sy}, {0, sy}, {sx, -sy},
                {0, -sy}, {-sx, sy}, {-sx, 0}, {-sx, -sy}};
        } else {
            directions = {
                {0, 0}, {0, sy}, {sx, sy}, {sx, 0}, {-sx, sy},
                {-sx, 0}, {sx, -sy}, {0, -sy}, {-sx, -sy}};
        }

        bool soil_moved = false;
        for (auto xy = 0; xy < directions.size(); xy++) {
            // Determining cell to investigate
            int ii_t = ii_n + directions[xy][0];
            int jj_t = jj_n + directions[xy][1];
            if (
                ((sim_out->body_[0][ii_t][jj_t] != 0.0) ||
                (sim_out->body_[1][ii_t][jj_t] != 0.0)) &&
                (std::abs(new_cell_pos[2] - sim_out->body_[1][ii_t][jj_t]) - tol
                    < min_cell_height_diff)
            ) {
                // First bucket layer is present
                // Moving body_soil to new location
                sim_out->body_soil_[1][ii_t][jj_t] += (
                    sim_out->body_[1][ii_t][jj_t] -
                    sim_out->body_soil_[0][ii_t][jj_t] + h_soil);
                sim_out->body_soil_[0][ii_t][jj_t] = (
                    sim_out->body_[1][ii_t][jj_t]);

                // Calculating pos of cell in bucket frame
                auto pos = soil_simulator::CalcBucketFramePos(
                    ii_t, jj_t, sim_out->body_[1][ii_t][jj_t], grid, bucket);

                // Adding position to body_soil_pos
                sim_out->body_soil_pos_.push_back(soil_simulator::body_soil
                    {0, ii_t, jj_t, pos[0], pos[1], pos[2], h_soil});
                soil_moved = true;
                break;
            } else if (
                ((sim_out->body_[2][ii_t][jj_t] != 0.0) ||
                (sim_out->body_[3][ii_t][jj_t] != 0.0)) &&
                (std::abs(new_cell_pos[2] - sim_out->body_[3][ii_t][jj_t]) - tol
                    < min_cell_height_diff)
            ) {
                // Bucket is present
                // Moving body_soil to new location
                sim_out->body_soil_[3][ii_t][jj_t] += (
                    sim_out->body_[3][ii_t][jj_t] -
                    sim_out->body_soil_[2][ii_t][jj_t] + h_soil);
                sim_out->body_soil_[2][ii_t][jj_t] = (
                    sim_out->body_[3][ii_t][jj_t]);

                // Calculating pos of cell in bucket frame
                auto pos = soil_simulator::CalcBucketFramePos(
                    ii_t, jj_t, sim_out->body_[3][ii_t][jj_t], grid, bucket);

                // Adding position to body_soil_pos
                sim_out->body_soil_pos_.push_back(soil_simulator::body_soil
                     {2, ii_t, jj_t, pos[0], pos[1], pos[2], h_soil});
                soil_moved = true;
                break;
            }
        }
        if (!soil_moved) {
            // This should normally not happen, it is only for safety
            // Moving body_soil to terrain
            sim_out->terrain_[ii_n][jj_n] += (
                old_body_soil[ind+1][ii][jj] - old_body_soil[ind][ii][jj]);
            LOG(WARNING) << "WARNING\nBucket soil could not be updated.\n "
                "Soil is moved to the terrain to maintain mass conservation";
        }
    }

    // Updating new bucket position
    bucket->pos_ = pos;
    bucket->ori_ = ori;
}
