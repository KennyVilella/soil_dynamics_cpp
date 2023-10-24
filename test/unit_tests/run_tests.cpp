/*
This file runs all unit tests.

Copyright, 2023, Vilella Kenny.
*/
#include <glog/header.h>
#include <glog/logging.h>
#include <random>
#include "gtest/gtest.h"
#include "soil_simulator/soil_dynamics.hpp"

// Defining RNG
std::mt19937 soil_simulator::rng;

int main(int argc, char **argv) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0]);

    // Writting outputs to stderr instead of logfiles
    FLAGS_logtostdout = 1;

    // Disabling Warning messages
    FLAGS_minloglevel = 1;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
