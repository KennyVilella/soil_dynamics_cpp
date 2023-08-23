/*
This file implements the main function.

Copyright, 2023, Vilella Kenny.
*/
#include <random>
#include <glog/logging.h>
#include "src/soil_dynamics.hpp"

// Defining RNG
std::mt19937 soil_simulator::rng;

int main() {
    // Writting outputs to stderr instead of logfiles
    FLAGS_logtostderr = 1;

    return 0;
}
