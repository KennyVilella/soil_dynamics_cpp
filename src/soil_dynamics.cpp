/*
This file implements the main functions of the simulator.

Copyright, 2023, Vilella Kenny.
*/
#include <iostream>
#include <random>
#include <vector>
#include "src/soil_dynamics.hpp"
#include "src/types.hpp"
#include "src/bucket_pos.hpp"
#include "src/body_soil.hpp"
#include "src/intersecting_cells.hpp"
#include "src/relax.hpp"
#include "src/utils.hpp"

// Defining RNG
std::mt19937 soil_simulator::rng;

void soil_simulator::SoilDynamics::step(
    SimOut* sim_out, std::vector<float> pos, std::vector<float> ori,
    Grid grid, Bucket* bucket, SimParam sim_param, float tol
) {
    // Updating bucket position
    soil_simulator::CalcBucketPos(
        sim_out, pos, ori, grid, bucket, sim_param, tol);

    // Updating position of soil resting on the bucket
    soil_simulator::UpdateBodySoil(sim_out, pos, ori, grid, bucket, tol);

    // Moving intersecting soil cells
    soil_simulator::MoveIntersectingCells(sim_out, tol);

    // Assuming that the terrain is not at equilibrium
    sim_out->equilibrium_ = false;
}

void soil_simulator::SoilDynamics::check() {
}

void soil_simulator::SoilDynamics::WriteOutputs() {
}

int main() {
    soil_simulator::SoilDynamics sim;
    soil_simulator::Grid grid(4.0, 4.0, 4.0, 0.05, 0.05);
    std::cout << grid.vect_x_[0];
    return 0;
}
