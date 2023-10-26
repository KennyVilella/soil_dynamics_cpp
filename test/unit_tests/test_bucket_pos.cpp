/*
This file implements unit tests for the functions in bucket_pos.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <cmath>
#include "gtest/gtest.h"
#include "soil_simulator/bucket_pos.hpp"
#include "test/unit_tests/utility.hpp"

// To make the function call holds in a single line.
// It greatly improves readability.
using test_soil_simulator::SetHeight;

TEST(UnitTestBucketPos, CalcLinePos) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);

    // Declaring variables
    std::vector<float> a;
    std::vector<float> b;
    std::vector<std::vector<int>> line_pos;
    std::vector<std::vector<int>> line_pos_exp;

    // Test: BP-CL-1
    a = {0.0 + 1e-5, 0.0 - 1e-5, -0.06 + 1e-5};
    b = {1.0 - 1e-5, 0.0 - 1e-5,  0.0  - 1e-5};
    line_pos = soil_simulator::CalcLinePos(
        a, b, grid);
    line_pos_exp = {
        {10, 10, 9}, {11, 10, 9}, {12, 10, 9}, {13, 10, 9}, {14, 10, 9},
        {15, 10, 9}, {16, 10, 9}, {17, 10, 9}, {18, 10, 9}, {19, 10, 9},
        {20, 10, 9}};
    EXPECT_EQ(line_pos.size(), 11);
    EXPECT_EQ(line_pos, line_pos_exp);

    // Test: BP-CL-2
    a = {0.04 + 1e-5,  0.04 - 1e-5, -0.09 + 1e-5};
    b = {1.04 - 1e-5, -0.04 + 1e-5,   0.0 - 1e-5};
    line_pos = soil_simulator::CalcLinePos(a, b, grid);
    line_pos_exp = {
        {10, 10, 9}, {11, 10, 9}, {12, 10, 9}, {13, 10, 9}, {14, 10, 9},
        {15, 10, 9}, {16, 10, 9}, {17, 10, 9}, {18, 10, 9}, {19, 10, 9},
        {20, 10, 9}};
    EXPECT_EQ(line_pos.size(), 11);
    EXPECT_EQ(line_pos, line_pos_exp);

    // Test: BP-CL-3
    a = {0.0 - 1e-5, 0.0 + 1e-5, 0.0 - 1e-5};
    b = {0.0 - 1e-5, 1.0 - 1e-5, 0.0 - 1e-5};
    line_pos = soil_simulator::CalcLinePos(a, b, grid);
    line_pos_exp = {
        {10, 10, 9}, {10, 11, 9}, {10, 12, 9}, {10, 13, 9}, {10, 14, 9},
        {10, 15, 9}, {10, 16, 9}, {10, 17, 9}, {10, 18, 9}, {10, 19, 9},
        {10, 20, 9}};
    EXPECT_EQ(line_pos.size(), 11);
    EXPECT_EQ(line_pos, line_pos_exp);

    // Test: BP-CL-4
    a = {0.34 + 1e-5, 0.56 + 1e-5, 0.0 - 1e-5};
    b = {0.74 - 1e-5, 0.97 - 1e-5, 0.0 - 1e-5};
    line_pos = soil_simulator::CalcLinePos(a, b, grid);
    line_pos.erase(unique(line_pos.begin(), line_pos.end()), line_pos.end());
    line_pos_exp = {
        {13, 16, 9}, {14, 16, 9}, {14, 17, 9}, {15, 17, 9}, {15, 18, 9},
        {16, 18, 9}, {16, 19, 9}, {17, 19, 9}, {17, 20, 9}};
    EXPECT_EQ(line_pos.size(), 9);
    EXPECT_EQ(line_pos, line_pos_exp);

    // Test: BP-CL-5
    a = {0.34 + 1e-8, 0.0 - 1e-8, 0.56 + 1e-8};
    b = {0.74 - 1e-8, 0.0 - 1e-8, 0.97 - 1e-8};
    line_pos = soil_simulator::CalcLinePos(a, b, grid);
    line_pos.erase(unique(line_pos.begin(), line_pos.end()), line_pos.end());
    line_pos_exp = {
        {13, 10, 15}, {14, 10, 15}, {14, 10, 16}, {15, 10, 16}, {15, 10, 17},
        {16, 10, 17}, {16, 10, 18}, {17, 10, 18}, {17, 10, 19}};
    EXPECT_EQ(line_pos.size(), 9);
    EXPECT_EQ(line_pos, line_pos_exp);

    // Test: BP-CL-6
    a = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    b = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    line_pos = soil_simulator::CalcLinePos(a, b, grid);
    line_pos.erase(unique(line_pos.begin(), line_pos.end()), line_pos.end());
    line_pos_exp = {{15, 15, 14}};
    EXPECT_EQ(line_pos.size(), 1);
    EXPECT_EQ(line_pos, line_pos_exp);
    a = {0.55 - 1e-5, 0.55 - 1e-5, 0.55 - 1e-5};
    b = {0.55 - 1e-5, 0.55 - 1e-5, 0.55 - 1e-5};
    line_pos = soil_simulator::CalcLinePos(a, b, grid);
    line_pos.erase(unique(line_pos.begin(), line_pos.end()), line_pos.end());
    line_pos_exp = {{15, 15, 15}};
    EXPECT_EQ(line_pos.size(), 1);
    EXPECT_EQ(line_pos, line_pos_exp);
}

TEST(UnitTestBucketPos, DecomposeVectorRectangle) {
    // Setting up the environment
    float tol = 1.e-5;

    // Declaring variables
    std::vector<float> ab_ind;
    std::vector<float> ad_ind;
    std::vector<float> a_ind;
    int area_min_x;
    int area_min_y;
    int area_length_x;
    int area_length_y;
    std::vector<std::vector<float>> c_ab;
    std::vector<std::vector<float>> c_ad;
    std::vector<std::vector<bool>> in_rec;
    std::vector<std::vector<float>> c_ab_exp;
    std::vector<std::vector<float>> c_ad_exp;
    std::vector<std::vector<bool>> in_rec_exp;
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
    in_rec_exp = {
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, true,  true,  true,  true,  true,  false},
        {false, false, true,  true,  true,  true,  true,  false},
        {false, false, true,  true,  true,  true,  true,  false},
        {false, false, true,  true,  true,  true,  true,  false},
        {false, false, true,  true,  true,  true,  true,  false},
        {false, false, false, false, false, false, false, false}};
    c_ab_exp = {
        {-0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3},
        {-0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1},
        { 0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1},
        { 0.3,  0.3,  0.3,  0.3,  0.3,  0.3,  0.3,  0.3},
        { 0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5},
        { 0.7,  0.7,  0.7,  0.7,  0.7,  0.7,  0.7,  0.7},
        { 0.9,  0.9,  0.9,  0.9,  0.9,  0.9,  0.9,  0.9},
        { 1.1,  1.1,  1.1,  1.1,  1.1,  1.1,  1.1,  1.1}};
    c_ad_exp = {
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1}};
    EXPECT_EQ(nn, 25 * 4);
    EXPECT_EQ(in_rec, in_rec_exp);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            EXPECT_NEAR(c_ab[ii][jj], c_ab_exp[ii][jj], 1e-5);
            EXPECT_NEAR(c_ad[ii][jj], c_ad_exp[ii][jj], 1e-5);
        }

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
    in_rec_exp = {
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, true,  true,  true,  true,  true,  false},
        {false, false, true,  true,  true,  true,  true,  false},
        {false, false, true,  true,  true,  true,  true,  false},
        {false, false, true,  true,  true,  true,  true,  false},
        {false, false, true,  true,  true,  true,  true,  false},
        {false, false, false, false, false, false, false, false}};
    c_ab_exp = {
        {-1.2, -1.2, -1.2, -1.2, -1.2, -1.2, -1.2, -1.2},
        {-0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2},
        { 0.8,  0.8,  0.8,  0.8,  0.8,  0.8,  0.8,  0.8},
        { 1.8,  1.8,  1.8,  1.8,  1.8,  1.8,  1.8,  1.8},
        { 2.8,  2.8,  2.8,  2.8,  2.8,  2.8,  2.8,  2.8},
        { 3.8,  3.8,  3.8,  3.8,  3.8,  3.8,  3.8,  3.8},
        { 4.8,  4.8,  4.8,  4.8,  4.8,  4.8,  4.8,  4.8},
        { 5.8,  5.8,  5.8,  5.8,  5.8,  5.8,  5.8,  5.8}};
    c_ad_exp = {
        {-1.8, -0.8, 0.2, 1.2, 2.2, 3.2, 4.2, 5.2},
        {-1.8, -0.8, 0.2, 1.2, 2.2, 3.2, 4.2, 5.2},
        {-1.8, -0.8, 0.2, 1.2, 2.2, 3.2, 4.2, 5.2},
        {-1.8, -0.8, 0.2, 1.2, 2.2, 3.2, 4.2, 5.2},
        {-1.8, -0.8, 0.2, 1.2, 2.2, 3.2, 4.2, 5.2},
        {-1.8, -0.8, 0.2, 1.2, 2.2, 3.2, 4.2, 5.2},
        {-1.8, -0.8, 0.2, 1.2, 2.2, 3.2, 4.2, 5.2},
        {-1.8, -0.8, 0.2, 1.2, 2.2, 3.2, 4.2, 5.2}};
    EXPECT_EQ(nn, 25 * 4);
    EXPECT_EQ(in_rec, in_rec_exp);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            EXPECT_NEAR(c_ab[ii][jj], c_ab_exp[ii][jj] / 5.7, 1e-5);
            EXPECT_NEAR(c_ad[ii][jj], c_ad_exp[ii][jj] / 4.7, 1e-5);
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
    in_rec_exp = {
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, true,  true,  false, false, false, false},
        {false, false, true,  true,  false, false, false, false},
        {false, false, true,  true,  false, false, false, false},
        {false, false, true,  true,  false, false, false, false},
        {false, false, true,  true,  false, false, false, false},
        {false, false, false, false, false, false, false, false}};
    c_ab_exp = {
        {-0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3},
        {-0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1},
        { 0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1,  0.1},
        { 0.3,  0.3,  0.3,  0.3,  0.3,  0.3,  0.3,  0.3},
        { 0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5},
        { 0.7,  0.7,  0.7,  0.7,  0.7,  0.7,  0.7,  0.7},
        { 0.9,  0.9,  0.9,  0.9,  0.9,  0.9,  0.9,  0.9},
        { 1.1,  1.1,  1.1,  1.1,  1.1,  1.1,  1.1,  1.1}};
    c_ad_exp = {
        {-1.0 / 3, 0.0, 1.0 / 3, 2.0 / 3, 1.0, 4.0 / 3, 5.0 / 3, 2.0},
        {-1.0 / 3, 0.0, 1.0 / 3, 2.0 / 3, 1.0, 4.0 / 3, 5.0 / 3, 2.0},
        {-1.0 / 3, 0.0, 1.0 / 3, 2.0 / 3, 1.0, 4.0 / 3, 5.0 / 3, 2.0},
        {-1.0 / 3, 0.0, 1.0 / 3, 2.0 / 3, 1.0, 4.0 / 3, 5.0 / 3, 2.0},
        {-1.0 / 3, 0.0, 1.0 / 3, 2.0 / 3, 1.0, 4.0 / 3, 5.0 / 3, 2.0},
        {-1.0 / 3, 0.0, 1.0 / 3, 2.0 / 3, 1.0, 4.0 / 3, 5.0 / 3, 2.0},
        {-1.0 / 3, 0.0, 1.0 / 3, 2.0 / 3, 1.0, 4.0 / 3, 5.0 / 3, 2.0},
        {-1.0 / 3, 0.0, 1.0 / 3, 2.0 / 3, 1.0, 4.0 / 3, 5.0 / 3, 2.0}};
    EXPECT_EQ(nn, 10 * 4);
    EXPECT_EQ(in_rec, in_rec_exp);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            EXPECT_NEAR(c_ab[ii][jj], c_ab_exp[ii][jj], 1e-5);
            EXPECT_NEAR(c_ad[ii][jj], c_ad_exp[ii][jj], 1e-5);
        }

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
    in_rec_exp = {
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, true,  true,  true,  true,  true,  false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false}};
    c_ab_exp = {
        {-1.5, -1.5, -1.5, -1.5, -1.5, -1.5, -1.5, -1.5},
        {-0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5},
        { 0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5,  0.5},
        { 1.5,  1.5,  1.5,  1.5,  1.5,  1.5,  1.5,  1.5},
        { 2.5,  2.5,  2.5,  2.5,  2.5,  2.5,  2.5,  2.5},
        { 3.5,  3.5,  3.5,  3.5,  3.5,  3.5,  3.5,  3.5},
        { 4.5,  4.5,  4.5,  4.5,  4.5,  4.5,  4.5,  4.5},
        { 5.5,  5.5,  5.5,  5.5,  5.5,  5.5,  5.5,  5.5}};
    c_ad_exp = {
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1}};
    EXPECT_EQ(nn, 5 * 4);
    EXPECT_EQ(in_rec, in_rec_exp);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            EXPECT_NEAR(c_ab[ii][jj], c_ab_exp[ii][jj], 1e-5);
            EXPECT_NEAR(c_ad[ii][jj], c_ad_exp[ii][jj], 1e-5);
        }

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
    // Setting up the environment
    float tol = 1.e-5;

    // Declaring variables
    std::vector<float> ab_ind;
    std::vector<float> ac_ind;
    std::vector<float> a_ind;
    int area_min_x;
    int area_min_y;
    int area_length_x;
    int area_length_y;
    std::vector<std::vector<float>> c_ab;
    std::vector<std::vector<float>> c_ac;
    std::vector<std::vector<bool>> in_tri;
    std::vector<std::vector<float>> c_ab_exp;
    std::vector<std::vector<float>> c_ac_exp;
    std::vector<std::vector<bool>> in_tri_exp;
    int nn;

    // Test: BP-DVT-1
    a_ind = {10.0, 10.0, 10.0};
    ab_ind = {10.0, 0.0, 0.0};
    ac_ind = {0.0, 10.0, 0.0};
    area_min_x = 9;
    area_min_y = 9;
    area_length_x = 11;
    area_length_y = 10;
    std::tie(c_ab, c_ac, in_tri, nn) = soil_simulator::DecomposeVectorTriangle(
        ab_ind, ac_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    in_tri_exp = {
        {false, false, false, false, false, false, false, false, false, false},
        {false, true,  true,  true,  true,  true,  true,  true,  true,  true},
        {false, true,  true,  true,  true,  true,  true,  true,  true,  false},
        {false, true,  true,  true,  true,  true,  true,  true,  false, false},
        {false, true,  true,  true,  true,  true,  true,  false, false, false},
        {false, true,  true,  true,  true,  true,  false, false, false, false},
        {false, true,  true,  true,  true,  false, false, false, false, false},
        {false, true,  true,  true,  false, false, false, false, false, false},
        {false, true,  true,  false, false, false, false, false, false, false},
        {false, true,  false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false}};
    c_ab_exp = {
        {-0.05, -0.05, -0.05, -0.05, -0.05, -0.05, -0.05, -0.05, -0.05, -0.05},
        { 0.05,  0.05,  0.05,  0.05,  0.05,  0.05,  0.05,  0.05,  0.05,  0.05},
        { 0.15,  0.15,  0.15,  0.15,  0.15,  0.15,  0.15,  0.15,  0.15,  0.15},
        { 0.25,  0.25,  0.25,  0.25,  0.25,  0.25,  0.25,  0.25,  0.25,  0.25},
        { 0.35,  0.35,  0.35,  0.35,  0.35,  0.35,  0.35,  0.35,  0.35,  0.35},
        { 0.45,  0.45,  0.45,  0.45,  0.45,  0.45,  0.45,  0.45,  0.45,  0.45},
        { 0.55,  0.55,  0.55,  0.55,  0.55,  0.55,  0.55,  0.55,  0.55,  0.55},
        { 0.65,  0.65,  0.65,  0.65,  0.65,  0.65,  0.65,  0.65,  0.65,  0.65},
        { 0.75,  0.75,  0.75,  0.75,  0.75,  0.75,  0.75,  0.75,  0.75,  0.75},
        { 0.85,  0.85,  0.85,  0.85,  0.85,  0.85,  0.85,  0.85,  0.85,  0.85},
        { 0.95,  0.95,  0.95,  0.95,  0.95,  0.95,  0.95,  0.95,  0.95,  0.95},
        { 1.05,  1.05,  1.05,  1.05,  1.05,  1.05,  1.05,  1.05,  1.05,  1.05}};
    c_ac_exp = {
        {-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95},
        {-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95},
        {-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95},
        {-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95},
        {-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95},
        {-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95},
        {-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95},
        {-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95},
        {-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95},
        {-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95},
        {-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95},
        {-0.05, 0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95}};
    EXPECT_EQ(nn, 45 * 4);
    EXPECT_EQ(in_tri, in_tri_exp);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            EXPECT_NEAR(c_ab[ii][jj], c_ab_exp[ii][jj], 1e-5);
            EXPECT_NEAR(c_ac[ii][jj], c_ac_exp[ii][jj], 1e-5);
        }

    // Test: BP-DVT-2
    a_ind = {9.9, 9.7, 10.0};
    ab_ind = {9.7, 0.0, 0.0};
    ac_ind = {0.0, 10.4, 0.0};
    area_min_x = 9;
    area_min_y = 9;
    area_length_x = 11;
    area_length_y = 10;
    std::tie(c_ab, c_ac, in_tri, nn) = soil_simulator::DecomposeVectorTriangle(
        ab_ind, ac_ind, a_ind, area_min_x, area_min_y, area_length_x,
        area_length_y, tol);
    in_tri_exp = {
        {false, false, false, false, false, false, false, false, false, false},
        {false, true,  true,  true,  true,  true,  true,  true,  true,  true},
        {false, true,  true,  true,  true,  true,  true,  true,  true,  false},
        {false, true,  true,  true,  true,  true,  true,  true,  false, false},
        {false, true,  true,  true,  true,  true,  true,  false, false, false},
        {false, true,  true,  true,  true,  true,  false, false, false, false},
        {false, true,  true,  true,  true,  false, false, false, false, false},
        {false, true,  true,  true,  false, false, false, false, false, false},
        {false, true,  true,  false, false, false, false, false, false, false},
        {false, true,  false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false}};
    c_ab_exp = {
        {-0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4},
        {0.6,  0.6,  0.6,  0.6,  0.6,  0.6,  0.6,  0.6,  0.6,  0.6},
        {1.6,  1.6,  1.6,  1.6,  1.6,  1.6,  1.6,  1.6,  1.6,  1.6},
        {2.6,  2.6,  2.6,  2.6,  2.6,  2.6,  2.6,  2.6,  2.6,  2.6},
        {3.6,  3.6,  3.6,  3.6,  3.6,  3.6,  3.6,  3.6,  3.6,  3.6},
        {4.6,  4.6,  4.6,  4.6,  4.6,  4.6,  4.6,  4.6,  4.6,  4.6},
        {5.6,  5.6,  5.6,  5.6,  5.6,  5.6,  5.6,  5.6,  5.6,  5.6},
        {6.6,  6.6,  6.6,  6.6,  6.6,  6.6,  6.6,  6.6,  6.6,  6.6},
        {7.6,  7.6,  7.6,  7.6,  7.6,  7.6,  7.6,  7.6,  7.6,  7.6},
        {8.6,  8.6,  8.6,  8.6,  8.6,  8.6,  8.6,  8.6,  8.6,  8.6},
        {9.6,  9.6,  9.6,  9.6,  9.6,  9.6,  9.6,  9.6,  9.6,  9.6}};
    c_ac_exp = {
        {-0.2, 0.8, 1.8, 2.8, 3.8, 4.8, 5.8, 6.8, 7.8, 8.8},
        {-0.2, 0.8, 1.8, 2.8, 3.8, 4.8, 5.8, 6.8, 7.8, 8.8},
        {-0.2, 0.8, 1.8, 2.8, 3.8, 4.8, 5.8, 6.8, 7.8, 8.8},
        {-0.2, 0.8, 1.8, 2.8, 3.8, 4.8, 5.8, 6.8, 7.8, 8.8},
        {-0.2, 0.8, 1.8, 2.8, 3.8, 4.8, 5.8, 6.8, 7.8, 8.8},
        {-0.2, 0.8, 1.8, 2.8, 3.8, 4.8, 5.8, 6.8, 7.8, 8.8},
        {-0.2, 0.8, 1.8, 2.8, 3.8, 4.8, 5.8, 6.8, 7.8, 8.8},
        {-0.2, 0.8, 1.8, 2.8, 3.8, 4.8, 5.8, 6.8, 7.8, 8.8},
        {-0.2, 0.8, 1.8, 2.8, 3.8, 4.8, 5.8, 6.8, 7.8, 8.8},
        {-0.2, 0.8, 1.8, 2.8, 3.8, 4.8, 5.8, 6.8, 7.8, 8.8},
        {-0.2, 0.8, 1.8, 2.8, 3.8, 4.8, 5.8, 6.8, 7.8, 8.8},
        {-0.2, 0.8, 1.8, 2.8, 3.8, 4.8, 5.8, 6.8, 7.8, 8.8}};
    EXPECT_EQ(nn, 45 * 4);
    EXPECT_EQ(in_tri, in_tri_exp);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            EXPECT_NEAR(c_ab[ii][jj], c_ab_exp[ii][jj] / 9.7, 1e-5);
            EXPECT_NEAR(c_ac[ii][jj], c_ac_exp[ii][jj] / 10.4, 1e-5);
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
    in_tri_exp = {
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, true,  true,  false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false}};
    c_ab_exp = {
        {-1.2, -1.4, -1.6, -1.8, -2.0, -2.2, -2.4, -2.6},
        {-0.2, -0.4, -0.6, -0.8, -1.0, -1.2, -1.4, -1.6},
        { 0.8,  0.6,  0.4,  0.2,  0.0, -0.2, -0.4, -0.6},
        { 1.8,  1.6,  1.4,  1.2,  1.0,  0.8,  0.6,  0.4},
        { 2.8,  2.6,  2.4,  2.2,  2.0,  1.8,  1.6,  1.4},
        { 3.8,  3.6,  3.4,  3.2,  3.0,  2.8,  2.6,  2.4},
        { 4.8,  4.6,  4.4,  4.2,  4.0,  3.8,  3.6,  3.4},
        { 5.8,  5.6,  5.4,  5.2,  5.0,  4.8,  4.6,  4.4}};
    c_ac_exp = {
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1},
        {-0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1}};
    EXPECT_EQ(nn, 2 * 4);
    EXPECT_EQ(in_tri, in_tri_exp);
    for (auto ii = 0; ii < area_length_x; ii++)
        for (auto jj = 0; jj < area_length_y; jj++) {
            EXPECT_NEAR(c_ab[ii][jj], c_ab_exp[ii][jj], 1e-5);
            EXPECT_NEAR(c_ac[ii][jj], c_ac_exp[ii][jj], 1e-5);
        }

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
    float tol = 1e-5;

    // Declaring variables
    std::vector<float> a;
    std::vector<float> b;
    std::vector<float> c;
    std::vector<float> d;
    std::vector<std::vector<int>> rect_pos;
    std::vector<std::vector<int>> rect_pos_exp;

    // Creating a lambda function to check the results for any input orders
    // The results are sorted and duplicates are removed
    auto CheckResults = [&]() {
        // Checking first input order
        rect_pos = soil_simulator::CalcRectanglePos(a, b, c, d, grid, tol);
        sort(rect_pos.begin(), rect_pos.end());
        rect_pos.erase(
            unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
        EXPECT_EQ(rect_pos.size(), rect_pos_exp.size());
        EXPECT_EQ(rect_pos, rect_pos_exp);
        // Checking second input order
        rect_pos = soil_simulator::CalcRectanglePos(a, d, c, b, grid, tol);
        sort(rect_pos.begin(), rect_pos.end());
        rect_pos.erase(
            unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
        EXPECT_EQ(rect_pos.size(), rect_pos_exp.size());
        EXPECT_EQ(rect_pos, rect_pos_exp);
        // Checking third input order
        rect_pos = soil_simulator::CalcRectanglePos(c, b, a, d, grid, tol);
        sort(rect_pos.begin(), rect_pos.end());
        rect_pos.erase(
            unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
        EXPECT_EQ(rect_pos.size(), rect_pos_exp.size());
        EXPECT_EQ(rect_pos, rect_pos_exp);
        // Checking fourth input order
        rect_pos = soil_simulator::CalcRectanglePos(b, c, d, a, grid, tol);
        sort(rect_pos.begin(), rect_pos.end());
        rect_pos.erase(
            unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
        EXPECT_EQ(rect_pos.size(), rect_pos_exp.size());
        EXPECT_EQ(rect_pos, rect_pos_exp);
        // Checking fifth input order
        rect_pos = soil_simulator::CalcRectanglePos(c, d, a, b, grid, tol);
        sort(rect_pos.begin(), rect_pos.end());
        rect_pos.erase(
            unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
        EXPECT_EQ(rect_pos.size(), rect_pos_exp.size());
        EXPECT_EQ(rect_pos, rect_pos_exp);
        // Checking sixth input order
        rect_pos = soil_simulator::CalcRectanglePos(d, a, b, c, grid, tol);
        sort(rect_pos.begin(), rect_pos.end());
        rect_pos.erase(
            unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
        EXPECT_EQ(rect_pos.size(), rect_pos_exp.size());
        EXPECT_EQ(rect_pos, rect_pos_exp);
        // Checking seventh input order
        rect_pos = soil_simulator::CalcRectanglePos(d, c, b, a, grid, tol);
        sort(rect_pos.begin(), rect_pos.end());
        rect_pos.erase(
            unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
        EXPECT_EQ(rect_pos.size(), rect_pos_exp.size());
        EXPECT_EQ(rect_pos, rect_pos_exp);
        // Checking eigth input order
        rect_pos = soil_simulator::CalcRectanglePos(b, a, d, c, grid, tol);
        sort(rect_pos.begin(), rect_pos.end());
        rect_pos.erase(
            unique(rect_pos.begin(), rect_pos.end()), rect_pos.end());
        EXPECT_EQ(rect_pos.size(), rect_pos_exp.size());
        EXPECT_EQ(rect_pos, rect_pos_exp);
    };

    // Test: BP-CR-1
    a = {0.0 + 1e-5, 0.0 + 1e-5, 0.0 - 1e-5};
    b = {0.5 - 1e-5, 0.0 + 1e-5, 0.0 - 1e-5};
    c = {0.5 - 1e-5, 0.5 - 1e-5, 0.0 - 1e-5};
    d = {0.0 + 1e-5, 0.5 - 1e-5, 0.0 - 1e-5};
    rect_pos_exp = {
        {10, 10, 9}, {10, 11, 9}, {10, 12, 9}, {10, 13, 9}, {10, 14, 9},
        {10, 15, 9}, {11, 10, 9}, {11, 11, 9}, {11, 12, 9}, {11, 13, 9},
        {11, 14, 9}, {11, 15, 9}, {12, 10, 9}, {12, 11, 9}, {12, 12, 9},
        {12, 13, 9}, {12, 14, 9}, {12, 15, 9}, {13, 10, 9}, {13, 11, 9},
        {13, 12, 9}, {13, 13, 9}, {13, 14, 9}, {13, 15, 9}, {14, 10, 9},
        {14, 11, 9}, {14, 12, 9}, {14, 13, 9}, {14, 14, 9}, {14, 15, 9},
        {15, 10, 9}, {15, 11, 9}, {15, 12, 9}, {15, 13, 9}, {15, 14, 9},
        {15, 15, 9}};
    CheckResults();

    // Test: BP-CR-2
    a = {0.0 + 1e-5, -0.05 + 1e-5, 0.0 - 1e-5};
    b = {0.5 - 1e-5, -0.05 + 1e-5, 0.0 - 1e-5};
    c = {0.5 - 1e-5,  0.25 - 1e-5, 0.0 - 1e-5};
    d = {0.0 + 1e-5,  0.25 - 1e-5, 0.0 - 1e-5};
    rect_pos_exp = {
        {10, 10, 9}, {10, 11, 9}, {10, 12, 9}, {11, 10, 9}, {11, 11, 9},
        {11, 12, 9}, {12, 10, 9}, {12, 11, 9}, {12, 12, 9}, {13, 10, 9},
        {13, 11, 9}, {13, 12, 9}, {14, 10, 9}, {14, 11, 9}, {14, 12, 9},
        {15, 10, 9}, {15, 11, 9}, {15, 12, 9}};
    CheckResults();

    // Test: BP-CR-3
    a = {0.0 + 1e-5, 0.0 - 1e-5, 0.0 + 1e-5};
    b = {0.5 - 1e-5, 0.0 - 1e-5, 0.0 + 1e-5};
    c = {0.5 - 1e-5, 0.0 - 1e-5, 0.5 - 1e-5};
    d = {0.0 + 1e-5, 0.0 - 1e-5, 0.5 - 1e-5};
    rect_pos_exp = {
        {10, 10, 10}, {10, 10, 11}, {10, 10, 12}, {10, 10, 13}, {10, 10, 14},
        {11, 10, 10}, {11, 10, 14}, {12, 10, 10}, {12, 10, 14}, {13, 10, 10},
        {13, 10, 14}, {14, 10, 10}, {14, 10, 14}, {15, 10, 10}, {15, 10, 11},
        {15, 10, 12}, {15, 10, 13}, {15, 10, 14}};
    CheckResults();

    // Test: BP-CR-4
    a = {0.5 + 1e-5, 0.0 + 1e-5, 0.5 + 1e-5};
    b = {0.6 - 1e-5, 0.0 + 1e-5, 0.6 - 1e-5};
    c = {0.6 - 1e-5, 0.5 - 1e-5, 0.6 - 1e-5};
    d = {0.5 + 1e-5, 0.5 - 1e-5, 0.5 + 1e-5};
    rect_pos_exp = {
        {15, 10, 15}, {15, 11, 15}, {15, 12, 15}, {15, 13, 15}, {15, 14, 15},
        {15, 15, 15}, {16, 10, 15}, {16, 11, 15}, {16, 12, 15}, {16, 13, 15},
        {16, 14, 15}, {16, 15, 15}};
    CheckResults();

    // Test: BP-CR-5
    a = {0.34 + 1e-5, 0.57 + 1e-5, 0.0 - 1e-5};
    b = {0.74 - 1e-5, 0.97 - 1e-5, 0.0 - 1e-5};
    c = {0.44 + 1e-5, 0.67 + 1e-5, 0.0 - 1e-5};
    d = {0.64 - 1e-5, 0.87 - 1e-5, 0.0 - 1e-5};
    rect_pos_exp = {
        {13, 16, 9}, {14, 16, 9}, {14, 17, 9}, {15, 17, 9}, {15, 18, 9},
        {16, 18, 9}, {16, 19, 9}, {17, 19, 9}, {17, 20, 9}};
    CheckResults();

    // Test: BP-CR-6
    a = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    b = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    c = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    d = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    rect_pos_exp = {{15, 15, 14}};
    CheckResults();

    // Test: BP-CR-7
    a = {0.55 - 1e-5, 0.55 - 1e-5, 0.5 - 1e-5};
    b = {0.55 - 1e-5, 0.55 - 1e-5, 0.5 - 1e-5};
    c = {0.55 - 1e-5, 0.55 - 1e-5, 0.5 - 1e-5};
    d = {0.55 - 1e-5, 0.55 - 1e-5, 0.5 - 1e-5};
    rect_pos_exp = {{15, 15, 14}};
    CheckResults();
}

TEST(UnitTestBucketPos, CalcTrianglePos) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    float tol = 1e-5;

    // Declaring variables
    std::vector<float> a;
    std::vector<float> b;
    std::vector<float> c;
    std::vector<std::vector<int>> tri_pos;
    std::vector<std::vector<int>> tri_pos_exp;

    // Creating a lambda function to check the results for any input orders
    // The results are sorted and duplicates are removed
    auto CheckResults = [&]() {
        // Checking first input order
        tri_pos = soil_simulator::CalcTrianglePos(a, b, c, grid, tol);
        sort(tri_pos.begin(), tri_pos.end());
        tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
        EXPECT_EQ(tri_pos.size(), tri_pos_exp.size());
        EXPECT_EQ(tri_pos, tri_pos_exp);
        // Checking second input order
        tri_pos = soil_simulator::CalcTrianglePos(a, c, b, grid, tol);
        sort(tri_pos.begin(), tri_pos.end());
        tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
        EXPECT_EQ(tri_pos.size(), tri_pos_exp.size());
        EXPECT_EQ(tri_pos, tri_pos_exp);
        // Checking third input order
        tri_pos = soil_simulator::CalcTrianglePos(b, a, c, grid, tol);
        sort(tri_pos.begin(), tri_pos.end());
        tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
        EXPECT_EQ(tri_pos.size(), tri_pos_exp.size());
        EXPECT_EQ(tri_pos, tri_pos_exp);
        // Checking fourth input order
        tri_pos = soil_simulator::CalcTrianglePos(b, c, a, grid, tol);
        sort(tri_pos.begin(), tri_pos.end());
        tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
        EXPECT_EQ(tri_pos.size(), tri_pos_exp.size());
        EXPECT_EQ(tri_pos, tri_pos_exp);
        // Checking fifth input order
        tri_pos = soil_simulator::CalcTrianglePos(c, a, b, grid, tol);
        sort(tri_pos.begin(), tri_pos.end());
        tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
        EXPECT_EQ(tri_pos.size(), tri_pos_exp.size());
        EXPECT_EQ(tri_pos, tri_pos_exp);
        // Checking sixth input order
        tri_pos = soil_simulator::CalcTrianglePos(c, b, a, grid, tol);
        sort(tri_pos.begin(), tri_pos.end());
        tri_pos.erase(unique(tri_pos.begin(), tri_pos.end()), tri_pos.end());
        EXPECT_EQ(tri_pos.size(), tri_pos_exp.size());
        EXPECT_EQ(tri_pos, tri_pos_exp);
    };

    // Test: BP-CT-1
    a = {0.0 + 1e-5, 0.0 + 1e-5, 0.0 - 1e-5};
    b = {1.0 - 1e-5, 0.0 + 1e-5, 0.0 - 1e-5};
    c = {0.0 + 1e-5, 1.0 - 1e-5, 0.0 - 1e-5};
    tri_pos_exp = {
        {10, 10, 9}, {10, 11, 9}, {10, 12, 9}, {10, 13, 9}, {10, 14, 9},
        {10, 15, 9}, {10, 16, 9}, {10, 17, 9}, {10, 18, 9}, {10, 19, 9},
        {10, 20, 9}, {11, 10, 9}, {11, 11, 9}, {11, 12, 9}, {11, 13, 9},
        {11, 14, 9}, {11, 15, 9}, {11, 16, 9}, {11, 17, 9}, {11, 18, 9},
        {11, 19, 9}, {11, 20, 9}, {12, 10, 9}, {12, 11, 9}, {12, 12, 9},
        {12, 13, 9}, {12, 14, 9}, {12, 15, 9}, {12, 16, 9}, {12, 17, 9},
        {12, 18, 9}, {12, 19, 9}, {13, 10, 9}, {13, 11, 9}, {13, 12, 9},
        {13, 13, 9}, {13, 14, 9}, {13, 15, 9}, {13, 16, 9}, {13, 17, 9},
        {13, 18, 9}, {14, 10, 9}, {14, 11, 9}, {14, 12, 9}, {14, 13, 9},
        {14, 14, 9}, {14, 15, 9}, {14, 16, 9}, {14, 17, 9}, {15, 10, 9},
        {15, 11, 9}, {15, 12, 9}, {15, 13, 9}, {15, 14, 9}, {15, 15, 9},
        {15, 16, 9}, {16, 10, 9}, {16, 11, 9}, {16, 12, 9}, {16, 13, 9},
        {16, 14, 9}, {16, 15, 9}, {17, 10, 9}, {17, 11, 9}, {17, 12, 9},
        {17, 13, 9}, {17, 14, 9}, {18, 10, 9}, {18, 11, 9}, {18, 12, 9},
        {18, 13, 9}, {19, 10, 9}, {19, 11, 9}, {19, 12, 9}, {20, 10, 9},
        {20, 11, 9}};
    CheckResults();

    // Test: BP-CT-2
    a = {0.0 + 1e-5, 0.0 - 1e-5, 0.0 + 1e-5};
    b = {1.0 - 1e-5, 0.0 - 1e-5, 0.0 + 1e-5};
    c = {0.0 + 1e-5, 0.0 - 1e-5, 1.0 - 1e-5};
    tri_pos_exp = {
        {10, 10, 10}, {10, 10, 11}, {10, 10, 12}, {10, 10, 13}, {10, 10, 14},
        {10, 10, 15}, {10, 10, 16}, {10, 10, 17}, {10, 10, 18}, {10, 10, 19},
        {11, 10, 10}, {11, 10, 18}, {11, 10, 19}, {12, 10, 10}, {12, 10, 17},
        {12, 10, 18}, {13, 10, 10}, {13, 10, 16}, {13, 10, 17}, {14, 10, 10},
        {14, 10, 15}, {14, 10, 16}, {15, 10, 10}, {15, 10, 14}, {15, 10, 15},
        {16, 10, 10}, {16, 10, 13}, {16, 10, 14}, {17, 10, 10}, {17, 10, 12},
        {17, 10, 13}, {18, 10, 10}, {18, 10, 11}, {18, 10, 12}, {19, 10, 10},
        {19, 10, 11}, {20, 10, 10}};
    CheckResults();

    // Test: BP-CT-3
    a = {0.5 + 1e-5, 0.0 + 1e-5, 0.5 + 1e-5};
    b = {0.6 - 1e-5, 0.0 + 1e-5, 0.6 - 1e-5};
    c = {0.6 - 2e-5, 0.5 - 1e-5, 0.6 - 2e-5};
    tri_pos_exp = {
        {15, 10, 15}, {15, 11, 15}, {15, 12, 15}, {15, 13, 15}, {16, 10, 15},
        {16, 11, 15}, {16, 12, 15}, {16, 13, 15}, {16, 14, 15}, {16, 15, 15}};
    CheckResults();

    // Test: BP-CT-4
    a = {0.34 + 1e-5, 0.56 + 1e-5, 0.0 - 1e-5};
    b = {0.74 - 1e-5, 0.97 - 1e-5, 0.0 - 1e-5};
    c = {0.74 - 1e-5, 0.97 - 1e-5, 0.0 - 1e-5};
    tri_pos_exp = {
        {13, 16, 9}, {14, 16, 9}, {14, 17, 9}, {15, 17, 9}, {15, 18, 9},
        {16, 18, 9}, {16, 19, 9}, {17, 19, 9}, {17, 20, 9}};
    CheckResults();

    // Test: BP-CT-5
    a = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    b = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    c = {0.5 - 1e-5, 0.5 - 1e-5, 0.5 - 1e-5};
    tri_pos_exp = {{15, 15, 14}};
    CheckResults();
}

TEST(UnitTestBucketPos, IncludeNewBodyPos) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    SetHeight(sim_out, 6, 9, NAN, 1.0, 2.0, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 8, 11, NAN, 0.5, 0.6, NAN, NAN, 0.8, 0.9, NAN, NAN);
    SetHeight(sim_out, 9, 8, NAN, NAN, NAN, NAN, NAN, 1.2, 1.4, NAN, NAN);

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
