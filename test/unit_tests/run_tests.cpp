/*
This file runs all unit tests.

Copyright, 2023, Vilella Kenny.
*/
#include <random>
#include "gtest/gtest.h"
#include "src/soil_dynamics.hpp"

// Defining RNG
std::mt19937 soil_simulator::rng;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
