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
    // increment (1e-5) is added or removed to the input in order to make sure
    // that the input coordinates do not correspond to a cell border.
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);

    // -- Testing for a line following the X axis --
    std::vector<float> a = {0.0 + 1e-5, 0.0 - 1e-5, -0.06 + 1e-5};
    std::vector<float> b = {1.0 - 1e-5, 0.0 - 1e-5,  0.0  - 1e-5};
    float delta = 0.1;
    std::vector<std::vector<int>> line_pos = soil_simulator::CalcLinePos(
        a, b, delta, grid);

    EXPECT_EQ(line_pos.size(), 11);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {10, 10, 10}));
    EXPECT_TRUE((line_pos[1] == std::vector<int> {11, 10, 10}));
    EXPECT_TRUE((line_pos[2] == std::vector<int> {12, 10, 10}));
    EXPECT_TRUE((line_pos[3] == std::vector<int> {13, 10, 10}));
    EXPECT_TRUE((line_pos[4] == std::vector<int> {14, 10, 10}));
    EXPECT_TRUE((line_pos[5] == std::vector<int> {15, 10, 10}));
    EXPECT_TRUE((line_pos[6] == std::vector<int> {16, 10, 10}));
    EXPECT_TRUE((line_pos[7] == std::vector<int> {17, 10, 10}));
    EXPECT_TRUE((line_pos[8] == std::vector<int> {18, 10, 10}));
    EXPECT_TRUE((line_pos[9] == std::vector<int> {19, 10, 10}));
    EXPECT_TRUE((line_pos[10] == std::vector<int> {20, 10, 10}));

    // -- Testing for a line following the X axis with a larger delta --
    a = {0.0 + 1e-5, 0.0 - 1e-5, 0.0 - 1e-5};
    b = {1.0 - 1e-5, 0.0 - 1e-5, 0.0 - 1e-5};
    delta = 0.5;
    line_pos = soil_simulator::CalcLinePos(a, b, delta, grid);

    EXPECT_EQ(line_pos.size(), 3);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {10, 10, 10}));
    EXPECT_TRUE((line_pos[1] == std::vector<int> {15, 10, 10}));
    EXPECT_TRUE((line_pos[2] == std::vector<int> {20, 10, 10}));

    // -- Testing that the rounding is done properly --
    a = {0.04 + 1e-5,  0.04 - 1e-5, -0.09 + 1e-5};
    b = {1.04 - 1e-5, -0.04 + 1e-5,   0.0 - 1e-5};
    delta = 0.1;
    line_pos = soil_simulator::CalcLinePos(a, b, delta, grid);

    EXPECT_EQ(line_pos.size(), 11);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {10, 10, 10}));
    EXPECT_TRUE((line_pos[1] == std::vector<int> {11, 10, 10}));
    EXPECT_TRUE((line_pos[2] == std::vector<int> {12, 10, 10}));
    EXPECT_TRUE((line_pos[3] == std::vector<int> {13, 10, 10}));
    EXPECT_TRUE((line_pos[4] == std::vector<int> {14, 10, 10}));
    EXPECT_TRUE((line_pos[5] == std::vector<int> {15, 10, 10}));
    EXPECT_TRUE((line_pos[6] == std::vector<int> {16, 10, 10}));
    EXPECT_TRUE((line_pos[7] == std::vector<int> {17, 10, 10}));
    EXPECT_TRUE((line_pos[8] == std::vector<int> {18, 10, 10}));
    EXPECT_TRUE((line_pos[9] == std::vector<int> {19, 10, 10}));
    EXPECT_TRUE((line_pos[10] == std::vector<int> {20, 10, 10}));

    // -- Testing for a line following the Y axis --
    a = {0.0 - 1e-5, 0.0 + 1e-5, 0.0 - 1e-5};
    b = {0.0 - 1e-5, 1.0 - 1e-5, 0.0 - 1e-5};
    delta = 0.1;
    line_pos = soil_simulator::CalcLinePos(a, b, delta, grid);

    EXPECT_EQ(line_pos.size(), 11);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {10, 10, 10}));
    EXPECT_TRUE((line_pos[1] == std::vector<int> {10, 11, 10}));
    EXPECT_TRUE((line_pos[2] == std::vector<int> {10, 12, 10}));
    EXPECT_TRUE((line_pos[3] == std::vector<int> {10, 13, 10}));
    EXPECT_TRUE((line_pos[4] == std::vector<int> {10, 14, 10}));
    EXPECT_TRUE((line_pos[5] == std::vector<int> {10, 15, 10}));
    EXPECT_TRUE((line_pos[6] == std::vector<int> {10, 16, 10}));
    EXPECT_TRUE((line_pos[7] == std::vector<int> {10, 17, 10}));
    EXPECT_TRUE((line_pos[8] == std::vector<int> {10, 18, 10}));
    EXPECT_TRUE((line_pos[9] == std::vector<int> {10, 19, 10}));
    EXPECT_TRUE((line_pos[10] == std::vector<int> {10, 20, 10}));

    // -- Testing for an arbitrary line (results obtained manually) --
    a = {0.34 + 1e-5, 0.56 + 1e-5, 0.0 - 1e-5};
    b = {0.74 - 1e-5, 0.97 - 1e-5, 0.0 - 1e-5};
    delta = 0.01;
    line_pos = soil_simulator::CalcLinePos(a, b, delta, grid);
    line_pos.erase(unique(line_pos.begin(), line_pos.end()), line_pos.end());

    EXPECT_EQ(line_pos.size(), 9);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {13, 16, 10}));
    EXPECT_TRUE((line_pos[1] == std::vector<int> {14, 16, 10}));
    EXPECT_TRUE((line_pos[2] == std::vector<int> {14, 17, 10}));
    EXPECT_TRUE((line_pos[3] == std::vector<int> {15, 17, 10}));
    EXPECT_TRUE((line_pos[4] == std::vector<int> {15, 18, 10}));
    EXPECT_TRUE((line_pos[5] == std::vector<int> {16, 18, 10}));
    EXPECT_TRUE((line_pos[6] == std::vector<int> {16, 19, 10}));
    EXPECT_TRUE((line_pos[7] == std::vector<int> {17, 19, 10}));
    EXPECT_TRUE((line_pos[8] == std::vector<int> {17, 20, 10}));

    // -- Testing for an arbitrary line in the XZ plane --
    a = {0.34 + 1e-8, 0.0 - 1e-8, 0.56 + 1e-8};
    b = {0.74 - 1e-8, 0.0 - 1e-8, 0.97 - 1e-8};
    delta = 0.01;
    line_pos = soil_simulator::CalcLinePos(a, b, delta, grid);
    line_pos.erase(unique(line_pos.begin(), line_pos.end()), line_pos.end());

    EXPECT_EQ(line_pos.size(), 9);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {13, 10, 16}));
    EXPECT_TRUE((line_pos[1] == std::vector<int> {14, 10, 16}));
    EXPECT_TRUE((line_pos[2] == std::vector<int> {14, 10, 17}));
    EXPECT_TRUE((line_pos[3] == std::vector<int> {15, 10, 17}));
    EXPECT_TRUE((line_pos[4] == std::vector<int> {15, 10, 18}));
    EXPECT_TRUE((line_pos[5] == std::vector<int> {16, 10, 18}));
    EXPECT_TRUE((line_pos[6] == std::vector<int> {16, 10, 19}));
    EXPECT_TRUE((line_pos[7] == std::vector<int> {17, 10, 19}));
    EXPECT_TRUE((line_pos[8] == std::vector<int> {17, 10, 20}));

    // -- Testing for the edge case where the line is a point --
    a = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    b = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    delta = 0.01;
    line_pos = soil_simulator::CalcLinePos(a, b, delta, grid);
    line_pos.erase(unique(line_pos.begin(), line_pos.end()), line_pos.end());

    EXPECT_EQ(line_pos.size(), 1);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {15, 15, 15}));

    // -- Testing for the edge case where the line is a point --
    a = {0.55 - 1e-5, 0.55 - 1e-5, 0.55 - 1e-5};
    b = {0.55 - 1e-5, 0.55 - 1e-5, 0.55 - 1e-5};
    delta = 0.01;
    line_pos = soil_simulator::CalcLinePos(a, b, delta, grid);
    line_pos.erase(unique(line_pos.begin(), line_pos.end()), line_pos.end());

    EXPECT_EQ(line_pos.size(), 1);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {15, 15, 16}));
}
