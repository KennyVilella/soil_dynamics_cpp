/*
This file runs all unit tests.

Copyright, 2023, Vilella Kenny.
*/
#include <random>
#include <glog/logging.h>
#include "gtest/gtest.h"
#include "src/soil_dynamics.hpp"

// Defining RNG
std::mt19937 soil_simulator::rng;

int main(int argc, char **argv) {
    // Writting outputs to stderr instead of logfiles
    FLAGS_logtostderr = 1;

    // Disabling Warning messages
    FLAGS_minloglevel = 2;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
