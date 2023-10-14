/*
This file implements unit tests for the functions in bucket_pos.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include "gtest/gtest.h"
#include "soil_simulator/bucket_pos.hpp"
#include "test/unit_tests/utility.hpp"

TEST(UnitTestBucketPos, CalcLinePos) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);

    // Declaring variables
    std::vector<float> a;
    std::vector<float> b;
    std::vector<std::vector<int>> line_pos;

    // Test: BP-CL-1
    a = {0.0 + 1e-5, 0.0 - 1e-5, -0.06 + 1e-5};
    b = {1.0 - 1e-5, 0.0 - 1e-5,  0.0  - 1e-5};
    line_pos = soil_simulator::CalcLinePos(
        a, b, grid);
    EXPECT_EQ(line_pos.size(), 11);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {10, 10, 9}));
    EXPECT_TRUE((line_pos[1] == std::vector<int> {11, 10, 9}));
    EXPECT_TRUE((line_pos[2] == std::vector<int> {12, 10, 9}));
    EXPECT_TRUE((line_pos[3] == std::vector<int> {13, 10, 9}));
    EXPECT_TRUE((line_pos[4] == std::vector<int> {14, 10, 9}));
    EXPECT_TRUE((line_pos[5] == std::vector<int> {15, 10, 9}));
    EXPECT_TRUE((line_pos[6] == std::vector<int> {16, 10, 9}));
    EXPECT_TRUE((line_pos[7] == std::vector<int> {17, 10, 9}));
    EXPECT_TRUE((line_pos[8] == std::vector<int> {18, 10, 9}));
    EXPECT_TRUE((line_pos[9] == std::vector<int> {19, 10, 9}));
    EXPECT_TRUE((line_pos[10] == std::vector<int> {20, 10, 9}));

    // Test: BP-CL-2
    a = {0.04 + 1e-5,  0.04 - 1e-5, -0.09 + 1e-5};
    b = {1.04 - 1e-5, -0.04 + 1e-5,   0.0 - 1e-5};
    line_pos = soil_simulator::CalcLinePos(a, b, grid);
    EXPECT_EQ(line_pos.size(), 11);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {10, 10, 9}));
    EXPECT_TRUE((line_pos[1] == std::vector<int> {11, 10, 9}));
    EXPECT_TRUE((line_pos[2] == std::vector<int> {12, 10, 9}));
    EXPECT_TRUE((line_pos[3] == std::vector<int> {13, 10, 9}));
    EXPECT_TRUE((line_pos[4] == std::vector<int> {14, 10, 9}));
    EXPECT_TRUE((line_pos[5] == std::vector<int> {15, 10, 9}));
    EXPECT_TRUE((line_pos[6] == std::vector<int> {16, 10, 9}));
    EXPECT_TRUE((line_pos[7] == std::vector<int> {17, 10, 9}));
    EXPECT_TRUE((line_pos[8] == std::vector<int> {18, 10, 9}));
    EXPECT_TRUE((line_pos[9] == std::vector<int> {19, 10, 9}));
    EXPECT_TRUE((line_pos[10] == std::vector<int> {20, 10, 9}));

    // Test: BP-CL-3
    a = {0.0 - 1e-5, 0.0 + 1e-5, 0.0 - 1e-5};
    b = {0.0 - 1e-5, 1.0 - 1e-5, 0.0 - 1e-5};
    line_pos = soil_simulator::CalcLinePos(a, b, grid);
    EXPECT_EQ(line_pos.size(), 11);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {10, 10, 9}));
    EXPECT_TRUE((line_pos[1] == std::vector<int> {10, 11, 9}));
    EXPECT_TRUE((line_pos[2] == std::vector<int> {10, 12, 9}));
    EXPECT_TRUE((line_pos[3] == std::vector<int> {10, 13, 9}));
    EXPECT_TRUE((line_pos[4] == std::vector<int> {10, 14, 9}));
    EXPECT_TRUE((line_pos[5] == std::vector<int> {10, 15, 9}));
    EXPECT_TRUE((line_pos[6] == std::vector<int> {10, 16, 9}));
    EXPECT_TRUE((line_pos[7] == std::vector<int> {10, 17, 9}));
    EXPECT_TRUE((line_pos[8] == std::vector<int> {10, 18, 9}));
    EXPECT_TRUE((line_pos[9] == std::vector<int> {10, 19, 9}));
    EXPECT_TRUE((line_pos[10] == std::vector<int> {10, 20, 9}));

    // Test: BP-CL-4
    a = {0.34 + 1e-5, 0.56 + 1e-5, 0.0 - 1e-5};
    b = {0.74 - 1e-5, 0.97 - 1e-5, 0.0 - 1e-5};
    line_pos = soil_simulator::CalcLinePos(a, b, grid);
    line_pos.erase(unique(line_pos.begin(), line_pos.end()), line_pos.end());
    EXPECT_EQ(line_pos.size(), 9);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {13, 16, 9}));
    EXPECT_TRUE((line_pos[1] == std::vector<int> {14, 16, 9}));
    EXPECT_TRUE((line_pos[2] == std::vector<int> {14, 17, 9}));
    EXPECT_TRUE((line_pos[3] == std::vector<int> {15, 17, 9}));
    EXPECT_TRUE((line_pos[4] == std::vector<int> {15, 18, 9}));
    EXPECT_TRUE((line_pos[5] == std::vector<int> {16, 18, 9}));
    EXPECT_TRUE((line_pos[6] == std::vector<int> {16, 19, 9}));
    EXPECT_TRUE((line_pos[7] == std::vector<int> {17, 19, 9}));
    EXPECT_TRUE((line_pos[8] == std::vector<int> {17, 20, 9}));

    // Test: BP-CL-5
    a = {0.34 + 1e-8, 0.0 - 1e-8, 0.56 + 1e-8};
    b = {0.74 - 1e-8, 0.0 - 1e-8, 0.97 - 1e-8};
    line_pos = soil_simulator::CalcLinePos(a, b, grid);
    line_pos.erase(unique(line_pos.begin(), line_pos.end()), line_pos.end());
    EXPECT_EQ(line_pos.size(), 9);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {13, 10, 15}));
    EXPECT_TRUE((line_pos[1] == std::vector<int> {14, 10, 15}));
    EXPECT_TRUE((line_pos[2] == std::vector<int> {14, 10, 16}));
    EXPECT_TRUE((line_pos[3] == std::vector<int> {15, 10, 16}));
    EXPECT_TRUE((line_pos[4] == std::vector<int> {15, 10, 17}));
    EXPECT_TRUE((line_pos[5] == std::vector<int> {16, 10, 17}));
    EXPECT_TRUE((line_pos[6] == std::vector<int> {16, 10, 18}));
    EXPECT_TRUE((line_pos[7] == std::vector<int> {17, 10, 18}));
    EXPECT_TRUE((line_pos[8] == std::vector<int> {17, 10, 19}));

    // Test: BP-CL-6
    a = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    b = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    line_pos = soil_simulator::CalcLinePos(a, b, grid);
    line_pos.erase(unique(line_pos.begin(), line_pos.end()), line_pos.end());
    EXPECT_EQ(line_pos.size(), 1);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {15, 15, 14}));
    a = {0.55 - 1e-5, 0.55 - 1e-5, 0.55 - 1e-5};
    b = {0.55 - 1e-5, 0.55 - 1e-5, 0.55 - 1e-5};
    line_pos = soil_simulator::CalcLinePos(a, b, grid);
    line_pos.erase(unique(line_pos.begin(), line_pos.end()), line_pos.end());
    EXPECT_EQ(line_pos.size(), 1);
    EXPECT_TRUE((line_pos[0] == std::vector<int> {15, 15, 15}));
}

TEST(UnitTestBucketPos, DecomposeVectorRectangle) {
    // Declaring variables
    std::vector<float> ab_ind;
    std::vector<float> ad_ind;
    std::vector<float> a_ind;
    int area_min_x;
    int area_min_y;
    int area_length_x;
    int area_length_y;
    float tol = 1.e-5;
    std::vector<std::vector<float>> c_ab;
    std::vector<std::vector<float>> c_ad;
    std::vector<std::vector<bool>> in_rec;
    int nn;

    // Test: BP-DVR-1
    a_ind = {10.0, 10.0, 10.0};
    ab_ind = {5.0, 0.0, 0.0};
    ad_ind = {0.0, 5.0, 0.0};
    area_min_x = 8;
    area_min_y = 8;
    area_length_x = 8;
    area_length_y = 8;
    std::tie(c_ab, c_ad, in_rec, nn) = soil_simulator::DecomposeVectorRectangle(
        ab_ind, ad_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    EXPECT_EQ(nn, 25 * 4);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            if ((ii > 1.5) && (ii < 6.5) && (jj > 1.5) && (jj < 6.5))
                EXPECT_EQ(in_rec[ii][jj], true);
            else
                EXPECT_EQ(in_rec[ii][jj], false);
        }
    for (auto jj = 2; jj < 7; jj++)
        EXPECT_NEAR(c_ab[2][jj], 0.1, 1e-5);
    for (auto jj = 2; jj < 7; jj++)
        EXPECT_NEAR(c_ab[3][jj], 0.3, 1e-5);
    for (auto jj = 2; jj < 7; jj++)
        EXPECT_NEAR(c_ab[4][jj], 0.5, 1e-5);
    for (auto jj = 2; jj < 7; jj++)
        EXPECT_NEAR(c_ab[5][jj], 0.7, 1e-5);
    for (auto jj = 2; jj < 7; jj++)
        EXPECT_NEAR(c_ab[6][jj], 0.9, 1e-5);
    for (auto ii = 2; ii < 7; ii++)
        EXPECT_NEAR(c_ad[ii][2], 0.1, 1e-5);
    for (auto ii = 2; ii < 7; ii++)
        EXPECT_NEAR(c_ad[ii][3], 0.3, 1e-5);
    for (auto ii = 2; ii < 7; ii++)
        EXPECT_NEAR(c_ad[ii][4], 0.5, 1e-5);
    for (auto ii = 2; ii < 7; ii++)
        EXPECT_NEAR(c_ad[ii][5], 0.7, 1e-5);
    for (auto ii = 2; ii < 7; ii++)
        EXPECT_NEAR(c_ad[ii][6], 0.9, 1e-5);

    // Test: BP-DVR-2
    a_ind = {9.7, 10.3, 4.3};
    ab_ind = {5.7, 0.0, 0.0};
    ad_ind = {0.0, 4.7, 0.0};
    area_min_x = 8;
    area_min_y = 8;
    area_length_x = 8;
    area_length_y = 8;
    std::tie(c_ab, c_ad, in_rec, nn) = soil_simulator::DecomposeVectorRectangle(
        ab_ind, ad_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    EXPECT_EQ(nn, 25 * 4);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            if ((ii > 1.5) && (ii < 6.5) && (jj > 1.5) && (jj < 6.5))
                EXPECT_EQ(in_rec[ii][jj], true);
            else
                EXPECT_EQ(in_rec[ii][jj], false);
        }

    // Test: BP-DVR-3
    a_ind = {10.0 + 1e-12, 9.5 + 1e-12, 5.0};
    ab_ind = {5.0 - 1e-12, 0.0, 2.4};
    ad_ind = {0.0, 3.0 - 1e-12, -0.3};
    area_min_x = 8;
    area_min_y = 8;
    area_length_x = 8;
    area_length_y = 8;
    std::tie(c_ab, c_ad, in_rec, nn) = soil_simulator::DecomposeVectorRectangle(
        ab_ind, ad_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    EXPECT_EQ(nn, 10 * 4);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            if ((ii > 1.5) && (ii < 6.5) && (jj > 1.5) && (jj < 3.5))
                EXPECT_EQ(in_rec[ii][jj], true);
            else
                EXPECT_EQ(in_rec[ii][jj], false);
        }
    for (auto jj = 2; jj < 8; jj++)
        EXPECT_NEAR(c_ab[2][jj], 0.1, 1e-5);
    for (auto jj = 2; jj < 8; jj++)
        EXPECT_NEAR(c_ab[3][jj], 0.3, 1e-5);
    for (auto jj = 2; jj < 8; jj++)
        EXPECT_NEAR(c_ab[4][jj], 0.5, 1e-5);
    for (auto jj = 2; jj < 8; jj++)
        EXPECT_NEAR(c_ab[5][jj], 0.7, 1e-5);
    for (auto jj = 2; jj < 8; jj++)
        EXPECT_NEAR(c_ab[6][jj], 0.9, 1e-5);
    for (auto ii = 2; ii < 7; ii++)
        EXPECT_NEAR(c_ad[ii][2], 1.0 / 3.0, 1e-5);
    for (auto ii = 2; ii < 7; ii++)
        EXPECT_NEAR(c_ad[ii][3], 2.0 / 3.0, 1e-5);
    for (auto ii = 2; ii < 7; ii++)
        EXPECT_NEAR(c_ad[ii][4], 1.0, 1e-5);

    // Test: BP-DVR-4
    a_ind = {15.0, 10.0, 5.0};
    ab_ind = {1.0, 0.0, 2.4};
    ad_ind = {0.0, 5.0, -0.3};
    area_min_x = 13;
    area_min_y = 8;
    area_length_x = 8;
    area_length_y = 8;
    std::tie(c_ab, c_ad, in_rec, nn) = soil_simulator::DecomposeVectorRectangle(
        ab_ind, ad_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    EXPECT_EQ(nn, 5 * 4);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            if ((ii == 2) && (jj > 1.5) && (jj < 6.5))
                EXPECT_EQ(in_rec[ii][jj], true);
            else
                EXPECT_EQ(in_rec[ii][jj], false);
        }
    for (auto jj = 2; jj < 8; jj++)
        EXPECT_NEAR(c_ab[2][jj], 0.5, 1e-5);
    for (auto jj = 2; jj < 8; jj++)
        EXPECT_NEAR(c_ab[3][jj], 1.5, 1e-5);
    EXPECT_NEAR(c_ad[2][2], 0.1, 1e-5);
    EXPECT_NEAR(c_ad[3][2], 0.1, 1e-5);
    EXPECT_NEAR(c_ad[2][3], 0.3, 1e-5);
    EXPECT_NEAR(c_ad[3][3], 0.3, 1e-5);
    EXPECT_NEAR(c_ad[2][4], 0.5, 1e-5);
    EXPECT_NEAR(c_ad[3][4], 0.5, 1e-5);
    EXPECT_NEAR(c_ad[2][5], 0.7, 1e-5);
    EXPECT_NEAR(c_ad[3][5], 0.7, 1e-5);
    EXPECT_NEAR(c_ad[2][6], 0.9, 1e-5);
    EXPECT_NEAR(c_ad[3][6], 0.9, 1e-5);
    EXPECT_NEAR(c_ad[2][7], 1.1, 1e-5);
    EXPECT_NEAR(c_ad[3][7], 1.1, 1e-5);

    // Test: BP-DVR-5
    a_ind = {14.2, 10.3, 5.0};
    ab_ind = {2.3, 1.2, 2.4};
    ad_ind = {4.6, 2.4, -0.3};
    area_min_x = 13;
    area_min_y = 9;
    area_length_x = 8;
    area_length_y = 8;
    std::tie(c_ab, c_ad, in_rec, nn) = soil_simulator::DecomposeVectorRectangle(
        ab_ind, ad_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    EXPECT_EQ(nn, 0);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++)
            EXPECT_EQ(in_rec[ii][jj], false);

    // Test: BP-DVR-6
    a_ind = {14.2, 10.3, 5.0};
    ab_ind = {0.0, 0.0, 0.0};
    ad_ind = {0.0, 0.0, 0.0};
    area_min_x = 13;
    area_min_y = 9;
    area_length_x = 8;
    area_length_y = 8;
    std::tie(c_ab, c_ad, in_rec, nn) = soil_simulator::DecomposeVectorRectangle(
        ab_ind, ad_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    EXPECT_EQ(nn, 0);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++)
            EXPECT_EQ(in_rec[ii][jj], false);
}

TEST(UnitTestBucketPos, DecomposeVectorTriangle) {
    // Declaring variables
    std::vector<float> ab_ind;
    std::vector<float> ac_ind;
    std::vector<float> a_ind;
    int area_min_x;
    int area_min_y;
    int area_length_x;
    int area_length_y;
    float tol = 1.e-5;
    std::vector<std::vector<float>> c_ab;
    std::vector<std::vector<float>> c_ac;
    std::vector<std::vector<bool>> in_tri;
    int nn;

    // Test: BP-DVT-1
    a_ind = {10.0, 10.0, 10.0};
    ab_ind = {10.0, 0.0, 0.0};
    ac_ind = {0.0, 10.0, 0.0};
    area_min_x = 8;
    area_min_y = 8;
    area_length_x = 15;
    area_length_y = 15;
    std::tie(c_ab, c_ac, in_tri, nn) = soil_simulator::DecomposeVectorTriangle(
        ab_ind, ac_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    EXPECT_EQ(nn, 45 * 4);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_EQ(in_tri[ii][0], false);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_EQ(in_tri[ii][1], false);
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 10.5))
            EXPECT_EQ(in_tri[ii][2], true);
        else
            EXPECT_EQ(in_tri[ii][2], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 9.5))
            EXPECT_EQ(in_tri[ii][3], true);
        else
            EXPECT_EQ(in_tri[ii][3], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 8.5))
            EXPECT_EQ(in_tri[ii][4], true);
        else
            EXPECT_EQ(in_tri[ii][4], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 7.5))
            EXPECT_EQ(in_tri[ii][5], true);
        else
            EXPECT_EQ(in_tri[ii][5], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 6.5))
            EXPECT_EQ(in_tri[ii][6], true);
        else
            EXPECT_EQ(in_tri[ii][6], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 5.5))
            EXPECT_EQ(in_tri[ii][7], true);
        else
            EXPECT_EQ(in_tri[ii][7], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 4.5))
            EXPECT_EQ(in_tri[ii][8], true);
        else
            EXPECT_EQ(in_tri[ii][8], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 3.5))
            EXPECT_EQ(in_tri[ii][9], true);
        else
            EXPECT_EQ(in_tri[ii][9], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 2.5))
            EXPECT_EQ(in_tri[ii][10], true);
        else
            EXPECT_EQ(in_tri[ii][10], false);
    }
    for (auto jj = 0; jj < area_length_y; jj++)
        EXPECT_NEAR(c_ab[2][jj], 0.05, 1e-5);
    for (auto jj = 0; jj < area_length_y; jj++)
        EXPECT_NEAR(c_ab[3][jj], 0.15, 1e-5);
    for (auto jj = 0; jj < area_length_y; jj++)
        EXPECT_NEAR(c_ab[4][jj], 0.25, 1e-5);
    for (auto jj = 0; jj < area_length_y; jj++)
        EXPECT_NEAR(c_ab[5][jj], 0.35, 1e-5);
    for (auto jj = 0; jj < area_length_y; jj++)
        EXPECT_NEAR(c_ab[6][jj], 0.45, 1e-5);
    for (auto jj = 0; jj < area_length_y; jj++)
        EXPECT_NEAR(c_ab[7][jj], 0.55, 1e-5);
    for (auto jj = 0; jj < area_length_y; jj++)
        EXPECT_NEAR(c_ab[8][jj], 0.65, 1e-5);
    for (auto jj = 0; jj < area_length_y; jj++)
        EXPECT_NEAR(c_ab[9][jj], 0.75, 1e-5);
    for (auto jj = 0; jj < area_length_y; jj++)
        EXPECT_NEAR(c_ab[10][jj], 0.85, 1e-5);
    for (auto jj = 0; jj < area_length_y; jj++)
        EXPECT_NEAR(c_ab[11][jj], 0.95, 1e-5);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_NEAR(c_ac[ii][2], 0.05, 1e-5);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_NEAR(c_ac[ii][3], 0.15, 1e-5);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_NEAR(c_ac[ii][4], 0.25, 1e-5);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_NEAR(c_ac[ii][5], 0.35, 1e-5);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_NEAR(c_ac[ii][6], 0.45, 1e-5);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_NEAR(c_ac[ii][7], 0.55, 1e-5);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_NEAR(c_ac[ii][8], 0.65, 1e-5);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_NEAR(c_ac[ii][9], 0.75, 1e-5);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_NEAR(c_ac[ii][10], 0.85, 1e-5);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_NEAR(c_ac[ii][11], 0.95, 1e-5);

    // Test: BP-DVT-2
    a_ind = {9.9, 9.7, 10.0};
    ab_ind = {9.7, 0.0, 0.0};
    ac_ind = {0.0, 10.4, 0.0};
    area_min_x = 8;
    area_min_y = 8;
    area_length_x = 15;
    area_length_y = 15;
    std::tie(c_ab, c_ac, in_tri, nn) = soil_simulator::DecomposeVectorTriangle(
        ab_ind, ac_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    EXPECT_EQ(nn, 45 * 4);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_EQ(in_tri[ii][0], false);
    for (auto ii = 0; ii < area_length_x; ii++)
        EXPECT_EQ(in_tri[ii][1], false);
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 10.5))
            EXPECT_EQ(in_tri[ii][2], true);
        else
            EXPECT_EQ(in_tri[ii][2], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 9.5))
            EXPECT_EQ(in_tri[ii][3], true);
        else
            EXPECT_EQ(in_tri[ii][3], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 8.5))
            EXPECT_EQ(in_tri[ii][4], true);
        else
            EXPECT_EQ(in_tri[ii][4], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 7.5))
            EXPECT_EQ(in_tri[ii][5], true);
        else
            EXPECT_EQ(in_tri[ii][5], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 6.5))
            EXPECT_EQ(in_tri[ii][6], true);
        else
            EXPECT_EQ(in_tri[ii][6], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 5.5))
            EXPECT_EQ(in_tri[ii][7], true);
        else
            EXPECT_EQ(in_tri[ii][7], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 4.5))
            EXPECT_EQ(in_tri[ii][8], true);
        else
            EXPECT_EQ(in_tri[ii][8], false);
    }
    for (auto ii = 0; ii < area_length_x; ii++) {
        if ((ii > 1.5) && (ii < 2.5))
            EXPECT_EQ(in_tri[ii][10], true);
        else
            EXPECT_EQ(in_tri[ii][10], false);
    }

    // Test: BP-DVT-3
    a_ind = {15.0, 10.0, 10.0};
    ab_ind = {1.0, 0.0, 0.0};
    ac_ind = {1.0, 5.0, 0.0};
    area_min_x = 13;
    area_min_y = 8;
    area_length_x = 8;
    area_length_y = 8;
    std::tie(c_ab, c_ac, in_tri, nn) = soil_simulator::DecomposeVectorTriangle(
        ab_ind, ac_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    EXPECT_EQ(nn, 2 * 4);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            if ((ii == 2) && ((jj == 2) || (jj == 3)))
                EXPECT_EQ(in_tri[ii][jj], true);
            else
                EXPECT_EQ(in_tri[ii][jj], false);
        }
    EXPECT_NEAR(c_ab[2][2], 0.4, 1e-5);
    EXPECT_NEAR(c_ab[2][3], 0.2, 1e-5);
    EXPECT_NEAR(c_ab[2][4], 0.0, 1e-5);
    EXPECT_NEAR(c_ab[2][5], -0.2, 1e-5);
    EXPECT_NEAR(c_ab[3][2], 1.4, 1e-5);
    EXPECT_NEAR(c_ab[3][3], 1.2, 1e-5);
    EXPECT_NEAR(c_ab[3][4], 1.0, 1e-5);
    EXPECT_NEAR(c_ab[3][5], 0.8, 1e-5);
    EXPECT_NEAR(c_ab[3][6], 0.6, 1e-5);
    EXPECT_NEAR(c_ab[3][7], 0.4, 1e-5);
    EXPECT_NEAR(c_ac[2][2], 0.1, 1e-5);
    EXPECT_NEAR(c_ac[2][3], 0.3, 1e-5);
    EXPECT_NEAR(c_ac[2][4], 0.5, 1e-5);
    EXPECT_NEAR(c_ac[2][5], 0.7, 1e-5);
    EXPECT_NEAR(c_ac[3][2], 0.1, 1e-5);
    EXPECT_NEAR(c_ac[3][3], 0.3, 1e-5);
    EXPECT_NEAR(c_ac[3][4], 0.5, 1e-5);
    EXPECT_NEAR(c_ac[3][5], 0.7, 1e-5);
    EXPECT_NEAR(c_ac[3][6], 0.9, 1e-5);
    EXPECT_NEAR(c_ac[3][7], 1.1, 1e-5);

    // Test: BP-DVT-4
    a_ind = {15.0, 10.0, 10.0};
    ab_ind = {1.4, 0.7, 0.0};
    ac_ind = {2.8, 1.4, 0.0};
    area_min_x = 13;
    area_min_y = 8;
    area_length_x = 8;
    area_length_y = 8;
    std::tie(c_ab, c_ac, in_tri, nn) = soil_simulator::DecomposeVectorTriangle(
        ab_ind, ac_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    EXPECT_EQ(nn, 0);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++)
            EXPECT_EQ(in_tri[ii][jj], false);

    // Test: BP-DVT-5
    a_ind = {15.0, 10.0, 10.0};
    ab_ind = {0.0, 0.0, 0.0};
    ac_ind = {0.0, 0.0, 0.0};
    area_min_x = 13;
    area_min_y = 8;
    area_length_x = 8;
    area_length_y = 8;
    std::tie(c_ab, c_ac, in_tri, nn) = soil_simulator::DecomposeVectorTriangle(
        ab_ind, ac_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    EXPECT_EQ(nn, 0);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++)
            EXPECT_EQ(in_tri[ii][jj], false);
}

TEST(UnitTestBucketPos, CalcRectanglePos) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);

    // Declaring variables
    std::vector<float> a;
    std::vector<float> b;
    std::vector<float> c;
    std::vector<float> d;
    float tol = 1e-5;
    std::vector<std::vector<int>> rect_pos;

    // Test: BP-CR-1
    a = {0.0 + 1e-5, 0.0 + 1e-5, 0.0 - 1e-5};
    b = {0.5 - 1e-5, 0.0 + 1e-5, 0.0 - 1e-5};
    c = {0.5 - 1e-5, 0.5 - 1e-5, 0.0 - 1e-5};
    d = {0.0 + 1e-5, 0.5 - 1e-5, 0.0 - 1e-5};
    rect_pos = soil_simulator::CalcRectanglePos(
        a, b, c, d, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 36);
    EXPECT_TRUE((rect_pos[0] == std::vector<int> {10, 10, 9}));
    EXPECT_TRUE((rect_pos[1] == std::vector<int> {10, 11, 9}));
    EXPECT_TRUE((rect_pos[2] == std::vector<int> {10, 12, 9}));
    EXPECT_TRUE((rect_pos[3] == std::vector<int> {10, 13, 9}));
    EXPECT_TRUE((rect_pos[4] == std::vector<int> {10, 14, 9}));
    EXPECT_TRUE((rect_pos[5] == std::vector<int> {10, 15, 9}));
    EXPECT_TRUE((rect_pos[6] == std::vector<int> {11, 10, 9}));
    EXPECT_TRUE((rect_pos[7] == std::vector<int> {11, 11, 9}));
    EXPECT_TRUE((rect_pos[8] == std::vector<int> {11, 12, 9}));
    EXPECT_TRUE((rect_pos[9] == std::vector<int> {11, 13, 9}));
    EXPECT_TRUE((rect_pos[10] == std::vector<int> {11, 14, 9}));
    EXPECT_TRUE((rect_pos[11] == std::vector<int> {11, 15, 9}));
    EXPECT_TRUE((rect_pos[12] == std::vector<int> {12, 10, 9}));
    EXPECT_TRUE((rect_pos[13] == std::vector<int> {12, 11, 9}));
    EXPECT_TRUE((rect_pos[14] == std::vector<int> {12, 12, 9}));
    EXPECT_TRUE((rect_pos[15] == std::vector<int> {12, 13, 9}));
    EXPECT_TRUE((rect_pos[16] == std::vector<int> {12, 14, 9}));
    EXPECT_TRUE((rect_pos[17] == std::vector<int> {12, 15, 9}));
    EXPECT_TRUE((rect_pos[18] == std::vector<int> {13, 10, 9}));
    EXPECT_TRUE((rect_pos[19] == std::vector<int> {13, 11, 9}));
    EXPECT_TRUE((rect_pos[20] == std::vector<int> {13, 12, 9}));
    EXPECT_TRUE((rect_pos[21] == std::vector<int> {13, 13, 9}));
    EXPECT_TRUE((rect_pos[22] == std::vector<int> {13, 14, 9}));
    EXPECT_TRUE((rect_pos[23] == std::vector<int> {13, 15, 9}));
    EXPECT_TRUE((rect_pos[24] == std::vector<int> {14, 10, 9}));
    EXPECT_TRUE((rect_pos[25] == std::vector<int> {14, 11, 9}));
    EXPECT_TRUE((rect_pos[26] == std::vector<int> {14, 12, 9}));
    EXPECT_TRUE((rect_pos[27] == std::vector<int> {14, 13, 9}));
    EXPECT_TRUE((rect_pos[28] == std::vector<int> {14, 14, 9}));
    EXPECT_TRUE((rect_pos[29] == std::vector<int> {14, 15, 9}));
    EXPECT_TRUE((rect_pos[30] == std::vector<int> {15, 10, 9}));
    EXPECT_TRUE((rect_pos[31] == std::vector<int> {15, 11, 9}));
    EXPECT_TRUE((rect_pos[32] == std::vector<int> {15, 12, 9}));
    EXPECT_TRUE((rect_pos[33] == std::vector<int> {15, 13, 9}));
    EXPECT_TRUE((rect_pos[34] == std::vector<int> {15, 14, 9}));
    EXPECT_TRUE((rect_pos[35] == std::vector<int> {15, 15, 9}));

    // Test: BP-CR-2
    rect_pos = soil_simulator::CalcRectanglePos(a, d, c, b, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 36);
    EXPECT_TRUE((rect_pos[0] == std::vector<int> {10, 10, 9}));
    EXPECT_TRUE((rect_pos[1] == std::vector<int> {10, 11, 9}));
    EXPECT_TRUE((rect_pos[2] == std::vector<int> {10, 12, 9}));
    EXPECT_TRUE((rect_pos[3] == std::vector<int> {10, 13, 9}));
    EXPECT_TRUE((rect_pos[4] == std::vector<int> {10, 14, 9}));
    EXPECT_TRUE((rect_pos[5] == std::vector<int> {10, 15, 9}));
    EXPECT_TRUE((rect_pos[6] == std::vector<int> {11, 10, 9}));
    EXPECT_TRUE((rect_pos[7] == std::vector<int> {11, 11, 9}));
    EXPECT_TRUE((rect_pos[8] == std::vector<int> {11, 12, 9}));
    EXPECT_TRUE((rect_pos[9] == std::vector<int> {11, 13, 9}));
    EXPECT_TRUE((rect_pos[10] == std::vector<int> {11, 14, 9}));
    EXPECT_TRUE((rect_pos[11] == std::vector<int> {11, 15, 9}));
    EXPECT_TRUE((rect_pos[12] == std::vector<int> {12, 10, 9}));
    EXPECT_TRUE((rect_pos[13] == std::vector<int> {12, 11, 9}));
    EXPECT_TRUE((rect_pos[14] == std::vector<int> {12, 12, 9}));
    EXPECT_TRUE((rect_pos[15] == std::vector<int> {12, 13, 9}));
    EXPECT_TRUE((rect_pos[16] == std::vector<int> {12, 14, 9}));
    EXPECT_TRUE((rect_pos[17] == std::vector<int> {12, 15, 9}));
    EXPECT_TRUE((rect_pos[18] == std::vector<int> {13, 10, 9}));
    EXPECT_TRUE((rect_pos[19] == std::vector<int> {13, 11, 9}));
    EXPECT_TRUE((rect_pos[20] == std::vector<int> {13, 12, 9}));
    EXPECT_TRUE((rect_pos[21] == std::vector<int> {13, 13, 9}));
    EXPECT_TRUE((rect_pos[22] == std::vector<int> {13, 14, 9}));
    EXPECT_TRUE((rect_pos[23] == std::vector<int> {13, 15, 9}));
    EXPECT_TRUE((rect_pos[24] == std::vector<int> {14, 10, 9}));
    EXPECT_TRUE((rect_pos[25] == std::vector<int> {14, 11, 9}));
    EXPECT_TRUE((rect_pos[26] == std::vector<int> {14, 12, 9}));
    EXPECT_TRUE((rect_pos[27] == std::vector<int> {14, 13, 9}));
    EXPECT_TRUE((rect_pos[28] == std::vector<int> {14, 14, 9}));
    EXPECT_TRUE((rect_pos[29] == std::vector<int> {14, 15, 9}));
    EXPECT_TRUE((rect_pos[30] == std::vector<int> {15, 10, 9}));
    EXPECT_TRUE((rect_pos[31] == std::vector<int> {15, 11, 9}));
    EXPECT_TRUE((rect_pos[32] == std::vector<int> {15, 12, 9}));
    EXPECT_TRUE((rect_pos[33] == std::vector<int> {15, 13, 9}));
    EXPECT_TRUE((rect_pos[34] == std::vector<int> {15, 14, 9}));
    EXPECT_TRUE((rect_pos[35] == std::vector<int> {15, 15, 9}));

    // Test: BP-CR-3
    rect_pos = soil_simulator::CalcRectanglePos(c, b, a, d, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 36);
    EXPECT_TRUE((rect_pos[0] == std::vector<int> {10, 10, 9}));
    EXPECT_TRUE((rect_pos[1] == std::vector<int> {10, 11, 9}));
    EXPECT_TRUE((rect_pos[2] == std::vector<int> {10, 12, 9}));
    EXPECT_TRUE((rect_pos[3] == std::vector<int> {10, 13, 9}));
    EXPECT_TRUE((rect_pos[4] == std::vector<int> {10, 14, 9}));
    EXPECT_TRUE((rect_pos[5] == std::vector<int> {10, 15, 9}));
    EXPECT_TRUE((rect_pos[6] == std::vector<int> {11, 10, 9}));
    EXPECT_TRUE((rect_pos[7] == std::vector<int> {11, 11, 9}));
    EXPECT_TRUE((rect_pos[8] == std::vector<int> {11, 12, 9}));
    EXPECT_TRUE((rect_pos[9] == std::vector<int> {11, 13, 9}));
    EXPECT_TRUE((rect_pos[10] == std::vector<int> {11, 14, 9}));
    EXPECT_TRUE((rect_pos[11] == std::vector<int> {11, 15, 9}));
    EXPECT_TRUE((rect_pos[12] == std::vector<int> {12, 10, 9}));
    EXPECT_TRUE((rect_pos[13] == std::vector<int> {12, 11, 9}));
    EXPECT_TRUE((rect_pos[14] == std::vector<int> {12, 12, 9}));
    EXPECT_TRUE((rect_pos[15] == std::vector<int> {12, 13, 9}));
    EXPECT_TRUE((rect_pos[16] == std::vector<int> {12, 14, 9}));
    EXPECT_TRUE((rect_pos[17] == std::vector<int> {12, 15, 9}));
    EXPECT_TRUE((rect_pos[18] == std::vector<int> {13, 10, 9}));
    EXPECT_TRUE((rect_pos[19] == std::vector<int> {13, 11, 9}));
    EXPECT_TRUE((rect_pos[20] == std::vector<int> {13, 12, 9}));
    EXPECT_TRUE((rect_pos[21] == std::vector<int> {13, 13, 9}));
    EXPECT_TRUE((rect_pos[22] == std::vector<int> {13, 14, 9}));
    EXPECT_TRUE((rect_pos[23] == std::vector<int> {13, 15, 9}));
    EXPECT_TRUE((rect_pos[24] == std::vector<int> {14, 10, 9}));
    EXPECT_TRUE((rect_pos[25] == std::vector<int> {14, 11, 9}));
    EXPECT_TRUE((rect_pos[26] == std::vector<int> {14, 12, 9}));
    EXPECT_TRUE((rect_pos[27] == std::vector<int> {14, 13, 9}));
    EXPECT_TRUE((rect_pos[28] == std::vector<int> {14, 14, 9}));
    EXPECT_TRUE((rect_pos[29] == std::vector<int> {14, 15, 9}));
    EXPECT_TRUE((rect_pos[30] == std::vector<int> {15, 10, 9}));
    EXPECT_TRUE((rect_pos[31] == std::vector<int> {15, 11, 9}));
    EXPECT_TRUE((rect_pos[32] == std::vector<int> {15, 12, 9}));
    EXPECT_TRUE((rect_pos[33] == std::vector<int> {15, 13, 9}));
    EXPECT_TRUE((rect_pos[34] == std::vector<int> {15, 14, 9}));
    EXPECT_TRUE((rect_pos[35] == std::vector<int> {15, 15, 9}));

    // Test: BP-CR-4
    rect_pos = soil_simulator::CalcRectanglePos(b, c, d, a, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 36);
    rect_pos = soil_simulator::CalcRectanglePos(c, d, a, b, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 36);
    rect_pos = soil_simulator::CalcRectanglePos(d, a, b, c, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 36);
    rect_pos = soil_simulator::CalcRectanglePos(d, c, b, a, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 36);
    rect_pos = soil_simulator::CalcRectanglePos(b, a, d, c, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 36);

    // Test: BP-CR-5
    a = {0.0 + 1e-5, -0.05 + 1e-5, 0.0 - 1e-5};
    b = {0.5 - 1e-5, -0.05 + 1e-5, 0.0 - 1e-5};
    c = {0.5 - 1e-5,  0.25 - 1e-5, 0.0 - 1e-5};
    d = {0.0 + 1e-5,  0.25 - 1e-5, 0.0 - 1e-5};
    rect_pos = soil_simulator::CalcRectanglePos(a, b, c, d, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    EXPECT_TRUE((rect_pos[0] == std::vector<int> {10, 10, 9}));
    EXPECT_TRUE((rect_pos[1] == std::vector<int> {10, 11, 9}));
    EXPECT_TRUE((rect_pos[2] == std::vector<int> {10, 12, 9}));
    EXPECT_TRUE((rect_pos[3] == std::vector<int> {11, 10, 9}));
    EXPECT_TRUE((rect_pos[4] == std::vector<int> {11, 11, 9}));
    EXPECT_TRUE((rect_pos[5] == std::vector<int> {11, 12, 9}));
    EXPECT_TRUE((rect_pos[6] == std::vector<int> {12, 10, 9}));
    EXPECT_TRUE((rect_pos[7] == std::vector<int> {12, 11, 9}));
    EXPECT_TRUE((rect_pos[8] == std::vector<int> {12, 12, 9}));
    EXPECT_TRUE((rect_pos[9] == std::vector<int> {13, 10, 9}));
    EXPECT_TRUE((rect_pos[10] == std::vector<int> {13, 11, 9}));
    EXPECT_TRUE((rect_pos[11] == std::vector<int> {13, 12, 9}));
    EXPECT_TRUE((rect_pos[12] == std::vector<int> {14, 10, 9}));
    EXPECT_TRUE((rect_pos[13] == std::vector<int> {14, 11, 9}));
    EXPECT_TRUE((rect_pos[14] == std::vector<int> {14, 12, 9}));
    EXPECT_TRUE((rect_pos[15] == std::vector<int> {15, 10, 9}));
    EXPECT_TRUE((rect_pos[16] == std::vector<int> {15, 11, 9}));
    EXPECT_TRUE((rect_pos[17] == std::vector<int> {15, 12, 9}));
    rect_pos = soil_simulator::CalcRectanglePos(a, d, c, b, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    rect_pos = soil_simulator::CalcRectanglePos(c, b, a, d, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    rect_pos = soil_simulator::CalcRectanglePos(b, c, d, a, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    rect_pos = soil_simulator::CalcRectanglePos(c, d, a, b, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    rect_pos = soil_simulator::CalcRectanglePos(d, a, b, c, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    rect_pos = soil_simulator::CalcRectanglePos(d, c, b, a, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    rect_pos = soil_simulator::CalcRectanglePos(b, a, d, c, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);

    // Test: BP-CR-6
    a = {0.0 + 1e-5, 0.0 - 1e-5, 0.0 + 1e-5};
    b = {0.5 - 1e-5, 0.0 - 1e-5, 0.0 + 1e-5};
    c = {0.5 - 1e-5, 0.0 - 1e-5, 0.5 - 1e-5};
    d = {0.0 + 1e-5, 0.0 - 1e-5, 0.5 - 1e-5};
    rect_pos = soil_simulator::CalcRectanglePos(a, b, c, d, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    EXPECT_TRUE((rect_pos[0] == std::vector<int> {10, 10, 10}));
    EXPECT_TRUE((rect_pos[1] == std::vector<int> {10, 10, 11}));
    EXPECT_TRUE((rect_pos[2] == std::vector<int> {10, 10, 12}));
    EXPECT_TRUE((rect_pos[3] == std::vector<int> {10, 10, 13}));
    EXPECT_TRUE((rect_pos[4] == std::vector<int> {10, 10, 14}));
    EXPECT_TRUE((rect_pos[5] == std::vector<int> {11, 10, 10}));
    EXPECT_TRUE((rect_pos[6] == std::vector<int> {11, 10, 14}));
    EXPECT_TRUE((rect_pos[7] == std::vector<int> {12, 10, 10}));
    EXPECT_TRUE((rect_pos[8] == std::vector<int> {12, 10, 14}));
    EXPECT_TRUE((rect_pos[9] == std::vector<int> {13, 10, 10}));
    EXPECT_TRUE((rect_pos[10] == std::vector<int> {13, 10, 14}));
    EXPECT_TRUE((rect_pos[11] == std::vector<int> {14, 10, 10}));
    EXPECT_TRUE((rect_pos[12] == std::vector<int> {14, 10, 14}));
    EXPECT_TRUE((rect_pos[13] == std::vector<int> {15, 10, 10}));
    EXPECT_TRUE((rect_pos[14] == std::vector<int> {15, 10, 11}));
    EXPECT_TRUE((rect_pos[15] == std::vector<int> {15, 10, 12}));
    EXPECT_TRUE((rect_pos[16] == std::vector<int> {15, 10, 13}));
    EXPECT_TRUE((rect_pos[17] == std::vector<int> {15, 10, 14}));
    rect_pos = soil_simulator::CalcRectanglePos(a, d, c, b, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    rect_pos = soil_simulator::CalcRectanglePos(c, b, a, d, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    rect_pos = soil_simulator::CalcRectanglePos(b, c, d, a, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    rect_pos = soil_simulator::CalcRectanglePos(c, d, a, b, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    rect_pos = soil_simulator::CalcRectanglePos(d, a, b, c, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    rect_pos = soil_simulator::CalcRectanglePos(d, c, b, a, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);
    rect_pos = soil_simulator::CalcRectanglePos(b, a, d, c, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 18);

    // Test: BP-CR-7
    a = {0.5 + 1e-5, 0.0 + 1e-5, 0.5 + 1e-5};
    b = {0.6 - 1e-5, 0.0 + 1e-5, 0.6 - 1e-5};
    c = {0.6 - 1e-5, 0.5 - 1e-5, 0.6 - 1e-5};
    d = {0.5 + 1e-5, 0.5 - 1e-5, 0.5 + 1e-5};
    rect_pos = soil_simulator::CalcRectanglePos(a, b, c, d, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 12);
    EXPECT_TRUE((rect_pos[0] == std::vector<int> {15, 10, 15}));
    EXPECT_TRUE((rect_pos[1] == std::vector<int> {15, 11, 15}));
    EXPECT_TRUE((rect_pos[2] == std::vector<int> {15, 12, 15}));
    EXPECT_TRUE((rect_pos[3] == std::vector<int> {15, 13, 15}));
    EXPECT_TRUE((rect_pos[4] == std::vector<int> {15, 14, 15}));
    EXPECT_TRUE((rect_pos[5] == std::vector<int> {15, 15, 15}));
    EXPECT_TRUE((rect_pos[6] == std::vector<int> {16, 10, 15}));
    EXPECT_TRUE((rect_pos[7] == std::vector<int> {16, 11, 15}));
    EXPECT_TRUE((rect_pos[8] == std::vector<int> {16, 12, 15}));
    EXPECT_TRUE((rect_pos[9] == std::vector<int> {16, 13, 15}));
    EXPECT_TRUE((rect_pos[10] == std::vector<int> {16, 14, 15}));
    EXPECT_TRUE((rect_pos[11] == std::vector<int> {16, 15, 15}));
    rect_pos = soil_simulator::CalcRectanglePos(a, d, c, b, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 12);
    rect_pos = soil_simulator::CalcRectanglePos(c, b, a, d, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 12);
    rect_pos = soil_simulator::CalcRectanglePos(b, c, d, a, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 12);
    rect_pos = soil_simulator::CalcRectanglePos(c, d, a, b, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 12);
    rect_pos = soil_simulator::CalcRectanglePos(d, a, b, c, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 12);
    rect_pos = soil_simulator::CalcRectanglePos(d, c, b, a, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 12);
    rect_pos = soil_simulator::CalcRectanglePos(b, a, d, c, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 12);

    // Test: BP-CR-8
    a = {0.34 + 1e-5, 0.57 + 1e-5, 0.0 - 1e-5};
    b = {0.74 - 1e-5, 0.97 - 1e-5, 0.0 - 1e-5};
    c = {0.44 + 1e-5, 0.67 + 1e-5, 0.0 - 1e-5};
    d = {0.64 - 1e-5, 0.87 - 1e-5, 0.0 - 1e-5};
    rect_pos = soil_simulator::CalcRectanglePos(a, b, c, d, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 9);
    EXPECT_TRUE((rect_pos[0] == std::vector<int> {13, 16, 9}));
    EXPECT_TRUE((rect_pos[1] == std::vector<int> {14, 16, 9}));
    EXPECT_TRUE((rect_pos[2] == std::vector<int> {14, 17, 9}));
    EXPECT_TRUE((rect_pos[3] == std::vector<int> {15, 17, 9}));
    EXPECT_TRUE((rect_pos[4] == std::vector<int> {15, 18, 9}));
    EXPECT_TRUE((rect_pos[5] == std::vector<int> {16, 18, 9}));
    EXPECT_TRUE((rect_pos[6] == std::vector<int> {16, 19, 9}));
    EXPECT_TRUE((rect_pos[7] == std::vector<int> {17, 19, 9}));
    EXPECT_TRUE((rect_pos[8] == std::vector<int> {17, 20, 9}));

    // Test: BP-CR-9
    a = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    b = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    c = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    d = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    rect_pos = soil_simulator::CalcRectanglePos(a, b, c, d, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 1);
    EXPECT_TRUE((rect_pos[0] == std::vector<int> {15, 15, 14}));

    // Test: BP-CR-10
    a = {0.55 - 1e-5, 0.55 - 1e-5, 0.5 - 1e-5};
    b = {0.55 - 1e-5, 0.55 - 1e-5, 0.5 - 1e-5};
    c = {0.55 - 1e-5, 0.55 - 1e-5, 0.5 - 1e-5};
    d = {0.55 - 1e-5, 0.55 - 1e-5, 0.5 - 1e-5};
    rect_pos = soil_simulator::CalcRectanglePos(a, b, c, d, grid, tol);
    sort(rect_pos.begin(), rect_pos.end());
    rect_pos.erase(unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
    EXPECT_EQ(rect_pos.size(), 1);
    EXPECT_TRUE((rect_pos[0] == std::vector<int> {15, 15, 14}));
}

TEST(UnitTestBucketPos, CalcTrianglePos) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);

    // Declaring variables
    std::vector<float> a;
    std::vector<float> b;
    std::vector<float> c;
    float tol = 1e-5;
    std::vector<std::vector<int>> tri_pos;

    // Test: BP-CT-1
    a = {0.0 + 1e-5, 0.0 + 1e-5, 0.0 - 1e-5};
    b = {1.0 - 1e-5, 0.0 + 1e-5, 0.0 - 1e-5};
    c = {0.0 + 1e-5, 1.0 - 1e-5, 0.0 - 1e-5};
    tri_pos = soil_simulator::CalcTrianglePos(a, b, c, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 76);
    EXPECT_TRUE((tri_pos[0] == std::vector<int> {10, 10, 9}));
    EXPECT_TRUE((tri_pos[1] == std::vector<int> {10, 11, 9}));
    EXPECT_TRUE((tri_pos[2] == std::vector<int> {10, 12, 9}));
    EXPECT_TRUE((tri_pos[3] == std::vector<int> {10, 13, 9}));
    EXPECT_TRUE((tri_pos[4] == std::vector<int> {10, 14, 9}));
    EXPECT_TRUE((tri_pos[5] == std::vector<int> {10, 15, 9}));
    EXPECT_TRUE((tri_pos[6] == std::vector<int> {10, 16, 9}));
    EXPECT_TRUE((tri_pos[7] == std::vector<int> {10, 17, 9}));
    EXPECT_TRUE((tri_pos[8] == std::vector<int> {10, 18, 9}));
    EXPECT_TRUE((tri_pos[9] == std::vector<int> {10, 19, 9}));
    EXPECT_TRUE((tri_pos[10] == std::vector<int> {10, 20, 9}));
    EXPECT_TRUE((tri_pos[11] == std::vector<int> {11, 10, 9}));
    EXPECT_TRUE((tri_pos[12] == std::vector<int> {11, 11, 9}));
    EXPECT_TRUE((tri_pos[13] == std::vector<int> {11, 12, 9}));
    EXPECT_TRUE((tri_pos[14] == std::vector<int> {11, 13, 9}));
    EXPECT_TRUE((tri_pos[15] == std::vector<int> {11, 14, 9}));
    EXPECT_TRUE((tri_pos[16] == std::vector<int> {11, 15, 9}));
    EXPECT_TRUE((tri_pos[17] == std::vector<int> {11, 16, 9}));
    EXPECT_TRUE((tri_pos[18] == std::vector<int> {11, 17, 9}));
    EXPECT_TRUE((tri_pos[19] == std::vector<int> {11, 18, 9}));
    EXPECT_TRUE((tri_pos[20] == std::vector<int> {11, 19, 9}));
    EXPECT_TRUE((tri_pos[21] == std::vector<int> {11, 20, 9}));
    EXPECT_TRUE((tri_pos[22] == std::vector<int> {12, 10, 9}));
    EXPECT_TRUE((tri_pos[23] == std::vector<int> {12, 11, 9}));
    EXPECT_TRUE((tri_pos[24] == std::vector<int> {12, 12, 9}));
    EXPECT_TRUE((tri_pos[25] == std::vector<int> {12, 13, 9}));
    EXPECT_TRUE((tri_pos[26] == std::vector<int> {12, 14, 9}));
    EXPECT_TRUE((tri_pos[27] == std::vector<int> {12, 15, 9}));
    EXPECT_TRUE((tri_pos[28] == std::vector<int> {12, 16, 9}));
    EXPECT_TRUE((tri_pos[29] == std::vector<int> {12, 17, 9}));
    EXPECT_TRUE((tri_pos[30] == std::vector<int> {12, 18, 9}));
    EXPECT_TRUE((tri_pos[31] == std::vector<int> {12, 19, 9}));
    EXPECT_TRUE((tri_pos[32] == std::vector<int> {13, 10, 9}));
    EXPECT_TRUE((tri_pos[33] == std::vector<int> {13, 11, 9}));
    EXPECT_TRUE((tri_pos[34] == std::vector<int> {13, 12, 9}));
    EXPECT_TRUE((tri_pos[35] == std::vector<int> {13, 13, 9}));
    EXPECT_TRUE((tri_pos[36] == std::vector<int> {13, 14, 9}));
    EXPECT_TRUE((tri_pos[37] == std::vector<int> {13, 15, 9}));
    EXPECT_TRUE((tri_pos[38] == std::vector<int> {13, 16, 9}));
    EXPECT_TRUE((tri_pos[39] == std::vector<int> {13, 17, 9}));
    EXPECT_TRUE((tri_pos[40] == std::vector<int> {13, 18, 9}));
    EXPECT_TRUE((tri_pos[41] == std::vector<int> {14, 10, 9}));
    EXPECT_TRUE((tri_pos[42] == std::vector<int> {14, 11, 9}));
    EXPECT_TRUE((tri_pos[43] == std::vector<int> {14, 12, 9}));
    EXPECT_TRUE((tri_pos[44] == std::vector<int> {14, 13, 9}));
    EXPECT_TRUE((tri_pos[45] == std::vector<int> {14, 14, 9}));
    EXPECT_TRUE((tri_pos[46] == std::vector<int> {14, 15, 9}));
    EXPECT_TRUE((tri_pos[47] == std::vector<int> {14, 16, 9}));
    EXPECT_TRUE((tri_pos[48] == std::vector<int> {14, 17, 9}));
    EXPECT_TRUE((tri_pos[49] == std::vector<int> {15, 10, 9}));
    EXPECT_TRUE((tri_pos[50] == std::vector<int> {15, 11, 9}));
    EXPECT_TRUE((tri_pos[51] == std::vector<int> {15, 12, 9}));
    EXPECT_TRUE((tri_pos[52] == std::vector<int> {15, 13, 9}));
    EXPECT_TRUE((tri_pos[53] == std::vector<int> {15, 14, 9}));
    EXPECT_TRUE((tri_pos[54] == std::vector<int> {15, 15, 9}));
    EXPECT_TRUE((tri_pos[55] == std::vector<int> {15, 16, 9}));
    EXPECT_TRUE((tri_pos[56] == std::vector<int> {16, 10, 9}));
    EXPECT_TRUE((tri_pos[57] == std::vector<int> {16, 11, 9}));
    EXPECT_TRUE((tri_pos[58] == std::vector<int> {16, 12, 9}));
    EXPECT_TRUE((tri_pos[59] == std::vector<int> {16, 13, 9}));
    EXPECT_TRUE((tri_pos[60] == std::vector<int> {16, 14, 9}));
    EXPECT_TRUE((tri_pos[61] == std::vector<int> {16, 15, 9}));
    EXPECT_TRUE((tri_pos[62] == std::vector<int> {17, 10, 9}));
    EXPECT_TRUE((tri_pos[63] == std::vector<int> {17, 11, 9}));
    EXPECT_TRUE((tri_pos[64] == std::vector<int> {17, 12, 9}));
    EXPECT_TRUE((tri_pos[65] == std::vector<int> {17, 13, 9}));
    EXPECT_TRUE((tri_pos[66] == std::vector<int> {17, 14, 9}));
    EXPECT_TRUE((tri_pos[67] == std::vector<int> {18, 10, 9}));
    EXPECT_TRUE((tri_pos[68] == std::vector<int> {18, 11, 9}));
    EXPECT_TRUE((tri_pos[69] == std::vector<int> {18, 12, 9}));
    EXPECT_TRUE((tri_pos[70] == std::vector<int> {18, 13, 9}));
    EXPECT_TRUE((tri_pos[71] == std::vector<int> {19, 10, 9}));
    EXPECT_TRUE((tri_pos[72] == std::vector<int> {19, 11, 9}));
    EXPECT_TRUE((tri_pos[73] == std::vector<int> {19, 12, 9}));
    EXPECT_TRUE((tri_pos[74] == std::vector<int> {20, 10, 9}));
    EXPECT_TRUE((tri_pos[75] == std::vector<int> {20, 11, 9}));

    // Test: BP-CT-2
    tri_pos = soil_simulator::CalcTrianglePos(b, a, c, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 76);
    EXPECT_TRUE((tri_pos[0] == std::vector<int> {10, 10, 9}));
    EXPECT_TRUE((tri_pos[1] == std::vector<int> {10, 11, 9}));
    EXPECT_TRUE((tri_pos[2] == std::vector<int> {10, 12, 9}));
    EXPECT_TRUE((tri_pos[3] == std::vector<int> {10, 13, 9}));
    EXPECT_TRUE((tri_pos[4] == std::vector<int> {10, 14, 9}));
    EXPECT_TRUE((tri_pos[5] == std::vector<int> {10, 15, 9}));
    EXPECT_TRUE((tri_pos[6] == std::vector<int> {10, 16, 9}));
    EXPECT_TRUE((tri_pos[7] == std::vector<int> {10, 17, 9}));
    EXPECT_TRUE((tri_pos[8] == std::vector<int> {10, 18, 9}));
    EXPECT_TRUE((tri_pos[9] == std::vector<int> {10, 19, 9}));
    EXPECT_TRUE((tri_pos[10] == std::vector<int> {10, 20, 9}));
    EXPECT_TRUE((tri_pos[11] == std::vector<int> {11, 10, 9}));
    EXPECT_TRUE((tri_pos[12] == std::vector<int> {11, 11, 9}));
    EXPECT_TRUE((tri_pos[13] == std::vector<int> {11, 12, 9}));
    EXPECT_TRUE((tri_pos[14] == std::vector<int> {11, 13, 9}));
    EXPECT_TRUE((tri_pos[15] == std::vector<int> {11, 14, 9}));
    EXPECT_TRUE((tri_pos[16] == std::vector<int> {11, 15, 9}));
    EXPECT_TRUE((tri_pos[17] == std::vector<int> {11, 16, 9}));
    EXPECT_TRUE((tri_pos[18] == std::vector<int> {11, 17, 9}));
    EXPECT_TRUE((tri_pos[19] == std::vector<int> {11, 18, 9}));
    EXPECT_TRUE((tri_pos[20] == std::vector<int> {11, 19, 9}));
    EXPECT_TRUE((tri_pos[21] == std::vector<int> {11, 20, 9}));
    EXPECT_TRUE((tri_pos[22] == std::vector<int> {12, 10, 9}));
    EXPECT_TRUE((tri_pos[23] == std::vector<int> {12, 11, 9}));
    EXPECT_TRUE((tri_pos[24] == std::vector<int> {12, 12, 9}));
    EXPECT_TRUE((tri_pos[25] == std::vector<int> {12, 13, 9}));
    EXPECT_TRUE((tri_pos[26] == std::vector<int> {12, 14, 9}));
    EXPECT_TRUE((tri_pos[27] == std::vector<int> {12, 15, 9}));
    EXPECT_TRUE((tri_pos[28] == std::vector<int> {12, 16, 9}));
    EXPECT_TRUE((tri_pos[29] == std::vector<int> {12, 17, 9}));
    EXPECT_TRUE((tri_pos[30] == std::vector<int> {12, 18, 9}));
    EXPECT_TRUE((tri_pos[31] == std::vector<int> {12, 19, 9}));
    EXPECT_TRUE((tri_pos[32] == std::vector<int> {13, 10, 9}));
    EXPECT_TRUE((tri_pos[33] == std::vector<int> {13, 11, 9}));
    EXPECT_TRUE((tri_pos[34] == std::vector<int> {13, 12, 9}));
    EXPECT_TRUE((tri_pos[35] == std::vector<int> {13, 13, 9}));
    EXPECT_TRUE((tri_pos[36] == std::vector<int> {13, 14, 9}));
    EXPECT_TRUE((tri_pos[37] == std::vector<int> {13, 15, 9}));
    EXPECT_TRUE((tri_pos[38] == std::vector<int> {13, 16, 9}));
    EXPECT_TRUE((tri_pos[39] == std::vector<int> {13, 17, 9}));
    EXPECT_TRUE((tri_pos[40] == std::vector<int> {13, 18, 9}));
    EXPECT_TRUE((tri_pos[41] == std::vector<int> {14, 10, 9}));
    EXPECT_TRUE((tri_pos[42] == std::vector<int> {14, 11, 9}));
    EXPECT_TRUE((tri_pos[43] == std::vector<int> {14, 12, 9}));
    EXPECT_TRUE((tri_pos[44] == std::vector<int> {14, 13, 9}));
    EXPECT_TRUE((tri_pos[45] == std::vector<int> {14, 14, 9}));
    EXPECT_TRUE((tri_pos[46] == std::vector<int> {14, 15, 9}));
    EXPECT_TRUE((tri_pos[47] == std::vector<int> {14, 16, 9}));
    EXPECT_TRUE((tri_pos[48] == std::vector<int> {14, 17, 9}));
    EXPECT_TRUE((tri_pos[49] == std::vector<int> {15, 10, 9}));
    EXPECT_TRUE((tri_pos[50] == std::vector<int> {15, 11, 9}));
    EXPECT_TRUE((tri_pos[51] == std::vector<int> {15, 12, 9}));
    EXPECT_TRUE((tri_pos[52] == std::vector<int> {15, 13, 9}));
    EXPECT_TRUE((tri_pos[53] == std::vector<int> {15, 14, 9}));
    EXPECT_TRUE((tri_pos[54] == std::vector<int> {15, 15, 9}));
    EXPECT_TRUE((tri_pos[55] == std::vector<int> {15, 16, 9}));
    EXPECT_TRUE((tri_pos[56] == std::vector<int> {16, 10, 9}));
    EXPECT_TRUE((tri_pos[57] == std::vector<int> {16, 11, 9}));
    EXPECT_TRUE((tri_pos[58] == std::vector<int> {16, 12, 9}));
    EXPECT_TRUE((tri_pos[59] == std::vector<int> {16, 13, 9}));
    EXPECT_TRUE((tri_pos[60] == std::vector<int> {16, 14, 9}));
    EXPECT_TRUE((tri_pos[61] == std::vector<int> {16, 15, 9}));
    EXPECT_TRUE((tri_pos[62] == std::vector<int> {17, 10, 9}));
    EXPECT_TRUE((tri_pos[63] == std::vector<int> {17, 11, 9}));
    EXPECT_TRUE((tri_pos[64] == std::vector<int> {17, 12, 9}));
    EXPECT_TRUE((tri_pos[65] == std::vector<int> {17, 13, 9}));
    EXPECT_TRUE((tri_pos[66] == std::vector<int> {17, 14, 9}));
    EXPECT_TRUE((tri_pos[67] == std::vector<int> {18, 10, 9}));
    EXPECT_TRUE((tri_pos[68] == std::vector<int> {18, 11, 9}));
    EXPECT_TRUE((tri_pos[69] == std::vector<int> {18, 12, 9}));
    EXPECT_TRUE((tri_pos[70] == std::vector<int> {18, 13, 9}));
    EXPECT_TRUE((tri_pos[71] == std::vector<int> {19, 10, 9}));
    EXPECT_TRUE((tri_pos[72] == std::vector<int> {19, 11, 9}));
    EXPECT_TRUE((tri_pos[73] == std::vector<int> {19, 12, 9}));
    EXPECT_TRUE((tri_pos[74] == std::vector<int> {20, 10, 9}));
    EXPECT_TRUE((tri_pos[75] == std::vector<int> {20, 11, 9}));

    // Test: BP-CT-3
    tri_pos = soil_simulator::CalcTrianglePos(c, a, b, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 76);
    EXPECT_TRUE((tri_pos[0] == std::vector<int> {10, 10, 9}));
    EXPECT_TRUE((tri_pos[1] == std::vector<int> {10, 11, 9}));
    EXPECT_TRUE((tri_pos[2] == std::vector<int> {10, 12, 9}));
    EXPECT_TRUE((tri_pos[3] == std::vector<int> {10, 13, 9}));
    EXPECT_TRUE((tri_pos[4] == std::vector<int> {10, 14, 9}));
    EXPECT_TRUE((tri_pos[5] == std::vector<int> {10, 15, 9}));
    EXPECT_TRUE((tri_pos[6] == std::vector<int> {10, 16, 9}));
    EXPECT_TRUE((tri_pos[7] == std::vector<int> {10, 17, 9}));
    EXPECT_TRUE((tri_pos[8] == std::vector<int> {10, 18, 9}));
    EXPECT_TRUE((tri_pos[9] == std::vector<int> {10, 19, 9}));
    EXPECT_TRUE((tri_pos[10] == std::vector<int> {10, 20, 9}));
    EXPECT_TRUE((tri_pos[11] == std::vector<int> {11, 10, 9}));
    EXPECT_TRUE((tri_pos[12] == std::vector<int> {11, 11, 9}));
    EXPECT_TRUE((tri_pos[13] == std::vector<int> {11, 12, 9}));
    EXPECT_TRUE((tri_pos[14] == std::vector<int> {11, 13, 9}));
    EXPECT_TRUE((tri_pos[15] == std::vector<int> {11, 14, 9}));
    EXPECT_TRUE((tri_pos[16] == std::vector<int> {11, 15, 9}));
    EXPECT_TRUE((tri_pos[17] == std::vector<int> {11, 16, 9}));
    EXPECT_TRUE((tri_pos[18] == std::vector<int> {11, 17, 9}));
    EXPECT_TRUE((tri_pos[19] == std::vector<int> {11, 18, 9}));
    EXPECT_TRUE((tri_pos[20] == std::vector<int> {11, 19, 9}));
    EXPECT_TRUE((tri_pos[21] == std::vector<int> {11, 20, 9}));
    EXPECT_TRUE((tri_pos[22] == std::vector<int> {12, 10, 9}));
    EXPECT_TRUE((tri_pos[23] == std::vector<int> {12, 11, 9}));
    EXPECT_TRUE((tri_pos[24] == std::vector<int> {12, 12, 9}));
    EXPECT_TRUE((tri_pos[25] == std::vector<int> {12, 13, 9}));
    EXPECT_TRUE((tri_pos[26] == std::vector<int> {12, 14, 9}));
    EXPECT_TRUE((tri_pos[27] == std::vector<int> {12, 15, 9}));
    EXPECT_TRUE((tri_pos[28] == std::vector<int> {12, 16, 9}));
    EXPECT_TRUE((tri_pos[29] == std::vector<int> {12, 17, 9}));
    EXPECT_TRUE((tri_pos[30] == std::vector<int> {12, 18, 9}));
    EXPECT_TRUE((tri_pos[31] == std::vector<int> {12, 19, 9}));
    EXPECT_TRUE((tri_pos[32] == std::vector<int> {13, 10, 9}));
    EXPECT_TRUE((tri_pos[33] == std::vector<int> {13, 11, 9}));
    EXPECT_TRUE((tri_pos[34] == std::vector<int> {13, 12, 9}));
    EXPECT_TRUE((tri_pos[35] == std::vector<int> {13, 13, 9}));
    EXPECT_TRUE((tri_pos[36] == std::vector<int> {13, 14, 9}));
    EXPECT_TRUE((tri_pos[37] == std::vector<int> {13, 15, 9}));
    EXPECT_TRUE((tri_pos[38] == std::vector<int> {13, 16, 9}));
    EXPECT_TRUE((tri_pos[39] == std::vector<int> {13, 17, 9}));
    EXPECT_TRUE((tri_pos[40] == std::vector<int> {13, 18, 9}));
    EXPECT_TRUE((tri_pos[41] == std::vector<int> {14, 10, 9}));
    EXPECT_TRUE((tri_pos[42] == std::vector<int> {14, 11, 9}));
    EXPECT_TRUE((tri_pos[43] == std::vector<int> {14, 12, 9}));
    EXPECT_TRUE((tri_pos[44] == std::vector<int> {14, 13, 9}));
    EXPECT_TRUE((tri_pos[45] == std::vector<int> {14, 14, 9}));
    EXPECT_TRUE((tri_pos[46] == std::vector<int> {14, 15, 9}));
    EXPECT_TRUE((tri_pos[47] == std::vector<int> {14, 16, 9}));
    EXPECT_TRUE((tri_pos[48] == std::vector<int> {14, 17, 9}));
    EXPECT_TRUE((tri_pos[49] == std::vector<int> {15, 10, 9}));
    EXPECT_TRUE((tri_pos[50] == std::vector<int> {15, 11, 9}));
    EXPECT_TRUE((tri_pos[51] == std::vector<int> {15, 12, 9}));
    EXPECT_TRUE((tri_pos[52] == std::vector<int> {15, 13, 9}));
    EXPECT_TRUE((tri_pos[53] == std::vector<int> {15, 14, 9}));
    EXPECT_TRUE((tri_pos[54] == std::vector<int> {15, 15, 9}));
    EXPECT_TRUE((tri_pos[55] == std::vector<int> {15, 16, 9}));
    EXPECT_TRUE((tri_pos[56] == std::vector<int> {16, 10, 9}));
    EXPECT_TRUE((tri_pos[57] == std::vector<int> {16, 11, 9}));
    EXPECT_TRUE((tri_pos[58] == std::vector<int> {16, 12, 9}));
    EXPECT_TRUE((tri_pos[59] == std::vector<int> {16, 13, 9}));
    EXPECT_TRUE((tri_pos[60] == std::vector<int> {16, 14, 9}));
    EXPECT_TRUE((tri_pos[61] == std::vector<int> {16, 15, 9}));
    EXPECT_TRUE((tri_pos[62] == std::vector<int> {17, 10, 9}));
    EXPECT_TRUE((tri_pos[63] == std::vector<int> {17, 11, 9}));
    EXPECT_TRUE((tri_pos[64] == std::vector<int> {17, 12, 9}));
    EXPECT_TRUE((tri_pos[65] == std::vector<int> {17, 13, 9}));
    EXPECT_TRUE((tri_pos[66] == std::vector<int> {17, 14, 9}));
    EXPECT_TRUE((tri_pos[67] == std::vector<int> {18, 10, 9}));
    EXPECT_TRUE((tri_pos[68] == std::vector<int> {18, 11, 9}));
    EXPECT_TRUE((tri_pos[69] == std::vector<int> {18, 12, 9}));
    EXPECT_TRUE((tri_pos[70] == std::vector<int> {18, 13, 9}));
    EXPECT_TRUE((tri_pos[71] == std::vector<int> {19, 10, 9}));
    EXPECT_TRUE((tri_pos[72] == std::vector<int> {19, 11, 9}));
    EXPECT_TRUE((tri_pos[73] == std::vector<int> {19, 12, 9}));
    EXPECT_TRUE((tri_pos[74] == std::vector<int> {20, 10, 9}));
    EXPECT_TRUE((tri_pos[75] == std::vector<int> {20, 11, 9}));

    // Test: BP-CT-4
    tri_pos = soil_simulator::CalcTrianglePos(a, c, b, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 76);
    tri_pos = soil_simulator::CalcTrianglePos(b, c, a, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 76);
    tri_pos = soil_simulator::CalcTrianglePos(c, b, a, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 76);

    // Test: BP-CT-5
    a = {0.0 + 1e-5, 0.0 - 1e-5, 0.0 + 1e-5};
    b = {1.0 - 1e-5, 0.0 - 1e-5, 0.0 + 1e-5};
    c = {0.0 + 1e-5, 0.0 - 1e-5, 1.0 - 1e-5};
    tri_pos = soil_simulator::CalcTrianglePos(a, b, c, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 37);
    EXPECT_TRUE((tri_pos[0] == std::vector<int> {10, 10, 10}));
    EXPECT_TRUE((tri_pos[1] == std::vector<int> {10, 10, 11}));
    EXPECT_TRUE((tri_pos[2] == std::vector<int> {10, 10, 12}));
    EXPECT_TRUE((tri_pos[3] == std::vector<int> {10, 10, 13}));
    EXPECT_TRUE((tri_pos[4] == std::vector<int> {10, 10, 14}));
    EXPECT_TRUE((tri_pos[5] == std::vector<int> {10, 10, 15}));
    EXPECT_TRUE((tri_pos[6] == std::vector<int> {10, 10, 16}));
    EXPECT_TRUE((tri_pos[7] == std::vector<int> {10, 10, 17}));
    EXPECT_TRUE((tri_pos[8] == std::vector<int> {10, 10, 18}));
    EXPECT_TRUE((tri_pos[9] == std::vector<int> {10, 10, 19}));
    EXPECT_TRUE((tri_pos[10] == std::vector<int> {11, 10, 10}));
    EXPECT_TRUE((tri_pos[11] == std::vector<int> {11, 10, 18}));
    EXPECT_TRUE((tri_pos[12] == std::vector<int> {11, 10, 19}));
    EXPECT_TRUE((tri_pos[13] == std::vector<int> {12, 10, 10}));
    EXPECT_TRUE((tri_pos[14] == std::vector<int> {12, 10, 17}));
    EXPECT_TRUE((tri_pos[15] == std::vector<int> {12, 10, 18}));
    EXPECT_TRUE((tri_pos[16] == std::vector<int> {13, 10, 10}));
    EXPECT_TRUE((tri_pos[17] == std::vector<int> {13, 10, 16}));
    EXPECT_TRUE((tri_pos[18] == std::vector<int> {13, 10, 17}));
    EXPECT_TRUE((tri_pos[19] == std::vector<int> {14, 10, 10}));
    EXPECT_TRUE((tri_pos[20] == std::vector<int> {14, 10, 15}));
    EXPECT_TRUE((tri_pos[21] == std::vector<int> {14, 10, 16}));
    EXPECT_TRUE((tri_pos[22] == std::vector<int> {15, 10, 10}));
    EXPECT_TRUE((tri_pos[23] == std::vector<int> {15, 10, 14}));
    EXPECT_TRUE((tri_pos[24] == std::vector<int> {15, 10, 15}));
    EXPECT_TRUE((tri_pos[25] == std::vector<int> {16, 10, 10}));
    EXPECT_TRUE((tri_pos[26] == std::vector<int> {16, 10, 13}));
    EXPECT_TRUE((tri_pos[27] == std::vector<int> {16, 10, 14}));
    EXPECT_TRUE((tri_pos[28] == std::vector<int> {17, 10, 10}));
    EXPECT_TRUE((tri_pos[29] == std::vector<int> {17, 10, 12}));
    EXPECT_TRUE((tri_pos[30] == std::vector<int> {17, 10, 13}));
    EXPECT_TRUE((tri_pos[31] == std::vector<int> {18, 10, 10}));
    EXPECT_TRUE((tri_pos[32] == std::vector<int> {18, 10, 11}));
    EXPECT_TRUE((tri_pos[33] == std::vector<int> {18, 10, 12}));
    EXPECT_TRUE((tri_pos[34] == std::vector<int> {19, 10, 10}));
    EXPECT_TRUE((tri_pos[35] == std::vector<int> {19, 10, 11}));
    EXPECT_TRUE((tri_pos[36] == std::vector<int> {20, 10, 10}));
    tri_pos = soil_simulator::CalcTrianglePos(b, a, c, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 37);
    tri_pos = soil_simulator::CalcTrianglePos(c, a, b, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 37);
    tri_pos = soil_simulator::CalcTrianglePos(a, c, b, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 37);
    tri_pos = soil_simulator::CalcTrianglePos(b, c, a, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 37);
    tri_pos = soil_simulator::CalcTrianglePos(c, b, a, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 37);

    // Test: BP-CT-6
    a = {0.5 + 1e-5, 0.0 + 1e-5, 0.5 + 1e-5};
    b = {0.6 - 1e-5, 0.0 + 1e-5, 0.6 - 1e-5};
    c = {0.6 - 2e-5, 0.5 - 1e-5, 0.6 - 2e-5};
    tri_pos = soil_simulator::CalcTrianglePos(a, b, c, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 10);
    EXPECT_TRUE((tri_pos[0] == std::vector<int> {15, 10, 15}));
    EXPECT_TRUE((tri_pos[1] == std::vector<int> {15, 11, 15}));
    EXPECT_TRUE((tri_pos[2] == std::vector<int> {15, 12, 15}));
    EXPECT_TRUE((tri_pos[3] == std::vector<int> {15, 13, 15}));
    EXPECT_TRUE((tri_pos[4] == std::vector<int> {16, 10, 15}));
    EXPECT_TRUE((tri_pos[5] == std::vector<int> {16, 11, 15}));
    EXPECT_TRUE((tri_pos[6] == std::vector<int> {16, 12, 15}));
    EXPECT_TRUE((tri_pos[7] == std::vector<int> {16, 13, 15}));
    EXPECT_TRUE((tri_pos[8] == std::vector<int> {16, 14, 15}));
    EXPECT_TRUE((tri_pos[9] == std::vector<int> {16, 15, 15}));
    tri_pos = soil_simulator::CalcTrianglePos(b, a, c, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 10);
    tri_pos = soil_simulator::CalcTrianglePos(c, a, b, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 10);
    tri_pos = soil_simulator::CalcTrianglePos(a, c, b, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 10);
    tri_pos = soil_simulator::CalcTrianglePos(b, c, a, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 10);
    tri_pos = soil_simulator::CalcTrianglePos(c, b, a, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 10);

    // Test: BP-CT-7
    a = {0.34 + 1e-5, 0.56 + 1e-5, 0.0 - 1e-5};
    b = {0.74 - 1e-5, 0.97 - 1e-5, 0.0 - 1e-5};
    c = {0.74 - 1e-5, 0.97 - 1e-5, 0.0 - 1e-5};
    tri_pos = soil_simulator::CalcTrianglePos(a, b, c, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 9);
    EXPECT_TRUE((tri_pos[0] == std::vector<int> {13, 16, 9}));
    EXPECT_TRUE((tri_pos[1] == std::vector<int> {14, 16, 9}));
    EXPECT_TRUE((tri_pos[2] == std::vector<int> {14, 17, 9}));
    EXPECT_TRUE((tri_pos[3] == std::vector<int> {15, 17, 9}));
    EXPECT_TRUE((tri_pos[4] == std::vector<int> {15, 18, 9}));
    EXPECT_TRUE((tri_pos[5] == std::vector<int> {16, 18, 9}));
    EXPECT_TRUE((tri_pos[6] == std::vector<int> {16, 19, 9}));
    EXPECT_TRUE((tri_pos[7] == std::vector<int> {17, 19, 9}));
    EXPECT_TRUE((tri_pos[8] == std::vector<int> {17, 20, 9}));

    // Test: BP-CT-8
    a = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    b = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    c = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    tri_pos = soil_simulator::CalcTrianglePos(a, b, c, grid, tol);
    sort(tri_pos.begin(), tri_pos.end());
    tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
    EXPECT_EQ(tri_pos.size(), 1);
    EXPECT_TRUE((tri_pos[0] == std::vector<int> {15, 15, 14}));
}

TEST(UnitTestBucketPos, IncludeNewBodyPos) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    sim_out->body_[0][6][9] = 1.0;
    sim_out->body_[1][6][9] = 2.0;
    sim_out->body_[0][8][11] = 0.5;
    sim_out->body_[1][8][11] = 0.6;
    sim_out->body_[2][8][11] = 0.8;
    sim_out->body_[3][8][11] = 0.9;
    sim_out->body_[2][9][8] = 1.2;
    sim_out->body_[3][9][8] = 1.4;

    // Test: BP-INB-1
    soil_simulator::IncludeNewBodyPos(sim_out, 5, 5, 0.1, 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][5][5], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][5][5], 0.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][5][5], 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][5][5], 0.0, 1.e-5);

    // Test: BP-INB-2
    soil_simulator::IncludeNewBodyPos(sim_out, 6, 9, 0.1, 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][6][9], 1.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][6][9], 2.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][6][9], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][6][9], 0.2, 1.e-5);

    // Test: BP-INB-3
    soil_simulator::IncludeNewBodyPos(sim_out, 9, 8, 1.6, 1.7, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][9][8], 1.6, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][9][8], 1.7, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][9][8], 1.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][9][8], 1.4, 1.e-5);

    // Test: BP-INB-4
    soil_simulator::IncludeNewBodyPos(sim_out, 6, 9, 0.2, 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][6][9], 1.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][6][9], 2.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][6][9], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][6][9], 0.4, 1.e-5);

    // Test: BP-INB-5
    soil_simulator::IncludeNewBodyPos(sim_out, 6, 9, -0.2, 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][6][9], 1.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][6][9], 2.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][6][9], -0.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][6][9], 0.4, 1.e-5);

    // Test: BP-INB-6
    soil_simulator::IncludeNewBodyPos(sim_out, 6, 9, 2.0, 2.5, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][6][9], 1.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][6][9], 2.5, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][6][9], -0.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][6][9], 0.4, 1.e-5);

    // Test: BP-INB-7
    soil_simulator::IncludeNewBodyPos(sim_out, 6, 9, 0.7, 1.0, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][6][9], 0.7, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][6][9], 2.5, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][6][9], -0.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][6][9], 0.4, 1.e-5);

    // Test: BP-INB-8
    soil_simulator::IncludeNewBodyPos(sim_out, 6, 9, -0.4, 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][6][9], 0.7, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][6][9], 2.5, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][6][9], -0.4, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][6][9], 0.6, 1.e-5);

    // Test: BP-INB-9
    soil_simulator::IncludeNewBodyPos(sim_out, 8, 11, 0.6, 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][8][11], 0.5, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][8][11], 0.9, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][8][11], 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][8][11], 0.0, 1.e-5);

    // Test: BP-INB-10
    soil_simulator::IncludeNewBodyPos(sim_out, 6, 9, 0.9, 2.5, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][6][9], 0.7, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][6][9], 2.5, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][6][9], -0.4, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][6][9], 0.6, 1.e-5);

    // Test: BP-INB-11
    soil_simulator::IncludeNewBodyPos(sim_out, 6, 9, -0.4, 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][6][9], 0.7, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][6][9], 2.5, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][6][9], -0.4, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][6][9], 0.6, 1.e-5);

    // Test: BP-INB-12
    soil_simulator::IncludeNewBodyPos(sim_out, 6, 9, 3.0, 3.1, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][6][9], 0.7, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][6][9], 3.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][6][9], -0.4, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][6][9], 0.6, 1.e-5);

    delete sim_out;
}

TEST(UnitTestBucketPos, UpdateBody) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    // Declaring variables
    std::vector<std::vector<int>> area_pos;

    // Test: BP-UB-1
    area_pos = {
        {5, 5, 9}, {5, 5, 13}, {6, 6, 15}, {7, 11, 9}, {7, 11, 10}, {7, 12, 10},
        {7, 12, 11}, {7, 13, 9}, {10, 10, 9}};
    soil_simulator::UpdateBody(area_pos, sim_out, grid, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][5][5], -0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][5][5], 0.4, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][6][6], 0.5, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][6][6], 0.6, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][7][11], -0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][7][11], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][7][12], 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][7][12], 0.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][7][13], -0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][7][13], 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][10][10], -0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][10][10], 0.0, 1.e-5);

    // Test: BP-UB-2
    area_pos = {
        {4, 4, 9}, {5, 5, 13}, {6, 6, 8}, {7, 11, 10}, {7, 11, 13}, {7, 12, 7},
        {7, 12, 10}, {7, 13, 7}, {7, 13, 12}, {10, 10, 11}};
    soil_simulator::UpdateBody(area_pos, sim_out, grid, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][4][4], -0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][4][4], 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][5][5], -0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][5][5], 0.4, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][6][6], 0.5, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][6][6], 0.6, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][6][6], -0.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][6][6], -0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][7][11], -0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][7][11], 0.4, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][7][12], -0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][7][12], 0.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][7][13], -0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][7][13], 0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][10][10], -0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][10][10], 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][10][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][10][10], 0.2, 1.e-5);

    // Test: BP-UB-3
    area_pos = {{6, 6, 6}, {6, 6, 17}};
    soil_simulator::UpdateBody(area_pos, sim_out, grid, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][6][6], -0.4, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][6][6], 0.8, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][6][6], 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][6][6], 0.0, 1.e-5);

    // Test: BP-UB-4
    area_pos = {{{10, 10, 13}}};
    soil_simulator::UpdateBody(area_pos, sim_out, grid, 1e-5);
    EXPECT_NEAR(sim_out->body_[0][10][10], -0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][10][10], 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_[2][10][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_[3][10][10], 0.4, 1.e-5);

    delete sim_out;
}

TEST(UnitTestBucketPos, CalcBucketPos) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimParam sim_param(0.785, 4, 4);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    // Declaring variables
    std::vector<float> o_pos;
    std::vector<float> j_pos;
    std::vector<float> b_pos;
    std::vector<float> t_pos;
    std::vector<float> ori;
    std::vector<float> pos;
    std::vector<std::vector<int>> body_pos;

    // Test: BP-CB-1
    o_pos = {0.0, 0.0, 0.0};
    j_pos = {0.0, 0.0, 0.0};
    b_pos = {0.5, 0.01, 0.0};
    t_pos = {0.5, 0.0, 0.0};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    ori = {1.0, 0.0, 0.0, 0.0};
    pos = {0.0, 0.0, 0.0};
    soil_simulator::CalcBucketPos(
        sim_out, pos, ori, grid, bucket, sim_param, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][10][10], -0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][10][10], 0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][11][10], -0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][11][10], 0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][12][10], -0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][12][10], 0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][13][10], -0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][13][10], 0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][14][10], -0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][14][10], 0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][15][10], -0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[1][15][10], 0.3, 1.e-5);
    EXPECT_EQ(sim_out->bucket_area_[0][0], 6);
    EXPECT_EQ(sim_out->bucket_area_[0][1], 19);
    EXPECT_EQ(sim_out->bucket_area_[1][0], 6);
    EXPECT_EQ(sim_out->bucket_area_[1][1], 14);
    // Resetting values
    body_pos = {
        {0, 10, 10}, {0, 11, 10}, {0, 12, 10}, {0, 13, 10}, {0, 14, 10},
        {0, 15, 10}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, {});

    // Test: BP-CB-2
    b_pos = {0.5, 0.0, -0.01};
    t_pos = {0.5, 0.0, 0.0};
    *bucket = soil_simulator::Bucket(o_pos, j_pos, b_pos, t_pos, 0.5);
    soil_simulator::CalcBucketPos(
        sim_out, pos, ori, grid, bucket, sim_param, 1.e-5);
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++)
                if (((ii == 0) || (ii == 1)) &&
                    (jj < 16) && (jj > 9) && (kk < 13) && (kk > 7)) {
                    if (ii == 0)
                        EXPECT_NEAR(sim_out->body_[ii][jj][kk], -0.1, 1.e-5);
                    else
                        EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                } else {
                    EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                }
    EXPECT_EQ(sim_out->bucket_area_[0][0], 6);
    EXPECT_EQ(sim_out->bucket_area_[0][1], 19);
    EXPECT_EQ(sim_out->bucket_area_[1][0], 4);
    EXPECT_EQ(sim_out->bucket_area_[1][1], 16);
    // Resetting values
    body_pos = {
        {0, 10, 8}, {0, 10, 9}, {0, 10, 10}, {0, 10, 11}, {0, 10, 12},
        {0, 11, 8}, {0, 11, 9}, {0, 11, 10}, {0, 11, 11}, {0, 11, 12},
        {0, 12, 8}, {0, 12, 9}, {0, 12, 10}, {0, 12, 11}, {0, 12, 12},
        {0, 13, 8}, {0, 13, 9}, {0, 13, 10}, {0, 13, 11}, {0, 13, 12},
        {0, 14, 8}, {0, 14, 9}, {0, 14, 10}, {0, 14, 11}, {0, 14, 12},
        {0, 15, 8}, {0, 15, 9}, {0, 15, 10}, {0, 15, 11}, {0, 15, 12}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, {});

    // Test: BP-CB-3
    b_pos = {0.0, 0.0, -0.5};
    t_pos = {0.5, 0.0, -0.5};
    *bucket = soil_simulator::Bucket(o_pos, j_pos, b_pos, t_pos, 0.5);
    ori = {0.707107, 0.0, -0.707107, 0.0};  // -pi/2 rotation around the Y axis
    pos = {0.0, 0.0, -0.1};
    soil_simulator::CalcBucketPos(
        sim_out, pos, ori, grid, bucket, sim_param, 1.e-5);
    for (auto jj = 5; jj < 11; jj++)
        for (auto kk = 8; kk < 13; kk++)
            EXPECT_NEAR(sim_out->body_[1][jj][kk], -0.1, 1.e-5);
    for (auto kk = 8; kk < 13; kk++) {
        EXPECT_NEAR(sim_out->body_[0][5][kk], -0.6, 1.e-5);
        EXPECT_NEAR(sim_out->body_[0][10][kk], -0.2, 1.e-5);
    }
    for (auto jj = 6; jj < 10; jj++)
        for (auto kk = 9; kk < 12; kk++)
            EXPECT_NEAR(sim_out->body_[0][jj][kk], -0.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][6][8], -0.6, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][6][12], -0.6, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][7][8], -0.5, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][7][12], -0.5, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][8][8], -0.4, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][8][12], -0.4, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][9][8], -0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_[0][9][12], -0.3, 1.e-5);
    EXPECT_EQ(sim_out->bucket_area_[0][0], 1);
    EXPECT_EQ(sim_out->bucket_area_[0][1], 14);
    EXPECT_EQ(sim_out->bucket_area_[1][0], 4);
    EXPECT_EQ(sim_out->bucket_area_[1][1], 16);
    // Resetting values
    body_pos = {
        {0, 5, 8}, {0, 5, 9}, {0, 5, 10}, {0, 5, 11}, {0, 5, 12},
        {0, 6, 8}, {0, 6, 9}, {0, 6, 10}, {0, 6, 11}, {0, 6, 12},
        {0, 7, 8}, {0, 7, 9}, {0, 7, 10}, {0, 7, 11}, {0, 7, 12},
        {0, 8, 8}, {0, 8, 9}, {0, 8, 10}, {0, 8, 11}, {0, 8, 12},
        {0, 9, 8}, {0, 9, 9}, {0, 9, 10}, {0, 9, 11}, {0, 9, 12},
        {0, 10, 8}, {0, 10, 9}, {0, 10, 10}, {0, 10, 11}, {0, 10, 12}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, {});

    delete bucket;
    delete sim_out;
}
