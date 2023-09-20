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
#include "soil_simulator/bucket_pos.hpp"
#include "soil_simulator/body_soil.hpp"
#include "soil_simulator/intersecting_cells.hpp"
#include "soil_simulator/relax.hpp"
#include "soil_simulator/utils.hpp"

bool soil_simulator::SoilDynamics::step(
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
    soil_simulator::CalcBucketPos(
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

        // Randomizing body_soil_pos to reduce asymmetry
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

void soil_simulator::SoilDynamics::check(
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
