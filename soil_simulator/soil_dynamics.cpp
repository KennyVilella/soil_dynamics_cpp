/*
This file implements the main functions of the simulator.

Copyright, 2023, Vilella Kenny.
*/
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include "soil_simulator/soil_dynamics.hpp"
#include "soil_simulator/types.hpp"
#include "soil_simulator/body_pos.hpp"
#include "soil_simulator/body_soil.hpp"
#include "soil_simulator/intersecting_cells.hpp"
#include "soil_simulator/relax.hpp"
#include "soil_simulator/utils.hpp"

void soil_simulator::SoilDynamics::Init(
    SimOut* sim_out, Grid grid, float amp_noise
) {
    // Generating a random permutation table
    std::vector<int> perm_table(256);
    // Filling table
    for (auto ii = 0; ii < 256; ii++)
        perm_table[ii] = ii;
    // Randomizing table
    // random_suffle is not used because it is machine dependent,
    // which makes unit testing difficult
    for (auto aa = 255; aa > 0; aa--) {
        std::uniform_int_distribution<int> dist(0, aa);
        int bb = dist(rng);
        std::swap(perm_table[aa], perm_table[bb]);
    }

    // Generating initial terrain using Simplex noise
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++) {
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++) {
            // Setting grid position rescaled to [0, 1]
            float x = (
                (grid.vect_x_[0] - grid.vect_x_[ii]) / (2 * grid.vect_x_[0]));
            float y = (
                (grid.vect_y_[0] - grid.vect_y_[jj]) / (2 * grid.vect_y_[0]));

            // Calculating Simplex noise using several frequencies
            float noise_value = amp_noise * (
                0.5 * soil_simulator::simplex_noise(1*x, 1*y, perm_table) +
                0.25 * soil_simulator::simplex_noise(2*x, 2*y, perm_table) +
                0.125 * soil_simulator::simplex_noise(4*x, 4*y, perm_table) +
                0.0625 * soil_simulator::simplex_noise(8*x, 8*y, perm_table));

            // Rounding noise_value to grid values
            noise_value = (
                grid.cell_size_z_ * round(noise_value / grid.cell_size_z_));

            // Setting terrain with noise
            sim_out->terrain_[ii][jj] = noise_value;
        }
    }
}

bool soil_simulator::SoilDynamics::Step(
    SimOut* sim_out, std::vector<float> pos, std::vector<float> ori,
    Grid grid, Bucket* bucket, SimParam sim_param, float tol
) {
    // Checking movement made by the bucket
    auto soil_update = soil_simulator::CheckBucketMovement(
        pos, ori, grid, bucket);

    if (!soil_update) {
        // Bucket has not moved enough
        return false;
    }

    // Updating bucket position
    soil_simulator::CalcBodyPos(
        sim_out, pos, ori, grid, bucket, sim_param, tol);

    // Updating position of soil resting on the bucket
    soil_simulator::UpdateBodySoil(sim_out, pos, ori, grid, bucket, tol);

    // Moving intersecting soil cells
    soil_simulator::MoveIntersectingCells(sim_out, grid, bucket, tol);

    // Assuming that the terrain is not at equilibrium
    sim_out->equilibrium_ = false;

    // Iterating until equilibrium or maximum number of iterations is reached
    int it = 0;
    while (!sim_out->equilibrium_ && it < sim_param.max_iterations_) {
        it++;

        // Updating impact_area
        sim_out->impact_area_[0][0] = std::min(
            sim_out->bucket_area_[0][0], sim_out->relax_area_[0][0]);
        sim_out->impact_area_[1][0] = std::min(
            sim_out->bucket_area_[1][0], sim_out->relax_area_[1][0]);
        sim_out->impact_area_[0][1] = std::max(
            sim_out->bucket_area_[0][1], sim_out->relax_area_[0][1]);
        sim_out->impact_area_[1][1] = std::max(
            sim_out->bucket_area_[1][1], sim_out->relax_area_[1][1]);

        // Relaxing the terrain
        RelaxTerrain(sim_out, grid, bucket, sim_param, tol);

        // Randomizing body_soil_pos_ to reduce asymmetry
        // random_suffle is not used because it is machine dependent,
        // which makes unit testing difficult
        for (int aa = sim_out->body_soil_pos_.size() - 1; aa > 0; aa--) {
            std::uniform_int_distribution<int> dist(0, aa);
            int bb = dist(rng);
            std::swap(sim_out->body_soil_pos_[aa], sim_out->body_soil_pos_[bb]);
        }

        // Relaxing the soil resting on the bucket
        RelaxBodySoil(sim_out, grid, bucket, sim_param, tol);
    }
    return true;
}

void soil_simulator::SoilDynamics::Check(
    SimOut* sim_out, float init_volume, Grid grid, float tol
) {
    // Checking mass conservation
    soil_simulator::CheckVolume(sim_out, init_volume, grid, tol);

    // Checking consistency of simulation outputs
    soil_simulator::CheckSoil(sim_out, tol);
}

void soil_simulator::SoilDynamics::WriteOutputs(
        SimOut* sim_out, Grid grid, Bucket* bucket
) {
    // Writing terrain_ and body_soil_
    soil_simulator::WriteSoil(sim_out, grid);

    // Writing bucket corners
    soil_simulator::WriteBucket(bucket);
}
