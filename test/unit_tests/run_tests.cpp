/*
This file runs all unit tests.

Copyright, 2023, Vilella Kenny.
*/
#include <random>
#include "gtest/gtest.h"

// Setting RNG
std::mt19937 rng;

int main(int argc, char **argv) {
    // Set RNG seed
    rng.seed(1234);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
