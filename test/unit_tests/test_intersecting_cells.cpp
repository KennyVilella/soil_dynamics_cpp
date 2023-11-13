/*
This file implements unit tests for the functions in intersecting_cells.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <cmath>
#include <random>
#include "gtest/gtest.h"
#include "soil_simulator/intersecting_cells.hpp"
#include "soil_simulator/utils.hpp"
#include "test/unit_tests/utility.hpp"

// To make the function call holds in a single line.
// It greatly improves readability.
using test_soil_simulator::SetHeight;
using test_soil_simulator::PushBodySoilPos;
using test_soil_simulator::CheckHeight;
using test_soil_simulator::CheckBodySoilPos;
using test_soil_simulator::ResetValueAndTest;
using soil_simulator::LocateIntersectingCells;

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
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
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
        SetHeight(sim_out, 10, 15, NAN, 0.3, 0.7, 0.7, 0.9, -0.2, 0., 0., 0.9);
        PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
        PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.9);
    };

    // Test: IC-MBS-1
    SetInitState();
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{5, 7}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-2
    SetInitState();
    SetHeight(sim_out, 5, 7, NAN, 0.0, 0.3, NAN, NAN, NAN, NAN, NAN, NAN);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, true);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-3
    SetInitState();
    SetHeight(sim_out, 5, 7, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{5, 7}}, {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-4
    SetInitState();
    SetHeight(sim_out, 5, 7, NAN, 0.0, 0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.2, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    CheckHeight(sim_out, 5, 7, NAN, 0.2, 0.8, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-5
    SetInitState();
    SetHeight(sim_out, 5, 7, NAN, 0.0, 0.1, 0.1, 0.4, NAN, NAN, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 5, 7, posA, 0.3);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    CheckHeight(sim_out, 5, 7, NAN, 0.1, 1.0, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-6
    SetInitState();
    SetHeight(sim_out, 5, 7, NAN, NAN, NAN, NAN, NAN, 0.0, 0.6, 0.6, 0.7);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 5, 7, posA, 0.1);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, true);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    CheckHeight(sim_out, 5, 7, NAN, NAN, NAN, 0.6, 0.7);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-7
    SetInitState();
    SetHeight(sim_out, 5, 7, NAN, NAN, NAN, NAN, NAN, 0.3, 0.6, NAN, NAN);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{5, 7}}, {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-8
    SetInitState();
    SetHeight(sim_out, 5, 7, -0.2, NAN, NAN, NAN, NAN, -0.2, 0.0, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    CheckHeight(sim_out, 5, 7, NAN, NAN, NAN, 0.0, 0.6);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{5, 7}}, {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-9
    SetInitState();
    SetHeight(sim_out, 5, 7, -0.2, NAN, NAN, NAN, NAN, -0.2, 0.0, 0.0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 2, 5, 7, posA, 0.3);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    CheckHeight(sim_out, 5, 7, NAN, NAN, NAN, 0.0, 0.9);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{5, 7}}, {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-10
    SetInitState();
    SetHeight(sim_out, 5, 7, NAN, 0.0, 0.1, 0.1, 0.2, 0.2, 0.4, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 5, 7, posA, 0.2);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    EXPECT_EQ(ind, 0);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    CheckHeight(sim_out, 5, 7, NAN, 0.1, 0.2, NAN, NAN);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-11
    SetInitState();
    SetHeight(sim_out, 5, 7, NAN, 0.0, 0.2, NAN, NAN, 0.8, 0.9, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.2, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    CheckHeight(sim_out, 5, 7, NAN, 0.2, 0.8, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-12
    SetInitState();
    SetHeight(sim_out, 5, 7, NAN, 0.0, 0.1, NAN, NAN, 0.4, 0.9, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.3, 1e-5);
    EXPECT_EQ(ind, 0);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    CheckHeight(sim_out, 5, 7, NAN, 0.1, 0.4, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 5, 7, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-13
    SetInitState();
    SetHeight(sim_out, 5, 7, NAN, 0.0, 0.1, 0.1, 0.2, 0.9, 1.0, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 5, 7, posA, 0.1);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    CheckHeight(sim_out, 5, 7, NAN, 0.1, 0.8, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-14
    SetInitState();
    SetHeight(sim_out, 5, 7, NAN, 0.0, 0.1, 0.1, 0.2, 0.4, 0.5, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 5, 7, posA, 0.1);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.4, 1e-5);
    EXPECT_EQ(ind, 0);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    CheckHeight(sim_out, 5, 7, NAN, 0.1, 0.4, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 5, 7, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{0, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-15
    SetInitState();
    SetHeight(sim_out, 5, 7, NAN, 0.6, 0.7, NAN, NAN, 0.0, 0.1, 0.1, 0.6);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 2, 5, 7, posA, 0.5);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    EXPECT_EQ(ind, 2);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    CheckHeight(sim_out, 5, 7, NAN, NAN, NAN, 0.1, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-16
    SetInitState();
    SetHeight(sim_out, 5, 7, -0.1, 0.8, 0.9, NAN, NAN, -0.1, 0.0, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    CheckHeight(sim_out, 5, 7, NAN, NAN, NAN, 0.0, 0.6);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{5, 7}}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-17
    SetInitState();
    SetHeight(sim_out, 5, 7, -0.1, 0.3, 0.9, NAN, NAN, -0.1, 0.2, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.2, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.5, 1e-5);
    EXPECT_EQ(ind, 2);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    CheckHeight(sim_out, 5, 7, NAN, NAN, NAN, 0.2, 0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 5, 7, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{5, 7}}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-18
    SetInitState();
    SetHeight(sim_out, 5, 7, -0.1, 0.8, 0.9, NAN, NAN, -0.1, 0.0, 0.0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 2, 5, 7, posA, 0.2);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    CheckHeight(sim_out, 5, 7, NAN, NAN, NAN, 0.0, 0.8);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 5, 7, posA, 0.6);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{5, 7}}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
        {{2, 5, 7}, {0, 10, 15}, {2, 10, 15}});

    // Test: IC-MBS-19
    SetInitState();
    SetHeight(sim_out, 5, 7, -0.1, 0.6, 0.9, NAN, NAN, -0.1, 0.0, 0.0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 2, 5, 7, posA, 0.2);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.2, 1e-5);
    EXPECT_EQ(ind, 2);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    CheckHeight(sim_out, 5, 7, NAN, NAN, NAN, 0.0, 0.6);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 5, 7, posA, 0.4);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{5, 7}}, {{0, 5, 7}, {2, 5, 7}, {0, 10, 15}, {2, 10, 15}},
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
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};

    // Declaring variables
    std::vector<float> pos0;
    std::vector<float> pos2;
    std::vector<float> posA;
    std::vector<float> posB;
    std::vector<float> posC;
    std::vector<std::vector<int>> body_pos;
    std::vector<std::vector<int>> body_soil_pos;

    // Test: IC-MIBS-1
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-2
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-3
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.4, 1.0, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-4
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, 0.3, 0.4, 1.0, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-5
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.5, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-6
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.4, 0.7, 0.7, 0.8, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, 0.3, 0.7, 0.8, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-7
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, 0.4, 0.4, 0.7, 0.7, 0.8, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, 0.7, 0.7, 0.8, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-8
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.2, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.5, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-9
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 16}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-10
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, NAN, NAN, NAN, NAN, 0.4, 1.0, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-11
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, 0.3, NAN, NAN, NAN, NAN, 0.4, 0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-12
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-13
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, NAN, NAN, NAN, NAN, 0.4, 0.5, 0.5, 0.8);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.5, grid, bucket);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.3);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, 0.3, NAN, NAN, 0.5, 0.8);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-14
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, 0.3, NAN, NAN, NAN, NAN, 0.4, 0.5, 0.5, 0.6);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.5, grid, bucket);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, 0.6, NAN, NAN, 0.5, 0.6);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-15
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.1, 0.1, 0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.1, 0.5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-16
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.9, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 16}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-17
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, NAN, 0.4, 1.0, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-18
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, 0.4, 0.4, 1.0, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-19
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.5, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-20
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, NAN, 0.4, 1.0, 1.0, 2.0, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 1.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 1.0);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    CheckHeight(sim_out, 11, 15, 0.3, 1.0, 2.0, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-21
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, 0.4, 0.4, 0.5, 0.5, 0.6, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    CheckHeight(sim_out, 11, 15, 0.7, 0.5, 0.6, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-22
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.2, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.5, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-23
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.7, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 16}}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-24
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, NAN, NAN, NAN, NAN, NAN, 0.4, 1.0, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-25
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, 0.2, NAN, NAN, NAN, NAN, 0.4, 1.0, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-26
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-27
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, NAN, NAN, NAN, NAN, NAN, 0.4, 0.5, 0.5, 0.9);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.5, grid, bucket);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    CheckHeight(sim_out, 11, 15, 0.3, NAN, NAN, 0.5, 0.9);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-28
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, 0.2, NAN, NAN, NAN, NAN, 0.4, 1.0, 1.0, 1.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 1.0, grid, bucket);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.2);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    CheckHeight(sim_out, 11, 15, 0.5, NAN, NAN, 1.0, 1.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-29
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.1, 0.1, 0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.1, 0.5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-30
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.7, 0.0, 0.3, 0.3, 0.8);
    SetHeight(sim_out, 11, 15, NAN, NAN, NAN, NAN, NAN, 0.3, 0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.7, 0.3, 0.5);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 16}}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}});















    // Test: IC-MIBS-31
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.5, 0.7, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.5, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-32
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.4, 0.7, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.4, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-33
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, 0.2, 0.3, 0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-34
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.4, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-35
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, 0.2, 0.4, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posB, 0.2);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.6, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-36
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.2, 0.3, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-37
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, 0.2, NAN, NAN, NAN, NAN, 0.3, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-38
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.2, 0.4);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-39
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.4, NAN, NAN, 0.6, 0.8, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.1, 0.1, 0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.4, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.4, 0.6, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.1, 0.3);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 11, 15, posA, 0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-40
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.2, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-41
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, NAN, NAN, 0.7, 0.8, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.4, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-42
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.8, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.3, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-43
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, 0.2, 0.3, 0.7, 0.8, 0.8, 0.9);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posB, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.5, 0.8, 0.9);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}, {2, 12, 15}});

    // Test: IC-MIBS-44
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, 0.2, 0.3, 0.4, 0.8, 0.8, 0.9);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posB, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.4, 0.8, 0.9);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}, {2, 12, 15}});

    // Test: IC-MIBS-45
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, 0.2, 0.3, 0.3, 0.8, 0.8, 0.9);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.3, 0.8, 0.9);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}, {2, 12, 15}});

    // Test: IC-MIBS-46
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.7, 0.8, NAN, NAN, 0.0, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.2, 0.4);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-47
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.3, 0.8, NAN, NAN, 0.0, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.2, 0.3);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-48
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.8, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, 0.8, 0.9, 0.2, 0.5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}, {2, 12, 15}});

    // Test: IC-MIBS-49
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.4, 0.8, 0.8, 0.9, 0.0, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.8, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, 0.8, 0.9, 0.2, 0.4);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}, {2, 12, 15}});

    // Test: IC-MIBS-50
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.7, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.3, 0.8, 0.8, 0.9, 0.0, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 12, 15, NAN, 0.8, 0.9, 0.2, 0.3);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 12, 15}, {2, 12, 15}});








    // Test: IC-MIBS-51
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.2, 0.5, 0.7, 0.7, 0.9);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.2);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.5, 0.7, 0.9);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-52
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.2, 0.4, 0.7, 0.7, 0.9);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.2);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.4, 0.7, 0.9);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-53
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.6, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, 0.1, 0.2, 0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.5);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.7, 0.8, 0.9);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-54
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.3, 0.3, 0.4, 0.5, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.3, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.3, 0.5, 0.8, 0.9);
    CheckHeight(sim_out, 12, 15, NAN, 0.3, 0.5, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-55
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.3, 0.3, 0.4, 0.5, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.3, 0.3, 0.4, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.3, 0.5, 0.8, 0.9);
    CheckHeight(sim_out, 12, 15, NAN, 0.3, 0.6, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-56
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.3, 0.3, 0.4, 0.5, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.3, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.3, 0.5, 0.8, 0.9);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{10, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-57
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.6, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, 0.1, NAN, NAN, NAN, NAN, 0.2, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.5);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.7, 0.8, 0.9);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-58
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.3, 0.3, 0.4, 0.5, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.3, 0.5, 0.8, 0.9);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.3, 0.5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-59
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.3, 0.3, 0.4, 0.5, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.3, 0.3, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.3, 0.5, 0.8, 0.9);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.3, 0.6);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-60
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.3, 0.3, 0.4, 0.5, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.3, 0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.3, 0.5, 0.8, 0.9);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{10, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-61
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.6, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.4, NAN, NAN, 0.7, 0.9, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.5);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.7, 0.8, 0.9);
    CheckHeight(sim_out, 12, 15, NAN, 0.4, 0.6, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-62
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.6, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.4, NAN, NAN, 0.5, 0.9, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.5);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.7, 0.8, 0.9);
    CheckHeight(sim_out, 12, 15, NAN, 0.4, 0.5, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-63
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.6, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.2, 0.2, 0.3, 0.7, 0.9, 0.9, 1.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.5);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posB, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.7, 0.8, 0.9);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.5, 0.9, 1.3);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[7], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 8);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-64
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.6, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.2, 0.2, 0.3, 0.4, 0.9, 0.9, 1.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.5);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posB, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.7, 0.8, 0.9);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.4, 0.9, 1.3);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[7], 0, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 8);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

    // Test: IC-MIBS-65
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.6, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.2, 0.2, 0.4, 0.4, 0.9, 0.9, 1.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.5);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.7, 0.8, 0.9);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

    // Test: IC-MIBS-66
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.6, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.7, 0.9, NAN, NAN, 0.1, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.5);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.7, 0.8, 0.9);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.4, 0.6);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-67
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.6, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.5, 0.9, NAN, NAN, 0.1, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.5);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.7, 0.8, 0.9);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.4, 0.5);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-68
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.6, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.7, 0.9, 0.9, 1.3, 0.1, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.5);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.4);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.7, 0.8, 0.9);
    CheckHeight(sim_out, 12, 15, NAN, 0.9, 1.3, 0.2, 0.5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[7], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 8);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-69
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.6, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.4, 0.9, 0.9, 1.3, 0.1, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.5);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.4);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.7, 0.8, 0.9);
    CheckHeight(sim_out, 12, 15, NAN, 0.9, 1.3, 0.2, 0.4);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[7], 2, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 8);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

    // Test: IC-MIBS-70
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.6, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.4, 0.9, 0.9, 1.3, 0.1, 0.2, 0.2, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.5);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.4);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.2);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.1, 0.7, 0.8, 0.9);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);











    // Test: IC-MIBS-71
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.4, 0.4, 0.7, 0.7, 0.9);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.3);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.2);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-72
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.7, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, 0.1, 0.2, 0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.6);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-73
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.3, 0.3, 0.5, 0.5, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.3, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.3, 0.6, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-74
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.3, 0.3, 0.5, 0.5, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.3, 0.3, 0.4, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.3, 0.7, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-75
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.3, 0.3, 0.5, 0.5, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.3, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-76
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.7, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, 0.1, NAN, NAN, NAN, NAN, 0.2, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.6);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-77
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.3, 0.3, 0.5, 0.5, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.3, 0.6);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-78
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.3, 0.3, 0.5, 0.5, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.3, 0.3, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.3, 0.7);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-79
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.3, 0.3, 0.5, 0.5, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.3, 0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-80
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.7, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.4, NAN, NAN, 0.7, 0.9, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.6);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.4, 0.7, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-81
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.7, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.4, NAN, NAN, 0.5, 0.9, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.6);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.4, 0.5, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 12, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-82
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.7, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.2, 0.2, 0.3, 0.7, 0.9, 0.9, 1.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.6);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posA, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.6, 0.9, 1.3);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-83
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.7, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.2, 0.2, 0.3, 0.4, 0.9, 0.9, 1.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.6);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posA, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.4, 0.9, 1.3);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 12, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

    // Test: IC-MIBS-84
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.7, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.2, 0.2, 0.4, 0.4, 0.9, 0.9, 1.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.6);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

    // Test: IC-MIBS-85
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.7, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.7, 0.9, NAN, NAN, 0.1, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.6);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.4, 0.7);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-86
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.7, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.5, 0.9, NAN, NAN, 0.1, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.6);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.4, 0.5);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 12, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-87
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.7, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.7, 0.9, 0.9, 1.3, 0.1, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.6);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.4);
    PushBodySoilPos(sim_out, 2, 12, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.9, 1.3, 0.2, 0.6);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 2, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-88
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.7, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.4, 0.9, 0.9, 1.3, 0.1, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.6);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.4);
    PushBodySoilPos(sim_out, 2, 12, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.9, 1.3, 0.2, 0.4);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 2, 12, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

    // Test: IC-MIBS-89
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.1, 0.1, 0.7, 0.7, 0.8, 0.8, 0.9);
    SetHeight(sim_out, 12, 15, NAN, 0.4, 0.9, 0.9, 1.3, 0.1, 0.2, 0.2, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.6);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.4);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.2);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);











    // Test: IC-MIBS-90
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-91
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.4, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.4);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-92
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, 0.2, 0.3, 0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-93
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.4, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-94
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, 0.2, 0.4, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posB, 0.2);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.6, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-95
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.2, 0.3, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-96
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, 0.2, NAN, NAN, NAN, NAN, 0.3, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-97
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.2, 0.4);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-98
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.6, 0.8, NAN, NAN, 0.0, 0.4, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.1, 0.1, 0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.4, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.4, 0.6);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.1, 0.3);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-99
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.2, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-100
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, NAN, NAN, 0.7, 0.8, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.4, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-101
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, NAN, NAN, 0.3, 0.8, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.3, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-102
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, 0.2, 0.3, 0.7, 0.8, 0.8, 0.9);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posB, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.5, 0.8, 0.9);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {0, 12, 15}, {2, 12, 15}});

    // Test: IC-MIBS-103
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, 0.2, 0.3, 0.4, 0.8, 0.8, 0.9);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posB, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.4, 0.8, 0.9);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {0, 12, 15}, {2, 12, 15}});

    // Test: IC-MIBS-104
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.2, 0.2, 0.3, 0.3, 0.8, 0.8, 0.9);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.3, 0.8, 0.9);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {0, 12, 15}, {2, 12, 15}});

    // Test: IC-MIBS-105
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.7, 0.8, NAN, NAN, 0.0, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.2, 0.4);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-106
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.3, 0.8, NAN, NAN, 0.0, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.2, 0.3);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-107
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.8, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, 0.8, 0.9, 0.2, 0.5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {0, 12, 15}, {2, 12, 15}});

    // Test: IC-MIBS-108
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.4, 0.8, 0.8, 0.9, 0.0, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.8, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, 0.8, 0.9, 0.2, 0.4);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {0, 12, 15}, {2, 12, 15}});

    // Test: IC-MIBS-109
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.3, 0.7, NAN, NAN, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 12, 15, NAN, 0.3, 0.8, 0.8, 0.9, 0.0, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.3);
    CheckHeight(sim_out, 12, 15, NAN, 0.8, 0.9, 0.2, 0.3);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}, {0, 12, 15}, {2, 12, 15}});








    // Test: IC-MIBS-110
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.7, 0.7, 0.9, 0.0, 0.1, 0.1, 0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.7, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.7, 0.9, 0.1, 0.5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-111
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.4, 0.7, 0.7, 0.9, 0.0, 0.1, 0.1, 0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.7, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.7, 0.9, 0.1, 0.4);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-112
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.6);
    SetHeight(sim_out, 12, 15, 0.1, 0.2, 0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.5);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.1, 0.7);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-113
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.8, 0.8, 0.9, 0.0, 0.3, 0.3, 0.4);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.3, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.3, 0.5);
    CheckHeight(sim_out, 12, 15, NAN, 0.3, 0.5, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-114
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.8, 0.8, 0.9, 0.0, 0.3, 0.3, 0.4);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.3, 0.3, 0.4, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.3, 0.5);
    CheckHeight(sim_out, 12, 15, NAN, 0.3, 0.6, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-115
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.8, 0.8, 0.9, 0.0, 0.3, 0.3, 0.4);
    SetHeight(sim_out, 12, 15, NAN, 0.3, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.3, 0.5);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{10, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-116
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.6);
    SetHeight(sim_out, 12, 15, 0.1, NAN, NAN, NAN, NAN, 0.2, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.5);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.1, 0.7);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-117
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.8, 0.8, 0.9, 0.0, 0.3, 0.3, 0.4);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.3, 0.5);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.3, 0.5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-118
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.8, 0.8, 0.9, 0.0, 0.3, 0.3, 0.4);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.3, 0.3, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.1);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.3, 0.5);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.3, 0.6);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-119
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.8, 0.8, 0.9, 0.0, 0.3, 0.3, 0.4);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.3, 0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.3, 0.5);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{10, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-120
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.6);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.4, NAN, NAN, 0.7, 0.9, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.5);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.1, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.4, 0.6, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-121
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.6);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.4, NAN, NAN, 0.5, 0.9, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.5);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.1, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.4, 0.5, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-122
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.6);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.2, 0.2, 0.3, 0.7, 0.9, 0.9, 1.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.5);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posB, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.1, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.5, 0.9, 1.3);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[7], 0, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 8);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-123
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.6);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.2, 0.2, 0.3, 0.4, 0.9, 0.9, 1.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.5);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posB, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.1, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.4, 0.9, 1.3);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[7], 0, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 8);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

    // Test: IC-MIBS-124
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.6);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.2, 0.2, 0.4, 0.4, 0.9, 0.9, 1.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.1, 0.7);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

    // Test: IC-MIBS-125
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.6);
    SetHeight(sim_out, 12, 15, NAN, 0.7, 0.9, NAN, NAN, 0.1, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.5);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.1, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.4, 0.6);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-126
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.6);
    SetHeight(sim_out, 12, 15, NAN, 0.5, 0.9, NAN, NAN, 0.1, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.5);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.1, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.4, 0.5);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-127
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.6);
    SetHeight(sim_out, 12, 15, NAN, 0.7, 0.9, 0.9, 1.3, 0.1, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.4);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.1, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.9, 1.3, 0.2, 0.5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[7], 2, 12, 15, posB, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 8);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-128
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.6);
    SetHeight(sim_out, 12, 15, NAN, 0.4, 0.9, 0.9, 1.3, 0.1, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.4);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.1, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.9, 1.3, 0.2, 0.4);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 2, 11, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[7], 2, 12, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 8);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

    // Test: IC-MIBS-129
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.6);
    SetHeight(sim_out, 12, 15, NAN, 0.4, 0.9, 0.9, 1.3, 0.1, 0.2, 0.2, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.4);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.2);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.8, 0.9, 0.1, 0.7);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 2, 11, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);











    // Test: IC-MIBS-130
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.4, 0.7, 0.7, 0.9, 0.0, 0.1, 0.1, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.7, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.3);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-131
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.7);
    SetHeight(sim_out, 12, 15, 0.1, 0.2, 0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.6);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-132
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.8, 0.8, 0.9, 0.0, 0.3, 0.3, 0.5);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.3, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.2);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.3, 0.6, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-133
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.8, 0.8, 0.9, 0.0, 0.3, 0.3, 0.5);
    SetHeight(sim_out, 12, 15, NAN, 0.0, 0.3, 0.3, 0.4, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.2);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.3, 0.7, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 0, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-134
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.8, 0.8, 0.9, 0.0, 0.3, 0.3, 0.5);
    SetHeight(sim_out, 12, 15, NAN, 0.3, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.2);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-135
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.7);
    SetHeight(sim_out, 12, 15, 0.1, NAN, NAN, NAN, NAN, 0.2, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.6);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-136
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.8, 0.8, 0.9, 0.0, 0.3, 0.3, 0.5);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.2);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.3, 0.6);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-137
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.8, 0.8, 0.9, 0.0, 0.3, 0.3, 0.5);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.3, 0.3, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.2);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 12, 15, posB, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.3, 0.7);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[5], 2, 12, 15, posB, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-138
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.5, 0.8, 0.8, 0.9, 0.0, 0.3, 0.3, 0.5);
    SetHeight(sim_out, 12, 15, NAN, NAN, NAN, NAN, NAN, 0.3, 0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.2);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}});

    // Test: IC-MIBS-139
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.7);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.4, NAN, NAN, 0.7, 0.9, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.6);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.4, 0.7, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-140
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.7);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.4, NAN, NAN, 0.5, 0.9, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.6);
    posB = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.4, 0.5, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 12, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15}});

    // Test: IC-MIBS-141
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.7);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.2, 0.2, 0.3, 0.7, 0.9, 0.9, 1.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.6);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posA, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.6, 0.9, 1.3);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-142
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.7);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.2, 0.2, 0.3, 0.4, 0.9, 0.9, 1.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.6);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, posA, 0.1);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.2, 0.4, 0.9, 1.3);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 12, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

    // Test: IC-MIBS-143
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.7);
    SetHeight(sim_out, 12, 15, NAN, 0.1, 0.2, 0.2, 0.4, 0.4, 0.9, 0.9, 1.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.6);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.4);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

    // Test: IC-MIBS-144
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.7);
    SetHeight(sim_out, 12, 15, NAN, 0.7, 0.9, NAN, NAN, 0.1, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.6);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.4, 0.7);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-145
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.7);
    SetHeight(sim_out, 12, 15, NAN, 0.5, 0.9, NAN, NAN, 0.1, 0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.6);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.4, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, NAN, NAN, 0.4, 0.5);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 12, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(
        sim_out, {{13, 15}}, body_pos,
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {2, 12, 15}});

    // Test: IC-MIBS-146
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.7);
    SetHeight(sim_out, 12, 15, NAN, 0.7, 0.9, 0.9, 1.3, 0.1, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.6);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.4);
    PushBodySoilPos(sim_out, 2, 12, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.9, 1.3, 0.2, 0.6);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 2, 12, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {}, body_pos, body_soil_pos);

    // Test: IC-MIBS-147
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.7);
    SetHeight(sim_out, 12, 15, NAN, 0.4, 0.9, 0.9, 1.3, 0.1, 0.2, 0.2, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.6);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    posA = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.4);
    PushBodySoilPos(sim_out, 2, 12, 15, posA, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 12, 15, NAN, 0.9, 1.3, 0.2, 0.4);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 2, 12, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

    // Test: IC-MIBS-148
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.7, 0.8, 0.8, 0.9, 0.0, 0.1, 0.1, 0.7);
    SetHeight(sim_out, 12, 15, NAN, 0.4, 0.9, 0.9, 1.3, 0.1, 0.2, 0.2, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(11, 15, 0.8, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.6);
    pos0 = soil_simulator::CalcBucketFramePos(12, 15, 0.9, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(12, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 12, 15, pos0, 0.4);
    PushBodySoilPos(sim_out, 2, 12, 15, pos2, 0.2);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[13][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    body_soil_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {2, 11, 15}, {0, 12, 15},
        {2, 12, 15}};
    ResetValueAndTest(sim_out, {{13, 15}}, body_pos, body_soil_pos);

















    // Test: IC-MIBS-60
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.2, 0.2, 0.8, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.6);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 1.1, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 15}});

    // Test: IC-MIBS-61
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.2, 0.2, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 2, 11, 15, posA, 0.3);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.2, 0.8);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 11, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-62
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, -0.6, -0.5, -0.5, 0.0, -0.3, 0.0, 0.0, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, -0.5, -0.3, 0.0, 0.1);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{11, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-63
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 1.2, 0.8, 0.9, 0.9, 1.7);
    SetHeight(sim_out, 11, 15, NAN, 0.1, 0.2, 0.2, 0.4, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 16, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 14, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.9, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.8);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.4);
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 11, 15, posA, 0.2);
    posB = soil_simulator::CalcBucketFramePos(11, 16, 0.1, grid, bucket);
    posC = soil_simulator::CalcBucketFramePos(11, 14, 0.1, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.8, 0.9, 1.7);
    CheckHeight(sim_out, 11, 15, NAN, 0.2, 0.6, NAN, NAN);
    CheckHeight(sim_out, 11, 16, NAN, 0.1, 0.2, NAN, NAN);
    CheckHeight(sim_out, 11, 14, NAN, 0.1, 0.2, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.1, 1.e-5);
    CheckBodySoilPos(sim_out->body_soil_pos_[6], 0, 11, 15, posA, 0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[7], 0, 11, 16, posB, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[8], 0, 11, 14, posC, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 9);
    ResetValueAndTest(
        sim_out, {},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 14}, {0, 11, 15}, {0, 11, 16}},
        {{0, 10, 15}, {2, 10, 15}, {0, 11, 14}, {0, 11, 15}, {0, 11, 16}});

    // Test: IC-MIBS-64
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.5, 0.6, 0.6, 0.9, 0.0, 0.3, 0.3, 0.5);
    SetHeight(sim_out, 11, 15, NAN, NAN, NAN, NAN, NAN, 0.0, 0.1, 0.1, 0.9);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.3);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    pos2 = soil_simulator::CalcBucketFramePos(11, 15, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 2, 11, 15, pos2, 0.8);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.6, 0.9, 0.3, 0.5);
    CheckHeight(sim_out, 11, 15, NAN, NAN, NAN, 0.1, 0.9);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {}, {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}},
        {{0, 10, 15}, {2, 10, 15}, {2, 11, 15}});

    // Test: IC-MIBS-65
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Repeating the same movement with a different seed
    soil_simulator::rng.seed(2000);
    sim_out->terrain_[11][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_pos_[0].h_soil = 0.5;
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->terrain_[9][16], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{9, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: IC-MIBS-66
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 15, NAN, 0.0, 0.3, 0.3, 0.8, 0.5, 0.6, 0.6, 0.7);
    SetHeight(sim_out, 11, 15, NAN, 0.0, 0.7, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 16, NAN, 0.0, 0.7, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 16, NAN, 0.0, 0.7, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 9, 16, NAN, 0.0, 0.7, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 9, 15, NAN, 0.0, 0.7, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 9, 14, NAN, 0.0, 0.7, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 14, NAN, 0.0, 0.7, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 14, NAN, 0.0, 0.7, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    testing::internal::CaptureStdout();
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
     std::string warning_msg = testing::internal::GetCapturedStdout();
    std::string exp_msg = "The extra soil has been arbitrarily removed.";
    size_t string_loc = warning_msg.find(exp_msg);
    EXPECT_TRUE(string_loc != std::string::npos);
    CheckHeight(sim_out, 10, 15, NAN, 0.3, 0.5, 0.6, 0.7);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    body_pos = {
        {0, 10, 15}, {2, 10, 15}, {0, 11, 15}, {0, 11, 16}, {0, 10, 16},
        {0, 9, 16}, {0, 9, 15}, {0, 9, 14}, {0, 10, 14}, {0, 11, 14}};
    ResetValueAndTest(sim_out, {}, body_pos, {{0, 10, 15}, {2, 10, 15}});

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

    // Declaring variables
    std::vector<std::vector<int>> intersecting_cells;

    // Test: IC-LIC-1
    SetHeight(sim_out, 5, 10, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    intersecting_cells = LocateIntersectingCells(sim_out, 1e-5);
    EXPECT_EQ(intersecting_cells.size(), 0);
    ResetValueAndTest(sim_out, {}, {{0, 5, 10}}, {});

    // Test: IC-LIC-2
    SetHeight(sim_out, 11, 11, -0.1, -0.1, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    intersecting_cells = LocateIntersectingCells(sim_out, 1e-5);
    EXPECT_EQ(intersecting_cells.size(), 0);
    ResetValueAndTest(sim_out, {{11, 11}}, {{0, 11, 11}}, {});

    // Test: IC-LIC-3
    SetHeight(sim_out, 6, 10, NAN, NAN, NAN, NAN, NAN, 0.0, 0.1, NAN, NAN);
    intersecting_cells = LocateIntersectingCells(sim_out, 1e-5);
    EXPECT_EQ(intersecting_cells.size(), 0);
    ResetValueAndTest(sim_out, {}, {{2, 6, 10}}, {});

    // Test: IC-LIC-4
    SetHeight(sim_out, 7, 10, NAN, 0.0, 0.1, NAN, NAN, 0.2, 0.3, NAN, NAN);
    intersecting_cells = LocateIntersectingCells(sim_out, 1e-5);
    EXPECT_EQ(intersecting_cells.size(), 0);
    ResetValueAndTest(sim_out, {}, {{0, 7, 10}, {2, 7, 10}}, {});

    // Test: IC-LIC-5
    SetHeight(sim_out, 10, 11, 0.1, -0.1, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    intersecting_cells = LocateIntersectingCells(sim_out, 1e-5);
    EXPECT_TRUE((intersecting_cells[0] == std::vector<int> {0, 10, 11}));
    EXPECT_EQ(intersecting_cells.size(), 1);
    ResetValueAndTest(sim_out, {{10, 11}}, {{0, 10, 11}}, {});

    // Test: IC-LIC-6
    SetHeight(sim_out, 10, 12, 0.1, NAN, NAN, NAN, NAN, -0.1, 0.0, NAN, NAN);
    intersecting_cells = LocateIntersectingCells(sim_out, 1e-5);
    EXPECT_TRUE((intersecting_cells[0] == std::vector<int> {2, 10, 12}));
    EXPECT_EQ(intersecting_cells.size(), 1);
    ResetValueAndTest(sim_out, {{10, 12}}, {{2, 10, 12}}, {});

    // Test: IC-LIC-7
    SetHeight(sim_out, 10, 13, 0.1, -0.2, 0.0, NAN, NAN, 0.0, 0.3, NAN, NAN);
    intersecting_cells = LocateIntersectingCells(sim_out, 1e-5);
    EXPECT_TRUE((intersecting_cells[0] == std::vector<int> {0, 10, 13}));
    EXPECT_TRUE((intersecting_cells[1] == std::vector<int> {2, 10, 13}));
    EXPECT_EQ(intersecting_cells.size(), 2);
    ResetValueAndTest(sim_out, {{10, 13}}, {{0, 10, 13}, {2, 10, 13}}, {});

    // Test: IC-LIC-8
    SetHeight(sim_out, 10, 14, 0.1, 0.2, 0.3, NAN, NAN, -0.1, 0.0, NAN, NAN);
    intersecting_cells = LocateIntersectingCells(sim_out, 1e-5);
    EXPECT_TRUE((intersecting_cells[0] == std::vector<int> {2, 10, 14}));
    EXPECT_EQ(intersecting_cells.size(), 1);
    ResetValueAndTest(sim_out, {{10, 14}}, {{0, 10, 14}, {2, 10, 14}}, {});

    // Test: IC-LIC-9
    SetHeight(sim_out, 10, 15, 0.1, -0.3, -0.2, NAN, NAN, 0.5, 0.6, NAN, NAN);
    intersecting_cells = LocateIntersectingCells(sim_out, 1e-5);
    EXPECT_TRUE((intersecting_cells[0] == std::vector<int> {0, 10, 15}));
    EXPECT_EQ(intersecting_cells.size(), 1);
    ResetValueAndTest(sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: IC-LIC-10
    SetHeight(sim_out, 10, 16, 0.1, -0.3, -0.2, NAN, NAN, -0.6, -0.4, NAN, NAN);
    intersecting_cells = LocateIntersectingCells(sim_out, 1e-5);
    EXPECT_TRUE((intersecting_cells[0] == std::vector<int> {0, 10, 16}));
    EXPECT_TRUE((intersecting_cells[1] == std::vector<int> {2, 10, 16}));
    EXPECT_EQ(intersecting_cells.size(), 2);
    ResetValueAndTest(sim_out, {{10, 16}}, {{0, 10, 16}, {2, 10, 16}}, {});

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
    SetHeight(sim_out, 10, 16, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 18, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    sim_out->terrain_[11][17] = 0.1;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][17], 0.1, 1e-5);
    body_pos = {
        {0, 10, 16}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17}, {0, 11, 18},
        {0, 12, 16}, {0, 12, 17}, {0, 12, 18}};
    ResetValueAndTest(sim_out, {{10, 17}}, body_pos, {});

    // Test: IC-MIB-2
    for (auto ii = 10; ii < 12; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    SetHeight(sim_out, 12, 16, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 18, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    sim_out->terrain_[11][17] = 0.2;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][17], 0.2, 1e-5);
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17},
        {0, 11, 18}, {0, 12, 16}, {0, 12, 18}};
    ResetValueAndTest(sim_out, {{12, 17}}, body_pos, {});

    // Test: IC-MIB-3
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 17; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    SetHeight(sim_out, 10, 16, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 16, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    sim_out->terrain_[11][17] = 0.05;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][16], 0.05, 1e-5);
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 10, 18}, {0, 11, 17}, {0, 11, 18},
        {0, 12, 16}, {0, 12, 17}, {0, 12, 18}};
    ResetValueAndTest(sim_out, {{11, 16}}, body_pos, {});

    // Test: IC-MIB-4
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 18; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    SetHeight(sim_out, 10, 18, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 18, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    sim_out->terrain_[11][17] = 0.25;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][18], 0.25, 1e-5);
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17},
        {0, 12, 16}, {0, 12, 17}, {0, 12, 18}};
    ResetValueAndTest(sim_out, {{11, 18}}, body_pos, {});

    // Test: IC-MIB-5
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 17; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    SetHeight(sim_out, 11, 16, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 16, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    sim_out->terrain_[11][17] = 0.4;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.4, 1e-5);
    body_pos = {
        {0, 10, 17}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17}, {0, 11, 18},
        {0, 12, 16}, {0, 12, 17}, {0, 12, 18}};
    ResetValueAndTest(sim_out, {{10, 16}}, body_pos, {});

    // Test: IC-MIB-6
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 17; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    SetHeight(sim_out, 10, 16, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 16, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    sim_out->terrain_[11][17] = 0.1;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][16], 0.1, 1e-5);
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17},
        {0, 11, 18}, {0, 12, 17}, {0, 12, 18}};
    ResetValueAndTest(sim_out, {{12, 16}}, body_pos, {});

    // Test: IC-MIB-7
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 18; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    SetHeight(sim_out, 11, 18, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 18, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    sim_out->terrain_[11][17] = 0.5;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][18], 0.5, 1e-5);
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 11, 16}, {0, 11, 17}, {0, 11, 18},
        {0, 12, 16}, {0, 12, 17}, {0, 12, 18}};
    ResetValueAndTest(sim_out, {{10, 18}}, body_pos, {});

    // Test: IC-MIB-8
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 18; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.5;
        }
    SetHeight(sim_out, 10, 18, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 18, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    sim_out->terrain_[11][17] = 0.8;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][18], 0.8, 1e-5);
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17},
        {0, 11, 18}, {0, 12, 16}, {0, 12, 17}};
    ResetValueAndTest(sim_out, {{12, 18}}, body_pos, {});

    // Test: IC-MIB-9
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 18; jj++) {
            sim_out->body_[2][ii][jj] = 0.0;
            sim_out->body_[3][ii][jj] = 0.5;
        }
    SetHeight(sim_out, 11, 18, NAN, NAN, NAN, NAN, NAN, 0.0, 0.5, NAN, NAN);
    SetHeight(sim_out, 12, 18, NAN, NAN, NAN, NAN, NAN, 0.0, 0.5, NAN, NAN);
    sim_out->terrain_[11][17] = 0.5;
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][18], 0.5, 1e-5);
    body_pos = {
        {2, 10, 16}, {2, 10, 17}, {2, 11, 16}, {2, 11, 17}, {2, 11, 18},
        {2, 12, 16}, {2, 12, 17}, {2, 12, 18}};
    ResetValueAndTest(sim_out, {{10, 18}}, body_pos, {});

    // Test: IC-MIB-10
    SetHeight(sim_out, 10, 16, NAN, NAN, NAN, NAN, NAN, 0.0, 0.5, NAN, NAN);
    SetHeight(sim_out, 10, 17, NAN, NAN, NAN, NAN, NAN, 0.0, 0.5, NAN, NAN);
    SetHeight(sim_out, 11, 16, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 17, 0.5, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 16, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 17, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 16, NAN, NAN, NAN, NAN, NAN, 0.6, 0.8, NAN, NAN);
    SetHeight(sim_out, 12, 17, NAN, NAN, NAN, NAN, NAN, 0.6, 0.8, NAN, NAN);
    SetHeight(sim_out, 11, 18, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 18, NAN, NAN, NAN, NAN, NAN, 0.0, 0.5, NAN, NAN);
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][18], 0.5, 1e-5);
    body_pos = {
        {2, 10, 16}, {2, 10, 17}, {0, 11, 16}, {0, 11, 17}, {0, 12, 16},
        {2, 12, 16}, {0, 12, 17}, {2, 12, 17}, {0, 11, 18}, {2, 12, 18}};
    ResetValueAndTest(sim_out, {{10, 18}}, body_pos, {});

    // Test: IC-MIB-11
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 18; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.2;
        }
    SetHeight(sim_out, 10, 18, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 18, NAN, 0.0, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 17, 0.8, 0.5, 0.6, NAN, NAN, -0.2, 0.3, NAN, NAN);
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][17], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][18], 1.0, 1e-5);
    body_pos = {
        {0, 10, 16}, {0, 10, 17}, {0, 10, 18}, {0, 11, 16}, {0, 11, 17},
        {2, 11, 17}, {0, 11, 18}, {0, 12, 16}, {0, 12, 17}};
    ResetValueAndTest(sim_out, {{12, 18}, {11, 17}}, body_pos, {});

    // Test: IC-MIB-12
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.2;
        }
    SetHeight(sim_out, 8, 17, NAN, 0.0, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 17, 0.5, -0.4, 0.6, NAN, NAN, NAN, NAN, NAN, NAN);
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][17], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[8][17], 0.9, 1e-5);
    body_pos = {};
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            body_pos.push_back(std::vector<int> {0, ii, jj});
        }
    ResetValueAndTest(sim_out, {{8, 17}, {11, 17}}, body_pos, {});

    // Test: IC-MIB-13
    soil_simulator::rng.seed(1234);
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.2;
        }
    SetHeight(sim_out, 8, 17, NAN, 0.25, 0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 17, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 17, 0.5, -0.5, 0.6, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 17, NAN, 0.2, 0.3, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 13, 17, NAN, 0.05, 0.4, NAN, NAN, 0.6, 0.7, NAN, NAN);
    SetHeight(sim_out, 13, 19, NAN, 0.3, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 14, 20, NAN, 0.0, 0.0, NAN, NAN, 0.2, 0.4, NAN, NAN);
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][17], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][17], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[8][17], 0.15, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][17], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][17], 0.05, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][19], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[14][20], 0.2, 1e-5);
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
    ResetValueAndTest(
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
    SetHeight(sim_out, 8, 17, NAN, 0.25, 0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 17, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 17, 0.8, -0.5, 0.6, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 17, NAN, 0.2, 0.3, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 13, 17, NAN, 0.05, 0.4, NAN, NAN, 0.6, 0.7, NAN, NAN);
    SetHeight(sim_out, 13, 19, NAN, 0.3, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 14, 20, NAN, 0.0, 0.0, NAN, NAN, 0.2, 0.4, NAN, NAN);
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][17], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][17], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[8][17], 0.25, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][17], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][17], 0.05, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][19], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[14][20], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[15][17], 0.2, 1e-5);
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
    ResetValueAndTest(sim_out, terrain_pos, body_pos, {});

    // Test: IC-MIB-15
    soil_simulator::rng.seed(1234);
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.2;
        }
    SetHeight(sim_out, 8, 17, NAN, 0.25, 0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 17, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 17, 0.6, -0.5, 0.6, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 17, NAN, 0.2, 0.3, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 13, 17, NAN, 0.05, 0.4, NAN, NAN, 0.6, 0.7, NAN, NAN);
    SetHeight(sim_out, 13, 19, NAN, 0.3, 0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 14, 20, NAN, 0.0, 0.0, NAN, NAN, 0.2, 0.4, NAN, NAN);
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][17], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][17], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[8][17], 0.25, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][17], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][17], 0.05, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][19], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[14][20], 0.2, 1e-5);
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
    ResetValueAndTest(
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
    ResetValueAndTest(sim_out, {}, body_pos, {});

    // Test: IC-MIB-17
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 11, 17, 0.5, -0.4, 0.6, NAN, NAN, NAN, NAN, NAN, NAN);
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
    ResetValueAndTest(sim_out, {{11, 17}, {10, 18}}, {{0, 11, 17}}, {});

    delete sim_out;
}
