/*
This file implements the functions used to move the soil resting on the bucket
following its movement.

Copyright, 2023, Vilella Kenny.
*/
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include "src/body_soil.hpp"
#include "src/types.hpp"
#include "src/utils.hpp"

/// In this function, the movement applied to the base of the soil column is
/// calculated and the soil is moved to this new location. It is however
/// difficult to track accurately each bucket wall. This is currently done by
/// looking at the height difference between the previous and new soil
/// locations, if this height difference is lower than `cell_size_xy_`, it is
/// assumed to be the same bucket wall. Some errors may however be present and
/// further testing is required. If no bucket wall is present, the soil is moved
/// down to the terrain.
///
/// The new positions of the soil resting on the bucket are collected into
/// `sim_out.body_soil_pos_` and duplicates are removed.
void soil_simulator::UpdateBodySoil(
    SimOut* sim_out, std::vector<float> pos, std::vector<float> ori, Grid grid,
    Bucket* bucket, float tol
) {
    // Removing duplicates in body_soil_pos
    sort(sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());
    sim_out->body_soil_pos_.erase(unique(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end()),
        sim_out->body_soil_pos_.end());

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
    for (auto nn = 0; nn < old_body_soil_pos.size(); nn++) {
        int ind = old_body_soil_pos[nn][0];
        int ii = old_body_soil_pos[nn][1];
        int jj = old_body_soil_pos[nn][2];

        if (
            (old_body_soil[ind][ii][jj] == 0.0) &&
            (old_body_soil[ind+1][ii][jj] == 0.0)) {
            // No bucket soil at that position
            continue;
        }

        // Calculating cell's position in bucket frame
        std::vector<float> cell_pos = {
            grid.vect_x_[ii] - bucket->pos_[0],
            grid.vect_y_[jj] - bucket->pos_[1],
            old_body_soil[ind][ii][jj] - bucket->pos_[2]};

        // Inversing rotation
        std::vector<float> inv_ori = {
            bucket->ori_[0], -bucket->ori_[1], -bucket->ori_[2],
            -bucket->ori_[3]};

        // Calculating reference position of cell in bucket frame
        auto cell_local_pos = soil_simulator::CalcRotationQuaternion(
            inv_ori, cell_pos);

        // Calculating new cell position
        auto new_cell_pos = soil_simulator::CalcRotationQuaternion(
            ori, cell_local_pos);
        new_cell_pos[0] += pos[0];
        new_cell_pos[1] += pos[1];
        new_cell_pos[2] += pos[2];

        // Calculating new cell indices
        int ii_n = static_cast<int>(round(
            new_cell_pos[0] / grid.cell_size_xy_ + grid.half_length_x_));
        int jj_n = static_cast<int>(round(
            new_cell_pos[1] / grid.cell_size_xy_ + grid.half_length_y_));

        if (
            ((sim_out->body_[0][ii_n][jj_n] != 0.0) ||
            (sim_out->body_[1][ii_n][jj_n] != 0.0)) &&
            (abs(new_cell_pos[2] - sim_out->body_[1][ii_n][jj_n]) - tol
                < grid.cell_size_xy_)
        ) {
            // Bucket is present
            // Moving body_soil to new location
            sim_out->body_soil_[1][ii_n][jj_n] += (
                (sim_out->body_[1][ii_n][jj_n] -
                sim_out->body_soil_[0][ii_n][jj_n]) +
                (old_body_soil[ind+1][ii][jj] - old_body_soil[ind][ii][jj]));
            sim_out->body_soil_[0][ii_n][jj_n] = sim_out->body_[1][ii_n][jj_n];

            // Adding position to body_soil_pos
            sim_out->body_soil_pos_.insert(sim_out->body_soil_pos_.end(),
                std::vector<int> {0, ii_n, jj_n});
        } else if (
            ((sim_out->body_[2][ii_n][jj_n] != 0.0) ||
            (sim_out->body_[3][ii_n][jj_n] != 0.0)) &&
            (abs(new_cell_pos[2] - sim_out->body_[3][ii_n][jj_n]) - tol
                < grid.cell_size_xy_)
        ) {
            // Bucket is present
            // Moving body_soil to new location
            sim_out->body_soil_[3][ii_n][jj_n] += (
                (sim_out->body_[3][ii_n][jj_n] -
                sim_out->body_soil_[2][ii_n][jj_n]) +
                (old_body_soil[ind+1][ii][jj] - old_body_soil[ind][ii][jj]));
            sim_out->body_soil_[2][ii_n][jj_n] = sim_out->body_[3][ii_n][jj_n];

            // Adding position to body_soil_pos
            sim_out->body_soil_pos_.insert(sim_out->body_soil_pos_.end(),
                std::vector<int> {2, ii_n, jj_n});
        } else {
            // Bucket is not present
            // Moving body_soil to terrain
            // This may be problematic because another bucket wall may
            // interfere, but it seems to be a very edge case
            sim_out->terrain_[ii_n][jj_n] += (
                old_body_soil[ind+1][ii][jj] - old_body_soil[ind][ii][jj]);
        }
    }

    // Removing duplicates in body_soil_pos
    sort(sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());
    sim_out->body_soil_pos_.erase(unique(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end()),
        sim_out->body_soil_pos_.end());

    // Updating new bucket position
    bucket->pos_ = pos;
    bucket->ori_ = ori;
}
