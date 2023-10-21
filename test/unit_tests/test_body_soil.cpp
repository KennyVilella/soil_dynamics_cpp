/*
This file implements unit tests for the functions in body_soil.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <cmath>
#include "gtest/gtest.h"
#include "soil_simulator/body_soil.hpp"
#include "test/unit_tests/utility.hpp"

// To make the function call holds in a single line.
// It greatly improves readability.
using test_soil_simulator::SetHeight;
using test_soil_simulator::CheckHeight;

TEST(UnitTestBodySoil, UpdateBodySoil) {
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
    std::vector<float> pos;
    std::vector<float> ori;
    std::vector<std::vector<int>> body_pos;

    // Test: BS-UBS-1
    pos = {grid.cell_size_xy_, 0.0, 0.0};
    ori = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 11, 10, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 11, 10, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 11, 10, {0.0, 0.0, 0.0}, 0.1);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 11, 10}}, {{0, 11, 10}});

    // Test: BS-UBS-2
    pos = {grid.cell_size_xy_, 0.0, 0.0};
    ori = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 11, 10, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, NAN, NAN, NAN, NAN, 0.1, 0.2);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 11, 10, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 11, 10, {0.0, 0.0, 0.0}, 0.1);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 11, 10}}, {{0, 11, 10}});

    // Test: BS-UBS-3
    pos = {grid.cell_size_xy_, 0.0, 0.0};
    ori = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 11, 10, NAN, NAN, NAN, NAN, NAN, 0.0, 0.1, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 11, 10, NAN, NAN, NAN, 0.1, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 2, 11, 10, {0.0, 0.0, 0.0}, 0.1);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{2, 11, 10}}, {{2, 11, 10}});

    // Test: BS-UBS-4
    pos = {grid.cell_size_xy_, 0.0, 0.0};
    ori = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 11, 10, NAN, NAN, NAN, NAN, NAN, 0.0, 0.1, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, NAN, NAN, NAN, NAN, 0.1, 0.2);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 11, 10, NAN, NAN, NAN, 0.1, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 2, 11, 10, {0.0, 0.0, 0.0}, 0.1);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{2, 11, 10}}, {{2, 11, 10}});

    // Test: BS-UBS-5
    pos = {0.0, 0.0, 0.0};
    ori = {0.707107, 0.0, 0.0, -0.707107};
    SetHeight(sim_out, 10, 11, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 11, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 11, {0.1, 0.0, 0.0}, 0.1);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 11}}, {{0, 10, 11}});

    // Test: BS-UBS-6
    pos = {0.0, 0.0, 0.0};
    ori = {0.92388, 0.0, 0.0, -0.382683};
    SetHeight(sim_out, 11, 11, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 11, 11, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 11, 11, {0.1, 0.0, 0.0}, 0.1);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 11, 11}}, {{0, 11, 11}});

    // Test: BS-UBS-7
    pos = {grid.cell_size_xy_, 0.0, 0.0};
    ori = {0.92388, 0.0, 0.0, -0.382683};
    SetHeight(sim_out, 12, 11, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 12, 11, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 12, 11, {0.1, 0.0, 0.0}, 0.1);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 12, 11}}, {{0, 12, 11}});

    // Test: BS-UBS-8
    pos = {0.0, 0.0, 0.0};
    ori = {0.0, 0.0, 1.0, 0.0};
    SetHeight(sim_out, 11, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->terrain_[9][10], 0.1, 1.e-5);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 10}}, {}, {});

    // Test: BS-UBS-9
    pos = {0.0, 0.0, 0.0};
    ori = {0.707107, 0.0, 0.707107, 0.0};
    SetHeight(sim_out, 10, 10, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 10, NAN, NAN, NAN, 0.1, 0.3, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {1, 12, 10, 0.2, 0.0, 0.0, 0.2});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 10, NAN, 0.1, 0.4, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 10, {0.1, 0.0, 0.0}, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 10, {0.2, 0.0, 0.0}, 0.2);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 10}}, {{0, 10, 10}});

    // Test: BS-UBS-10
    pos = {0.0, 0.0, 0.0};
    ori = {0.707107, 0.0, 0.707107, 0.0};
    SetHeight(sim_out, 10, 10, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 10, NAN, NAN, NAN, NAN, NAN, NAN, NAN, 0.1, 0.3);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 10, 0.2, 0.0, 0.0, 0.2});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 10, NAN, 0.1, 0.4, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 10, {0.1, 0.0, 0.0}, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 10, {0.2, 0.0, 0.0}, 0.2);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 10}}, {{0, 10, 10}});

    // Test: BS-UBS-11
    pos = {0.0, 0.0, 0.0};
    ori = {0.707107, 0.0, 0.707107, 0.0};
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, NAN, NAN, 0.0, 0.1, NAN, NAN);
    SetHeight(sim_out, 11, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 12, 10, NAN, NAN, NAN, NAN, NAN, NAN, NAN, 0.1, 0.3);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 10, 0.2, 0.0, 0.0, 0.2});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.4);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 2, 10, 10, {0.1, 0.0, 0.0}, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 10, {0.2, 0.0, 0.0}, 0.2);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{2, 10, 10}}, {{2, 10, 10}});

    // ---------------------------------------------------------------------- //
    // The tests below are specific to the current implementation and may     //
    // become obsolete when the implementation change                         //
    // ---------------------------------------------------------------------- //

    // Test: BS-UBS-12
    SetHeight(sim_out, 10, 10, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.15, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 10, 0.0, 0.0, 0.0, 0.05});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 10, NAN, 0.0, 0.0, NAN, NAN);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 10}}, {{0, 10, 10}});

    // Test: BS-UBS-13
    SetHeight(sim_out, 10, 10, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.195, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 10, 0.0, 0.0, 0.0, 0.095});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 10, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 10, {0.0, 0.0, 0.0}, 0.1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 10}}, {{0, 10, 10}});

    // Test: BS-UBS-14
    pos = {grid.cell_size_xy_, 0.01, 0.0};
    ori = {1.0, 0.0, 0.0, 0.0};
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 9; jj < 12; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.1;
        }
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 11, 10, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 11, 10, {0.0, 0.0, 0.0}, 0.1);
    // Testing for second direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 11, 10, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 12, 10, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 12, 10, {0.0, 0.0, 0.0}, 0.1);
    // Testing for third direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 12, 10, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 12, 11, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 12, 11, {0.0, 0.0, 0.0}, 0.1);
    // Testing for fouth direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 12, 11, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 11, 11, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 11, 11, {0.0, 0.0, 0.0}, 0.1);
    // Testing for fifth direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 11, 11, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 12, 9, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 12, 9, {0.0, 0.0, 0.0}, 0.1);
    // Testing for sixth direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 12, 9, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 11, 9, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 11, 9, {0.0, 0.0, 0.0}, 0.1);
    // Testing for seventh direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 11, 9, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 11, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 11, {0.0, 0.0, 0.0}, 0.1);
    // Testing for eighth direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 10, 11, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 10, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 10, {0.0, 0.0, 0.0}, 0.1);
    // Testing for ninth direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 10, 10, NAN, 0.2, 0.3, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 9, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 9, {0.0, 0.0, 0.0}, 0.1);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    body_pos = {
        {0, 10, 9}, {0, 10, 10}, {0, 10, 11}, {0, 11, 9}, {0, 11, 10},
        {0, 11, 11}, {0, 12, 9}, {0, 12, 10}, {0, 12, 11}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, {{0, 10, 9}});

    // Test: BS-UBS-15
    pos = {-0.01, -grid.cell_size_xy_, 0.0};
    ori = {1.0, 0.0, 0.0, 0.0};
    for (auto ii = 9; ii < 12; ii++)
        for (auto jj = 8; jj < 11; jj++)
            CheckHeight(sim_out, ii, jj, NAN, 0.0, 0.1, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 9, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 9, {0.0, 0.0, 0.0}, 0.1);
    // Testing for second direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 10, 9, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 8, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 8, {0.0, 0.0, 0.0}, 0.1);
    // Testing for third direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 10, 8, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 9, 8, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 9, 8, {0.0, 0.0, 0.0}, 0.1);
    // Testing for fouth direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 9, 8, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 9, 9, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 9, 9, {0.0, 0.0, 0.0}, 0.1);
    // Testing for fifth direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 9, 9, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 11, 8, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 11, 8, {0.0, 0.0, 0.0}, 0.1);
    // Testing for sixth direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 11, 8, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 11, 9, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 11, 9, {0.0, 0.0, 0.0}, 0.1);
    // Testing for seventh direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 11, 9, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 9, 10, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 9, 10, {0.0, 0.0, 0.0}, 0.1);
    // Testing for eighth direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 9, 10, NAN, 0.2, 0.3, 0.0, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 10, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 10, {0.0, 0.0, 0.0}, 0.1);
    // Testing for ninth direction
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    SetHeight(sim_out, 10, 10, NAN, 0.2, 0.3, 0.1, 0.2, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_[0].ii = 10;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 11, 10, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 11, 10, {0.0, 0.0, 0.0}, 0.1);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    body_pos = {
        {0, 9, 8}, {0, 9, 9}, {0, 9, 10}, {0, 10, 8}, {0, 10, 9},
        {0, 10, 10}, {0, 11, 8}, {0, 11, 9}, {0, 11, 10}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, {{0, 11, 10}});

    // Test: BS-UBS-16
    pos = {grid.cell_size_xy_, 0.01, 0.0};
    ori = {1.0, 0.0, 0.0, 0.0};
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 9; jj < 12; jj++) {
            sim_out->body_[0][ii][jj] = 0.2;
            sim_out->body_[1][ii][jj] = 0.3;
        }
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 9, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 10, 9, NAN, 0.1, 0.2, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 9, {0.0, 0.0, 0.0}, 0.1);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    body_pos = {
        {0, 10, 9}, {0, 10, 10}, {0, 10, 11}, {0, 11, 9}, {0, 11, 10},
        {0, 11, 11}, {0, 12, 9}, {0, 12, 10}, {0, 12, 11}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, {{0, 10, 9}});

    // Test: BS-UBS-17
    pos = {grid.cell_size_xy_, 0.01, 0.0};
    ori = {1.0, 0.0, 0.0, 0.0};
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 9; jj < 12; jj++)
            CheckHeight(sim_out, ii, jj, NAN, 0.2, 0.3, NAN, NAN);
    SetHeight(sim_out, 10, 10, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 11, 9, NAN, -0.2, -0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    CheckHeight(sim_out, 11, 9, NAN, -0.1, 0.0, NAN, NAN);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 11, 9, {0.0, 0.0, 0.0}, 0.1);
    // Resetting values
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};
    body_pos = {
        {0, 10, 9}, {0, 10, 10}, {0, 10, 11}, {0, 11, 9}, {0, 11, 10},
        {0, 11, 11}, {0, 12, 9}, {0, 12, 10}, {0, 12, 11}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, {{0, 11, 9}});

    delete sim_out;
    delete bucket;
}
