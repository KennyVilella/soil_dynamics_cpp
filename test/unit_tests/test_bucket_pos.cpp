/*
This file implements unit tests for the functions in bucket_pos.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include "gtest/gtest.h"
#include "src/bucket_pos.cpp"

TEST(UnitTestBucketPos, CalcLinePos) {
    // Note that this function does not account for the case where the line
    // follows a cell border. It is therefore necessary to solve this potential
    // ambiguity before calling the function. As a result, a small
    // increment (1e-8) is added or removed to the input in order to make sure
    // that the input coordinates do not correspond to a cell border.
    soil_simulator::Grid grid(4.0, 4.0, 4.0, 0.05, 0.01);

    // -- Testing for a line following the X axis --
    std::vector<float> a = {0.0 + 1e-8, 0.0 - 1e-8, -0.06 + 1e-8};
    std::vector<float> b = {1.0 - 1e-8, 0.0 - 1e-8,  0.0  - 1e-8};
    float delta = 0.1;
    std::vector<std::vector<float>> line_pos = soil_simulator::CalcLinePos(
        a, b, delta, grid);

    EXPECT_EQ(5, 5);
}
