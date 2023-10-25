/*
This file implements utility functions for unit testing.

Copyright, 2023, Vilella Kenny.
*/
#include <cmath>
#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "test/unit_tests/utility.hpp"

void test_soil_simulator::SetHeight(
    soil_simulator::SimOut* sim_out, int ii, int jj, float terrain,
    float body_0, float body_1, float body_soil_0, float body_soil_1,
    float body_2, float body_3, float body_soil_2, float body_soil_3
) {
    // Setting terrain
    if (!std::isnan(terrain))
        sim_out->terrain_[ii][jj] = terrain;

    // Setting body
    if (!std::isnan(body_0))
        sim_out->body_[0][ii][jj] = body_0;
    if (!std::isnan(body_1))
        sim_out->body_[1][ii][jj] = body_1;
    if (!std::isnan(body_2))
        sim_out->body_[2][ii][jj] = body_2;
    if (!std::isnan(body_3))
        sim_out->body_[3][ii][jj] = body_3;

    // Setting body_soil
    if (!std::isnan(body_soil_0))
        sim_out->body_soil_[0][ii][jj] = body_soil_0;
    if (!std::isnan(body_soil_1))
        sim_out->body_soil_[1][ii][jj] = body_soil_1;
    if (!std::isnan(body_soil_2))
        sim_out->body_soil_[2][ii][jj] = body_soil_2;
    if (!std::isnan(body_soil_3))
        sim_out->body_soil_[3][ii][jj] = body_soil_3;
}

void test_soil_simulator::CheckHeight(
    soil_simulator::SimOut* sim_out, int ii, int jj, float terrain,
    float body_soil_0, float body_soil_1, float body_soil_2, float body_soil_3
) {
    // Checking terrain'
    if (!std::isnan(terrain))
        EXPECT_NEAR(sim_out->terrain_[ii][jj], terrain, 1e-5);

    // Checking body_soil
    if (!std::isnan(body_soil_0))
        EXPECT_NEAR(sim_out->body_soil_[0][ii][jj], body_soil_0, 1e-5);
    if (!std::isnan(body_soil_1))
        EXPECT_NEAR(sim_out->body_soil_[1][ii][jj], body_soil_1, 1e-5);
    if (!std::isnan(body_soil_2))
        EXPECT_NEAR(sim_out->body_soil_[2][ii][jj], body_soil_2, 1e-5);
    if (!std::isnan(body_soil_3))
        EXPECT_NEAR(sim_out->body_soil_[3][ii][jj], body_soil_3, 1e-5);
}

void test_soil_simulator::ResetValueAndTest(
    soil_simulator::SimOut* sim_out, std::vector<std::vector<int>> terrain_pos,
    std::vector<std::vector<int>> body_pos,
    std::vector<std::vector<int>> body_soil_pos
) {
    // Resetting requested terrain
    for (auto nn = 0; nn < terrain_pos.size(); nn++) {
        int ii = terrain_pos[nn][0];
        int jj = terrain_pos[nn][1];
        sim_out->terrain_[ii][jj] = 0.0;
    }

    // Resetting requested body
    for (auto nn = 0; nn < body_pos.size(); nn++) {
        int ind = body_pos[nn][0];
        int ii = body_pos[nn][1];
        int jj = body_pos[nn][2];
        sim_out->body_[ind][ii][jj] = 0.0;
        sim_out->body_[ind+1][ii][jj] = 0.0;
    }

    // Resetting requested body_soil
    for (auto nn = 0; nn < body_soil_pos.size(); nn++) {
        int ind = body_soil_pos[nn][0];
        int ii = body_soil_pos[nn][1];
        int jj = body_soil_pos[nn][2];
        sim_out->body_soil_[ind][ii][jj] = 0.0;
        sim_out->body_soil_[ind+1][ii][jj] = 0.0;
    }

    // Checking that everything is properly reset
    for (auto ii = 0; ii < sim_out->body_.size(); ii++)
        for (auto jj = 0; jj < sim_out->body_[0].size(); jj++)
            for (auto kk = 0; kk < sim_out->body_[0][0].size(); kk++) {
                EXPECT_NEAR(sim_out->body_[ii][jj][kk], 0.0, 1.e-5);
                EXPECT_NEAR(sim_out->body_soil_[ii][jj][kk], 0.0, 1.e-5);
            }
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1.e-5);

    // Resetting body_soil_pos
    sim_out->body_soil_pos_.erase(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());
}

void test_soil_simulator::PushBodySoilPos(
    soil_simulator::SimOut* sim_out, int ind, int ii, int jj,
    std::vector<float> pos, float h_soil
) {
    // Pushing a new body soil position
    sim_out->body_soil_pos_.push_back(soil_simulator::body_soil
        {ind, ii, jj, pos[0], pos[1], pos[2], h_soil});
}

void test_soil_simulator::CheckBodySoilPos(
    soil_simulator::body_soil body_soil_pos, int ind, int ii, int jj,
    std::vector<float> pos, float h_soil
) {
    // Checking the body soil position
    EXPECT_EQ(body_soil_pos.ind, ind);
    EXPECT_EQ(body_soil_pos.ii, ii);
    EXPECT_EQ(body_soil_pos.jj, jj);
    EXPECT_NEAR(body_soil_pos.x_b, pos[0], 1.e-5);
    EXPECT_NEAR(body_soil_pos.y_b, pos[1], 1.e-5);
    EXPECT_NEAR(body_soil_pos.z_b, pos[2], 1.e-5);
    EXPECT_NEAR(body_soil_pos.h_soil, h_soil, 1.e-5);
}
