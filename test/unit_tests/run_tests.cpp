/*
This file runs all unit tests.

Copyright, 2023, Vilella Kenny.
*/
#include <random>
#include "gtest/gtest.h"
#include "src/soil_dynamics.hpp"

int main(int argc, char **argv) {
    extern std::mt19937 rng;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
