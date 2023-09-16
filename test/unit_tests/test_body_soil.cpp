/*
This file implements unit tests for the functions in body_soil.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include "gtest/gtest.h"
#include "soil_simulator/body_soil.hpp"

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
    sim_out->body_[0][11][10] = 0.0;
    sim_out->body_[1][11][10] = 0.0;
    sim_out->body_soil_[0][11][10] = 0.0;
    sim_out->body_soil_[1][11][10] = 0.0;
    // Checking that everything is resetted
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1.e-5);
            }
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1.e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

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
    sim_out->body_[0][11][10] = 0.0;
    sim_out->body_[1][11][10] = 0.0;
    sim_out->body_soil_[0][11][10] = 0.0;
    sim_out->body_soil_[1][11][10] = 0.0;
    // Checking that everything is resetted
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1.e-5);
            }
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1.e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

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
    sim_out->body_[2][11][10] = 0.0;
    sim_out->body_[3][11][10] = 0.0;
    sim_out->body_soil_[2][11][10] = 0.0;
    sim_out->body_soil_[3][11][10] = 0.0;
    // Checking that everything is resetted
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1.e-5);
            }
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1.e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

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
    sim_out->body_[2][11][10] = 0.0;
    sim_out->body_[3][11][10] = 0.0;
    sim_out->body_soil_[2][11][10] = 0.0;
    sim_out->body_soil_[3][11][10] = 0.0;
    // Checking that everything is resetted
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1.e-5);
            }
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1.e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

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
    sim_out->body_[0][10][11] = 0.0;
    sim_out->body_[1][10][11] = 0.0;
    sim_out->body_soil_[0][10][11] = 0.0;
    sim_out->body_soil_[1][10][11] = 0.0;
    // Checking that everything is resetted
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1.e-5);
            }
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1.e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

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
    sim_out->body_[0][11][11] = 0.0;
    sim_out->body_[1][11][11] = 0.0;
    sim_out->body_soil_[0][11][11] = 0.0;
    sim_out->body_soil_[1][11][11] = 0.0;
    // Checking that everything is resetted
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1.e-5);
            }
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1.e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

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
    sim_out->body_[0][12][11] = 0.0;
    sim_out->body_[1][12][11] = 0.0;
    sim_out->body_soil_[0][12][11] = 0.0;
    sim_out->body_soil_[1][12][11] = 0.0;
    // Checking that everything is resetted
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1.e-5);
            }
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1.e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

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
    sim_out->terrain_[9][10] = 0.0;
    // Checking that everything is resetted
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1.e-5);
            }
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1.e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

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
    sim_out->body_[0][10][10] = 0.0;
    sim_out->body_[1][10][10] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.0;
    sim_out->body_soil_[1][10][10] = 0.0;
    // Checking that everything is resetted
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1.e-5);
            }
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1.e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

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
    sim_out->body_[0][10][10] = 0.0;
    sim_out->body_[1][10][10] = 0.0;
    sim_out->body_soil_[0][10][10] = 0.0;
    sim_out->body_soil_[1][10][10] = 0.0;
    // Checking that everything is resetted
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1.e-5);
            }
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1.e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

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
    sim_out->body_[2][10][10] = 0.0;
    sim_out->body_[3][10][10] = 0.0;
    sim_out->body_soil_[2][10][10] = 0.0;
    sim_out->body_soil_[3][10][10] = 0.0;
    // Checking that everything is resetted
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1.e-5);
            }
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1.e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    delete sim_out;
    delete bucket;
}
