/*
This file implements the functions used to move the soil resting on the body
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
/// position of the soil column in the body frame is stored in the
/// `body_soil_pos_` member of the `SimOut` class.
///
/// It is difficult to track accurately each body wall. This is currently done
/// by determining the most likely location considering the movement made, and
/// moving the soil to this location if the vertical distance is lower than a
/// cell height. If it is not, the nine neighbouring cells are investigated.
/// If no neighbouring cell has a body wall located at a distance lower than
/// a cell height, the soil is moved to the cell with the lowest vertical
/// distance. Note that it may still potentially lead to an incorrect choice.
///
/// If no body wall is present, the soil is moved down to the terrain and a
/// warning is issued as it should normally not happen.
///
/// The new positions of the soil resting on the body are collected into
/// `sim_out.body_soil_pos_` along with the required information using the
/// `body_soil` struct.
void soil_simulator::UpdateBodySoil(
    SimOut* sim_out, std::vector<float> pos, std::vector<float> ori, Grid grid,
    Body* body, float tol
) {
    // Copying previous body_soil locations
    auto old_body_soil_pos = sim_out->body_soil_pos_;

    // Resetting body_soil
    for (auto nn = 0; nn < old_body_soil_pos.size(); nn++) {
        int ind = old_body_soil_pos[nn].ind;
        int ii = old_body_soil_pos[nn].ii;
        int jj = old_body_soil_pos[nn].jj;
        sim_out->body_soil_[ind][ii][jj] = 0.0;
        sim_out->body_soil_[ind+1][ii][jj] = 0.0;
    }

    // Resetting body_soil_pos
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // Iterating over all XY positions where body_soil is present
    float min_cell_height_diff = grid.cell_size_z_ + tol;
    for (auto nn = 0; nn < old_body_soil_pos.size(); nn++) {
        int ii = old_body_soil_pos[nn].ii;
        int jj = old_body_soil_pos[nn].jj;
        float x_b = old_body_soil_pos[nn].x_b;
        float y_b = old_body_soil_pos[nn].y_b;
        float z_b = old_body_soil_pos[nn].z_b;
        float h_soil = old_body_soil_pos[nn].h_soil;

        if (h_soil < 0.9 * grid.cell_size_z_) {
            // No soil to be moved
            // 0.9 has been chosen arbitrarily to account for potential
            // numerical errors, another value could be used
            continue;
        }

        // Converting h_soil to a multiple of cell_size_z to deal with
        // accumulating floating errors
        h_soil = grid.cell_size_z_ * round(h_soil / grid.cell_size_z_);

        // Calculating new cell position
        std::vector<float> cell_pos = {x_b, y_b, z_b};
        auto new_cell_pos = soil_simulator::CalcRotationQuaternion(
            ori, cell_pos);
        auto old_cell_pos = soil_simulator::CalcRotationQuaternion(
            body->ori_, cell_pos);

        // Calculating new cell position in global frame
        new_cell_pos[0] += pos[0];
        new_cell_pos[1] += pos[1];
        new_cell_pos[2] += pos[2];
        old_cell_pos[0] += body->pos_[0];
        old_cell_pos[1] += body->pos_[1];
        old_cell_pos[2] += body->pos_[2];

        // Establishing order of exploration
        std::vector<std::vector<int>> directions;
        float dx = new_cell_pos[0] - old_cell_pos[0];
        float dy = new_cell_pos[1] - old_cell_pos[1];
        int sx = copysign(1, dx);
        int sy = copysign(1, dy);
        if (std::abs(dx) > std::abs(dy)) {
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

        // Calculating new cell indices
        int ii_n = static_cast<int>(round(
            new_cell_pos[0] / grid.cell_size_xy_ + grid.half_length_x_));
        int jj_n = static_cast<int>(round(
            new_cell_pos[1] / grid.cell_size_xy_ + grid.half_length_y_));

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

            // Detecting presence of body
            bool body_presence_1 = (
                (sim_out->body_[0][ii_t][jj_t] != 0.0) ||
                (sim_out->body_[1][ii_t][jj_t] != 0.0));
            bool body_presence_3 = (
                (sim_out->body_[2][ii_t][jj_t] != 0.0) ||
                (sim_out->body_[3][ii_t][jj_t] != 0.0));

            if (body_presence_1) {
                // First body layer is present
                float dist = (
                    std::abs(new_cell_pos[2] - sim_out->body_[1][ii_t][jj_t]));
                if (dist < min_cell_height_diff) {
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
            if (body_presence_3) {
                // Second body layer is present
                float dist = (
                    std::abs(new_cell_pos[2] - sim_out->body_[3][ii_t][jj_t]));
                if (dist < min_cell_height_diff) {
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
                LOG(WARNING) << "WARNING\nBody soil could not be updated.\n"
                    "Soil is moved to the terrain to maintain mass "
                    "conservation.";
            }
        }
    }

    // Updating new body position
    body->pos_ = pos;
    body->ori_ = ori;
}
