/*
This file implements unit tests for the functions in body_soil.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include "gtest/gtest.h"
#include "soil_simulator/body_soil.hpp"
#include "test/unit_tests/utility.hpp"

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
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};

    // -- Testing for a simple lateral translation from (10, 10) to (11, 10) --
    auto pos = std::vector<float> {grid.cell_size_xy_, 0.0, 0.0};
    auto ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_[0][11][10] = 0.0;
    sim_out->body_[1][11][10] = 0.1;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][10], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].x_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 11, 10}}, {{0, 11, 10}});

    // -- Testing for a simple lateral translation (2) --
    pos = std::vector<float> {grid.cell_size_xy_, 0.0, 0.0};
    ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_[0][11][10] = 0.0;
    sim_out->body_[1][11][10] = 0.1;
    sim_out->body_soil_[2][10][10] = 0.1;
    sim_out->body_soil_[3][10][10] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][10], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].x_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 11, 10}}, {{0, 11, 10}});

    // -- Testing for a simple lateral translation (3) --
    pos = std::vector<float> {grid.cell_size_xy_, 0.0, 0.0};
    ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_[2][11][10] = 0.0;
    sim_out->body_[3][11][10] = 0.1;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][10], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].x_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{2, 11, 10}}, {{2, 11, 10}});

    // -- Testing for a simple lateral translation (4) --
    pos = std::vector<float> {grid.cell_size_xy_, 0.0, 0.0};
    ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_[2][11][10] = 0.0;
    sim_out->body_[3][11][10] = 0.1;
    sim_out->body_soil_[2][10][10] = 0.1;
    sim_out->body_soil_[3][10][10] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][11][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][11][10], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].x_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{2, 11, 10}}, {{2, 11, 10}});

    // -- Testing for a simple rotation from (11, 10) to (10, 11) --
    pos = std::vector<float> {0.0, 0.0, 0.0};
    ori = std::vector<float> {0.707107, 0.0, 0.0, -0.707107};
    sim_out->body_[0][10][11] = 0.0;
    sim_out->body_[1][10][11] = 0.1;
    sim_out->body_soil_[0][11][10] = 0.1;
    sim_out->body_soil_[1][11][10] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][11], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][11], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 11);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].x_b, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].y_b, 0., 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 11}}, {{0, 10, 11}});

    // -- Testing for a simple rotation from (11, 10) to (11, 11) --
    pos = std::vector<float> {0.0, 0.0, 0.0};
    ori = std::vector<float> {0.92388, 0.0, 0.0, -0.382683};
    sim_out->body_[0][11][11] = 0.0;
    sim_out->body_[1][11][11] = 0.1;
    sim_out->body_soil_[0][11][10] = 0.1;
    sim_out->body_soil_[1][11][10] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][11], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][11], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 11);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].x_b, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 11, 11}}, {{0, 11, 11}});

    // -- Testing for a rotation + translation from (11, 10) to (12, 11) --
    pos = std::vector<float> {grid.cell_size_xy_, 0.0, 0.0};
    ori = std::vector<float> {0.92388, 0.0, 0.0, -0.382683};
    sim_out->body_[0][12][11] = 0.0;
    sim_out->body_[1][12][11] = 0.1;
    sim_out->body_soil_[0][11][10] = 0.1;
    sim_out->body_soil_[1][11][10] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][11], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][11], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 11);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].x_b, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 12, 11}}, {{0, 12, 11}});

    // -- Testing for a large transformation --
    pos = std::vector<float> {0.0, 0.0, 0.0};
    ori = std::vector<float> {0.0, 0.0, 1.0, 0.0};
    sim_out->body_soil_[0][11][10] = 0.1;
    sim_out->body_soil_[1][11][10] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->terrain_[9][10], 0.1, 1.e-5);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 10}}, {}, {});

    // -- Testing when two body_soil move to the same position (1) --
    pos = std::vector<float> {0.0, 0.0, 0.0};
    ori = std::vector<float> {0.707107, 0.0, 0.707107, 0.0};
    sim_out->body_[0][10][10] = 0.0;
    sim_out->body_[1][10][10] = 0.1;
    sim_out->body_soil_[0][11][10] = 0.1;
    sim_out->body_soil_[1][11][10] = 0.2;
    sim_out->body_soil_[0][12][10] = 0.1;
    sim_out->body_soil_[1][12][10] = 0.3;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 12, 10, 0.2, 0.0, 0.0, 0.2});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][10], 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].x_b, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[1].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[1].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[1].jj, 10);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].x_b, 0.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 10}}, {{0, 10, 10}});

    // -- Testing when two body_soil move to the same position (2) --
    pos = std::vector<float> {0.0, 0.0, 0.0};
    ori = std::vector<float> {0.707107, 0.0, 0.707107, 0.0};
    sim_out->body_[0][10][10] = 0.0;
    sim_out->body_[1][10][10] = 0.1;
    sim_out->body_soil_[0][11][10] = 0.1;
    sim_out->body_soil_[1][11][10] = 0.2;
    sim_out->body_soil_[2][12][10] = 0.1;
    sim_out->body_soil_[3][12][10] = 0.3;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 10, 0.2, 0.0, 0.0, 0.2});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][10], 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].x_b, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[1].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[1].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[1].jj, 10);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].x_b, 0.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 10}}, {{0, 10, 10}});

    // -- Testing when two body_soil move to the same position (3) --
    pos = std::vector<float> {0.0, 0.0, 0.0};
    ori = std::vector<float> {0.707107, 0.0, 0.707107, 0.0};
    sim_out->body_[2][10][10] = 0.0;
    sim_out->body_[3][10][10] = 0.1;
    sim_out->body_soil_[0][11][10] = 0.1;
    sim_out->body_soil_[1][11][10] = 0.2;
    sim_out->body_soil_[2][12][10] = 0.1;
    sim_out->body_soil_[3][12][10] = 0.3;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 10, 0.1, 0.0, 0.0, 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 12, 10, 0.2, 0.0, 0.0, 0.2});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][10], 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].x_b, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[1].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[1].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[1].jj, 10);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].x_b, 0.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{2, 10, 10}}, {{2, 10, 10}});

    // ---------------------------------------------------------------------- //
    // The tests below are specific to the current implementation and may     //
    // become obsolete when the implementation change                         //
    // ---------------------------------------------------------------------- //

    // -- Testing that soil disappears if lower than a cell_size_z --
    sim_out->body_[0][10][10] = 0.0;
    sim_out->body_[1][10][10] = 0.1;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.15;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 10, 0.0, 0.0, 0.0, 0.05});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][10], 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][10], 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 10}}, {{0, 10, 10}});

    // -- Testing that h_soil is properly rounded --
    sim_out->body_[0][10][10] = 0.0;
    sim_out->body_[1][10][10] = 0.1;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.195;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 10, 0.0, 0.0, 0.0, 0.095});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][10], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].x_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].y_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].z_b, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, {{0, 10, 10}}, {{0, 10, 10}});

    // -- Testing that order where directions are checked is correct (1) --
    pos = std::vector<float> {grid.cell_size_xy_, 0.01, 0.0};
    ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 9; jj < 12; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.1;
        }
    sim_out->body_soil_[2][10][10] = 0.1;
    sim_out->body_soil_[3][10][10] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][10], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    // Testing for second direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][11][10] = 0.0;
    sim_out->body_soil_[1][11][10] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][11][10] = 0.2;
    sim_out->body_[1][11][10] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][10], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    // Testing for third direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][12][10] = 0.0;
    sim_out->body_soil_[1][12][10] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][12][10] = 0.2;
    sim_out->body_[1][12][10] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][11], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][11], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 11);
    // Testing for fouth direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][12][11] = 0.0;
    sim_out->body_soil_[1][12][11] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][12][11] = 0.2;
    sim_out->body_[1][12][11] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][11], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][11], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 11);
    // Testing for fifth direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][11][11] = 0.0;
    sim_out->body_soil_[1][11][11] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][11][11] = 0.2;
    sim_out->body_[1][11][11] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][12][9], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][12][9], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 9);
    // Testing for sixth direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][12][9] = 0.0;
    sim_out->body_soil_[1][12][9] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][12][9] = 0.2;
    sim_out->body_[1][12][9] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][9], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][9], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 9);
    // Testing for seventh direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][11][9] = 0.0;
    sim_out->body_soil_[1][11][9] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][11][9] = 0.2;
    sim_out->body_[1][11][9] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][11], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][11], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 11);
    // Testing for eighth direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][10][11] = 0.0;
    sim_out->body_soil_[1][10][11] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][10][11] = 0.2;
    sim_out->body_[1][10][11] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][10], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    // Testing for ninth direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][10][10] = 0.0;
    sim_out->body_soil_[1][10][10] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][10][10] = 0.2;
    sim_out->body_[1][10][10] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][9], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][9], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 9);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    std::vector<std::vector<int>> body_pos = {
        {0, 10, 9}, {0, 10, 10}, {0, 10, 11}, {0, 11, 9}, {0, 11, 10},
        {0, 11, 11}, {0, 12, 9}, {0, 12, 10}, {0, 12, 11}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, {{0, 10, 9}});

    // -- Testing that order where directions are checked is correct (2) --
    pos = std::vector<float> {-0.01, -grid.cell_size_xy_, 0.0};
    ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    for (auto ii = 9; ii < 12; ii++)
        for (auto jj = 8; jj < 11; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.1;
        }
    sim_out->body_soil_[2][10][10] = 0.1;
    sim_out->body_soil_[3][10][10] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][9], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][9], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 9);
    // Testing for second direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][10][9] = 0.0;
    sim_out->body_soil_[1][10][9] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][10][9] = 0.2;
    sim_out->body_[1][10][9] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][8], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][8], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 8);
    // Testing for third direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][10][8] = 0.0;
    sim_out->body_soil_[1][10][8] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][10][8] = 0.2;
    sim_out->body_[1][10][8] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][9][8], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][9][8], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 9);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 8);
    // Testing for fouth direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][9][8] = 0.0;
    sim_out->body_soil_[1][9][8] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][9][8] = 0.2;
    sim_out->body_[1][9][8] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][9][9], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][9][9], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 9);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 9);
    // Testing for fifth direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][9][9] = 0.0;
    sim_out->body_soil_[1][9][9] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][9][9] = 0.2;
    sim_out->body_[1][9][9] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][8], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][8], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 8);
    // Testing for sixth direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][11][8] = 0.0;
    sim_out->body_soil_[1][11][8] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][11][8] = 0.2;
    sim_out->body_[1][11][8] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][9], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][9], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 9);
    // Testing for seventh direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][11][9] = 0.0;
    sim_out->body_soil_[1][11][9] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][11][9] = 0.2;
    sim_out->body_[1][11][9] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][9][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][9][10], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 9);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    // Testing for eighth direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][9][10] = 0.0;
    sim_out->body_soil_[1][9][10] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][9][10] = 0.2;
    sim_out->body_[1][9][10] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][10], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    // Testing for ninth direction
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    sim_out->body_soil_[0][10][10] = 0.0;
    sim_out->body_soil_[1][10][10] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.1;
    sim_out->body_soil_[1][10][10] = 0.2;
    sim_out->body_soil_pos_[0].ii = 10;
    sim_out->body_[0][10][10] = 0.2;
    sim_out->body_[1][10][10] = 0.3;
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][10], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][10], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 10);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    body_pos = {
        {0, 9, 8}, {0, 9, 9}, {0, 9, 10}, {0, 10, 8}, {0, 10, 9},
        {0, 10, 10}, {0, 11, 8}, {0, 11, 9}, {0, 11, 10}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, {{0, 11, 10}});

    // -- Testing that soil is moved if close enough (1) --
    pos = std::vector<float> {grid.cell_size_xy_, 0.01, 0.0};
    ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 9; jj < 12; jj++) {
            sim_out->body_[0][ii][jj] = 0.2;
            sim_out->body_[1][ii][jj] = 0.3;
        }
    sim_out->body_soil_[2][10][10] = 0.1;
    sim_out->body_soil_[3][10][10] = 0.2;
    sim_out->body_[0][10][9] = 0.0;
    sim_out->body_[1][10][9] = 0.1;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][9], 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][9], 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 9);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    body_pos = {
        {0, 10, 9}, {0, 10, 10}, {0, 10, 11}, {0, 11, 9}, {0, 11, 10},
        {0, 11, 11}, {0, 12, 9}, {0, 12, 10}, {0, 12, 11}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, {{0, 10, 9}});

    // -- Testing that soil is moved if close enough (2) --
    pos = std::vector<float> {grid.cell_size_xy_, 0.01, 0.0};
    ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 9; jj < 12; jj++) {
            sim_out->body_[0][ii][jj] = 0.2;
            sim_out->body_[1][ii][jj] = 0.3;
        }
    sim_out->body_soil_[2][10][10] = 0.1;
    sim_out->body_soil_[3][10][10] = 0.2;
    sim_out->body_[0][11][9] = -0.2;
    sim_out->body_[1][11][9] = -0.1;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 10, 0.0, 0.0, 0.0, 0.1});
    soil_simulator::UpdateBodySoil(
        sim_out, pos, ori, grid, bucket, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][9], -0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][9], 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[0].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[0].jj, 9);
    // Resetting values
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    body_pos = {
        {0, 10, 9}, {0, 10, 10}, {0, 10, 11}, {0, 11, 9}, {0, 11, 10},
        {0, 11, 11}, {0, 12, 9}, {0, 12, 10}, {0, 12, 11}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, {}, body_pos, {{0, 11, 9}});

    delete sim_out;
    delete bucket;
}
