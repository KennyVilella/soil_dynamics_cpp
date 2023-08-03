/*
This file implements unit tests for the functions in intersecting_cells.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include "gtest/gtest.h"
#include "src/intersecting_cells.cpp"

TEST(UnitTestIntersectingCells, MoveBodySoil) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    sim_out->body_[0][10][15] = 0.3;
    sim_out->body_[1][10][15] = 0.7;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.7;
    sim_out->body_soil_[1][10][15] = 0.9;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.9;

    // -- Testing when soil is avalanching on the terrain --
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    auto [ind, ii, jj, h_soil, wall_presence] = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.6, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    sim_out->terrain_[5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when soil is avalanching below the first bucket layer --
    sim_out->body_[0][5][7] = 0.1;
    sim_out->body_[1][5][7] = 0.2;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.6, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    sim_out->terrain_[5][7] = 0.0;
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when the first bucket layer is blocking the movement --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.3;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, true);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.0, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when there is a lot of soil on first bucket layer --
    // -- Soil is avalanching on first bucket layer                 --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.4;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {0, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 1.0, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {0, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when soil is fully avalanching on first bucket layer --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.2;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.8, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {0, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when soil is fully avalanching on first bucket soil layer --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.2;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {0, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.8, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {0, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when soil is avalanching below the second bucket layer --
    sim_out->body_[2][5][7] = 0.3;
    sim_out->body_[3][5][7] = 0.6;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[5][7], 0.6, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {0, 5, 7}));
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->terrain_[5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when the second bucket layer is blocking the movement --
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.6;
    sim_out->body_soil_[2][5][7] = 0.6;
    sim_out->body_soil_[3][5][7] = 0.7;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, true);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.7, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {2, 5, 7}));
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when there is a lot of soil on second bucket layer --
    // -- but soil is still avalanching on it                        --
    sim_out->body_[2][5][7] = -0.2;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.3;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.9, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {2, 5, 7}));
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when soil is fully avalanching on second bucket layer --
    sim_out->body_[2][5][7] = -0.2;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.6, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {2, 5, 7}));
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when soil is fully avalanching on second bucket soil layer --
    sim_out->body_[2][5][7] = -0.2;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.2;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.8, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {2, 5, 7}));
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil fully filling the space (1) --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.2;
    sim_out->body_[2][5][7] = 0.2;
    sim_out->body_[3][5][7] = 0.4;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {0, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    EXPECT_EQ(ind, 0);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.2, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {0, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil fully filling the space (2) --
    sim_out->body_[0][5][7] = 0.6;
    sim_out->body_[1][5][7] = 0.7;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.1;
    sim_out->body_soil_[2][5][7] = 0.1;
    sim_out->body_soil_[3][5][7] = 0.6;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.6, 1e-5);
    EXPECT_EQ(ind, 2);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.6, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {2, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil is fully avalanching --
    // -- on bucket (1)                                                --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.2;
    sim_out->body_[2][5][7] = 0.8;
    sim_out->body_[3][5][7] = 0.9;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.8, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {0, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil is fully avalanching --
    // -- on bucket (2)                                                --
    sim_out->body_[0][5][7] = 0.8;
    sim_out->body_[1][5][7] = 0.9;
    sim_out->body_[2][5][7] = -0.1;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.6, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {2, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil is fully avalanching --
    // -- on bucket soil (1)                                           --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.2;
    sim_out->body_[2][5][7] = 0.9;
    sim_out->body_[3][5][7] = 1.0;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {0, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.8, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {0, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil is fully avalanching --
    // -- on bucket soil (2)                                           --
    sim_out->body_[0][5][7] = 0.8;
    sim_out->body_[1][5][7] = 0.9;
    sim_out->body_[2][5][7] = -0.1;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.2;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.8, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {2, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil is fully avalanching --
    // -- on bucket soil (3)                                           --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.2;
    sim_out->body_soil_[0][5][7] = 0.2;
    sim_out->body_soil_[1][5][7] = 0.3;
    sim_out->body_[2][5][7] = 0.9;
    sim_out->body_[3][5][7] = 1.;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {0, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.9, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {0, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil is fully avalanching --
    // -- on bucket soil (4)                                           --
    sim_out->body_[0][5][7] = 0.9;
    sim_out->body_[1][5][7] = 1.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.1;
    sim_out->body_soil_[2][5][7] = 0.1;
    sim_out->body_soil_[3][5][7] = 0.6;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.1, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.7, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {2, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil is partially avalanching --
    // -- on bucket (1)                                                    --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_[2][5][7] = 0.4;
    sim_out->body_[3][5][7] = 0.9;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.3, 1e-5);
    EXPECT_EQ(ind, 0);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.4, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {0, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil is partially avalanching --
    // -- on bucket (2)                                                    --
    sim_out->body_[0][5][7] = 0.3;
    sim_out->body_[1][5][7] = 0.9;
    sim_out->body_[2][5][7] = -0.1;
    sim_out->body_[3][5][7] = 0.2;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.5, 1e-5);
    EXPECT_EQ(ind, 2);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.3, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {2, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil is partially avalanching --
    // -- on bucket soil (1)                                               --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.1;
    sim_out->body_soil_[0][5][7] = 0.1;
    sim_out->body_soil_[1][5][7] = 0.2;
    sim_out->body_[2][5][7] = 0.4;
    sim_out->body_[3][5][7] = 0.5;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {0, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.4, 1e-5);
    EXPECT_EQ(ind, 0);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.4, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {0, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil is partially avalanching --
    // -- on bucket soil (2)                                               --
    sim_out->body_[0][5][7] = 0.6;
    sim_out->body_[1][5][7] = 0.9;
    sim_out->body_[2][5][7] = -0.1;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.2;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.6, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.2, 1e-5);
    EXPECT_EQ(ind, 2);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.6, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {2, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil is partially avalanching --
    // -- on bucket soil (3)                                               --
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.2;
    sim_out->body_soil_[0][5][7] = 0.2;
    sim_out->body_soil_[1][5][7] = 0.3;
    sim_out->body_[2][5][7] = 0.4;
    sim_out->body_[3][5][7] = 0.5;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {0, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.3, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.2, 1e-5);
    EXPECT_EQ(ind, 0);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[0][5][7], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][5][7], 0.4, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {0, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when two bucket layers and soil is partially avalanching --
    // -- on bucket soil (4)                                               --
    sim_out->body_[0][5][7] = 0.7;
    sim_out->body_[1][5][7] = 0.8;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.1;
    sim_out->body_soil_[2][5][7] = 0.1;
    sim_out->body_soil_[3][5][7] = 0.6;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 5, 7});
    std::tie(ind, ii, jj, h_soil, wall_presence) = soil_simulator::MoveBodySoil(
        sim_out, 2, 10, 15, 0.3, 5, 7, 0.3, false, 1e-5);
    EXPECT_EQ(wall_presence, false);
    EXPECT_NEAR(h_soil, 0.2, 1e-5);
    EXPECT_EQ(ind, 2);
    EXPECT_EQ(ii, 5);
    EXPECT_EQ(jj, 7);
    EXPECT_NEAR(sim_out->body_soil_[2][5][7], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][5][7], 0.7, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[2] == std::vector<int> {2, 5, 7}));
    sim_out->body_[0][5][7] = 0.0;
    sim_out->body_[1][5][7] = 0.0;
    sim_out->body_[2][5][7] = 0.0;
    sim_out->body_[3][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);
}

TEST(UnitTestIntersectingCells, MoveIntersectingBodySoil) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    // -- Testing when soil is avalanching on the terrain (1) --
    // -- First bucket layer at bottom                        --
    rng.seed(1234);
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_.push_back(std::vector<int> {2, 10, 15});
    soil_simulator::MoveIntersectingBodySoil(sim_out, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], 0.3, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[0] == std::vector<int> {0, 10, 15}));
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][5][7] = 0.0;
    sim_out->body_soil_[1][5][7] = 0.0;
    sim_out->body_soil_[2][5][7] = 0.0;
    sim_out->body_soil_[3][5][7] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing when soil is avalanching on the terrain (2) --
    // -- Second bucket layer at bottom                       --

    // -- Testing when soil is avalanching on the terrain (3) --
    // -- Bucket undergroumd                                  --

    // -- Testing when soil is avalanching below the first bucket layer (1) --

    // -- Testing when soil is avalanching below the first bucket layer (2) --

    // -- Testing when soil is avalanching below the first bucket layer (3) --

    // -- Testing when soil is avalanching below the first bucket layer --
    // -- despite the lack of available space                           --

    // -- Testing when soil is avalanching below the second bucket layer (1) --

    // -- Testing when soil is avalanching below the second bucket layer (2) --

    // -- Testing when soil is avalanching below the second bucket layer (3) --



    // -- Testing when soil is avalanching below the second bucket layer --
    // -- despite the lack of available space                            --

    // -- Testing when soil is fully avalanching on first bucket layer (1) --

    // -- Testing when soil is fully avalanching on first bucket layer (2) --

    // -- Testing when soil is fully avalanching on second bucket layer (1) --

    // -- Testing when soil is fully avalanching on second bucket layer (2) --

    // -- Testing when soil is fully avalanching on first bucket soil --
    // -- layer (1)                                                   --

    // -- Testing when soil is fully avalanching on first bucket soil --
    // -- layer (2)                                                   --

    // -- Testing when soil is fully avalanching on second bucket soil --
    // -- layer (1)                                                    --

    // -- Testing when soil is fully avalanching on the second bucket soil --
    // -- layer (2)                                                        --

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the first bucket layer (1)                  --



    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the first bucket layer (2)                  --

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the second bucket layer (1)                 --

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the second bucket layer (2)                 --

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the first bucket soil layer (1)             --

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the first bucket soil layer (2)             --

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the second bucket soil layer (1)            --

    // -- Testing when there are two bucket layers and soil is fully --
    // -- avalanching on the second bucket soil layer (2)            --

    // -- Testing when there are two bucket layers and soil is partially    --
    // -- avalanching on the first bucket layer and then on the terrain (1) --

    // -- Testing when there are two bucket layers and soil is partially    --
    // -- avalanching on the first bucket layer and then on the terrain (2) --

    // -- Testing when there are two bucket layers and soil is partially     --
    // -- avalanching on the second bucket layer and then on the terrain (1) --




    // -- Testing when there are two bucket layers and soil is partially     --
    // -- avalanching on the second bucket layer and then on the terrain (2) --

    // -- Testing when there are two bucket layers and soil is partially --
    // -- avalanching on first bucket soil layer and then on terrain (1) --

    // -- Testing when there are two bucket layers and soil is partially --
    // -- avalanching on first bucket soil layer and then on terrain (2) --

    // -- Testing when there are two bucket layers and soil is partially  --
    // -- avalanching on second bucket soil layer and then on terrain (1) --

    // -- Testing when there are two bucket layers and soil is partially  --
    // -- avalanching on second bucket soil layer and then on terrain (2) --

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the first bucket soil layer, then the soil is       --
    // -- avalanching on the terrain below the first bucket layer            --

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the  second bucket soil layer, then the soil is     --
    // -- avalanching on the terrain below the first bucket layer            --

    // -- Testing when there are two bucket layers and the soil on the first  --
    // -- bucket layer is blocking the movement, then the soil is avalanching --
    // -- on the terrain below the second bucket layer                        --

    // -- Testing when there are two bucket layers and the soil on the second --
    // -- bucket layer is blocking the movement, then the soil is avalanching --
    // -- on the terrain below the second bucket layer                        --

    // -- Testing when there are two bucket layers and the soil is partially  --
    // -- avalanching on the first bucket layer, then the soil is avalanching --
    // -- on the terrain below the first bucket layer                         --




    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the second bucket layer, then the soil is           --
    // -- avalanching on the terrain below the first bucket layer            --

    // -- Testing when there are two bucket layers and the soil is partially  --
    // -- avalanching on the first bucket layer, then the soil is avalanching --
    // -- on the terrain below the second bucket layer                        --

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the second bucket layer, then the soil is           --
    // -- avalanching on the terrain below the second bucket layer           --

    // -- Testing when there is a lot of soil on the first bucket layer but --
    // -- soil is still avalanching on it                                   --

    // -- Testing when there is a lot of soil on the second bucket layer but --
    // -- soil is still avalanching on it                                    --

    // -- Testing when there are two bucket layers and the soil on the first  --
    // -- bucket layer is blocking the movement, then the soil is avalanching --
    // -- on the first bucket layer                                           --

    // -- Testing when there are two bucket layers and the soil on the second --
    // -- bucket layer is blocking the movement, then the soil is avalanching --
    // -- on the first bucket layer                                           --

    // -- Testing when there are two bucket layers and the soil on the first  --
    // -- bucket layer is blocking the movement, then the soil is avalanching --
    // -- on the second bucket layer                                          --

    // -- Testing when there are two bucket layers and the soil on the second --
    // -- bucket layer is blocking the movement, then the soil is avalanching --
    // -- on the second bucket layer                                          --

    // -- Testing when there are two bucket layers and the soil is partially  --
    // -- avalanching on the first bucket layer, then the soil is avalanching --
    // -- on the first bucket layer                                           --




    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the second bucket layer, then the soil is           --
    // -- avalanching on the first bucket layer                              --

    // -- Testing when there are two bucket layers and the soil is partially  --
    // -- avalanching on the first bucket layer, then the soil is avalanching --
    // -- on the second bucket layer                                          --

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the second bucket layer, then the soil is           --
    // -- avalanching on the second bucket layer                             --

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

    // -- Testing when there are two bucket layers and the soil on the first  --
    // -- bucket layer is blocking the movement, then the soil is fully       --
    // -- avalanching on the second bucket soil layer                         --

    // -- Testing when there are two bucket layers and the soil on the second --
    // -- bucket layer is blocking the movement, then the soil is fully       --
    // -- avalanching on the second bucket soil layer                         --

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

    // -- Testing when there are two bucket layers and the soil on the second --
    // -- bucket layer is blocking the movement, then two bucket layers and   --
    // -- the soil is fully avalanching on the second bucket layer            --

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





    // -- Testing when there are two bucket layers and the soil on the first --
    // -- bucket layer is blocking the movement, then two bucket layers and  --
    // -- the soil on the first bucket layer is blocking the movement, then  --
    // -- two bucket layers and the soil on the second bucket layer is       --
    // -- blocking the movement, then two bucket layers and the soil on the  --
    // -- second bucket layer is blocking the movement, then two bucket      --
    // -- layers and the soil on the first bucket layer is blocking the      --
    // -- movement, then two bucket layers and the soil is fully avalanching --
    // -- on the first bucket layer                                          --

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the first bucket layer, then two bucket layers and  --
    // -- the soil is partially avalanching on the first bucket layer, then  --
    // -- two bucket layers and the soil is partially avalanching on the     --
    // -- second bucket layer, then two bucket layers and the soil is        --
    // -- partially avalanching on the second bucket layer, then two bucket  --
    // -- layers and the soil is partially avalanching on the first bucket   --
    // -- layer, then two bucket layers and the soil is fully avalanching on --
    // -- the second bucket layer                                            --

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

    // -- Testing when there are two bucket layers and the soil is partially --
    // -- avalanching on the second bucket layer, then two bucket layers and --
    // -- the soil is fully avalanching on the second bucket layer           --

    // -- Testing when there is nothing to move --

    // -- Testing randomness of movement --

    // -- Testing that warning is properly sent when soil cannot be moved --




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
    rng.seed(1234);
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
    rng.seed(1234);
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
    rng.seed(1234);
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

    delete sim_out;
}
