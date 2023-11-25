/*
This file implements unit tests for the classes in types.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <vector>
#include <exception>
#include <iostream>
#include "gtest/gtest.h"
#include "soil_simulator/types.hpp"

TEST(UnitTestTypes, Grid) {
    // Test: TY-G-1
    soil_simulator::Grid grid(4.0, 4.0, 4.0, 0.05, 0.01);
    EXPECT_EQ(grid.half_length_x_, 80);
    EXPECT_EQ(grid.half_length_y_, 80);
    EXPECT_EQ(grid.half_length_z_, 400);
    EXPECT_NEAR(grid.cell_size_xy_, 0.05, 1e-8);
    EXPECT_NEAR(grid.cell_size_z_, 0.01, 1e-8);
    EXPECT_NEAR(grid.cell_area_, 0.0025, 1e-8);
    EXPECT_NEAR(grid.cell_volume_, 0.000025, 1e-8);
    for (auto ii = 0 ; ii < grid.vect_x_.size() ; ii++)
        EXPECT_NEAR(grid.vect_x_[ii], -4.0 + ii * 0.05, 1e-5);
    for (auto ii = 0 ; ii < grid.vect_y_.size() ; ii++)
        EXPECT_NEAR(grid.vect_y_[ii], -4.0 + ii * 0.05, 1e-5);
    for (auto ii = 0 ; ii < grid.vect_z_.size() ; ii++)
        EXPECT_NEAR(grid.vect_z_[ii], -4.0 + (ii + 1) * 0.01, 1e-5);

    // Test: TY-G-2
    EXPECT_THROW(
        soil_simulator::Grid grid(4.0, 4.0, 4.0, 0.05, 0.0),
        std::invalid_argument);
    EXPECT_THROW(
        soil_simulator::Grid grid(4.0, 4.0, 4.0, 0.05, -0.01),
        std::invalid_argument);

    // Test: TY-G-3
    EXPECT_THROW(
        soil_simulator::Grid grid(4.0, 4.0, 4.0, 0.0, 0.01),
        std::invalid_argument);
    EXPECT_THROW(
        soil_simulator::Grid grid(4.0, 4.0, 4.0, -0.05, 0.01),
        std::invalid_argument);

    // Test: TY-G-4
    EXPECT_THROW(
        soil_simulator::Grid grid(0.0, 4.0, 4.0, 0.05, 0.01),
        std::invalid_argument);
    EXPECT_THROW(
        soil_simulator::Grid grid(-1.0, 4.0, 4.0, 0.05, 0.01),
        std::invalid_argument);

    // Test: TY-G-5
    EXPECT_THROW(
        soil_simulator::Grid grid(4.0, 0.0, 4.0, 0.05, 0.01),
        std::invalid_argument);
    EXPECT_THROW(
        soil_simulator::Grid grid(4.0, -2.0, 4.0, 0.05, 0.01),
        std::invalid_argument);

    // Test: TY-G-6
    EXPECT_THROW(
        soil_simulator::Grid grid(4.0, 4.0, 0.0, 0.05, 0.01),
        std::invalid_argument);
    EXPECT_THROW(
        soil_simulator::Grid grid(4.0, 4.0, -4.0, 0.05, 0.01),
        std::invalid_argument);

    // Test: TY-G-7
    EXPECT_THROW(
        soil_simulator::Grid grid(4.0, 4.0, 4.0, 0.05, 0.06),
        std::invalid_argument);
    EXPECT_NO_THROW(soil_simulator::Grid grid(4.0, 4.0, 4.0, 0.05, 0.05));

    // Test: TY-G-8
    EXPECT_THROW(
        soil_simulator::Grid grid(0.04, 4.0, 4.0, 0.05, 0.05),
        std::invalid_argument);
    EXPECT_NO_THROW(soil_simulator::Grid grid(0.05, 4.0, 4.0, 0.05, 0.05));

    // Test: TY-G-9
    EXPECT_THROW(
        soil_simulator::Grid grid(4.0, 0.01, 4.0, 0.05, 0.05),
        std::invalid_argument);
    EXPECT_NO_THROW(soil_simulator::Grid grid(4.0, 0.05, 4.0, 0.05, 0.05));

    // Test: TY-G-10
    EXPECT_THROW(
        soil_simulator::Grid grid(4.0, 4.0, 0.01, 0.05, 0.03),
        std::invalid_argument);
    EXPECT_NO_THROW(soil_simulator::Grid grid(4.0, 4.0, 0.03, 0.05, 0.03));
}

TEST(UnitTestTypes, Bucket) {
    // Setting up the environment
    std::vector<float> vect_1 = {0.0};
    std::vector<float> vect_2 = {0.0, 0.1};
    std::vector<float> vect_4 = {0.0, 0.1, 0.0, 0.3};
    std::vector<float> o_pos = {0.0, 0.1, 0.0};
    std::vector<float> j_pos = {0.0, 0.5, 0.0};
    std::vector<float> b_pos = {0.0, 0.5, 0.5};
    std::vector<float> t_pos = {0.5, 0.5, 0.0};

    // Test: TY-Bu-1
    soil_simulator::Bucket bucket(o_pos, j_pos, b_pos, t_pos, 0.5);
    EXPECT_NEAR(bucket.j_pos_init_[0], 0.0, 1e-8);
    EXPECT_NEAR(bucket.j_pos_init_[1], 0.4, 1e-8);
    EXPECT_NEAR(bucket.j_pos_init_[2], 0.0, 1e-8);
    EXPECT_NEAR(bucket.b_pos_init_[0], 0.0, 1e-8);
    EXPECT_NEAR(bucket.b_pos_init_[1], 0.4, 1e-8);
    EXPECT_NEAR(bucket.b_pos_init_[2], 0.5, 1e-8);
    EXPECT_NEAR(bucket.t_pos_init_[0], 0.5, 1e-8);
    EXPECT_NEAR(bucket.t_pos_init_[1], 0.4, 1e-8);
    EXPECT_NEAR(bucket.t_pos_init_[2], 0.0, 1e-8);
    EXPECT_EQ(bucket.width_, 0.5);
    for (auto ii = 0 ; ii < 3 ; ii++)
        EXPECT_EQ(bucket.pos_[ii], 0.0);
    for (auto ii = 0 ; ii < 4 ; ii++)
        EXPECT_EQ(bucket.ori_[ii], 0.0);

    // Test: TY-Bu-2
    EXPECT_THROW(
        soil_simulator::Bucket bucket(vect_1, j_pos, b_pos, t_pos, 0.5),
        std::invalid_argument);

    // Test: TY-Bu-3
    EXPECT_THROW(
        soil_simulator::Bucket bucket(o_pos, vect_2, b_pos, t_pos, 0.5),
        std::invalid_argument);

    // Test: TY-Bu-4
    EXPECT_THROW(
        soil_simulator::Bucket bucket(o_pos, j_pos, vect_4, t_pos, 0.5),
        std::invalid_argument);

    // Test: TY-Bu-5
    EXPECT_THROW(
        soil_simulator::Bucket bucket(o_pos, j_pos, b_pos, vect_2, 0.5),
        std::invalid_argument);

    // Test: TY-Bu-6
    EXPECT_THROW(
        soil_simulator::Bucket bucket(o_pos, b_pos, b_pos, t_pos, 0.5),
        std::invalid_argument);

    // Test: TY-Bu-7
    EXPECT_THROW(
        soil_simulator::Bucket bucket(o_pos, t_pos, b_pos, t_pos, 0.5),
        std::invalid_argument);

    // Test: TY-Bu-8
    EXPECT_THROW(
        soil_simulator::Bucket bucket(o_pos, j_pos, b_pos, b_pos, 0.5),
        std::invalid_argument);

    // Test: TY-Bu-9
    EXPECT_THROW(
        soil_simulator::Bucket bucket(o_pos, j_pos, b_pos, t_pos, 0.0),
        std::invalid_argument);
    EXPECT_THROW(
        soil_simulator::Bucket bucket(o_pos, j_pos, b_pos, t_pos, -0.5),
        std::invalid_argument);
}

TEST(UnitTestTypes, Blade) {
    // Setting up the environment
    std::vector<float> vect_1 = {0.0};
    std::vector<float> vect_2 = {0.0, 0.1};
    std::vector<float> vect_4 = {0.0, 0.1, 0.0, 0.3};
    std::vector<float> o_pos = {0.0, 0.1, 0.0};
    std::vector<float> j_pos = {0.0, 0.5, 0.0};
    std::vector<float> b_pos = {0.0, 0.5, 0.5};
    std::vector<float> t_pos = {0.5, 0.5, 0.0};

    // Test: TY-Bl-1
    soil_simulator::Blade blade(o_pos, j_pos, b_pos, t_pos, 0.5);
    EXPECT_NEAR(blade.j_pos_init_[0], 0.0, 1e-8);
    EXPECT_NEAR(blade.j_pos_init_[1], 0.4, 1e-8);
    EXPECT_NEAR(blade.j_pos_init_[2], 0.0, 1e-8);
    EXPECT_NEAR(blade.b_pos_init_[0], 0.0, 1e-8);
    EXPECT_NEAR(blade.b_pos_init_[1], 0.4, 1e-8);
    EXPECT_NEAR(blade.b_pos_init_[2], 0.5, 1e-8);
    EXPECT_NEAR(blade.t_pos_init_[0], 0.5, 1e-8);
    EXPECT_NEAR(blade.t_pos_init_[1], 0.4, 1e-8);
    EXPECT_NEAR(blade.t_pos_init_[2], 0.0, 1e-8);
    EXPECT_EQ(blade.width_, 0.5);
    for (auto ii = 0 ; ii < 3 ; ii++)
        EXPECT_EQ(blade.pos_[ii], 0.0);
    for (auto ii = 0 ; ii < 4 ; ii++)
        EXPECT_EQ(blade.ori_[ii], 0.0);

    // Test: TY-Bl-2
    EXPECT_THROW(
        soil_simulator::Blade blade(vect_1, j_pos, b_pos, t_pos, 0.5),
        std::invalid_argument);

    // Test: TY-Bl-3
    EXPECT_THROW(
        soil_simulator::Blade blade(o_pos, vect_2, b_pos, t_pos, 0.5),
        std::invalid_argument);

    // Test: TY-Bl-4
    EXPECT_THROW(
        soil_simulator::Blade blade(o_pos, j_pos, vect_4, t_pos, 0.5),
        std::invalid_argument);

    // Test: TY-Bl-5
    EXPECT_THROW(
        soil_simulator::Blade blade(o_pos, j_pos, b_pos, vect_2, 0.5),
        std::invalid_argument);

    // Test: TY-Bl-6
    EXPECT_THROW(
        soil_simulator::Blade blade(o_pos, b_pos, b_pos, t_pos, 0.5),
        std::invalid_argument);

    // Test: TY-Bl-7
    EXPECT_THROW(
        soil_simulator::Blade blade(o_pos, t_pos, b_pos, t_pos, 0.5),
        std::invalid_argument);

    // Test: TY-Bl-8
    EXPECT_THROW(
        soil_simulator::Blade blade(o_pos, j_pos, b_pos, b_pos, 0.5),
        std::invalid_argument);

    // Test: TY-Bl-9
    EXPECT_THROW(
        soil_simulator::Blade blade(o_pos, j_pos, b_pos, t_pos, 0.0),
        std::invalid_argument);
    EXPECT_THROW(
        soil_simulator::Blade blade(o_pos, j_pos, b_pos, t_pos, -0.5),
        std::invalid_argument);
}

TEST(UnitTestTypes, SimParam) {
    // Test: TY-SP-1
    soil_simulator::SimParam sim_param(0.85, 5, 4);
    EXPECT_NEAR(sim_param.repose_angle_, 0.85, 1e-7);
    EXPECT_EQ(sim_param.max_iterations_, 5);
    EXPECT_EQ(sim_param.cell_buffer_, 4);

    // Test: TY-SP-2
    EXPECT_THROW(
        soil_simulator::SimParam sim_param(3.14, 5, 4),
        std::invalid_argument);
    EXPECT_THROW(
        soil_simulator::SimParam sim_param(-0.85, 5, 4),
        std::invalid_argument);

    // Test: TY-SP-3
    EXPECT_NO_THROW(soil_simulator::SimParam sim_param(0.85, 0.0, 4));
    EXPECT_THROW(
        soil_simulator::SimParam sim_param(0.85, -5, 4),
        std::invalid_argument);

    // Test: TY-SP-4
    EXPECT_THROW(
        soil_simulator::SimParam sim_param(0.85, 5, 1),
        std::invalid_argument);
}

TEST(UnitTestTypes, SimOut) {
    // Test: TY-SO-1
    soil_simulator::Grid grid(2.0, 2.0, 2.0, 1.0, 0.01);
    soil_simulator::SimOut sim_out(grid);
    EXPECT_EQ(sim_out.equilibrium_, false);
    for (auto ii = 0 ; ii < 5 ; ii++)
        for (auto jj = 0 ; jj < 5 ; jj++)
            EXPECT_NEAR(sim_out.terrain_[ii][jj], 0.0, 1e-8);
    EXPECT_EQ(sim_out.terrain_.size(), 5);
    EXPECT_EQ(sim_out.terrain_[0].size(), 5);
    for (auto ii = 0 ; ii < 4 ; ii++)
        for (auto jj = 0 ; jj < 5 ; jj++)
            for (auto kk = 0 ; kk < 5 ; kk++) {
                EXPECT_NEAR(sim_out.body_[ii][jj][kk], 0.0, 1e-8);
                EXPECT_NEAR(sim_out.body_soil_[ii][jj][kk], 0.0, 1e-8);
            }
    EXPECT_EQ(sim_out.body_.size(), 4);
    EXPECT_EQ(sim_out.body_[0].size(), 5);
    EXPECT_EQ(sim_out.body_[0][0].size(), 5);
    EXPECT_EQ(sim_out.body_soil_.size(), 4);
    EXPECT_EQ(sim_out.body_soil_[0].size(), 5);
    EXPECT_EQ(sim_out.body_soil_[0][0].size(), 5);
    EXPECT_EQ(sim_out.body_area_[0][0], 1);
    EXPECT_EQ(sim_out.relax_area_[0][0], 1);
    EXPECT_EQ(sim_out.impact_area_[0][0], 1);
    EXPECT_EQ(sim_out.body_area_[0][1], 4);
    EXPECT_EQ(sim_out.relax_area_[0][1], 4);
    EXPECT_EQ(sim_out.impact_area_[0][1], 4);
    EXPECT_EQ(sim_out.body_area_[1][0], 1);
    EXPECT_EQ(sim_out.relax_area_[1][0], 1);
    EXPECT_EQ(sim_out.impact_area_[1][0], 1);
    EXPECT_EQ(sim_out.body_area_[1][1], 4);
    EXPECT_EQ(sim_out.relax_area_[1][1], 4);
    EXPECT_EQ(sim_out.impact_area_[1][1], 4);
}
