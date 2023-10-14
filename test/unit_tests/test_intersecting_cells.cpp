/*
This file implements unit tests for the functions in intersecting_cells.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <random>
#include "gtest/gtest.h"
#include "soil_simulator/intersecting_cells.hpp"
#include "soil_simulator/utils.hpp"
#include "test/unit_tests/utility.hpp"

TEST(UnitTestIntersectingCells, MoveBodySoil) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    auto pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.7, grid, bucket);
    auto pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);

    // Declaring variables
    int ind;
    int ii;
    int jj;
    float h_soil;
    bool wall_presence;
    std::vector<float> posA;

    // Creating a lambda function to set the initial state
    auto SetInitState = [&]() {
        sim_out->body_[0][10][15] = 0.3;
        sim_out->body_[1][10][15] = 0.7;
        sim_out->body_[2][10][15] = -0.2;
        sim_out->body_[3][10][15] = 0.0;
        sim_out->body_soil_[0][10][15] = 0.7;
        sim_out->body_soil_[1][10][15] = 0.9;
        sim_out->body_soil_[2][10][15] = 0.0;
        sim_out->body_soil_[3][10][15] = 0.9;
        sim_out->body_soil_pos_.push_back(soil_simulator::body_soil
            {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
        sim_out->body_soil_pos_.push_back(soil_simulator::body_soil
            {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    };

    // Test: IC-MBS-1
    SetInitState();
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{5, 7}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-2
    SetInitState();
    sim_out->body_[0][5][7] = 0.1;
    sim_out->body_[1][5][7] = 0.2;
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{5, 7}}, {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-3
    SetInitState();
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.3;
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, true);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-4
    SetInitState();
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.2;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.2, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.8, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-5
    SetInitState();
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.4;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 5, 7, posA[0], posA[1], posA[2], 0.3});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 1.0, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-6
    SetInitState();
    sim_out->body_[2][5][7] = 0.3;
    sim_out->body_[3][5][7] = 0.6;
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{5, 7}}, {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-7
    SetInitState();
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.6;
    sim_out->body_soil_[2][5][7] = 0.6;
    sim_out->body_soil_[3][5][7] = 0.7;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 5, 7, posA[0], posA[1], posA[2], 0.1});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, true);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.7, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-8
    SetInitState();
    sim_out->body_[2][5][7] = -0.2;
    sim_out->body_[3][5][7] = 0.0;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.6, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-9
    SetInitState();
    sim_out->body_[2][5][7] = -0.2;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.3;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 5, 7, posA[0], posA[1], posA[2], 0.3});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.9, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-10
    SetInitState();
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_[2][5][7] = 0.2;
    sim_out->body_[3][5][7] = 0.4;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.2;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 5, 7, posA[0], posA[1], posA[2], 0.2});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    EXPECT_EQ(ind, 0);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.2, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-11
    SetInitState();
    sim_out->body_[0][5][7] = 0.6;
    sim_out->body_[1][5][7] = 0.7;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.1;
    sim_out->body_soil_[2][5][7] = 0.1;
    sim_out->body_soil_[3][5][7] = 0.6;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 5, 7, posA[0], posA[1], posA[2], 0.5});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    EXPECT_EQ(ind, 2);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-12
    SetInitState();
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.2;
    sim_out->body_[2][5][7] = 0.8;
    sim_out->body_[3][5][7] = 0.9;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.2, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.8, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-13
    SetInitState();
    sim_out->body_[0][5][7] = 0.8;
    sim_out->body_[1][5][7] = 0.9;
    sim_out->body_[2][5][7] = -0.1;
    sim_out->body_[3][5][7] = 0.0;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.6, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-14
    SetInitState();
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_[2][5][7] = 0.9;
    sim_out->body_[3][5][7] = 1.0;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.2;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 5, 7, posA[0], posA[1], posA[2], 0.1});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.8, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-15
    SetInitState();
    sim_out->body_[0][5][7] = 0.8;
    sim_out->body_[1][5][7] = 0.9;
    sim_out->body_[2][5][7] = -0.1;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.2;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 5, 7, posA[0], posA[1], posA[2], 0.2});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.8, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-16
    SetInitState();
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_[2][5][7] = 0.4;
    sim_out->body_[3][5][7] = 0.9;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.3, 1e-5);
    EXPECT_EQ(ind, 0);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 5, 7, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-17
    SetInitState();
    sim_out->body_[0][5][7] = 0.3;
    sim_out->body_[1][5][7] = 0.9;
    sim_out->body_[2][5][7] = -0.1;
    sim_out->body_[3][5][7] = 0.2;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.2, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.5, 1e-5);
    EXPECT_EQ(ind, 2);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 5, 7, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-18
    SetInitState();
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_[2][5][7] = 0.4;
    sim_out->body_[3][5][7] = 0.5;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.2;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 5, 7, posA[0], posA[1], posA[2], 0.1});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.4, 1e-5);
    EXPECT_EQ(ind, 0);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 5, 7, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-19
    SetInitState();
    sim_out->body_[0][5][7] = 0.6;
    sim_out->body_[1][5][7] = 0.9;
    sim_out->body_[2][5][7] = -0.1;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.2;
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 5, 7, posA[0], posA[1], posA[2], 0.2});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.2, 1e-5);
    EXPECT_EQ(ind, 2);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.6, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 5, 7, posA, 0.4);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    delete bucket;
    delete sim_out;
}

TEST(UnitTestIntersectingCells, MoveIntersectingBodySoil) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};

    // Declaring variables
    std::vector<float> pos0;
    std::vector<float> pos2;
    std::vector<float> posA;
    std::vector<float> posB;
    std::vector<float> posC;
    std::vector<float> posD;
    std::vector<float> posE;
    std::vector<float> posF;
    std::vector<float> posG;
    std::vector<float> posH;
    std::vector<float> posI;
    std::vector<float> posJ;
    std::vector<float> posK;

    // Test: IC-MIBS-1
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-2
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-3
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.4;
    sim_out->body_[1][11][15] = 1.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-4
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[0][11][15] = 0.4;
    sim_out->body_[1][11][15] = 1.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-5
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->terrain_[11][15] = 0.4;
    sim_out->body_[0][11][15] = 0.4;
    sim_out->body_[1][11][15] = 0.7;
    sim_out->body_soil_[0][11][15] = 0.7;
    sim_out->body_soil_[1][11][15] = 0.8;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-6
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[2][11][15] = 0.4;
    sim_out->body_[3][11][15] = 1.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-7
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[2][11][15] = 0.4;
    sim_out->body_[3][11][15] = 1.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-8
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->terrain_[11][15] = 0.3;
    sim_out->body_[2][11][15] = 0.4;
    sim_out->body_[3][11][15] = 0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-9
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-10
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-11
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-12
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-13
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.1;
    sim_out->body_soil_[0][11][15] = 0.1;
    sim_out->body_soil_[1][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-14
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.1;
    sim_out->body_soil_[0][11][15] = 0.1;
    sim_out->body_soil_[1][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-15
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.1;
    sim_out->body_soil_[2][11][15] = 0.1;
    sim_out->body_soil_[3][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-16
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.1;
    sim_out->body_soil_[2][11][15] = 0.1;
    sim_out->body_soil_[3][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-17
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.2;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-18
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.2;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-19
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.5;
    sim_out->body_[1][11][15] = 0.7;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-20
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[0][11][15] = 0.5;
    sim_out->body_[1][11][15] = 0.7;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-21
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.1;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.7;
    sim_out->body_soil_[0][11][15] = 0.1;
    sim_out->body_soil_[1][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-22
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.1;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.7;
    sim_out->body_soil_[0][11][15] = 0.1;
    sim_out->body_soil_[1][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-23
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.5;
    sim_out->body_[1][11][15] = 0.7;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.1;
    sim_out->body_soil_[2][11][15] = 0.1;
    sim_out->body_soil_[3][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-24
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[0][11][15] = 0.5;
    sim_out->body_[1][11][15] = 0.7;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.1;
    sim_out->body_soil_[2][11][15] = 0.1;
    sim_out->body_soil_[3][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-25
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.2;
    sim_out->body_[2][11][15] = 0.4;
    sim_out->body_[3][11][15] = 0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-26
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.2;
    sim_out->body_[2][11][15] = 0.4;
    sim_out->body_[3][11][15] = 0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-27
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.4;
    sim_out->body_[1][11][15] = 0.7;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-28
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[0][11][15] = 0.4;
    sim_out->body_[1][11][15] = 0.7;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-29
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.1;
    sim_out->body_[2][11][15] = 0.4;
    sim_out->body_[3][11][15] = 0.7;
    sim_out->body_soil_[0][11][15] = 0.1;
    sim_out->body_soil_[1][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-30
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.1;
    sim_out->body_[2][11][15] = 0.9;
    sim_out->body_[3][11][15] = 1.3;
    sim_out->body_soil_[0][11][15] = 0.1;
    sim_out->body_soil_[1][11][15] = 0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.6});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-31
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.4;
    sim_out->body_[1][11][15] = 0.7;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.1;
    sim_out->body_soil_[2][11][15] = 0.1;
    sim_out->body_soil_[3][11][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-32
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    sim_out->body_[0][11][15] = 0.9;
    sim_out->body_[1][11][15] = 1.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.1;
    sim_out->body_soil_[2][11][15] = 0.1;
    sim_out->body_soil_[3][11][15] = 0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, posA[0], posA[1], posA[2], 0.6});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-33
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.1;
    sim_out->body_[2][11][15] = 0.7;
    sim_out->body_[3][11][15] = 0.8;
    sim_out->body_soil_[0][11][15] = 0.1;
    sim_out->body_soil_[1][11][15] = 0.6;
    sim_out->body_soil_[2][11][15] = 0.8;
    sim_out->body_soil_[3][11][15] = 0.9;
    sim_out->body_[0][12][15] = 0.2;
    sim_out->body_[1][12][15] = 0.4;
    sim_out->body_soil_[0][12][15] = 0.4;
    sim_out->body_soil_[1][12][15] = 0.8;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 12, 15, pos0[0], pos0[1], pos0[2], 0.4});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[5], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-34
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.9;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.8;
    sim_out->body_[1][11][15] = 0.9;
    sim_out->body_[2][11][15] = 0.3;
    sim_out->body_[3][11][15] = 0.4;
    sim_out->body_soil_[0][11][15] = 0.9;
    sim_out->body_soil_[1][11][15] = 1.2;
    sim_out->body_soil_[2][11][15] = 0.4;
    sim_out->body_soil_[3][11][15] = 0.7;
    sim_out->body_[0][12][15] = 0.5;
    sim_out->body_[1][12][15] = 0.6;
    sim_out->body_soil_[0][12][15] = 0.6;
    sim_out->body_soil_[1][12][15] = 0.8;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.6});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.9, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.3});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, posA[0], posA[1], posA[2], 0.3});
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 12, 15, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 1.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[5], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-35
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 1.1;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = -0.1;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.8;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.5;
    sim_out->body_soil_[2][11][15] = 0.8;
    sim_out->body_soil_[3][11][15] = 0.9;
    sim_out->body_[2][12][15] = 0.1;
    sim_out->body_[3][12][15] = 0.4;
    sim_out->body_soil_[2][12][15] = 0.4;
    sim_out->body_soil_[3][12][15] = 0.8;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.8});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.0, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 15, posA[0], posA[1], posA[2], 0.4});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-36
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 1.1;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.5;
    sim_out->body_[1][11][15] = 0.8;
    sim_out->body_[2][11][15] = 0.1;
    sim_out->body_[3][11][15] = 0.4;
    sim_out->body_soil_[0][11][15] = 0.8;
    sim_out->body_soil_[1][11][15] = 0.9;
    sim_out->body_soil_[2][11][15] = 0.4;
    sim_out->body_soil_[3][11][15] = 0.5;
    sim_out->body_[2][12][15] = 0.5;
    sim_out->body_[3][12][15] = 0.6;
    sim_out->body_soil_[2][12][15] = 0.6;
    sim_out->body_soil_[3][12][15] = 0.8;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.8});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 15, posA[0], posA[1], posA[2], 0.2});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-37
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.1;
    sim_out->body_[2][11][15] = 0.7;
    sim_out->body_[3][11][15] = 0.8;
    sim_out->body_soil_[0][11][15] = 0.1;
    sim_out->body_soil_[1][11][15] = 0.6;
    sim_out->body_soil_[2][11][15] = 0.8;
    sim_out->body_soil_[3][11][15] = 0.9;
    sim_out->body_[0][12][15] = 0.2;
    sim_out->body_[1][12][15] = 0.4;
    sim_out->body_soil_[0][12][15] = 0.4;
    sim_out->body_soil_[1][12][15] = 0.8;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 12, 15, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[5], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-38
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.9;
    sim_out->body_[1][11][15] = 1.0;
    sim_out->body_[2][11][15] = 0.1;
    sim_out->body_[3][11][15] = 0.2;
    sim_out->body_soil_[0][11][15] = 1.0;
    sim_out->body_soil_[1][11][15] = 1.2;
    sim_out->body_soil_[2][11][15] = 0.2;
    sim_out->body_soil_[3][11][15] = 0.8;
    sim_out->body_[0][12][15] = 0.5;
    sim_out->body_[1][12][15] = 0.6;
    sim_out->body_soil_[0][12][15] = 0.6;
    sim_out->body_soil_[1][12][15] = 0.8;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 1.0, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, posA[0], posA[1], posA[2], 0.6});
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 12, 15, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 1.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 1.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[5], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-39
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 1.1;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = -0.1;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.8;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.4;
    sim_out->body_soil_[2][11][15] = 0.8;
    sim_out->body_soil_[3][11][15] = 0.9;
    sim_out->body_[2][12][15] = 0.1;
    sim_out->body_[3][12][15] = 0.4;
    sim_out->body_soil_[2][12][15] = 0.4;
    sim_out->body_soil_[3][12][15] = 0.8;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.8});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.0, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.4});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[5], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-40
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 1.1;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.5;
    sim_out->body_[1][11][15] = 0.8;
    sim_out->body_[2][11][15] = 0.1;
    sim_out->body_[3][11][15] = 0.2;
    sim_out->body_soil_[0][11][15] = 0.8;
    sim_out->body_soil_[1][11][15] = 0.9;
    sim_out->body_soil_[2][11][15] = 0.2;
    sim_out->body_soil_[3][11][15] = 0.3;
    sim_out->body_[2][12][15] = 0.5;
    sim_out->body_[3][12][15] = 0.6;
    sim_out->body_soil_[2][12][15] = 0.6;
    sim_out->body_soil_[3][12][15] = 0.8;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.8});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[5], 2, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-41
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.3;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.8;
    sim_out->body_soil_[0][11][15] = 0.3;
    sim_out->body_soil_[1][11][15] = 0.5;
    sim_out->body_soil_[2][11][15] = 0.8;
    sim_out->body_soil_[3][11][15] = 0.9;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.3;
    sim_out->body_soil_[0][12][15] = 0.3;
    sim_out->body_soil_[1][12][15] = 0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 12, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[5], 0, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-42
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.5;
    sim_out->body_[1][11][15] = 0.8;
    sim_out->body_[2][11][15] = 0.3;
    sim_out->body_[3][11][15] = 0.4;
    sim_out->body_soil_[0][11][15] = 0.8;
    sim_out->body_soil_[1][11][15] = 0.9;
    sim_out->body_soil_[2][11][15] = 0.4;
    sim_out->body_soil_[3][11][15] = 0.5;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.1, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[4], 0, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-43
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.1;
    sim_out->body_[2][11][15] = 0.7;
    sim_out->body_[3][11][15] = 0.8;
    sim_out->body_soil_[0][11][15] = 0.1;
    sim_out->body_soil_[1][11][15] = 0.7;
    sim_out->body_soil_[2][11][15] = 0.8;
    sim_out->body_soil_[3][11][15] = 0.9;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.1;
    sim_out->body_soil_[2][12][15] = 0.1;
    sim_out->body_soil_[3][12][15] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.6});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 15, posA[0], posA[1], posA[2], 0.2});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[5], 2, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-44
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.5;
    sim_out->body_[1][11][15] = 0.8;
    sim_out->body_[2][11][15] = 0.3;
    sim_out->body_[3][11][15] = 0.4;
    sim_out->body_soil_[0][11][15] = 0.8;
    sim_out->body_soil_[1][11][15] = 0.9;
    sim_out->body_soil_[2][11][15] = 0.4;
    sim_out->body_soil_[3][11][15] = 0.5;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[4], 2, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-45
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.3;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.8;
    sim_out->body_soil_[0][11][15] = 0.3;
    sim_out->body_soil_[1][11][15] = 0.4;
    sim_out->body_soil_[2][11][15] = 0.8;
    sim_out->body_soil_[3][11][15] = 0.9;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.3;
    sim_out->body_soil_[0][12][15] = 0.3;
    sim_out->body_soil_[1][12][15] = 0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 12, 15, posB[0], posB[1], posB[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[5], 0, 11, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[6], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-46
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.5;
    sim_out->body_[1][11][15] = 0.8;
    sim_out->body_[2][11][15] = 0.3;
    sim_out->body_[3][11][15] = 0.4;
    sim_out->body_soil_[0][11][15] = 0.8;
    sim_out->body_soil_[1][11][15] = 0.9;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.1;
    sim_out->body_soil_[0][12][15] = 0.1;
    sim_out->body_soil_[1][12][15] = 0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.1});
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 12, 15, posB[0], posB[1], posB[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[5], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-47
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.4;
    sim_out->body_[2][11][15] = 0.6;
    sim_out->body_[3][11][15] = 0.8;
    sim_out->body_soil_[2][11][15] = 0.8;
    sim_out->body_soil_[3][11][15] = 0.9;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.1;
    sim_out->body_soil_[2][12][15] = 0.1;
    sim_out->body_soil_[3][12][15] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.4, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 15, posB[0], posB[1], posB[2], 0.2});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-48
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.5;
    sim_out->body_[1][11][15] = 0.8;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.3;
    sim_out->body_soil_[0][11][15] = 0.8;
    sim_out->body_soil_[1][11][15] = 0.9;
    sim_out->body_soil_[2][11][15] = 0.3;
    sim_out->body_soil_[3][11][15] = 0.4;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.2;
    sim_out->body_soil_[2][12][15] = 0.2;
    sim_out->body_soil_[3][12][15] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, posA[0], posA[1], posA[2], 0.1});
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 15, posB[0], posB[1], posB[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[5], 2, 11, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[6], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-49
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 1.5;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.2;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.7;
    sim_out->body_soil_[0][11][15] = 0.2;
    sim_out->body_soil_[1][11][15] = 0.3;
    sim_out->body_[0][12][15] = 0.2;
    sim_out->body_[1][12][15] = 0.5;
    sim_out->body_[0][10][16] = 0.7;
    sim_out->body_[1][10][16] = 0.8;
    sim_out->body_[2][10][16] = 0.0;
    sim_out->body_[3][10][16] = 0.1;
    sim_out->body_soil_[2][10][16] = 0.1;
    sim_out->body_soil_[3][10][16] = 0.4;
    sim_out->body_[2][10][17] = 0.0;
    sim_out->body_[3][10][17] = 0.5;
    sim_out->body_[0][11][14] = 0.0;
    sim_out->body_[1][11][14] = 0.1;
    sim_out->body_[2][11][14] = 0.4;
    sim_out->body_[3][11][14] = 1.0;
    sim_out->body_soil_[0][11][14] = 0.1;
    sim_out->body_soil_[1][11][14] = 0.2;
    sim_out->body_[2][12][13] = 0.1;
    sim_out->body_[3][12][13] = 0.7;
    sim_out->body_[0][9][14] = 0.5;
    sim_out->body_[1][9][14] = 0.7;
    sim_out->body_[2][9][14] = 0.0;
    sim_out->body_[3][9][14] = 0.3;
    sim_out->body_soil_[2][9][14] = 0.3;
    sim_out->body_soil_[3][9][14] = 0.4;
    sim_out->body_[2][8][13] = -0.2;
    sim_out->body_[3][8][13] = 1.0;
    sim_out->body_[0][11][16] = 0.0;
    sim_out->body_[1][11][16] = 0.1;
    sim_out->body_[2][11][16] = 0.8;
    sim_out->body_[3][11][16] = 0.9;
    sim_out->body_soil_[0][11][16] = 0.1;
    sim_out->body_soil_[1][11][16] = 0.8;
    sim_out->body_[0][12][17] = 1.5;
    sim_out->body_[1][12][17] = 1.7;
    sim_out->body_[2][12][17] = 0.1;
    sim_out->body_[3][12][17] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 1.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.1});
    posB = soil_simulator::CalcBucketFramePos(10, 16, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 16, posB[0], posB[1], posB[2], 0.3});
    posC = soil_simulator::CalcBucketFramePos(11, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 14, posC[0], posC[1], posC[2], 0.1});
    posD = soil_simulator::CalcBucketFramePos(9, 14, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 9, 14, posD[0], posD[1], posD[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 16, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 16, pos0[0], pos0[1], pos0[2], 0.7});
    posE = soil_simulator::CalcBucketFramePos(12, 17, 0.3, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][16], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][16], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][14], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][9][14], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][9][14], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][16], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][16], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][17], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][17], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[7], 0, 11, 15, posA, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[8], 2, 10, 16, posB, 0.3);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[9], 0, 11, 14, posC, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[10], 2, 9, 14, posD, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[11], 2, 12, 17, posE, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 12);
    // Resetting values
    std::vector<std::vector<int>> body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {0, 10, 16}, {2, 10, 16}, {2, 10, 17}, {0, 11, 14}, {2, 11, 14},
        {2, 12, 13}, {0, 9, 14}, {2, 9, 14}, {2, 8, 13}, {0, 11, 16},
        {2, 11, 16}, {0, 12, 17}, {2, 12, 17}};
    std::vector<std::vector<int>> body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 10, 16}, {0, 11, 14},
        {2, 9, 14}, {0, 11, 16}, {2, 12, 17}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-50
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 1.5;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.2;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.7;
    sim_out->body_soil_[0][11][15] = 0.2;
    sim_out->body_soil_[1][11][15] = 0.5;
    sim_out->body_[2][12][15] = 0.1;
    sim_out->body_[3][12][15] = 0.2;
    sim_out->body_soil_[2][12][15] = 0.2;
    sim_out->body_soil_[3][12][15] = 0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 1.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 15, posA[0], posA[1], posA[2], 0.3});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 1.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[4], 2, 12, 15, posA, 1.0);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-51
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 1.5;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.4;
    sim_out->body_[1][11][15] = 0.9;
    sim_out->body_[2][11][15] = 0.1;
    sim_out->body_[3][11][15] = 0.2;
    sim_out->body_soil_[2][11][15] = 0.2;
    sim_out->body_soil_[3][11][15] = 0.4;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.3;
    sim_out->body_soil_[2][12][15] = 0.3;
    sim_out->body_soil_[3][12][15] = 0.8;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 1.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 15, posA[0], posA[1], posA[2], 0.5});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 1.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[4], 2, 12, 15, posA, 1.0);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-52
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.2;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.7;
    sim_out->body_soil_[0][11][15] = 0.2;
    sim_out->body_soil_[1][11][15] = 0.5;
    sim_out->body_[0][12][15] = 0.2;
    sim_out->body_[1][12][15] = 0.5;
    sim_out->body_[0][10][16] = 0.7;
    sim_out->body_[1][10][16] = 0.8;
    sim_out->body_[2][10][16] = 0.0;
    sim_out->body_[3][10][16] = 0.1;
    sim_out->body_soil_[2][10][16] = 0.1;
    sim_out->body_soil_[3][10][16] = 0.7;
    sim_out->body_[2][10][17] = 0.0;
    sim_out->body_[3][10][17] = 0.5;
    sim_out->body_[0][11][14] = 0.0;
    sim_out->body_[1][11][14] = 0.1;
    sim_out->body_[2][11][14] = 0.9;
    sim_out->body_[3][11][14] = 1.0;
    sim_out->body_soil_[0][11][14] = 0.1;
    sim_out->body_soil_[1][11][14] = 0.9;
    sim_out->body_[2][12][13] = 0.1;
    sim_out->body_[3][12][13] = 0.7;
    sim_out->body_[0][9][14] = 0.5;
    sim_out->body_[1][9][14] = 0.7;
    sim_out->body_[2][9][14] = 0.0;
    sim_out->body_[3][9][14] = 0.4;
    sim_out->body_soil_[2][9][14] = 0.4;
    sim_out->body_soil_[3][9][14] = 0.5;
    sim_out->body_[2][8][13] = -0.2;
    sim_out->body_[3][8][13] = 1.0;
    sim_out->body_[0][11][16] = 0.0;
    sim_out->body_[1][11][16] = 0.1;
    sim_out->body_[2][11][16] = 0.8;
    sim_out->body_[3][11][16] = 0.9;
    sim_out->body_soil_[0][11][16] = 0.1;
    sim_out->body_soil_[1][11][16] = 0.8;
    sim_out->body_[0][12][17] = 0.1;
    sim_out->body_[1][12][17] = 0.3;
    sim_out->body_[2][12][17] = 0.6;
    sim_out->body_[3][12][17] = 0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.3});
    pos2 = soil_simulator::CalcBucketFramePos(10, 16, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 16, pos2[0], pos2[1], pos2[2], 0.6});
    pos0 = soil_simulator::CalcBucketFramePos(11, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 14, pos0[0], pos0[1], pos0[2], 0.8});
    pos2 = soil_simulator::CalcBucketFramePos(9, 14, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 9, 14, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 16, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 16, pos0[0], pos0[1], pos0[2], 0.7});
    posA = soil_simulator::CalcBucketFramePos(12, 17, 0.3, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][16], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][16], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][14], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][9][14], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][9][14], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][16], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][16], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][17], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][17], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[7], 0, 12, 17, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 8);
    // Resetting values
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {0, 10, 16}, {2, 10, 16}, {2, 10, 17}, {0, 11, 14}, {2, 11, 14},
        {2, 12, 13}, {0, 9, 14}, {2, 9, 14}, {2, 8, 13}, {0, 11, 16},
        {2, 11, 16}, {0, 12, 17}, {2, 12, 17}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 10, 16}, {0, 11, 14},
        {2, 9, 14}, {0, 11, 16}, {0, 12, 17}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-53
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.9;
    sim_out->body_[1][11][15] = 1.4;
    sim_out->body_[2][11][15] = 0.3;
    sim_out->body_[3][11][15] = 0.4;
    sim_out->body_soil_[2][11][15] = 0.4;
    sim_out->body_soil_[3][11][15] = 0.9;
    sim_out->body_[0][12][15] = 0.7;
    sim_out->body_[1][12][15] = 0.8;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.1, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-54
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 3.0;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.2;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.7;
    sim_out->body_[0][12][15] = 0.3;
    sim_out->body_[1][12][15] = 0.4;
    sim_out->body_[2][12][15] = 0.6;
    sim_out->body_[3][12][15] = 0.7;
    sim_out->body_soil_[0][12][15] = 0.4;
    sim_out->body_soil_[1][12][15] = 0.5;
    sim_out->body_[0][13][15] = 0.4;
    sim_out->body_[1][13][15] = 0.5;
    sim_out->body_[2][13][15] = 0.9;
    sim_out->body_[3][13][15] = 1.0;
    sim_out->body_soil_[0][13][15] = 0.5;
    sim_out->body_soil_[1][13][15] = 0.7;
    sim_out->body_[0][14][15] = 0.0;
    sim_out->body_[1][14][15] = 0.2;
    sim_out->body_[2][14][15] = 0.6;
    sim_out->body_[3][14][15] = 0.7;
    sim_out->body_[0][15][15] = 0.0;
    sim_out->body_[1][15][15] = 0.2;
    sim_out->body_[2][15][15] = 0.6;
    sim_out->body_[3][15][15] = 0.7;
    sim_out->body_soil_[0][15][15] = 0.2;
    sim_out->body_soil_[1][15][15] = 0.4;
    sim_out->body_[0][16][15] = 0.7;
    sim_out->body_[1][16][15] = 0.8;
    sim_out->body_[2][16][15] = 0.0;
    sim_out->body_[3][16][15] = 0.5;
    sim_out->body_[0][17][15] = 0.9;
    sim_out->body_[1][17][15] = 1.0;
    sim_out->body_[2][17][15] = 0.5;
    sim_out->body_[3][17][15] = 0.6;
    sim_out->body_soil_[2][17][15] = 0.6;
    sim_out->body_soil_[3][17][15] = 0.8;
    sim_out->body_[0][18][15] = 0.0;
    sim_out->body_[1][18][15] = 0.8;
    sim_out->body_[2][18][15] = 0.9;
    sim_out->body_[3][18][15] = 1.0;
    sim_out->body_[0][19][15] = 0.9;
    sim_out->body_[1][19][15] = 1.0;
    sim_out->body_[2][19][15] = 0.0;
    sim_out->body_[3][19][15] = 0.4;
    sim_out->body_[0][20][15] = 0.0;
    sim_out->body_[1][20][15] = 0.1;
    sim_out->body_[2][20][15] = 0.9;
    sim_out->body_[3][20][15] = 1.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 2.7});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 12, 15, posB[0], posB[1], posB[2], 0.1});
    posC = soil_simulator::CalcBucketFramePos(13, 15, 0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 13, 15, posC[0], posC[1], posC[2], 0.2});
    posE = soil_simulator::CalcBucketFramePos(15, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 15, 15, posE[0], posE[1], posE[2], 0.2});
    posG = soil_simulator::CalcBucketFramePos(17, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 17, 15, posG[0], posG[1], posG[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posD = soil_simulator::CalcBucketFramePos(14, 15, 0.2, grid, bucket);
    posF = soil_simulator::CalcBucketFramePos(16, 15, 0.5, grid, bucket);
    posH = soil_simulator::CalcBucketFramePos(18, 15, 0.8, grid, bucket);
    posI = soil_simulator::CalcBucketFramePos(19, 15, 0.4, grid, bucket);
    posJ = soil_simulator::CalcBucketFramePos(20, 15, 0.1, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][13][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][13][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][14][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][14][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][15][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][15][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][16][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][16][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][17][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][17][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][18][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][18][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][19][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][19][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][20][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][20][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[6], 0, 11, 15, posA, 0.3);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[7], 0, 12, 15, posB, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[8], 0, 13, 15, posC, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[9], 0, 14, 15, posD, 0.4);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[10], 0, 15, 15, posE, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[11], 2, 16, 15, posF, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[12], 2, 17, 15, posG, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[13], 0, 18, 15, posH, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[14], 2, 19, 15, posI, 0.5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[15], 0, 20, 15, posJ, 0.4);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 16);
    // Resetting values
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}, {0, 13, 15}, {2, 13, 15}, {0, 14, 15}, {2, 14, 15},
        {0, 15, 15}, {2, 15, 15}, {0, 16, 15}, {2, 16, 15}, {0, 17, 15},
        {2, 17, 15}, {0, 18, 15}, {2, 18, 15}, {0, 19, 15}, {2, 19, 15},
        {0, 20, 15}, {2, 20, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}, {0, 13, 15},
        {0, 14, 15}, {0, 15, 15}, {2, 16, 15}, {2, 17, 15}, {0, 18, 15},
        {2, 19, 15}, {0, 20, 15}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-55
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][9][15] = 0.0;
    sim_out->body_[1][9][15] = 0.3;
    sim_out->body_[2][9][15] = 0.5;
    sim_out->body_[3][9][15] = 0.6;
    sim_out->body_soil_[0][9][15] = 0.3;
    sim_out->body_soil_[1][9][15] = 3.0;
    sim_out->body_soil_[2][9][15] = 0.6;
    sim_out->body_soil_[3][9][15] = 0.7;
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.4;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.2;
    sim_out->body_[2][11][15] = 0.5;
    sim_out->body_[3][11][15] = 0.6;
    sim_out->body_soil_[0][11][15] = 0.2;
    sim_out->body_soil_[1][11][15] = 0.3;
    sim_out->body_[0][12][15] = 0.4;
    sim_out->body_[1][12][15] = 0.5;
    sim_out->body_[2][12][15] = 0.1;
    sim_out->body_[3][12][15] = 0.2;
    sim_out->body_[0][13][15] = 0.3;
    sim_out->body_[1][13][15] = 0.4;
    sim_out->body_[2][13][15] = -0.2;
    sim_out->body_[3][13][15] = -0.1;
    sim_out->body_soil_[2][13][15] = -0.1;
    sim_out->body_soil_[3][13][15] = 0.0;
    sim_out->body_[0][14][15] = 0.4;
    sim_out->body_[1][14][15] = 0.5;
    sim_out->body_[2][14][15] = 0.0;
    sim_out->body_[3][14][15] = 0.2;
    sim_out->body_[0][15][15] = 0.0;
    sim_out->body_[1][15][15] = 0.1;
    sim_out->body_[2][15][15] = 0.6;
    sim_out->body_[3][15][15] = 0.7;
    sim_out->body_soil_[0][15][15] = 0.1;
    sim_out->body_soil_[1][15][15] = 0.2;
    sim_out->body_[0][16][15] = 0.4;
    sim_out->body_[1][16][15] = 0.5;
    sim_out->body_[2][16][15] = 1.0;
    sim_out->body_[3][16][15] = 1.1;
    sim_out->body_soil_[0][16][15] = 0.5;
    sim_out->body_soil_[1][16][15] = 0.9;
    sim_out->body_[0][17][15] = 0.9;
    sim_out->body_[1][17][15] = 1.1;
    sim_out->body_[2][17][15] = 0.6;
    sim_out->body_[3][17][15] = 0.7;
    sim_out->body_[0][18][15] = 0.6;
    sim_out->body_[1][18][15] = 0.8;
    sim_out->body_[2][18][15] = 1.0;
    sim_out->body_[3][18][15] = 1.1;
    sim_out->body_[0][19][15] = 1.5;
    sim_out->body_[1][19][15] = 1.6;
    sim_out->body_[2][19][15] = 0.6;
    sim_out->body_[3][19][15] = 0.9;
    sim_out->body_soil_[2][19][15] = 0.9;
    sim_out->body_soil_[3][19][15] = 1.2;
    sim_out->body_[0][20][15] = 0.0;
    sim_out->body_[1][20][15] = 0.1;
    sim_out->body_[2][20][15] = 0.9;
    sim_out->body_[3][20][15] = 1.2;
    sim_out->body_soil_[0][20][15] = 0.1;
    sim_out->body_soil_[1][20][15] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(9, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(9, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 9, 15, pos0[0], pos0[1], pos0[2], 2.7});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 9, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    posB = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posB[0], posB[1], posB[2], 0.1});
    posD = soil_simulator::CalcBucketFramePos(13, 15, -0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 13, 15, posD[0], posD[1], posD[2], 0.1});
    posF = soil_simulator::CalcBucketFramePos(15, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 15, 15, posF[0], posF[1], posF[2], 0.1});
    posG = soil_simulator::CalcBucketFramePos(16, 15, 0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 16, 15, posG[0], posG[1], posG[2], 0.4});
    posJ = soil_simulator::CalcBucketFramePos(19, 15, 0.9, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 19, 15, posJ[0], posJ[1], posJ[2], 0.3});
    posK = soil_simulator::CalcBucketFramePos(20, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 20, 15, posK[0], posK[1], posK[2], 0.2});
    posC = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    posE = soil_simulator::CalcBucketFramePos(14, 15, 0.2, grid, bucket);
    posH = soil_simulator::CalcBucketFramePos(17, 15, 0.7, grid, bucket);
    posI = soil_simulator::CalcBucketFramePos(18, 15, 0.8, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][9][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][9][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][9][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][9][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][13][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][13][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][14][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][14][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][15][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][15][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][16][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][16][15], 1.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][17][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][17][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][18][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][18][15], 1.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][19][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][19][15], 1.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][20][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][20][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[9], 2, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[10], 0, 11, 15, posB, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[11], 2, 12, 15, posC, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[12], 2, 13, 15, posD, 0.3);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[13], 2, 14, 15, posE, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[14], 0, 15, 15, posF, 0.4);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[15], 0, 16, 15, posG, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[16], 2, 17, 15, posH, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[17], 0, 18, 15, posI, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[18], 2, 19, 15, posJ, 0.3);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[19], 0, 20, 15, posK, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 20);
    // Resetting values
    body_pos = {
        {0, 9, 15}, {2, 9, 15}, {0, 10, 15}, {2, 10, 15}, {0, 11, 15},
        {2, 11, 15}, {0, 12, 15}, {2, 12, 15}, {0, 13, 15}, {2, 13, 15},
        {0, 14, 15}, {2, 14, 15}, {0, 15, 15}, {2, 15, 15}, {0, 16, 15},
        {2, 16, 15}, {0, 17, 15}, {2, 17, 15}, {0, 18, 15}, {2, 18, 15},
        {0, 19, 15}, {2, 19, 15}, {0, 20, 15}, {2, 20, 15}};
    body_soil_pos = {
        {0, 9, 15}, {2, 9, 15}, {2, 10, 15}, {0, 11, 15}, {2, 12, 15},
        {2, 13, 15}, {2, 14, 15}, {0, 15, 15}, {0, 16, 15}, {2, 17, 15},
        {0, 18, 15}, {2, 19, 15}, {0, 20, 15}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-56
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.1;
    sim_out->body_[2][11][15] = 0.4;
    sim_out->body_[3][11][15] = 0.5;
    sim_out->body_soil_[0][11][15] = 0.1;
    sim_out->body_soil_[1][11][15] = 0.4;
    sim_out->body_[0][12][15] = -0.2;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_[2][12][15] = 0.2;
    sim_out->body_[3][12][15] = 0.4;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.2;
    sim_out->body_[0][13][15] = 0.4;
    sim_out->body_[1][13][15] = 0.6;
    sim_out->body_[2][13][15] = 0.0;
    sim_out->body_[3][13][15] = 0.1;
    sim_out->body_soil_[2][13][15] = 0.1;
    sim_out->body_soil_[3][13][15] = 0.4;
    sim_out->body_[0][14][15] = 0.4;
    sim_out->body_[1][14][15] = 0.9;
    sim_out->body_[2][14][15] = 0.2;
    sim_out->body_[3][14][15] = 0.3;
    sim_out->body_soil_[2][14][15] = 0.3;
    sim_out->body_soil_[3][14][15] = 0.4;
    sim_out->body_[0][15][15] = 0.0;
    sim_out->body_[1][15][15] = 0.4;
    sim_out->body_[2][15][15] = 0.6;
    sim_out->body_[3][15][15] = 0.8;
    sim_out->body_soil_[0][15][15] = 0.4;
    sim_out->body_soil_[1][15][15] = 0.6;
    sim_out->body_[0][16][15] = 0.1;
    sim_out->body_[1][16][15] = 0.2;
    sim_out->body_[2][16][15] = 0.9;
    sim_out->body_[3][16][15] = 1.1;
    sim_out->body_soil_[0][16][15] = 0.2;
    sim_out->body_soil_[1][16][15] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, pos0[0], pos0[1], pos0[2], 0.3});
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 12, 15, pos0[0], pos0[1], pos0[2], 0.2});
    pos2 = soil_simulator::CalcBucketFramePos(13, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 13, 15, pos2[0], pos2[1], pos2[2], 0.3});
    pos2 = soil_simulator::CalcBucketFramePos(14, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 14, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(15, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 15, 15, pos0[0], pos0[1], pos0[2], 0.4});
    posA = soil_simulator::CalcBucketFramePos(16, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 16, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][13][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][14][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][14][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][15][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][15][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][16][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][16][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[8], 0, 16, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 9);
    // Resetting values
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}, {0, 13, 15}, {2, 13, 15}, {0, 14, 15}, {2, 14, 15},
        {0, 15, 15}, {2, 15, 15}, {0, 16, 15}, {2, 16, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}, {2, 13, 15},
        {2, 14, 15}, {0, 15, 15}, {0, 16, 15}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-57
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 1.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.2;
    sim_out->body_[1][11][15] = 0.3;
    sim_out->body_[2][11][15] = 0.6;
    sim_out->body_[3][11][15] = 0.7;
    sim_out->body_soil_[0][11][15] = 0.3;
    sim_out->body_soil_[1][11][15] = 0.4;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.1;
    sim_out->body_[2][12][15] = 0.4;
    sim_out->body_[3][12][15] = 0.6;
    sim_out->body_soil_[0][12][15] = 0.1;
    sim_out->body_soil_[1][12][15] = 0.2;
    sim_out->body_[0][13][15] = 0.7;
    sim_out->body_[1][13][15] = 0.8;
    sim_out->body_[2][13][15] = 0.1;
    sim_out->body_[3][13][15] = 0.2;
    sim_out->body_[0][14][15] = 0.5;
    sim_out->body_[1][14][15] = 0.7;
    sim_out->body_[2][14][15] = 0.0;
    sim_out->body_[3][14][15] = 0.3;
    sim_out->body_soil_[2][14][15] = 0.3;
    sim_out->body_soil_[3][14][15] = 0.4;
    sim_out->body_[0][15][15] = 0.0;
    sim_out->body_[1][15][15] = 0.1;
    sim_out->body_[2][15][15] = 0.3;
    sim_out->body_[3][15][15] = 0.7;
    sim_out->body_[0][16][15] = 1.1;
    sim_out->body_[1][16][15] = 1.2;
    sim_out->body_[2][16][15] = 0.2;
    sim_out->body_[3][16][15] = 0.3;
    sim_out->body_soil_[2][16][15] = 0.3;
    sim_out->body_soil_[3][16][15] = 0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 1.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.1});
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 12, 15, posB[0], posB[1], posB[2], 0.1});
    posD = soil_simulator::CalcBucketFramePos(14, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 14, 15, posD[0], posD[1], posD[2], 0.1});
    posF = soil_simulator::CalcBucketFramePos(16, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 16, 15, posF[0], posF[1], posF[2], 0.1});
    posC = soil_simulator::CalcBucketFramePos(13, 15, 0.2, grid, bucket);
    posE = soil_simulator::CalcBucketFramePos(15, 15, 0.1, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][13][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][14][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][14][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][15][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][15][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][16][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][16][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[6], 0, 11, 15, posA, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[7], 0, 12, 15, posB, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[8], 2, 13, 15, posC, 0.5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[9], 2, 14, 15, posD, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[10], 0, 15, 15, posE, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[11], 2, 16, 15, posF, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 12);
    // Resetting values
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}, {0, 13, 15}, {2, 13, 15}, {0, 14, 15}, {2, 14, 15},
        {0, 15, 15}, {2, 15, 15}, {0, 16, 15}, {2, 16, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}, {2, 13, 15},
        {2, 14, 15}, {0, 15, 15}, {2, 16, 15}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-58
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 1.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.2;
    sim_out->body_[1][11][15] = 0.3;
    sim_out->body_[2][11][15] = 0.7;
    sim_out->body_[3][11][15] = 0.8;
    sim_out->body_soil_[0][11][15] = 0.3;
    sim_out->body_soil_[1][11][15] = 0.4;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.1;
    sim_out->body_[2][12][15] = 0.4;
    sim_out->body_[3][12][15] = 0.5;
    sim_out->body_soil_[0][12][15] = 0.1;
    sim_out->body_soil_[1][12][15] = 0.4;
    sim_out->body_[0][13][15] = 0.0;
    sim_out->body_[1][13][15] = 0.3;
    sim_out->body_[2][13][15] = 0.5;
    sim_out->body_[3][13][15] = 0.9;
    sim_out->body_soil_[0][13][15] = 0.3;
    sim_out->body_soil_[1][13][15] = 0.4;
    sim_out->body_[0][14][15] = 0.7;
    sim_out->body_[1][14][15] = 0.8;
    sim_out->body_[2][14][15] = 0.3;
    sim_out->body_[3][14][15] = 0.4;
    sim_out->body_soil_[2][14][15] = 0.4;
    sim_out->body_soil_[3][14][15] = 0.7;
    sim_out->body_[0][15][15] = 0.5;
    sim_out->body_[1][15][15] = 0.9;
    sim_out->body_[2][15][15] = 0.0;
    sim_out->body_[3][15][15] = 0.1;
    sim_out->body_soil_[2][15][15] = 0.1;
    sim_out->body_soil_[3][15][15] = 0.2;
    sim_out->body_[0][16][15] = 0.1;
    sim_out->body_[1][16][15] = 0.2;
    sim_out->body_[2][16][15] = 0.3;
    sim_out->body_[3][16][15] = 0.4;
    sim_out->body_soil_[0][16][15] = 0.2;
    sim_out->body_soil_[1][16][15] = 0.3;
    sim_out->body_[0][17][15] = 0.6;
    sim_out->body_[1][17][15] = 1.2;
    sim_out->body_[2][17][15] = 0.0;
    sim_out->body_[3][17][15] = 0.1;
    sim_out->body_soil_[2][17][15] = 0.1;
    sim_out->body_soil_[3][17][15] = 0.4;
    sim_out->body_[0][18][15] = 0.8;
    sim_out->body_[1][18][15] = 0.9;
    sim_out->body_[2][18][15] = 0.0;
    sim_out->body_[3][18][15] = 0.4;
    sim_out->body_soil_[2][18][15] = 0.4;
    sim_out->body_soil_[3][18][15] = 0.8;
    sim_out->body_[0][19][15] = 0.1;
    sim_out->body_[1][19][15] = 0.2;
    sim_out->body_[2][19][15] = 0.6;
    sim_out->body_[3][19][15] = 0.9;
    sim_out->body_soil_[0][19][15] = 0.2;
    sim_out->body_soil_[1][19][15] = 0.4;
    sim_out->body_[0][20][15] = -0.1;
    sim_out->body_[1][20][15] = 0.0;
    sim_out->body_[2][20][15] = 0.9;
    sim_out->body_[3][20][15] = 1.5;
    sim_out->body_soil_[0][20][15] = 0.0;
    sim_out->body_soil_[1][20][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 1.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 12, 15, pos0[0], pos0[1], pos0[2], 0.3});
    posB = soil_simulator::CalcBucketFramePos(13, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 13, 15, posB[0], posB[1], posB[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(14, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 14, 15, pos2[0], pos2[1], pos2[2], 0.3});
    posC = soil_simulator::CalcBucketFramePos(15, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 15, 15, posC[0], posC[1], posC[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(16, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 16, 15, pos0[0], pos0[1], pos0[2], 0.1});
    posD = soil_simulator::CalcBucketFramePos(17, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 17, 15, posD[0], posD[1], posD[2], 0.3});
    pos2 = soil_simulator::CalcBucketFramePos(18, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 18, 15, pos2[0], pos2[1], pos2[2], 0.4});
    posE = soil_simulator::CalcBucketFramePos(19, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 19, 15, posE[0], posE[1], posE[2], 0.2});
    posF = soil_simulator::CalcBucketFramePos(20, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 20, 15, posF[0], posF[1], posF[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][13][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][13][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][14][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][14][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][15][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][15][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][16][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][16][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][17][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][17][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][18][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][18][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][19][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][19][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][20][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][20][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[12], 0, 11, 15, posA, 0.3);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[13], 0, 13, 15, posB, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[14], 2, 15, 15, posC, 0.3);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[15], 2, 17, 15, posD, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[16], 0, 19, 15, posE, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[17], 0, 20, 15, posF, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 18);
    // Resetting values
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}, {0, 13, 15}, {2, 13, 15}, {0, 14, 15}, {2, 14, 15},
        {0, 15, 15}, {2, 15, 15}, {0, 16, 15}, {2, 16, 15}, {0, 17, 15},
        {2, 17, 15}, {0, 18, 15}, {2, 18, 15}, {0, 19, 15}, {2, 19, 15},
        {0, 20, 15}, {2, 20, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}, {0, 13, 15},
        {2, 14, 15}, {2, 15, 15}, {0, 16, 15}, {2, 17, 15}, {2, 18, 15},
        {0, 19, 15}, {0, 20, 15}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-59
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 1.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 1.0;
    sim_out->body_[1][11][15] = 1.2;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.2;
    sim_out->body_soil_[2][11][15] = 0.2;
    sim_out->body_soil_[3][11][15] = 0.4;
    sim_out->body_[0][12][15] = 0.9;
    sim_out->body_[1][12][15] = 1.2;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 1.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, posA[0], posA[1], posA[2], 0.1});
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.1, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 1.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][12][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.6);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[4], 2, 12, 15, posB, 0.7);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    // Resetting values
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-60
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.2;
    sim_out->body_soil_[0][11][15] = 0.2;
    sim_out->body_soil_[1][11][15] = 0.8;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.6});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 1.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-61
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.2;
    sim_out->body_soil_[2][11][15] = 0.2;
    sim_out->body_soil_[3][11][15] = 0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, posA[0], posA[1], posA[2], 0.3});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-62
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-63
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.8;
    sim_out->body_[3][10][15] = 0.9;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 1.2;
    sim_out->body_soil_[2][10][15] = 0.9;
    sim_out->body_soil_[3][10][15] = 1.7;
    sim_out->body_[0][11][15] = 0.1;
    sim_out->body_[1][11][15] = 0.2;
    sim_out->body_soil_[0][11][15] = 0.2;
    sim_out->body_soil_[1][11][15] = 0.4;
    sim_out->body_[0][11][16] = 0.0;
    sim_out->body_[1][11][16] = 0.1;
    sim_out->body_[0][11][14] = 0.0;
    sim_out->body_[1][11][14] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.9, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.8});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.4});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 15, posA[0], posA[1], posA[2], 0.2});
    posB = soil_simulator::CalcBucketFramePos(11, 16, 0.1, grid, bucket);
    posC = soil_simulator::CalcBucketFramePos(11, 14, 0.1, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 1.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][16], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][16], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][14], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.1, 1.e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[6], 0, 11, 15, posA, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[7], 0, 11, 16, posB, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[8], 0, 11, 14, posC, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 9);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 14}, {0, 11, 15}, {0, 11, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 14}, {0, 11, 15}, {0, 11, 16}});

    // Test: IC-MIBS-64
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.5;
    sim_out->body_[1][10][15] = 0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.6;
    sim_out->body_soil_[1][10][15] = 0.9;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.5;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.1;
    sim_out->body_soil_[2][11][15] = 0.1;
    sim_out->body_soil_[3][11][15] = 0.9;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.3});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 11, 15, pos2[0], pos2[1], pos2[2], 0.8});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.9, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][15], 0.9, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-65
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Repeating the same movement with a different seed
    soil_simulator::rng.seed(2000);
    sim_out->terrain_[11][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_pos_[0].h_soil = 0.5;
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][16], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    delete bucket;
    delete sim_out;
}

TEST(UnitTestIntersectingCells, LocateIntersectingCells) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    sim_out->bucket_area_[0][0] = 4;
    sim_out->bucket_area_[0][1] = 12;
    sim_out->bucket_area_[1][0] = 8;
    sim_out->bucket_area_[1][1] = 17;
    sim_out->terrain_[10][11] = 0.1;
    sim_out->terrain_[10][12] = 0.1;
    sim_out->terrain_[10][13] = 0.1;
    sim_out->terrain_[10][14] = 0.1;
    sim_out->terrain_[10][15] = 0.1;
    sim_out->terrain_[10][16] = 0.1;
    sim_out->terrain_[11][11] = -0.1;
    sim_out->body_[0][5][10] = 0.0;
    sim_out->body_[1][5][10] = 0.1;
    sim_out->body_[2][6][10] = 0.0;
    sim_out->body_[3][6][10] = 0.1;
    sim_out->body_[0][7][10] = 0.0;
    sim_out->body_[1][7][10] = 0.1;
    sim_out->body_[2][7][10] = 0.2;
    sim_out->body_[3][7][10] = 0.3;
    sim_out->body_[0][11][11] = -0.1;
    sim_out->body_[1][11][11] = 0.0;
    sim_out->body_[0][10][11] = 0.0;
    sim_out->body_[1][10][11] = 0.1;
    sim_out->body_[2][10][12] = -0.1;
    sim_out->body_[3][10][12] = 0.0;
    sim_out->body_[0][10][13] = -0.2;
    sim_out->body_[1][10][13] = 0.0;
    sim_out->body_[2][10][13] = 0.0;
    sim_out->body_[3][10][13] = 0.3;
    sim_out->body_[0][10][14] = 0.2;
    sim_out->body_[1][10][14] = 0.3;
    sim_out->body_[2][10][14] = -0.1;
    sim_out->body_[3][10][14] = 0.0;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_[0][10][16] = -0.3;
    sim_out->body_[1][10][16] = -0.2;
    sim_out->body_[2][10][16] = -0.6;
    sim_out->body_[3][10][16] = -0.4;

    // Declaring variables
    std::vector<std::vector<int>> intersecting_cells;

    // -- Testing that intersecting cells are properly located --
    intersecting_cells = soil_simulator::LocateIntersectingCells(
        sim_out, 1e-5);
    EXPECT_TRUE((intersecting_cells[0] == std::vector<int> {0, 10, 11}));
    EXPECT_TRUE((intersecting_cells[1] == std::vector<int> {2, 10, 12}));
    EXPECT_TRUE((intersecting_cells[2] == std::vector<int> {0, 10, 13}));
    EXPECT_TRUE((intersecting_cells[3] == std::vector<int> {2, 10, 13}));
    EXPECT_TRUE((intersecting_cells[4] == std::vector<int> {2, 10, 14}));
    EXPECT_TRUE((intersecting_cells[5] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((intersecting_cells[6] == std::vector<int> {0, 10, 16}));
    EXPECT_TRUE((intersecting_cells[7] == std::vector<int> {2, 10, 16}));
    EXPECT_EQ(intersecting_cells.size(), 8);

    delete sim_out;
}

TEST(UnitTestIntersectingCells, MoveIntersectingBody) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    sim_out->bucket_area_[0][0] = 1;
    sim_out->bucket_area_[0][1] = 20;
    sim_out->bucket_area_[1][0] = 1;
    sim_out->bucket_area_[1][1] = 20;

    // Declaring variables
    std::vector<std::vector<int>> body_pos;
    std::vector<std::vector<int>> terrain_pos;

    // Test: IC-MIB-1
    for (auto ii = 11; ii < 13; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    sim_out->body_[0][10][16] = 0.0;
    sim_out->body_[1][10][16] = 0.5;
    sim_out->body_[0][10][18] = 0.0;
    sim_out->body_[1][10][18] = 0.5;
    sim_out->terrain_[11][17] = 0.1;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][17], 0.1, 1e-5);
    // Resetting values
    body_pos = {
        {0, 10, 16}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17}, {0, 11, 18},
        {0, 12, 16}, {0, 12, 17}, {0, 12, 18}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 17}}, body_pos, {});

    // Test: IC-MIB-2
    for (auto ii = 10; ii < 12; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    sim_out->body_[0][12][16] = 0.0;
    sim_out->body_[1][12][16] = 0.5;
    sim_out->body_[0][12][18] = 0.0;
    sim_out->body_[1][12][18] = 0.5;
    sim_out->terrain_[11][17] = 0.2;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][17], 0.2, 1e-5);
    // Resetting values
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17},
        {0, 11, 18}, {0, 12, 16}, {0, 12, 18}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 17}}, body_pos, {});

    // Test: IC-MIB-3
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 17; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    sim_out->body_[0][10][16] = 0.0;
    sim_out->body_[1][10][16] = 0.5;
    sim_out->body_[0][12][16] = 0.0;
    sim_out->body_[1][12][16] = 0.5;
    sim_out->terrain_[11][17] = 0.05;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][16], 0.05, 1e-5);
    // Resetting values
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 10, 18}, {0, 11, 17}, {0, 11, 18},
        {0, 12, 16}, {0, 12, 17}, {0, 12, 18}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{11, 16}}, body_pos, {});

    // Test: IC-MIB-4
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 18; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    sim_out->body_[0][10][18] = 0.0;
    sim_out->body_[1][10][18] = 0.5;
    sim_out->body_[0][12][18] = 0.0;
    sim_out->body_[1][12][18] = 0.5;
    sim_out->terrain_[11][17] = 0.25;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][18], 0.25, 1e-5);
    // Resetting values
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17},
        {0, 12, 16}, {0, 12, 17}, {0, 12, 18}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{11, 18}}, body_pos, {});

    // Test: IC-MIB-5
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 17; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    sim_out->body_[0][11][16] = 0.0;
    sim_out->body_[1][11][16] = 0.5;
    sim_out->body_[0][12][16] = 0.0;
    sim_out->body_[1][12][16] = 0.5;
    sim_out->terrain_[11][17] = 0.4;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.4, 1e-5);
    // Resetting values
    body_pos = {
        {0, 10, 17}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17}, {0, 11, 18},
        {0, 12, 16}, {0, 12, 17}, {0, 12, 18}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 16}}, body_pos, {});

    // Test: IC-MIB-6
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 17; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    sim_out->body_[0][10][16] = 0.0;
    sim_out->body_[1][10][16] = 0.5;
    sim_out->body_[0][11][16] = 0.0;
    sim_out->body_[1][11][16] = 0.5;
    sim_out->terrain_[11][17] = 0.1;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][16], 0.1, 1e-5);
    // Resetting values
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17},
        {0, 11, 18}, {0, 12, 17}, {0, 12, 18}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 16}}, body_pos, {});

    // Test: IC-MIB-7
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 18; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    sim_out->body_[0][11][18] = 0.0;
    sim_out->body_[1][11][18] = 0.5;
    sim_out->body_[0][12][18] = 0.0;
    sim_out->body_[1][12][18] = 0.5;
    sim_out->terrain_[11][17] = 0.5;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][18], 0.5, 1e-5);
    // Resetting values
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 11, 16}, {0, 11, 17}, {0, 11, 18},
        {0, 12, 16}, {0, 12, 17}, {0, 12, 18}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 18}}, body_pos, {});

    // Test: IC-MIB-8
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 18; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    sim_out->body_[0][10][18] = 0.0;
    sim_out->body_[1][10][18] = 0.5;
    sim_out->body_[0][11][18] = 0.0;
    sim_out->body_[1][11][18] = 0.5;
    sim_out->terrain_[11][17] = 0.8;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][18], 0.8, 1e-5);
    // Resetting values
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17},
        {0, 11, 18}, {0, 12, 16}, {0, 12, 17}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 18}}, body_pos, {});

    // Test: IC-MIB-9
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 18; jj++) {
            sim_out->body_[2][ii][jj] = 0.0;
            sim_out->body_[3][ii][jj] = 0.5;
        }
    sim_out->body_[2][11][18] = 0.0;
    sim_out->body_[3][11][18] = 0.5;
    sim_out->body_[2][12][18] = 0.0;
    sim_out->body_[3][12][18] = 0.5;
    sim_out->terrain_[11][17] = 0.5;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][18], 0.5, 1e-5);
    // Resetting values
    body_pos = {
        {2, 10, 16}, {2, 10, 17}, {2, 11, 16}, {2, 11, 17}, {2, 11, 18},
        {2, 12, 16}, {2, 12, 17}, {2, 12, 18}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 18}}, body_pos, {});

    // Test: IC-MIB-10
    sim_out->body_[2][10][16] = 0.0;
    sim_out->body_[3][10][16] = 0.5;
    sim_out->body_[2][10][17] = 0.0;
    sim_out->body_[3][10][17] = 0.5;
    sim_out->body_[0][11][16] = 0.0;
    sim_out->body_[1][11][16] = 0.5;
    sim_out->body_[0][11][17] = 0.0;
    sim_out->body_[1][11][17] = 0.5;
    sim_out->body_[0][12][16] = 0.0;
    sim_out->body_[1][12][16] = 0.5;
    sim_out->body_[0][12][17] = 0.0;
    sim_out->body_[1][12][17] = 0.5;
    sim_out->body_[2][12][16] = 0.6;
    sim_out->body_[3][12][16] = 0.8;
    sim_out->body_[2][12][17] = 0.6;
    sim_out->body_[3][12][17] = 0.8;
    sim_out->body_[0][11][18] = 0.0;
    sim_out->body_[1][11][18] = 0.5;
    sim_out->body_[2][12][18] = 0.0;
    sim_out->body_[3][12][18] = 0.5;
    sim_out->terrain_[11][17] = 0.5;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][18], 0.5, 1e-5);
    // Resetting values
    body_pos = {
        {2, 10, 16}, {2, 10, 17}, {0, 11, 16}, {0, 11, 17}, {0, 12, 16},
        {2, 12, 16}, {0, 12, 17}, {2, 12, 17}, {0, 11, 18}, {2, 12, 18}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 18}}, body_pos, {});

    // Test: IC-MIB-11
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 18; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.2;
        }
    sim_out->body_[0][10][18] = 0.0;
    sim_out->body_[1][10][18] = 0.5;
    sim_out->body_[0][11][18] = 0.0;
    sim_out->body_[1][11][18] = 0.5;
    sim_out->body_[0][11][17] = 0.5;
    sim_out->body_[1][11][17] = 0.6;
    sim_out->body_[2][11][17] = -0.2;
    sim_out->body_[3][11][17] = 0.3;
    sim_out->terrain_[11][17] = 0.8;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][17], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][18], 1.0, 1e-5);
    // Resetting values
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17},
        {2, 11, 17}, {0, 11, 18}, {0, 12, 16}, {0, 12, 17}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{12, 18}, {11, 17}}, body_pos, {});

    // Test: IC-MIB-12
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.2;
        }
    sim_out->body_[0][11][17] = -0.4;
    sim_out->body_[1][11][17] = 0.6;
    sim_out->body_[0][8][17] = 0.0;
    sim_out->body_[1][8][17] = 0.0;
    sim_out->terrain_[11][17] = 0.5;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][17], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[8][17], 0.9, 1e-5);
    // Resetting values
    body_pos = {};
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            body_pos.push_back(std::vector<int> {0, ii, jj});
        }
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{8, 17}, {11, 17}}, body_pos, {});

    // Test: IC-MIB-13
    soil_simulator::rng.seed(1234);
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.2;
        }
    sim_out->body_[0][11][17] = -0.5;
    sim_out->body_[1][11][17] = 0.6;
    sim_out->body_[0][10][17] = 0.1;
    sim_out->body_[1][10][17] = 0.2;
    sim_out->body_[0][8][17] = 0.25;
    sim_out->body_[1][8][17] = 0.4;
    sim_out->body_[0][12][17] = 0.2;
    sim_out->body_[1][12][17] = 0.3;
    sim_out->body_[0][13][17] = 0.05;
    sim_out->body_[1][13][17] = 0.4;
    sim_out->body_[2][13][17] = 0.6;
    sim_out->body_[3][13][17] = 0.7;
    sim_out->body_[0][13][19] = 0.3;
    sim_out->body_[1][13][19] = 0.5;
    sim_out->body_[0][14][20] = 0.0;
    sim_out->body_[1][14][20] = 0.0;
    sim_out->body_[2][14][20] = 0.2;
    sim_out->body_[3][14][20] = 0.4;
    sim_out->terrain_[11][17] = 0.5;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][17], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][17], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[8][17], 0.15, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][17], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][17], 0.05, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][19], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[14][20], 0.2, 1e-5);
    // Resetting values
    body_pos = {
        {0, 8, 14}, {0, 8, 15}, {0, 8, 16}, {0, 8, 17}, {0, 8, 18}, {0, 8, 19},
        {0, 8, 20}, {0, 9, 14}, {0, 9, 15}, {0, 9, 16}, {0, 9, 17}, {0, 9, 18},
        {0, 9, 19}, {0, 9, 20}, {0, 10, 14}, {0, 10, 15}, {0, 10, 16},
        {0, 10, 17}, {0, 10, 18}, {0, 10, 19}, {0, 10, 20}, {0, 11, 14},
        {0, 11, 15}, {0, 11, 16}, {0, 11, 17}, {0, 11, 18}, {0, 11, 19},
        {0, 11, 20}, {0, 12, 14}, {0, 12, 15}, {0, 12, 16}, {0, 12, 17},
        {0, 12, 18}, {0, 12, 19}, {0, 12, 20}, {0, 13, 14}, {0, 13, 15},
        {0, 13, 16}, {0, 13, 17}, {0, 13, 18}, {0, 13, 19}, {0, 13, 20},
        {0, 14, 14}, {0, 14, 15}, {0, 14, 16}, {0, 14, 17}, {0, 14, 18},
        {0, 14, 19}, {2, 13, 17}, {2, 14, 20}};
    test_soil_simulator::ResetValueAndTest(
        sim_out,
        {{11, 17}, {10, 17}, {8, 17}, {12, 17}, {13, 17}, {13, 19}, {14, 20}},
        body_pos, {});

    // Test: IC-MIB-14
    soil_simulator::rng.seed(1234);
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.2;
        }
    sim_out->body_[0][11][17] = -0.5;
    sim_out->body_[1][11][17] = 0.6;
    sim_out->body_[0][10][17] = 0.1;
    sim_out->body_[1][10][17] = 0.2;
    sim_out->body_[0][8][17] = 0.25;
    sim_out->body_[1][8][17] = 0.4;
    sim_out->body_[0][12][17] = 0.2;
    sim_out->body_[1][12][17] = 0.3;
    sim_out->body_[0][13][17] = 0.05;
    sim_out->body_[1][13][17] = 0.4;
    sim_out->body_[2][13][17] = 0.6;
    sim_out->body_[3][13][17] = 0.7;
    sim_out->body_[0][13][19] = 0.3;
    sim_out->body_[1][13][19] = 0.5;
    sim_out->body_[0][14][20] = 0.0;
    sim_out->body_[1][14][20] = 0.0;
    sim_out->body_[2][14][20] = 0.2;
    sim_out->body_[3][14][20] = 0.4;
    sim_out->terrain_[11][17] = 0.8;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][17], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][17], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[8][17], 0.25, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][17], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][17], 0.05, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][19], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[14][20], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[15][17], 0.2, 1e-5);
    // Resetting values
    terrain_pos = {
        {11, 17}, {10, 17}, {8, 17}, {12, 17}, {13, 17}, {13, 19}, {14, 20},
        {15, 17}};
    body_pos = {
        {0, 8, 14}, {0, 8, 15}, {0, 8, 16}, {0, 8, 17}, {0, 8, 18}, {0, 8, 19},
        {0, 8, 20}, {0, 9, 14}, {0, 9, 15}, {0, 9, 16}, {0, 9, 17}, {0, 9, 18},
        {0, 9, 19}, {0, 9, 20}, {0, 10, 14}, {0, 10, 15}, {0, 10, 16},
        {0, 10, 17}, {0, 10, 18}, {0, 10, 19}, {0, 10, 20}, {0, 11, 14},
        {0, 11, 15}, {0, 11, 16}, {0, 11, 17}, {0, 11, 18}, {0, 11, 19},
        {0, 11, 20}, {0, 12, 14}, {0, 12, 15}, {0, 12, 16}, {0, 12, 17},
        {0, 12, 18}, {0, 12, 19}, {0, 12, 20}, {0, 13, 14}, {0, 13, 15},
        {0, 13, 16}, {0, 13, 17}, {0, 13, 18}, {0, 13, 19}, {0, 13, 20},
        {0, 14, 14}, {0, 14, 15}, {0, 14, 16}, {0, 14, 17}, {0, 14, 18},
        {0, 14, 19}, {2, 13, 17}, {2, 14, 20}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, terrain_pos, body_pos, {});

    // Test: IC-MIB-15
    soil_simulator::rng.seed(1234);
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.2;
        }
    sim_out->body_[0][11][17] = -0.5;
    sim_out->body_[1][11][17] = 0.6;
    sim_out->body_[0][10][17] = 0.1;
    sim_out->body_[1][10][17] = 0.2;
    sim_out->body_[0][8][17] = 0.25;
    sim_out->body_[1][8][17] = 0.4;
    sim_out->body_[0][12][17] = 0.2;
    sim_out->body_[1][12][17] = 0.3;
    sim_out->body_[0][13][17] = 0.05;
    sim_out->body_[1][13][17] = 0.4;
    sim_out->body_[2][13][17] = 0.6;
    sim_out->body_[3][13][17] = 0.7;
    sim_out->body_[0][13][19] = 0.3;
    sim_out->body_[1][13][19] = 0.5;
    sim_out->body_[0][14][20] = 0.0;
    sim_out->body_[1][14][20] = 0.0;
    sim_out->body_[2][14][20] = 0.2;
    sim_out->body_[3][14][20] = 0.4;
    sim_out->terrain_[11][17] = 0.6;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][17], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][17], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[8][17], 0.25, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][17], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][17], 0.05, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][19], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[14][20], 0.2, 1e-5);
    // Resetting values
    body_pos = {
        {0, 8, 14}, {0, 8, 15}, {0, 8, 16}, {0, 8, 17}, {0, 8, 18}, {0, 8, 19},
        {0, 8, 20}, {0, 9, 14}, {0, 9, 15}, {0, 9, 16}, {0, 9, 17}, {0, 9, 18},
        {0, 9, 19}, {0, 9, 20}, {0, 10, 14}, {0, 10, 15}, {0, 10, 16},
        {0, 10, 17}, {0, 10, 18}, {0, 10, 19}, {0, 10, 20}, {0, 11, 14},
        {0, 11, 15}, {0, 11, 16}, {0, 11, 17}, {0, 11, 18}, {0, 11, 19},
        {0, 11, 20}, {0, 12, 14}, {0, 12, 15}, {0, 12, 16}, {0, 12, 17},
        {0, 12, 18}, {0, 12, 19}, {0, 12, 20}, {0, 13, 14}, {0, 13, 15},
        {0, 13, 16}, {0, 13, 17}, {0, 13, 18}, {0, 13, 19}, {0, 13, 20},
        {0, 14, 14}, {0, 14, 15}, {0, 14, 16}, {0, 14, 17}, {0, 14, 18},
        {0, 14, 19}, {2, 13, 17}, {2, 14, 20}};
    test_soil_simulator::ResetValueAndTest(
        sim_out,
        {{11, 17}, {10, 17}, {8, 17}, {12, 17}, {13, 17}, {13, 19}, {14, 20}},
        body_pos, {});

    // Test: IC-MIB-16
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.2;
        }
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    // Resetting values
    body_pos = {
        {0, 8, 14}, {0, 8, 15}, {0, 8, 16}, {0, 8, 17}, {0, 8, 18}, {0, 8, 19},
        {0, 8, 20}, {0, 9, 14}, {0, 9, 15}, {0, 9, 16}, {0, 9, 17}, {0, 9, 18},
        {0, 9, 19}, {0, 9, 20}, {0, 10, 14}, {0, 10, 15}, {0, 10, 16},
        {0, 10, 17}, {0, 10, 18}, {0, 10, 19}, {0, 10, 20}, {0, 11, 14},
        {0, 11, 15}, {0, 11, 16}, {0, 11, 17}, {0, 11, 18}, {0, 11, 19},
        {0, 11, 20}, {0, 12, 14}, {0, 12, 15}, {0, 12, 16}, {0, 12, 17},
        {0, 12, 18}, {0, 12, 19}, {0, 12, 20}, {0, 13, 14}, {0, 13, 15},
        {0, 13, 16}, {0, 13, 17}, {0, 13, 18}, {0, 13, 19}, {0, 13, 20},
        {0, 14, 14}, {0, 14, 15}, {0, 14, 16}, {0, 14, 17}, {0, 14, 18},
        {0, 14, 19}, {0, 14, 20}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, {});

    // Test: IC-MIB-17
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][11][17] = -0.4;
    sim_out->body_[1][11][17] = 0.6;
    sim_out->terrain_[11][17] = 0.5;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][17], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][17], 0.9, 1e-5);
    sim_out->terrain_[12][17] = 0.0;
    // Repeating the same movement with a different seed
    soil_simulator::rng.seed(2000);
    sim_out->terrain_[11][17] = 0.5;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][17], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][18], 0.9, 1e-5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{11, 17}, {10, 18}}, {{0, 11, 17}}, {});

    delete sim_out;
}
