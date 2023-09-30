/*
This file implements unit tests for the functions in intersecting_cells.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <random>
#include "gtest/gtest.h"
#include "soil_simulator/intersecting_cells.hpp"
#include "soil_simulator/utils.hpp"

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
    sim_out->body_[0][10][15] = 0.3;
    sim_out->body_[1][10][15] = 0.7;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.7;
    sim_out->body_soil_[1][10][15] = 0.9;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.9;
    auto pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.7, grid, bucket);
    auto pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);


    // -- Testing when soil is avalanching on the terrain --
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    auto [ind, ii, jj, h_soil, wall_presence] = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    sim_out->terrain_[5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is avalanching below the first bucket layer --
    sim_out->body_[0][5][7] = 0.1;
    sim_out->body_[1][5][7] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    sim_out->terrain_[5][7] = 0.0;
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when the first bucket layer is blocking the movement --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.3;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, true);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.0, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there is a lot of soil on first bucket layer --
    // -- Soil is avalanching on first bucket layer                 --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.4;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    auto posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 5, 7, posA[0], posA[1], posA[2], 0.3});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 1.0, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is fully avalanching on first bucket layer --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.2, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.8, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is fully avalanching on first bucket soil layer --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 5, 7, posA[0], posA[1], posA[2], 0.1});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.8, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is avalanching below the second bucket layer --
    sim_out->body_[2][5][7] = 0.3;
    sim_out->body_[3][5][7] = 0.6;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->terrain_[5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when the second bucket layer is blocking the movement --
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.6;
    sim_out->body_soil_[2][5][7] = 0.6;
    sim_out->body_soil_[3][5][7] = 0.7;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
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
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there is a lot of soil on second bucket layer --
    // -- but soil is still avalanching on it                        --
    sim_out->body_[2][5][7] = -0.2;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.3;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 5, 7, posA[0], posA[1], posA[2], 0.3});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.9, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is fully avalanching on second bucket layer --
    sim_out->body_[2][5][7] = -0.2;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is fully avalanching on second bucket soil layer --
    sim_out->body_[2][5][7] = -0.2;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 5, 7, posA[0], posA[1], posA[2], 0.2});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.8, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil fully filling the space (1) --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_[2][5][7] = 0.2;
    sim_out->body_[3][5][7] = 0.4;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
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
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil fully filling the space (2) --
    sim_out->body_[0][5][7] = 0.6;
    sim_out->body_[1][5][7] = 0.7;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.1;
    sim_out->body_soil_[2][5][7] = 0.1;
    sim_out->body_soil_[3][5][7] = 0.6;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
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
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil is fully avalanching --
    // -- on bucket (1)                                                --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.2;
    sim_out->body_[2][5][7] = 0.8;
    sim_out->body_[3][5][7] = 0.9;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.2, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.8, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil is fully avalanching --
    // -- on bucket (2)                                                --
    sim_out->body_[0][5][7] = 0.8;
    sim_out->body_[1][5][7] = 0.9;
    sim_out->body_[2][5][7] = -0.1;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.6, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil is fully avalanching --
    // -- on bucket soil (1)                                           --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_[2][5][7] = 0.9;
    sim_out->body_[3][5][7] = 1.0;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 5, 7, posA[0], posA[1], posA[2], 0.1});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.8, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil is fully avalanching --
    // -- on bucket soil (2)                                           --
    sim_out->body_[0][5][7] = 0.8;
    sim_out->body_[1][5][7] = 0.9;
    sim_out->body_[2][5][7] = -0.1;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 5, 7, posA[0], posA[1], posA[2], 0.2});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.8, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil is fully avalanching --
    // -- on bucket soil (3)                                           --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.2;
    sim_out->body_[2][5][7] = 0.9;
    sim_out->body_[3][5][7] = 1.;
    sim_out->body_soil_[0][5][7] = 0.2;
    sim_out->body_soil_[1][5][7] = 0.3;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 5, 7, posA[0], posA[1], posA[2], 0.1});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.9, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil is fully avalanching --
    // -- on bucket soil (4)                                           --
    sim_out->body_[0][5][7] = 0.9;
    sim_out->body_[1][5][7] = 1.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.1;
    sim_out->body_soil_[2][5][7] = 0.1;
    sim_out->body_soil_[3][5][7] = 0.6;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 5, 7, posA[0], posA[1], posA[2], 0.5});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.1, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.7, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil is partially avalanching --
    // -- on bucket (1)                                                    --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_[2][5][7] = 0.4;
    sim_out->body_[3][5][7] = 0.9;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
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
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil is partially avalanching --
    // -- on bucket (2)                                                    --
    sim_out->body_[0][5][7] = 0.3;
    sim_out->body_[1][5][7] = 0.9;
    sim_out->body_[2][5][7] = -0.1;
    sim_out->body_[3][5][7] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
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
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil is partially avalanching --
    // -- on bucket soil (1)                                               --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_[2][5][7] = 0.4;
    sim_out->body_[3][5][7] = 0.5;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
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
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil is partially avalanching --
    // -- on bucket soil (2)                                               --
    sim_out->body_[0][5][7] = 0.6;
    sim_out->body_[1][5][7] = 0.9;
    sim_out->body_[2][5][7] = -0.1;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.2;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
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
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil is partially avalanching --
    // -- on bucket soil (3)                                               --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.2;
    sim_out->body_[2][5][7] = 0.4;
    sim_out->body_[3][5][7] = 0.5;
    sim_out->body_soil_[0][5][7] = 0.2;
    sim_out->body_soil_[1][5][7] = 0.3;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 5, 7, posA[0], posA[1], posA[2], 0.1});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.3, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.2, 1e-5);
    EXPECT_EQ(ind, 0);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.4, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when two bucket layers and soil is partially avalanching --
    // -- on bucket soil (4)                                               --
    sim_out->body_[0][5][7] = 0.7;
    sim_out->body_[1][5][7] = 0.8;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.1;
    sim_out->body_soil_[2][5][7] = 0.1;
    sim_out->body_soil_[3][5][7] = 0.6;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(5, 7, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 5, 7, posA[0], posA[1], posA[2], 0.5});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.3, false, grid, bucket, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.2, 1e-5);
    EXPECT_EQ(ind, 2);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.7, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 7);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

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

    // -- Testing when soil is avalanching on the terrain (1) --
    // -- First bucket layer at bottom                        --
    soil_simulator::rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    auto pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    auto pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
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
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is avalanching on the terrain (2) --
    // -- Second bucket layer at bottom                       --
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
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is avalanching on the terrain (3) --
    // -- Bucket undergroumd                                  --
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
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is avalanching below the first bucket layer (1) --
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
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is avalanching below the first bucket layer (2) --
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
    sim_out->body_[1][11][15] = 0.5;
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
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is avalanching below the first bucket layer (3) --
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
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is avalanching below the first bucket layer --
    // -- despite the lack of available space                           --
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
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is avalanching below the second bucket layer (1) --
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
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is avalanching below the second bucket layer (2) --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is avalanching below the second bucket layer (3) --
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
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is avalanching below the second bucket layer --
    // -- despite the lack of available space                            --
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
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is fully avalanching on first bucket layer (1) --
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
    auto posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][11][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][11][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is fully avalanching on first bucket layer (2) --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is fully avalanching on second bucket layer (1) --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is fully avalanching on second bucket layer (2) --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is fully avalanching on first bucket soil --
    // -- layer (1)                                                   --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is fully avalanching on first bucket soil --
    // -- layer (2)                                                   --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is fully avalanching on second bucket soil --
    // -- layer (1)                                                    --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when soil is fully avalanching on the second bucket soil --
    // -- layer (2)                                                        --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the first bucket layer (1)                  --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the first bucket layer (2)                  --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the second bucket layer (1)                 --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the second bucket layer (2)                 --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the first bucket soil layer (1)             --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the first bucket soil layer (2)             --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the second bucket soil layer (1)            --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the second bucket soil layer (2)            --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is partially    --
    // -- avalanching on the first bucket layer and then on the terrain (1) --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is partially    --
    // -- avalanching on the first bucket layer and then on the terrain (2) --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is partially     --
    // -- avalanching on the second bucket layer and then on the terrain (1) --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is partially     --
    // -- avalanching on the second bucket layer and then on the terrain (2) --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[2].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[2].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is partially --
    // -- avalanching on first bucket soil layer and then on terrain (1) --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is partially --
    // -- avalanching on first bucket soil layer and then on terrain (2) --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is partially  --
    // -- avalanching on second bucket soil layer and then on terrain (1) --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is partially  --
    // -- avalanching on second bucket soil layer and then on terrain (2) --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the first bucket soil layer, then the soil is       --
    // -- avalanching on the terrain below the first bucket layer            --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[5].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the second bucket soil layer, then the soil is      --
    // -- avalanching on the terrain below the first bucket layer            --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[5].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil on the first  --
    // -- bucket layer is blocking the movement, then the soil is avalanching --
    // -- on the terrain below the second bucket layer                        --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil on the second --
    // -- bucket layer is blocking the movement, then the soil is avalanching --
    // -- on the terrain below the second bucket layer                        --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially  --
    // -- avalanching on the first bucket layer, then the soil is avalanching --
    // -- on the terrain below the first bucket layer                         --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[5].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the second bucket layer, then the soil is           --
    // -- avalanching on the terrain below the first bucket layer            --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[5].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially  --
    // -- avalanching on the first bucket layer, then the soil is avalanching --
    // -- on the terrain below the second bucket layer                        --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[5].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the second bucket layer, then the soil is           --
    // -- avalanching on the terrain below the second bucket layer           --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[5].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there is a lot of soil on the first bucket layer but --
    // -- soil is still avalanching on it                                   --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there is a lot of soil on the second bucket layer but --
    // -- soil is still avalanching on it                                    --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->terrain_[12][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil on the first  --
    // -- bucket layer is blocking the movement, then the soil is avalanching --
    // -- on the first bucket layer                                           --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[5].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil on the second --
    // -- bucket layer is blocking the movement, then the soil is avalanching --
    // -- on the first bucket layer                                           --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[4].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil on the first  --
    // -- bucket layer is blocking the movement, then the soil is avalanching --
    // -- on the second bucket layer                                          --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[5].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil on the second --
    // -- bucket layer is blocking the movement, then the soil is avalanching --
    // -- on the second bucket layer                                          --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[4].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially  --
    // -- avalanching on the first bucket layer, then the soil is avalanching --
    // -- on the first bucket layer                                           --
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
    auto posB = soil_simulator::CalcBucketFramePos(12, 15, 0.3, grid, bucket);
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[5].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[6].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[6].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[6].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].x_b, posB[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].y_b, posB[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].z_b, posB[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the second bucket layer, then the soil is           --
    // -- avalanching on the first bucket layer                              --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[4].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[5].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].x_b, posB[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].y_b, posB[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].z_b, posB[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially  --
    // -- avalanching on the first bucket layer, then the soil is avalanching --
    // -- on the second bucket layer                                          --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[4].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[5].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].x_b, posB[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].y_b, posB[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].z_b, posB[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 6);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the second bucket layer, then the soil is           --
    // -- avalanching on the second bucket layer                             --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[5].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[5].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[5].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[6].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[6].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[6].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].x_b, posB[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].y_b, posB[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].z_b, posB[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 7);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially  --
    // -- avalanching on the first bucket layer, then the first bucket layer  --
    // -- is blocking the movement. New direction, two bucket layers and the  --
    // -- soil is partially avalanching on the second bucket layer, then the  --
    // -- first bucket layer is blocking the movement. New direction, two     --
    // -- bucket layers and the soil is partially avalanching on the first    --
    // -- bucket layer, then the second bucket layer is blocking the          --
    // -- movement. New direction two bucket layers and the soil is partially --
    // -- avalanching on the second bucket layer, then the second bucket      --
    // -- layer is blocking the movement. New direction, two bucket layers    --
    // -- and  the soil on the first bucket layer is blocking the movement,   --
    // -- then two bucket layers and the soil is fully avalanching on the     --
    // -- second bucket layer                                                 --
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
    auto posC = soil_simulator::CalcBucketFramePos(11, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 14, posC[0], posC[1], posC[2], 0.1});
    auto posD = soil_simulator::CalcBucketFramePos(9, 14, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 9, 14, posD[0], posD[1], posD[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(11, 16, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 11, 16, pos0[0], pos0[1], pos0[2], 0.7});
    auto posE = soil_simulator::CalcBucketFramePos(12, 17, 0.3, grid, bucket);
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
    EXPECT_EQ(sim_out->body_soil_pos_[7].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[7].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[7].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[8].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[8].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[8].jj, 16);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].x_b, posB[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].y_b, posB[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].z_b, posB[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[9].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[9].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[9].jj, 14);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].x_b, posC[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].y_b, posC[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].z_b, posC[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[10].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[10].ii, 9);
    EXPECT_EQ(sim_out->body_soil_pos_[10].jj, 14);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].x_b, posD[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].y_b, posD[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].z_b, posD[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[11].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[11].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[11].jj, 17);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].x_b, posE[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].y_b, posE[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].z_b, posE[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 12);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_[0][10][16] = 0.0;
    sim_out->body_[1][10][16] = 0.0;
    sim_out->body_[2][10][16] = 0.0;
    sim_out->body_[3][10][16] = 0.0;
    sim_out->body_[2][10][17] = 0.0;
    sim_out->body_[3][10][17] = 0.0;
    sim_out->body_[0][11][14] = 0.0;
    sim_out->body_[1][11][14] = 0.0;
    sim_out->body_[2][11][14] = 0.0;
    sim_out->body_[3][11][14] = 0.0;
    sim_out->body_[2][12][13] = 0.0;
    sim_out->body_[3][12][13] = 0.0;
    sim_out->body_[0][9][14] = 0.0;
    sim_out->body_[1][9][14] = 0.0;
    sim_out->body_[2][9][14] = 0.0;
    sim_out->body_[3][9][14] = 0.0;
    sim_out->body_[2][8][13] = 0.0;
    sim_out->body_[3][8][13] = 0.0;
    sim_out->body_[0][11][16] = 0.0;
    sim_out->body_[1][11][16] = 0.0;
    sim_out->body_[2][11][16] = 0.0;
    sim_out->body_[3][11][16] = 0.0;
    sim_out->body_[0][12][17] = 0.0;
    sim_out->body_[1][12][17] = 0.0;
    sim_out->body_[2][12][17] = 0.0;
    sim_out->body_[3][12][17] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][10][16] = 0.0;
    sim_out->body_soil_[3][10][16] = 0.0;
    sim_out->body_soil_[0][11][14] = 0.0;
    sim_out->body_soil_[1][11][14] = 0.0;
    sim_out->body_soil_[2][9][14] = 0.0;
    sim_out->body_soil_[3][9][14] = 0.0;
    sim_out->body_soil_[0][11][16] = 0.0;
    sim_out->body_soil_[1][11][16] = 0.0;
    sim_out->body_soil_[2][12][17] = 0.0;
    sim_out->body_soil_[3][12][17] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil on the first  --
    // -- bucket layer is blocking the movement, then the soil is fully       --
    // -- avalanching on the second bucket soil layer                         --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[4].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].h_soil, 1.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil on the second --
    // -- bucket layer is blocking the movement, then the soil is fully       --
    // -- avalanching on the second bucket soil layer                         --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[4].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].h_soil, 1.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil on the first  --
    // -- bucket layer is blocking the movement, then the first bucket layer  --
    // -- is blocking the movement. New direction, two bucket layers and the  --
    // -- soil on the second bucket layer is blocking the movement, then the  --
    // -- first bucket layer is blocking the movement. New direction, two     --
    // -- bucket layers and the soil on the first bucket layer is blocking    --
    // -- the movement, then the second bucket layer is blocking the movement --
    // --  New direction, two bucket layers and the soil on the second bucket --
    // -- layer is blocking the movement, then the second bucket layer is     --
    // -- blocking the movement. New direction, two bucket layers and the     --
    // -- soil on the first bucket layer is blocking the movement, then two   --
    // -- bucket layers and the soil is fully avalanching on the first bucket --
    // -- layer                                                               --
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
    EXPECT_EQ(sim_out->body_soil_pos_[7].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[7].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[7].jj, 17);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 8);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_[0][10][16] = 0.0;
    sim_out->body_[1][10][16] = 0.0;
    sim_out->body_[2][10][16] = 0.0;
    sim_out->body_[3][10][16] = 0.0;
    sim_out->body_[2][10][17] = 0.0;
    sim_out->body_[3][10][17] = 0.0;
    sim_out->body_[0][11][14] = 0.0;
    sim_out->body_[1][11][14] = 0.0;
    sim_out->body_[2][11][14] = 0.0;
    sim_out->body_[3][11][14] = 0.0;
    sim_out->body_[2][12][13] = 0.0;
    sim_out->body_[3][12][13] = 0.0;
    sim_out->body_[0][9][14] = 0.0;
    sim_out->body_[1][9][14] = 0.0;
    sim_out->body_[2][9][14] = 0.0;
    sim_out->body_[3][9][14] = 0.0;
    sim_out->body_[2][8][13] = 0.0;
    sim_out->body_[3][8][13] = 0.0;
    sim_out->body_[0][11][16] = 0.0;
    sim_out->body_[1][11][16] = 0.0;
    sim_out->body_[2][11][16] = 0.0;
    sim_out->body_[3][11][16] = 0.0;
    sim_out->body_[0][12][17] = 0.0;
    sim_out->body_[1][12][17] = 0.0;
    sim_out->body_[2][12][17] = 0.0;
    sim_out->body_[3][12][17] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][10][16] = 0.0;
    sim_out->body_soil_[3][10][16] = 0.0;
    sim_out->body_soil_[0][11][14] = 0.0;
    sim_out->body_soil_[1][11][14] = 0.0;
    sim_out->body_soil_[2][9][14] = 0.0;
    sim_out->body_soil_[3][9][14] = 0.0;
    sim_out->body_soil_[0][11][16] = 0.0;
    sim_out->body_soil_[1][11][16] = 0.0;
    sim_out->body_soil_[0][12][17] = 0.0;
    sim_out->body_soil_[1][12][17] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil on the second --
    // -- bucket layer is blocking the movement, then two bucket layers and   --
    // -- the soil is fully avalanching on the second bucket layer            --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially  --
    // -- avalanching on the first bucket layer, then two bucket layers and   --
    // -- soil is partially avalanching on the first bucket layer that is     --
    // -- higher, then two bucket layers and soil is partially avalanching    --
    // -- on the first bucket layer that is higher, then two bucket layers    --
    // -- and soil is partially avalanching on the first bucket layer, then   --
    // -- two bucket layers and soil is partially avalanching on the first    --
    // -- bucket layer, then two bucket layers and soil is partially          --
    // -- avalanching on the second bucket layer that is higher, then two     --
    // -- bucket layers and soil is partially avalanching on the second       --
    // -- bucket layer that is higher, then two bucket layers and soil is     --
    // -- partially avalanching on the first bucket layer that is higher,     --
    // -- then two bucket layers and soil is partially avalanching on the     --
    // -- second bucket layer, then two bucket layers and the soil is fully   --
    // -- avalanching on the first bucket layer                               --
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
    auto posG = soil_simulator::CalcBucketFramePos(17, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 17, 15, posG[0], posG[1], posG[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(11, 15, 0.2, grid, bucket);
    posD = soil_simulator::CalcBucketFramePos(14, 15, 0.2, grid, bucket);
    auto posF = soil_simulator::CalcBucketFramePos(16, 15, 0.5, grid, bucket);
    auto posH = soil_simulator::CalcBucketFramePos(18, 15, 0.8, grid, bucket);
    auto posI = soil_simulator::CalcBucketFramePos(19, 15, 0.4, grid, bucket);
    auto posJ = soil_simulator::CalcBucketFramePos(20, 15, 0.1, grid, bucket);
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
    EXPECT_EQ(sim_out->body_soil_pos_[6].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[6].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[6].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[7].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[7].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[7].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].x_b, posB[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].y_b, posB[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].z_b, posB[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[8].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[8].ii, 13);
    EXPECT_EQ(sim_out->body_soil_pos_[8].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].x_b, posC[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].y_b, posC[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].z_b, posC[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[9].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[9].ii, 14);
    EXPECT_EQ(sim_out->body_soil_pos_[9].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].x_b, posD[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].y_b, posD[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].z_b, posD[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[10].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[10].ii, 15);
    EXPECT_EQ(sim_out->body_soil_pos_[10].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].x_b, posE[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].y_b, posE[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].z_b, posE[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[11].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[11].ii, 16);
    EXPECT_EQ(sim_out->body_soil_pos_[11].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].x_b, posF[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].y_b, posF[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].z_b, posF[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[12].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[12].ii, 17);
    EXPECT_EQ(sim_out->body_soil_pos_[12].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[12].x_b, posG[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[12].y_b, posG[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[12].z_b, posG[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[12].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[13].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[13].ii, 18);
    EXPECT_EQ(sim_out->body_soil_pos_[13].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[13].x_b, posH[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[13].y_b, posH[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[13].z_b, posH[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[13].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[14].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[14].ii, 19);
    EXPECT_EQ(sim_out->body_soil_pos_[14].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[14].x_b, posI[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[14].y_b, posI[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[14].z_b, posI[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[14].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[15].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[15].ii, 20);
    EXPECT_EQ(sim_out->body_soil_pos_[15].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[15].x_b, posJ[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[15].y_b, posJ[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[15].z_b, posJ[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[15].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 16);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_[0][13][15] = 0.0;
    sim_out->body_[1][13][15] = 0.0;
    sim_out->body_[2][13][15] = 0.0;
    sim_out->body_[3][13][15] = 0.0;
    sim_out->body_[0][14][15] = 0.0;
    sim_out->body_[1][14][15] = 0.0;
    sim_out->body_[2][14][15] = 0.0;
    sim_out->body_[3][14][15] = 0.0;
    sim_out->body_[0][15][15] = 0.0;
    sim_out->body_[1][15][15] = 0.0;
    sim_out->body_[2][15][15] = 0.0;
    sim_out->body_[3][15][15] = 0.0;
    sim_out->body_[0][16][15] = 0.0;
    sim_out->body_[1][16][15] = 0.0;
    sim_out->body_[2][16][15] = 0.0;
    sim_out->body_[3][16][15] = 0.0;
    sim_out->body_[0][17][15] = 0.0;
    sim_out->body_[1][17][15] = 0.0;
    sim_out->body_[2][17][15] = 0.0;
    sim_out->body_[3][17][15] = 0.0;
    sim_out->body_[0][18][15] = 0.0;
    sim_out->body_[1][18][15] = 0.0;
    sim_out->body_[2][18][15] = 0.0;
    sim_out->body_[3][18][15] = 0.0;
    sim_out->body_[0][19][15] = 0.0;
    sim_out->body_[1][19][15] = 0.0;
    sim_out->body_[2][19][15] = 0.0;
    sim_out->body_[3][19][15] = 0.0;
    sim_out->body_[0][20][15] = 0.0;
    sim_out->body_[1][20][15] = 0.0;
    sim_out->body_[2][20][15] = 0.0;
    sim_out->body_[3][20][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.0;
    sim_out->body_soil_[0][13][15] = 0.0;
    sim_out->body_soil_[1][13][15] = 0.0;
    sim_out->body_soil_[0][14][15] = 0.0;
    sim_out->body_soil_[1][14][15] = 0.0;
    sim_out->body_soil_[0][15][15] = 0.0;
    sim_out->body_soil_[1][15][15] = 0.0;
    sim_out->body_soil_[2][16][15] = 0.0;
    sim_out->body_soil_[3][16][15] = 0.0;
    sim_out->body_soil_[2][17][15] = 0.0;
    sim_out->body_soil_[3][17][15] = 0.0;
    sim_out->body_soil_[0][18][15] = 0.0;
    sim_out->body_soil_[1][18][15] = 0.0;
    sim_out->body_soil_[2][19][15] = 0.0;
    sim_out->body_soil_[3][19][15] = 0.0;
    sim_out->body_soil_[0][20][15] = 0.0;
    sim_out->body_soil_[1][20][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and soil is partially      --
    // -- avalanching on the second bucket layer, then two bucket layers and  --
    // -- soil is partially avalanching on the first bucket layer, then two   --
    // -- bucket layers and soil is partially avalanching on the second       --
    // -- bucket layer, then two bucket layers and soil is partially          --
    // -- avalanching on the second bucket layer, then two bucket layers and  --
    // -- soil is partially avalanching on the second bucket layer that is    --
    // -- higher, then two bucket layers and soil is partially avalanching    --
    // -- on the first bucket layer, then two bucket layers and soil is       --
    // -- partially avalanching on the first bucket layer that is higher,     --
    // -- then two bucket layers and soil is partially avalanching on the     --
    // -- second bucket layer, then two bucket layers and soil is partially   --
    // -- avalanching on the first bucket layer that is higher, then two      --
    // -- bucket layers and soil is partially avalanching on the second       --
    // -- bucket layer that is higher, then two bucket layers and the soil is --
    // -- fully avalanching on the first bucket layer                         --
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
    auto posK = soil_simulator::CalcBucketFramePos(20, 15, 0.1, grid, bucket);
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
    EXPECT_EQ(sim_out->body_soil_pos_[9].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[9].ii, 10);
    EXPECT_EQ(sim_out->body_soil_pos_[9].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[10].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[10].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[10].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].x_b, posB[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].y_b, posB[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].z_b, posB[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[11].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[11].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[11].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].x_b, posC[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].y_b, posC[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].z_b, posC[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[12].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[12].ii, 13);
    EXPECT_EQ(sim_out->body_soil_pos_[12].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[12].x_b, posD[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[12].y_b, posD[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[12].z_b, posD[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[12].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[13].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[13].ii, 14);
    EXPECT_EQ(sim_out->body_soil_pos_[13].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[13].x_b, posE[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[13].y_b, posE[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[13].z_b, posE[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[13].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[14].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[14].ii, 15);
    EXPECT_EQ(sim_out->body_soil_pos_[14].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[14].x_b, posF[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[14].y_b, posF[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[14].z_b, posF[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[14].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[15].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[15].ii, 16);
    EXPECT_EQ(sim_out->body_soil_pos_[15].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[15].x_b, posG[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[15].y_b, posG[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[15].z_b, posG[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[15].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[16].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[16].ii, 17);
    EXPECT_EQ(sim_out->body_soil_pos_[16].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[16].x_b, posH[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[16].y_b, posH[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[16].z_b, posH[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[16].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[17].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[17].ii, 18);
    EXPECT_EQ(sim_out->body_soil_pos_[17].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[17].x_b, posI[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[17].y_b, posI[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[17].z_b, posI[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[17].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[18].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[18].ii, 19);
    EXPECT_EQ(sim_out->body_soil_pos_[18].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[18].x_b, posJ[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[18].y_b, posJ[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[18].z_b, posJ[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[18].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[19].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[19].ii, 20);
    EXPECT_EQ(sim_out->body_soil_pos_[19].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[19].x_b, posK[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[19].y_b, posK[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[19].z_b, posK[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[19].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 20);
    sim_out->body_[0][9][15] = 0.0;
    sim_out->body_[1][9][15] = 0.0;
    sim_out->body_[2][9][15] = 0.0;
    sim_out->body_[3][9][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_[0][13][15] = 0.0;
    sim_out->body_[1][13][15] = 0.0;
    sim_out->body_[2][13][15] = 0.0;
    sim_out->body_[3][13][15] = 0.0;
    sim_out->body_[0][14][15] = 0.0;
    sim_out->body_[1][14][15] = 0.0;
    sim_out->body_[2][14][15] = 0.0;
    sim_out->body_[3][14][15] = 0.0;
    sim_out->body_[0][15][15] = 0.0;
    sim_out->body_[1][15][15] = 0.0;
    sim_out->body_[2][15][15] = 0.0;
    sim_out->body_[3][15][15] = 0.0;
    sim_out->body_[0][16][15] = 0.0;
    sim_out->body_[1][16][15] = 0.0;
    sim_out->body_[2][16][15] = 0.0;
    sim_out->body_[3][16][15] = 0.0;
    sim_out->body_[0][17][15] = 0.0;
    sim_out->body_[1][17][15] = 0.0;
    sim_out->body_[2][17][15] = 0.0;
    sim_out->body_[3][17][15] = 0.0;
    sim_out->body_[0][18][15] = 0.0;
    sim_out->body_[1][18][15] = 0.0;
    sim_out->body_[2][18][15] = 0.0;
    sim_out->body_[3][18][15] = 0.0;
    sim_out->body_[0][19][15] = 0.0;
    sim_out->body_[1][19][15] = 0.0;
    sim_out->body_[2][19][15] = 0.0;
    sim_out->body_[3][19][15] = 0.0;
    sim_out->body_[0][20][15] = 0.0;
    sim_out->body_[1][20][15] = 0.0;
    sim_out->body_[2][20][15] = 0.0;
    sim_out->body_[3][20][15] = 0.0;
    sim_out->body_soil_[0][9][15] = 0.0;
    sim_out->body_soil_[1][9][15] = 0.0;
    sim_out->body_soil_[2][9][15] = 0.0;
    sim_out->body_soil_[3][9][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    sim_out->body_soil_[2][13][15] = 0.0;
    sim_out->body_soil_[3][13][15] = 0.0;
    sim_out->body_soil_[2][14][15] = 0.0;
    sim_out->body_soil_[3][14][15] = 0.0;
    sim_out->body_soil_[0][15][15] = 0.0;
    sim_out->body_soil_[1][15][15] = 0.0;
    sim_out->body_soil_[0][16][15] = 0.0;
    sim_out->body_soil_[1][16][15] = 0.0;
    sim_out->body_soil_[2][17][15] = 0.0;
    sim_out->body_soil_[3][17][15] = 0.0;
    sim_out->body_soil_[0][18][15] = 0.0;
    sim_out->body_soil_[1][18][15] = 0.0;
    sim_out->body_soil_[2][19][15] = 0.0;
    sim_out->body_soil_[3][19][15] = 0.0;
    sim_out->body_soil_[0][20][15] = 0.0;
    sim_out->body_soil_[1][20][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil on the first --
    // -- bucket layer is blocking the movement, then two bucket layers and  --
    // -- the soil on the first bucket layer is blocking the movement, then  --
    // -- two bucket layers and the soil on the second bucket layer is       --
    // -- blocking the movement, then two bucket layers and the soil on the  --
    // -- second bucket layer is blocking the movement, then two bucket      --
    // -- layers and the soil on the first bucket layer is blocking the      --
    // -- movement, then two bucket layers and the soil is fully avalanching --
    // -- on the first bucket layer                                          --
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
    EXPECT_EQ(sim_out->body_soil_pos_[8].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[8].ii, 16);
    EXPECT_EQ(sim_out->body_soil_pos_[8].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 9);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_[0][13][15] = 0.0;
    sim_out->body_[1][13][15] = 0.0;
    sim_out->body_[2][13][15] = 0.0;
    sim_out->body_[3][13][15] = 0.0;
    sim_out->body_[0][14][15] = 0.0;
    sim_out->body_[1][14][15] = 0.0;
    sim_out->body_[2][14][15] = 0.0;
    sim_out->body_[3][14][15] = 0.0;
    sim_out->body_[0][15][15] = 0.0;
    sim_out->body_[1][15][15] = 0.0;
    sim_out->body_[2][15][15] = 0.0;
    sim_out->body_[3][15][15] = 0.0;
    sim_out->body_[0][16][15] = 0.0;
    sim_out->body_[1][16][15] = 0.0;
    sim_out->body_[2][16][15] = 0.0;
    sim_out->body_[3][16][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.0;
    sim_out->body_soil_[2][13][15] = 0.0;
    sim_out->body_soil_[3][13][15] = 0.0;
    sim_out->body_soil_[2][14][15] = 0.0;
    sim_out->body_soil_[3][14][15] = 0.0;
    sim_out->body_soil_[0][15][15] = 0.0;
    sim_out->body_soil_[1][15][15] = 0.0;
    sim_out->body_soil_[0][16][15] = 0.0;
    sim_out->body_soil_[1][16][15] = 0.0;
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the first bucket layer, then two bucket layers and  --
    // -- the soil is partially avalanching on the first bucket layer, then  --
    // -- two bucket layers and the soil is partially avalanching on the     --
    // -- second bucket layer, then two bucket layers and the soil is        --
    // -- partially avalanching on the second bucket layer, then two bucket  --
    // -- layers and the soil is partially avalanching on the first bucket   --
    // -- layer, then two bucket layers and the soil is fully avalanching on --
    // -- the second bucket layer                                            --
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
    EXPECT_EQ(sim_out->body_soil_pos_[6].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[6].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[6].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[7].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[7].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[7].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].x_b, posB[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].y_b, posB[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].z_b, posB[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[8].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[8].ii, 13);
    EXPECT_EQ(sim_out->body_soil_pos_[8].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].x_b, posC[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].y_b, posC[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].z_b, posC[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[9].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[9].ii, 14);
    EXPECT_EQ(sim_out->body_soil_pos_[9].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].x_b, posD[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].y_b, posD[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].z_b, posD[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[9].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[10].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[10].ii, 15);
    EXPECT_EQ(sim_out->body_soil_pos_[10].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].x_b, posE[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].y_b, posE[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].z_b, posE[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[10].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[11].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[11].ii, 16);
    EXPECT_EQ(sim_out->body_soil_pos_[11].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].x_b, posF[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].y_b, posF[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].z_b, posF[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[11].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 12);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_[0][13][15] = 0.0;
    sim_out->body_[1][13][15] = 0.0;
    sim_out->body_[2][13][15] = 0.0;
    sim_out->body_[3][13][15] = 0.0;
    sim_out->body_[0][14][15] = 0.0;
    sim_out->body_[1][14][15] = 0.0;
    sim_out->body_[2][14][15] = 0.0;
    sim_out->body_[3][14][15] = 0.0;
    sim_out->body_[0][15][15] = 0.0;
    sim_out->body_[1][15][15] = 0.0;
    sim_out->body_[2][15][15] = 0.0;
    sim_out->body_[3][15][15] = 0.0;
    sim_out->body_[0][16][15] = 0.0;
    sim_out->body_[1][16][15] = 0.0;
    sim_out->body_[2][16][15] = 0.0;
    sim_out->body_[3][16][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.0;
    sim_out->body_soil_[2][13][15] = 0.0;
    sim_out->body_soil_[3][13][15] = 0.0;
    sim_out->body_soil_[2][14][15] = 0.0;
    sim_out->body_soil_[3][14][15] = 0.0;
    sim_out->body_soil_[0][15][15] = 0.0;
    sim_out->body_soil_[1][15][15] = 0.0;
    sim_out->body_soil_[2][16][15] = 0.0;
    sim_out->body_soil_[3][16][15] = 0.0;
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially  --
    // -- avalanching on the first bucket layer, then two bucket layers and   --
    // -- the soil on the first bucket layer is blocking the movement, then   --
    // -- two bucket layers and the soil is partially avalanching on the      --
    // -- first bucket layer, then two bucket layers and the soil on the      --
    // -- second bucket layer is blocking the movement, then two bucket       --
    // -- layers and the soil is partially avalanching on the second bucket   --
    // -- layer, then two bucket layers and the soil on the first bucket      --
    // --layer is blocking the movement, then two bucket layers and the soil  --
    // -- is partially avalanching on the second bucket layer, then two       --
    // -- bucket layers and the soil on the second bucket layer is blocking   --
    // -- the movement, then two bucket layers and the soil is partially      --
    // -- avalanching on the first bucket layer, then two bucket layers and   --
    // -- the soil is fully avalanching on the first bucket layer             --
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
    EXPECT_EQ(sim_out->body_soil_pos_[12].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[12].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[12].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[12].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[12].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[12].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[12].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[13].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[13].ii, 13);
    EXPECT_EQ(sim_out->body_soil_pos_[13].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[13].x_b, posB[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[13].y_b, posB[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[13].z_b, posB[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[13].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[14].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[14].ii, 15);
    EXPECT_EQ(sim_out->body_soil_pos_[14].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[14].x_b, posC[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[14].y_b, posC[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[14].z_b, posC[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[14].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[15].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[15].ii, 17);
    EXPECT_EQ(sim_out->body_soil_pos_[15].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[15].x_b, posD[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[15].y_b, posD[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[15].z_b, posD[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[15].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[16].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[16].ii, 19);
    EXPECT_EQ(sim_out->body_soil_pos_[16].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[16].x_b, posE[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[16].y_b, posE[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[16].z_b, posE[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[16].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[17].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[17].ii, 20);
    EXPECT_EQ(sim_out->body_soil_pos_[17].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[17].x_b, posF[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[17].y_b, posF[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[17].z_b, posF[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[17].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 18);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_[0][13][15] = 0.0;
    sim_out->body_[1][13][15] = 0.0;
    sim_out->body_[2][13][15] = 0.0;
    sim_out->body_[3][13][15] = 0.0;
    sim_out->body_[0][14][15] = 0.0;
    sim_out->body_[1][14][15] = 0.0;
    sim_out->body_[2][14][15] = 0.0;
    sim_out->body_[3][14][15] = 0.0;
    sim_out->body_[0][15][15] = 0.0;
    sim_out->body_[1][15][15] = 0.0;
    sim_out->body_[2][15][15] = 0.0;
    sim_out->body_[3][15][15] = 0.0;
    sim_out->body_[0][16][15] = 0.0;
    sim_out->body_[1][16][15] = 0.0;
    sim_out->body_[2][16][15] = 0.0;
    sim_out->body_[3][16][15] = 0.0;
    sim_out->body_[0][17][15] = 0.0;
    sim_out->body_[1][17][15] = 0.0;
    sim_out->body_[2][17][15] = 0.0;
    sim_out->body_[3][17][15] = 0.0;
    sim_out->body_[0][18][15] = 0.0;
    sim_out->body_[1][18][15] = 0.0;
    sim_out->body_[2][18][15] = 0.0;
    sim_out->body_[3][18][15] = 0.0;
    sim_out->body_[0][19][15] = 0.0;
    sim_out->body_[1][19][15] = 0.0;
    sim_out->body_[2][19][15] = 0.0;
    sim_out->body_[3][19][15] = 0.0;
    sim_out->body_[0][20][15] = 0.0;
    sim_out->body_[1][20][15] = 0.0;
    sim_out->body_[2][20][15] = 0.0;
    sim_out->body_[3][20][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[0][12][15] = 0.0;
    sim_out->body_soil_[1][12][15] = 0.0;
    sim_out->body_soil_[0][13][15] = 0.0;
    sim_out->body_soil_[1][13][15] = 0.0;
    sim_out->body_soil_[2][14][15] = 0.0;
    sim_out->body_soil_[3][14][15] = 0.0;
    sim_out->body_soil_[2][15][15] = 0.0;
    sim_out->body_soil_[3][15][15] = 0.0;
    sim_out->body_soil_[0][16][15] = 0.0;
    sim_out->body_soil_[1][16][15] = 0.0;
    sim_out->body_soil_[2][17][15] = 0.0;
    sim_out->body_soil_[3][17][15] = 0.0;
    sim_out->body_soil_[2][18][15] = 0.0;
    sim_out->body_soil_[3][18][15] = 0.0;
    sim_out->body_soil_[0][19][15] = 0.0;
    sim_out->body_soil_[1][19][15] = 0.0;
    sim_out->body_soil_[0][20][15] = 0.0;
    sim_out->body_soil_[1][20][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the second bucket layer, then two bucket layers and --
    // -- the soil is fully avalanching on the second bucket layer           --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[3].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[3].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ind, 2);
    EXPECT_EQ(sim_out->body_soil_pos_[4].ii, 12);
    EXPECT_EQ(sim_out->body_soil_pos_[4].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].x_b, posB[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].y_b, posB[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].z_b, posB[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[4].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_[0][12][15] = 0.0;
    sim_out->body_[1][12][15] = 0.0;
    sim_out->body_[2][12][15] = 0.0;
    sim_out->body_[3][12][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    sim_out->body_soil_[2][12][15] = 0.0;
    sim_out->body_soil_[3][12][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when several layers of soil is present at a given location --
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
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[3].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[6].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[6].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[6].jj, 15);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].x_b, posA[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].y_b, posA[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].z_b, posA[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[6].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[7].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[7].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[7].jj, 16);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].x_b, posB[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].y_b, posB[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].z_b, posB[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[7].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_[8].ind, 0);
    EXPECT_EQ(sim_out->body_soil_pos_[8].ii, 11);
    EXPECT_EQ(sim_out->body_soil_pos_[8].jj, 14);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].x_b, posC[0], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].y_b, posC[1], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].z_b, posC[2], 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[8].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 9);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[0][11][14] = 0.0;
    sim_out->body_[1][11][14] = 0.0;
    sim_out->body_[0][11][15] = 0.0;
    sim_out->body_[1][11][15] = 0.0;
    sim_out->body_[0][11][16] = 0.0;
    sim_out->body_[1][11][16] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[0][11][14] = 0.0;
    sim_out->body_soil_[1][11][14] = 0.0;
    sim_out->body_soil_[0][11][15] = 0.0;
    sim_out->body_soil_[1][11][15] = 0.0;
    sim_out->body_soil_[0][11][16] = 0.0;
    sim_out->body_soil_[1][11][16] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing when there is nothing to move --
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
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_[2][11][15] = 0.0;
    sim_out->body_[3][11][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->body_soil_[2][11][15] = 0.0;
    sim_out->body_soil_[3][11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing randomness of movement --
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
    sim_out->terrain_[9][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());

    // -- Testing that warning is properly sent when soil cannot be moved --
    // TBD: It is not straightforward to have proper warning system
    // This should be improved in the implementation in order to be able to
    // test it

    // -- Testing that everything has been reset properly --
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1e-5);
            }

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

    // -- Testing that intersecting cells are properly located --
    auto intersecting_cells = soil_simulator::LocateIntersectingCells(
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

    // -- Testing for a single intersecting cells in the -X direction --
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
    EXPECT_NEAR(sim_out->terrain_[11][17], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][17], 0.1, 1e-5);
    sim_out->terrain_[10][17] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }

    // -- Testing for a single intersecting cells in the +X direction --
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
    EXPECT_NEAR(sim_out->terrain_[11][17], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][17], 0.2, 1e-5);
    sim_out->terrain_[12][17] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }

    // -- Testing for a single intersecting cells in the -Y direction --
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
    EXPECT_NEAR(sim_out->terrain_[11][17], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][16], 0.05, 1e-5);
    sim_out->terrain_[11][16] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }

    // -- Testing for a single intersecting cells in the +Y direction --
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
    EXPECT_NEAR(sim_out->terrain_[11][17], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][18], 0.25, 1e-5);
    sim_out->terrain_[11][18] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }

    // -- Testing for a single intersecting cells in the -X-Y direction --
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
    EXPECT_NEAR(sim_out->terrain_[11][17], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.4, 1e-5);
    sim_out->terrain_[10][16] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }

    // -- Testing for a single intersecting cells in the +X-Y direction --
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
    EXPECT_NEAR(sim_out->terrain_[11][17], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][16], 0.1, 1e-5);
    sim_out->terrain_[12][16] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }

    // -- Testing for a single intersecting cells in the -X+Y direction --
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
    EXPECT_NEAR(sim_out->terrain_[11][17], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][18], 0.5, 1e-5);
    sim_out->terrain_[10][18] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }

    // -- Testing for a single intersecting cells in the +X+Y direction --
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
    EXPECT_NEAR(sim_out->terrain_[11][17], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][18], 0.8, 1e-5);
    sim_out->terrain_[12][18] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }

    // -- Testing for a single intersecting cells in the second bucket layer --
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
    EXPECT_NEAR(sim_out->terrain_[11][17], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][18], 0.5, 1e-5);
    sim_out->terrain_[10][18] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[2][ii][jj] = 0.0;
            sim_out->body_[3][ii][jj] = 0.0;
        }

    // -- Testing for a single intersecting cells with various bucket layer --
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
    EXPECT_NEAR(sim_out->terrain_[11][17], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][18], 0.5, 1e-5);
    sim_out->terrain_[10][18] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
            sim_out->body_[2][ii][jj] = 0.0;
            sim_out->body_[3][ii][jj] = 0.0;
        }

    // -- Testing for single intersecting cells with all bucket under terrain --
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
    sim_out->terrain_[12][18] = 0.0;
    sim_out->terrain_[11][17] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 10; ii < 13; ii++)
        for (auto jj = 16; jj < 19; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }
    sim_out->body_[2][11][17] = 0.0;
    sim_out->body_[3][11][17] = 0.0;

    // -- Testing for a single intersecting cells under a large bucket --
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
    sim_out->terrain_[8][17] = 0.0;
    sim_out->terrain_[11][17] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }

    // -- Testing when soil is moved by small amount (1) --
    // Soil is fitting under the bucket
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
    sim_out->terrain_[11][17] = 0.0;
    sim_out->terrain_[10][17] = 0.0;
    sim_out->terrain_[8][17] = 0.0;
    sim_out->terrain_[12][17] = 0.0;
    sim_out->terrain_[13][17] = 0.0;
    sim_out->terrain_[13][19] = 0.0;
    sim_out->terrain_[14][20] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }
    sim_out->body_[2][13][17] = 0.0;
    sim_out->body_[3][13][17] = 0.0;
    sim_out->body_[2][14][20] = 0.0;
    sim_out->body_[3][14][20] = 0.0;

    // -- Testing when soil is moved by small amount (2) --
    // Soil is going out of the bucket
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
    sim_out->terrain_[11][17] = 0.0;
    sim_out->terrain_[10][17] = 0.0;
    sim_out->terrain_[8][17] = 0.0;
    sim_out->terrain_[12][17] = 0.0;
    sim_out->terrain_[13][17] = 0.0;
    sim_out->terrain_[13][19] = 0.0;
    sim_out->terrain_[14][20] = 0.0;
    sim_out->terrain_[15][17] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }
    sim_out->body_[2][13][17] = 0.0;
    sim_out->body_[3][13][17] = 0.0;
    sim_out->body_[2][14][20] = 0.0;
    sim_out->body_[3][14][20] = 0.0;

    // -- Testing when soil is moved by small amount (3) --
    // Soil is just fitting under the bucket
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
    sim_out->terrain_[11][17] = 0.0;
    sim_out->terrain_[10][17] = 0.0;
    sim_out->terrain_[8][17] = 0.0;
    sim_out->terrain_[12][17] = 0.0;
    sim_out->terrain_[13][17] = 0.0;
    sim_out->terrain_[13][19] = 0.0;
    sim_out->terrain_[14][20] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }
    sim_out->body_[2][13][17] = 0.0;
    sim_out->body_[3][13][17] = 0.0;
    sim_out->body_[2][14][20] = 0.0;
    sim_out->body_[3][14][20] = 0.0;

    // -- Testing when there is nothing to move --
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.2;
        }
    soil_simulator::MoveIntersectingBody(sim_out, 1e-5);
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    for (auto ii = 8; ii < 15; ii++)
        for (auto jj = 14; jj < 21; jj++) {
            sim_out->body_[0][ii][jj] = 0.0;
            sim_out->body_[1][ii][jj] = 0.0;
        }

    // -- Testing randomness of movement --
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
    sim_out->terrain_[11][17] = 0.0;
    sim_out->terrain_[10][18] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
    sim_out->body_[0][11][17] = 0.0;
    sim_out->body_[1][11][17] = 0.0;

    // -- Testing that everything has been reset properly --
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1e-5);
            }

    delete sim_out;
}
