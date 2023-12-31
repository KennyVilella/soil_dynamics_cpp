/*
This file implements the main function.

Copyright, 2023, Vilella Kenny.
*/
#include <glog/logging.h>
#include <random>
#include "soil_simulator/soil_dynamics.hpp"

// Defining RNG
std::mt19937 soil_simulator::rng;

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);

    // Writing outputs to stderr instead of logfiles
    FLAGS_logtostderr = 1;

    return 0;
}
