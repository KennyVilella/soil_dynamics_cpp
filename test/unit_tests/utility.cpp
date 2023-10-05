/*
This file implements utility functions for unit testing.

Copyright, 2023, Vilella Kenny.
*/
#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "test/unit_tests/utility.hpp"

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
