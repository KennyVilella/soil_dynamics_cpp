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

TEST(UnitTestBucketPos, DecomposeVectorRectangle) {
    // Note that the function does not account for the case where the rectangle
    // follows a cell border. It is therefore necessary to solve this potential
    // ambiguity before calling the function. As a result, a small
    // increment (1e-12) is sometimes added or removed to the input in order to
    // make sure that the input coordinates do not correspond to a cell border.
    std::vector<float> ab_ind;
    std::vector<float> ad_ind;
    std::vector<float> a_ind;
    int area_min_x;
    int area_min_y;
    int area_length_x;
    int area_length_y;
    float tol = 1.e-5;

    // -- Testing for a simple rectangle in the XY plane --
    a_ind = {10.0, 10.0, 10.0};
    ab_ind = {5.0, 0.0, 0.0};
    ad_ind = {0.0, 5.0, 0.0};
    area_min_x = 8;
    area_min_y = 8;
    area_length_x = 8;
    area_length_y = 8;
    auto [c_ab, c_ad, in_rec, nn] = soil_simulator::DecomposeVectorRectangle(
        ab_ind, ad_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    // Checking the number of cells inside the rectangle area
    EXPECT_EQ(nn, 25 * 4);
    // Checking cells inside the rectangle area
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            if ((ii > 1.5) && (ii < 6.5) && (jj > 1.5) && (jj < 6.5))
                EXPECT_EQ(in_rec[ii][jj], true);
            else
                EXPECT_EQ(in_rec[ii][jj], false);
        }
    // Checking decomposition in terms of AB component
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
    // Checking decomposition in terms of AD component
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

    // -- Testing for not rounded indices --
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
    // Checking the number of cells inside the rectangle area
    EXPECT_EQ(nn, 25 * 4);
    // Checking cells inside the rectangle area
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            if ((ii > 1.5) && (ii < 6.5) && (jj > 1.5) && (jj < 6.5))
                EXPECT_EQ(in_rec[ii][jj], true);
            else
                EXPECT_EQ(in_rec[ii][jj], false);
        }

    // -- Testing for a simple rectangle in the XY plane at cell border --
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
    // Checking the number of cells inside the rectangle area
    EXPECT_EQ(nn, 10 * 4);
    // Checking cells inside the rectangle area
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            if ((ii > 1.5) && (ii < 6.5) && (jj > 1.5) && (jj < 3.5))
                EXPECT_EQ(in_rec[ii][jj], true);
            else
                EXPECT_EQ(in_rec[ii][jj], false);
        }
    // Checking decomposition in terms of AB component
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
    // Checking decomposition in terms of AD component
    for (auto ii = 2; ii < 7; ii++)
        EXPECT_NEAR(c_ad[ii][2], 1.0 / 3.0, 1e-5);
    for (auto ii = 2; ii < 7; ii++)
        EXPECT_NEAR(c_ad[ii][3], 2.0 / 3.0, 1e-5);
    for (auto ii = 2; ii < 7; ii++)
        EXPECT_NEAR(c_ad[ii][4], 1.0, 1e-5);

    // -- Testing for a simple rectangle in the XYZ plane --
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
    // Checking the number of cells inside the rectangle area
    EXPECT_EQ(nn, 5 * 4);
    // Checking cells inside the rectangle area
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            if ((ii == 2) && (jj > 1.5) && (jj < 6.5))
                EXPECT_EQ(in_rec[ii][jj], true);
            else
                EXPECT_EQ(in_rec[ii][jj], false);
        }
    // Checking decomposition in terms of AB component
    for (auto jj = 2; jj < 8; jj++)
        EXPECT_NEAR(c_ab[2][jj], 0.5, 1e-5);
    for (auto jj = 2; jj < 8; jj++)
        EXPECT_NEAR(c_ab[3][jj], 1.5, 1e-5);
    // Checking decomposition in terms of AD component
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

    // -- Testing for the edge case where the rectangle is a line --
    // Note that no decomposition can be mathematically made
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
    // Checking the number of cells inside the rectangle area
    EXPECT_EQ(nn, 0);
    // Checking cells inside the rectangle area
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++)
            EXPECT_EQ(in_rec[ii][jj], false);

    // -- Testing for the edge case where the rectangle is a point --
    // Note that no decomposition can be mathematically made
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
    // Checking the number of cells inside the rectangle area
    EXPECT_EQ(nn, 0);
    // Checking cells inside the rectangle area
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++)
            EXPECT_EQ(in_rec[ii][jj], false);
}

TEST(UnitTestBucketPos, DecomposeVectorTriangle) {
    // Note that the function does not account for the case where
    // the triangle follows a cell border.
    std::vector<float> ab_ind;
    std::vector<float> ac_ind;
    std::vector<float> a_ind;
    int area_min_x;
    int area_min_y;
    int area_length_x;
    int area_length_y;
    float tol = 1.e-5;

    // -- Testing for a simple triangle in the XY plane --
    a_ind = {10.0, 10.0, 10.0};
    ab_ind = {10.0, 0.0, 0.0};
    ac_ind = {0.0, 10.0, 0.0};
    area_min_x = 8;
    area_min_y = 8;
    area_length_x = 15;
    area_length_y = 15;
    auto [c_ab, c_ac, in_tri, nn] = soil_simulator::DecomposeVectorTriangle(
        ab_ind, ac_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    // Checking the number of cells inside the triangle area
    EXPECT_EQ(nn, 45 * 4);
    // Checking cells inside the triangle area
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
    // Checking decomposition in terms of AB component
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
    // Checking decomposition in terms of AC component
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

    // -- Testing for not rounded indices --
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
    // Checking the number of cells inside the triangle area
    EXPECT_EQ(nn, 45 * 4);
    // Checking cells inside the triangle area
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

    // -- Testing for a simple triangle in the XYZ plane --
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
    // Checking the number of cells inside the triangle area
    EXPECT_EQ(nn, 2 * 4);
    // Checking cells inside the triangle area
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            if ((ii == 2) && ((jj == 2) || (jj == 3)))
                EXPECT_EQ(in_tri[ii][jj], true);
            else
                EXPECT_EQ(in_tri[ii][jj], false);
        }
    // Checking decomposition in terms of AB component
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
    // Checking decomposition in terms of AC component
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

    // -- Testing for the edge case where the triangle is a line --
    // Note that no decomposition can be mathematically made
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
    // Checking the number of cells inside the triangle area
    EXPECT_EQ(nn, 0);
    // Checking cells inside the triangle area
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++)
            EXPECT_EQ(in_tri[ii][jj], false);

    // -- Testing for the edge case where the triangle is a point --
    // Note that no decomposition can be mathematically made
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
    // Checking the number of cells inside the triangle area
    EXPECT_EQ(nn, 0);
    // Checking cells inside the triangle area
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) 
            EXPECT_EQ(in_tri[ii][jj], false);
}
