/*
This file implements the main function.

Copyright, 2023, Vilella Kenny.
*/
#include <glog/logging.h>
#include <random>
#include "soil_simulator/soil_dynamics.hpp"
#include "test/example/soil_evolution.hpp"

// Defining RNG
std::mt19937 soil_simulator::rng;

/// This function removes the prefix of glog message.
void EmptyPrefix(std::ostream &s, const google::LogMessageInfo &l, void*) {
    s;
}

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0], &EmptyPrefix);

    // Writting outputs to stderr instead of logfiles
    FLAGS_logtostderr = 1;

    // Launching a simulation
    soil_simulator::SoilEvolution(false, true, true, true, false);

    return 0;
}
