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
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);

    // -- Testing for a line following the X axis --
    std::vector<float> a = {0.0 + 1e-8, 0.0 - 1e-8, -0.06 + 1e-8};
    std::vector<float> b = {1.0 - 1e-8, 0.0 - 1e-8,  0.0  - 1e-8};
    float delta = 0.1;
    std::vector<std::vector<int>> line_pos = soil_simulator::CalcLinePos(
        a, b, delta, grid);

    EXPECT_EQ(line_pos.size(), 11);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {11, 11, 11}));
    EXPECT_TRUE((line_pos[1] == std::vector<int> {12, 11, 11}));
    EXPECT_TRUE((line_pos[2] == std::vector<int> {13, 11, 11}));
    EXPECT_TRUE((line_pos[3] == std::vector<int> {14, 11, 11}));
    EXPECT_TRUE((line_pos[4] == std::vector<int> {15, 11, 11}));
    EXPECT_TRUE((line_pos[5] == std::vector<int> {16, 11, 11}));
    EXPECT_TRUE((line_pos[6] == std::vector<int> {17, 11, 11}));
    EXPECT_TRUE((line_pos[7] == std::vector<int> {18, 11, 11}));
    EXPECT_TRUE((line_pos[8] == std::vector<int> {19, 11, 11}));
    EXPECT_TRUE((line_pos[9] == std::vector<int> {20, 11, 11}));
    EXPECT_TRUE((line_pos[10] == std::vector<int> {21, 11, 11}));


}
