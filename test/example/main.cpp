/*
This file implements the main function.

Copyright, 2023, Vilella Kenny.
*/
#include <random>
#include <glog/logging.h>
#include "src/soil_dynamics.hpp"
#include "test/example/soil_evolution.hpp"

// Defining RNG
std::mt19937 soil_simulator::rng;

int main() {
    // Writting outputs to stderr instead of logfiles
    FLAGS_logtostderr = 1;

    // Launching a simulation
    soil_simulator::SoilEvolution(true, false, true, true, false);

    return 0;
}
