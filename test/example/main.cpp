/*
This file implements the main function.

Copyright, 2023, Vilella Kenny.
*/
#include <random>
#include "src/soil_dynamics.hpp"
#include "test/example/soil_evolution.hpp"

// Defining RNG
std::mt19937 soil_simulator::rng;

int main() {
    // Launching a simulation
    soil_simulator::SoilEvolution(true, false, true, true, false);

    return 0;
}
