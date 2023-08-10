/*
This file implements unit tests for the functions in relax.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include "gtest/gtest.h"
#include "src/relax.cpp"

TEST(UnitTestRelax, LocateUnstableTerrainCell) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    sim_out->impact_area_[0][0] = 2;
    sim_out->impact_area_[0][1] = 17;
    sim_out->impact_area_[1][0] = 2;
    sim_out->impact_area_[1][1] = 17;
    sim_out->terrain_[2][2] = -0.1;
    sim_out->terrain_[5][2] = -0.2;
    sim_out->terrain_[11][13] = -0.2;
    sim_out->terrain_[5][13] = 0.2;
    sim_out->terrain_[7][13] = 0.1;
    sim_out->terrain_[15][5] = -0.4;
    sim_out->terrain_[15][6] = -0.2;

    // -- Testing that all unstable cells are properly located --
    auto unstable_cells = soil_simulator::LocateUnstableTerrainCell(
        sim_out, 0.1, 1e-5);
    EXPECT_TRUE((unstable_cells[0] == std::vector<int> {4, 2}));
    EXPECT_TRUE((unstable_cells[1] == std::vector<int> {5, 3}));
    EXPECT_TRUE((unstable_cells[2] == std::vector<int> {5, 13}));
    EXPECT_TRUE((unstable_cells[3] == std::vector<int> {6, 2}));
    EXPECT_TRUE((unstable_cells[4] == std::vector<int> {10, 13}));
    EXPECT_TRUE((unstable_cells[5] == std::vector<int> {11, 12}));
    EXPECT_TRUE((unstable_cells[6] == std::vector<int> {11, 14}));
    EXPECT_TRUE((unstable_cells[7] == std::vector<int> {12, 13}));
    EXPECT_TRUE((unstable_cells[8] == std::vector<int> {14, 5}));
    EXPECT_TRUE((unstable_cells[9] == std::vector<int> {14, 6}));
    EXPECT_TRUE((unstable_cells[10] == std::vector<int> {15, 4}));
    EXPECT_TRUE((unstable_cells[11] == std::vector<int> {15, 6}));
    EXPECT_TRUE((unstable_cells[12] == std::vector<int> {15, 7}));
    EXPECT_TRUE((unstable_cells[13] == std::vector<int> {16, 5}));
    EXPECT_TRUE((unstable_cells[14] == std::vector<int> {16, 6}));
    EXPECT_EQ(unstable_cells.size(), 15);

    delete sim_out;
}

TEST(UnitTestRelax, CheckUnstableTerrainCell) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    int status;

    // -- Testing case where there is no bucket and soil is not unstable --
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);

    // -- Testing case where there is no bucket and soil is unstable --
    sim_out->terrain_[10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 400);
    sim_out->terrain_[10][15] = 0.0;

    // -- Testing case where there is first bucket layer with space under it --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 141);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;

    // -- Testing case with first bucket layer and soil avalanche on it --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 142);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;

    // -- Testing case with first bucket layer and it is high enough to --
    // -- prevent the soil from avalanching                             --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;

    // -- Testing case where there is first bucket layer with bucket soil --
    // -- and it has space under it                                       --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.3;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 131);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there is first bucket layer with bucket soil --
    // -- and soil should avalanche on it                                 --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.3;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 132);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there is first bucket layer with bucket soil --
    // -- and it is high enough to prevent the soil from avalanching      --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case with second bucket layer and it has space under it --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.1;
    sim_out->body_[3][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 221);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case with second bucket layer and soil avalanche on it --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 222);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case with second bucket layer and it is high enough to --
    // -- prevent the soil from avalanching                              --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case where there is second bucket layer with bucket soil --
    // -- and it has space under it                                        --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.3;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 211);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there is second bucket layer with bucket soil --
    // -- and soil should avalanche on it                                  --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.3;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 212);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there is second bucket layer with bucket soil --
    // -- and it is high enough to prevent the soil from avalanching       --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, first layer being lower --
    // -- and it has space under it                                    --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 321);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, first layer being lower --
    // -- and soil should avalanche on the second bucket layer         --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 322);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, first layer being lower and    --
    // -- second bucket layer is high enough to prevent soil from avalanching --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, first layer with bucket soil --
    // -- being lower and has space under it                                --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 321);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case with two bucket layers, first layer with bucket soil --
    // -- being lower, and soil should avalanche on the second bucket layer --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 322);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case with two bucket layers, first layer with bucket soil --
    // -- being lower, second bucket layer is high enough to prevent soil   --
    // -- from avalanching                                                  --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- it has space under it, while second layer is with bucket soil    --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 311);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should avalanche on second bucket layer with bucket soil    --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 312);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- second bucket layer with bucket soil is high enough to prevent   --
    // -- soil from avalanching                                            --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- first layer being lower and it has space under it                --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 311);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- first layer being lower, soil should avalanche on second         --
    // -- bucket layer                                                     --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 312);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- first layer being lower, second bucket layer is high enough to   --
    // -- prevent soil from avalanching                                    --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- it has space under it                                             --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 341);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil should avalanche on the first bucket layer                   --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 342);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, second layer being lower and  --
    // -- first bucket layer is high enough to prevent soil from avalanching --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, second layer with bucket soil --
    // -- being lower and it has space under it                              --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 341);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, second layer with bucket soil --
    // -- being lower, soil should avalanche on the first bucket layer       --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 342);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, second layer with bucket soil --
    // -- being lower, first bucket layer is high enough to prevent soil     --
    // -- from avalanching                                                   --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- has space under it, while the first layer is with bucket soil     --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 331);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil should avalanche on first bucket layer with bucket soil      --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 332);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- first bucket layer with bucket soil is high enough to prevent     --
    // -- soil from avalanching                                             --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- second layer being lower and has space under it                  --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 331);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- second layer being lower, soil avalanche on first bucket layer   --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 332);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- second layer being lower, first bucket layer is high enough to   --
    // -- prevent soil from avalanching                                    --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing edge case where a lot of space under the bucket is present --
    sim_out->terrain_[10][15] = -1.0;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.6, 1e-5);
    EXPECT_EQ(status, 141);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;

    // -- Testing edge case for soil avalanching on the bucket --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.1;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;

    // -- Testing edge case for soil avalanching on terrain --
    sim_out->terrain_[10][15] = -0.4;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.4, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][15] = 0.0;

    delete sim_out;
}

TEST(UnitTestRelax, RelaxUnstableTerrainCell) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    // -- Testing case where there is no bucket and soil is unstable --
    sim_out->terrain_[10][14] = 0.4;
    sim_out->terrain_[10][15] = 0.1;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 400, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.2, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and it has space under it, the soil fully avalanche        --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.3;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 211, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and it has space under it, the soil partially avalanche    --
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.5;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 211, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil should avalanche on it                            --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.3;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 212, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there is the second bucket layer and it has --
    // -- space under it, the soil fully avalanche                       --
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.3;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 221, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case where there is the second bucket layer and it has --
    // -- space under it, the soil partially avalanche                   --
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 221, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case where there is the second bucket layer and soil --
    // -- should avalanche on it                                       --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 0, 0};
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 222, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there is the first bucket layer with bucket --
    // -- soil and it has space under it, the soil fully avalanche       --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.3;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 131, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there is the first bucket layer with bucket --
    // -- soil and it has space under it, the soil partially avalanche   --
    sim_out->terrain_[10][15] = -0.7;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_soil_[0][10][15] = -0.1;
    sim_out->body_soil_[1][10][15] = 0.3;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 131, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.3, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there is the first bucket layer with bucket --
    // -- soil and soil should avalanche on it                           --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.3;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 132, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there is the first bucket layer and it has --
    // -- space under it, the soil fully avalanche                      --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.5;
    sim_out->body_[1][10][15] = -0.2;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 141, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.5, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;

    // -- Testing case where there is the first bucket layer and it has --
    // -- space under it, the soil partially avalanche                  --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.1;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 141, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;

    // -- Testing case where there is the first bucket layer and soil --
    // -- should avalanche on it                                      --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 0, 0};
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 142, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {0, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- the first layer being lower and it has space under it,           --
    // -- the soil fully avalanche                                         --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 311, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- the first layer being lower and it has space under it,           --
    // -- the soil partially avalanche                                     --
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.3;
    sim_out->body_[3][10][15] = 0.5;
    sim_out->body_soil_[0][10][15] = -0.1;
    sim_out->body_soil_[1][10][15] = 0.2;
    sim_out->body_soil_[2][10][15] = 0.5;
    sim_out->body_soil_[3][10][15] = 0.7;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 311, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- the first layer being lower, and soil should avalanche on the    --
    // -- second bucket layer                                              --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 312, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers, the first layer --
    // -- being lower and it has space under it, the soil fully avalanche --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 321, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers, the first layer     --
    // -- being lower and it has space under it, the soil partially avalanche --
    sim_out->terrain_[10][15] = -0.5;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.2;
    sim_out->body_[3][10][15] = 0.4;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 321, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers, the first layer   --
    // -- being lower, and soil should avalanche on the second bucket layer --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 0, 0};
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 322, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers, the second layer  --
    // -- being lower and has space under it, while the first layer is with --
    // -- bucket soil, the soil fully avalanche                             --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 331, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there are two bucket layers, the second layer  --
    // -- being lower and has space under it, while the first layer is with --
    // -- bucket soil, the soil partially avalanche                         --
    sim_out->terrain_[10][15] = -0.9;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.1;
    sim_out->body_soil_[1][10][15] = 0.0;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 331, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there are two bucket layers, the second layer --
    // -- being lower, and soil should avalanche on the first bucket layer --
    // -- with bucket soil                                                 --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 332, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there are two bucket layers, the second layer --
    // -- being lower and it has space under it, the soil fully avalanche  --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 341, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers, the second layer    --
    // -- being lower and it has space under it, the soil partially avalanche --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 341, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers, the second layer --
    // -- being lower, and soil should avalanche on the first bucket layer --
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 0, 0};
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 342, 0.1, 10, 14, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {0, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;

    delete sim_out;
}

TEST(UnitTestRelax, RelaxTerrain) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    soil_simulator::SimParam sim_param(0.785, 3, 4);
    sim_out->impact_area_[0][0] = 4;
    sim_out->impact_area_[0][1] = 16;
    sim_out->impact_area_[1][0] = 9;
    sim_out->impact_area_[1][1] = 20;

    // -- Testing case with no bucket and soil is unstable --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with first bucket layer and it has space under it --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with first bucket layer and soil avalanche on it --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 0, 0};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {0, 10, 15}));
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with the first bucket layer and it is high enough to --
    // --  prevent the soil from avalanching                                --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case where there is the first bucket layer with bucket soil --
    // -- and it has space under it                                           --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case where there is the first bucket layer with bucket soil --
    // -- and soil should avalanche on it                                     --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.3;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->terrain_[9][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case where there is the first bucket layer with bucket soil --
    // -- and it is high enough to prevent the soil from avalanching          --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with second bucket layer and it has space under it --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.1;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with second bucket layer and soil avalanche on it --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 0, 0};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with second bucket layer and it is high enough to --
    // --  prevent the soil from avalanching                             --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and it has space under it                                  --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.1;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil should avalanche on it                            --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and it is high enough to prevent the soil from avalanching --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- it has space under it                                            --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should avalanche on the second bucket layer                 --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 0, 0};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, first layer being lower and    --
    // -- second bucket layer is high enough to prevent soil from avalanching --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, first layer with bucket soil --
    // -- being lower and has space under it                                --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.5, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, first layer with bucket soil --
    // -- being lower and soil should avalanche on the second bucket layer  --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {2, 10, 15}));
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
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

    // -- Testing case with two bucket layers, first layer with bucket soil --
    // -- being lower and the second bucket layer is high enough to prevent --
    // -- soil from avalanching                                             --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.5, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- it has space under it, while second layer is with bucket soil    --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = 0.4;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should avalanche on second bucket layer with bucket soil    --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- second bucket layer with bucket soil is high enough to prevent   --
    // -- the soil from avalanching                                        --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case where there are two bucket layers with bucket soil, --
    // first layer being lower and it has space under it                   --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.1;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(2, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_[1] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
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

    // -- Testing case where there are two bucket layers with bucket soil,  --
    // -- first layer being lower and soil avalanche on second bucket layer --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(2, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_[1] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
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

    // -- Testing case where there are two bucket layers with bucket soil,   --
    // -- first layer being lower and the second bucket layer is high enough --
    // -- to prevent the soil from avalanching                               --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(2, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_[1] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
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

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- it has space under it                                             --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil should avalanche on the first bucket layer                   --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 0, 0};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {0, 10, 15}));
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, second layer being lower and  --
    // -- first bucket layer is high enough to prevent soil from avalanching --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, second layer with bucket soil --
    // -- being lower and it has space under it                              --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.5, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, second layer with bucket soil --
    // -- being lower and soil should avalanche on the first bucket layer    --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.5, 1e-5);
    EXPECT_TRUE((sim_out->body_soil_pos_[1] == std::vector<int> {0, 10, 15}));
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
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

    // -- Testing case with two bucket layers, second layer with bucket soil --
    // -- being lower and first bucket layer is high enough to prevent the   --
    // -- soil from avalanching                                              --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.5, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- has space under it, while the first layer is with bucket soil     --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil should avalanche on the first bucket layer with bucket soil  --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- first bucket layer with bucket soil is high enough to prevent the --
    // --soil from avalanching                                              --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(1, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- second layer being lower and has space under it                  --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(2, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_[1] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.5, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
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

    // -- Testing case where there are two bucket layers with bucket soil,  --
    // -- second layer being lower and soil avalanche on first bucket layer --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(2, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_[1] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.5, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
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

    // -- Testing case where there are two bucket layers with bucket soil,  --
    // -- second layer being lower and first bucket layer is high enough to --
    // -- prevent the soil from avalanching                                 --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    sim_out->body_soil_pos_.resize(2, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 10, 15};
    sim_out->body_soil_pos_[1] = std::vector<int> {2, 10, 15};
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.5, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
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

    // -- Testing edge case where a lot of space under the bucket is present --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->terrain_[9][15] = 0.0;
    sim_out->terrain_[10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing edge case for soil avalanching on terrain --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.4;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[10][16] = 0.0;
    sim_out->terrain_[9][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    // -- Testing randomization --
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.2;
    sim_out->relax_area_[0][0] = 2;
    sim_out->relax_area_[0][1] = 20;
    sim_out->relax_area_[1][0] = 2;
    sim_out->relax_area_[1][1] = 20;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    soil_simulator::rng.seed(201);
    sim_out->terrain_[10][15] = -0.2;
    sim_out->terrain_[10][16] = 0.0;
    soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], -0.1, 1e-5);
    sim_out->terrain_[10][15] = 0.0;
    sim_out->terrain_[11][15] = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            EXPECT_NEAR(sim_out->terrain_[ii][jj], 0.0, 1e-5);

    delete sim_out;
}

TEST(UnitTestRelax, CheckUnstableBodyCell) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    int status;

    // -- Testing case where there is no bucket and soil is unstable --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 40);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    // -- Testing case with first bucket layer and soil avalanche on it --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 14);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    // -- Testing case where there is the first bucket layer with bucket soil --
    // -- and soil should avalanche on it                                     --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_soil_[0][10][15] = -0.1;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 13);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case with second bucket layer and soil avalanche on it --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 22);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil should avalanche on it                            --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = -0.1;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 21);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers, the first layer --
    // -- being lower and soil should avalanche on it                     --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.2;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 34);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    // -- Testing case with two bucket layers, the first layer with bucket --
    // -- soil being lower and soil should avalanche on it                 --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.2;
    sim_out->body_soil_[0][10][15] = -0.1;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 33);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case with two bucket layers, the first layer being lower --
    // -- and soil avalanche on it, while second layer is with bucket soil --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.2;
    sim_out->body_soil_[2][10][15] = 0.2;
    sim_out->body_soil_[3][10][15] = 0.3;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 34);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- the first layer being lower and soil should avalanche on it      --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.2;
    sim_out->body_soil_[0][10][15] = -0.1;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.2;
    sim_out->body_soil_[3][10][15] = 0.3;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 33);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, the first layer being lower --
    // -- and soil should avalanche on the second bucket layer             --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 32);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    // -- Testing case with two bucket layers, the first layer with bucket  --
    // -- soil being lower and soil should avalanche on second bucket layer --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 32);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case with two bucket layers, the first layer being lower --
    // -- and soil should avalanche on the second bucket layer with soil   --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = -0.1;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 31);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- the first layer being lower and soil should avalanche on the     --
    // -- second bucket layer                                              --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = -0.1;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 31);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, the second layer being lower --
    // -- and soil should avalanche on it                                   --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 32);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    // -- Testing case with two bucket layers, the second layer with bucket --
    // -- soil being lower and soil should avalanche on it                  --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = -0.1;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 31);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, the second layer being lower --
    // -- and soil avalanche on it, while first layer is with bucket soil   --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[0][10][15] = 0.2;
    sim_out->body_soil_[1][10][15] = 0.3;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 32);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- the second layer being lower and soil should avalanche on it     --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[0][10][15] = 0.2;
    sim_out->body_soil_[1][10][15] = 0.3;
    sim_out->body_soil_[2][10][15] = -0.1;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 31);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, the second layer being lower --
    // -- and soil should avalanche on the first bucket layer               --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 34);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    // -- Testing case with two bucket layers, the second layer with bucket --
    // -- soil being lower and soil should avalanche on first bucket layer  --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 34);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, the second layer being lower --
    // -- and soil should avalanche on the first bucket layer with soil     --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.1;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 33);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with bucket soil,  --
    // -- second layer being lower and soil avalanche on first bucket layer --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.1;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 33);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil fully cover the space between the two layers, first bucket   --
    // -- layer is too high for soil to avalanche                           --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = 0.2;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with soil, second  --
    // -- layer being lower and soil fully cover the space between the two  --
    // -- layers, first bucket soil layer is too high for soil to avalanche --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.1;
    sim_out->body_soil_[1][10][15] = 0.2;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil fully cover the space between the two layers, second bucket --
    // -- layer is too high for soil to avalanche                          --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.1;
    sim_out->body_[3][10][15] = 0.5;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with soil, first    --
    // -- layer being lower and soil fully cover the space between the two   --
    // -- layers, second bucket soil layer is too high for soil to avalanche --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.1;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.1;
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil fully cover the space between the two layers, but soil can   --
    // -- avalanche on the first bucket layer                               --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 34);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with soil, second  --
    // -- layer being lower and soil fully cover the space between the two  --
    // -- layers, but the soil can avalanche on the first bucket soil layer --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.1;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 33);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil fully cover the space between the two layers, but the soil  --
    // -- can avalanche on the second bucket layer                         --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 32);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case where there are two bucket layers with soil, first    --
    // -- layer being lower and soil fully cover the space between the two   --
    // -- layers, but the soil can avalanche on the second bucket soil layer --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = -0.1;
    sim_out->body_soil_[3][10][15] = 0.0;
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 31);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case where there is no bucket and soil is not unstable --
    sim_out->terrain_[10][15] = 0.1;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    // -- Testing case with first bucket layer and soil is not unstable --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = 0.1;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    // -- Testing case where there is the first bucket layer with bucket --
    // -- soil and soil is not unstable                                  --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.1;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;

    // -- Testing case with second bucket layer and soil is not unstable --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.1;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil is not unstable                                   --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.1;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers and soil is not unstable (1) --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.1;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers and soil is not unstable (2) --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.2;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = 0.1;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers and soil is not unstable (3) --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.1;
    sim_out->body_soil_[2][10][15] = 0.1;
    sim_out->body_soil_[3][10][15] = 0.3;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.4;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;

    // -- Testing case with two bucket layers and soil is not unstable (4) --
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.2;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.4;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    delete sim_out;
}

TEST(UnitTestRelax, RelaxUnstableBodyCell) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    std::vector<std::vector<int>> *body_soil_pos = (
        new std::vector<std::vector<int>>);

    // -- Testing the case with no bucket and soil should partially avalanche --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = 0.0;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 40, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    // -- Testing the case with no bucket and soil should fully avalanche --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.2;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 40, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;

    // -- Testing the case with first bucket layer and soil should partially --
    // -- avalanche on it                                                    --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = 0.0;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 14, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {0, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with first bucket layer and soil should fully --
    // -- avalanche on it                                                --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 14, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {0, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there is the first bucket layer with bucket --
    // -- soil and soil should partially avalanche on it                     --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = 0.0;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 13, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there is the first bucket layer with bucket --
    // -- soil and soil should fully avalanche on it                         --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = -0.1;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 13, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with second bucket layer and soil  should --
    // -- partially avalanche on it                                  --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 22, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {2, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with second bucket layer and soil should fully --
    // -- avalanche on it                                                 --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 22, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {2, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there is the second bucket layer with bucket --
    // -- soil and soil should partially avalanche on it                      --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = -0.1;
    sim_out->body_soil_[3][10][15] = 0.0;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 21, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there is the second bucket layer with bucket --
    // -- soil and soil should fully avalanche on it                          --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = -0.1;
    sim_out->body_soil_[3][10][15] = 0.0;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 21, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, first layer being lower --
    // -- and soil should partially avalanche on it                        --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.3;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {0, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, first layer being lower --
    // -- and soil should fully avalanche on it                            --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.1;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {0, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case whith two bucket layers, first layer being lower --
    // -- and soil should partially avalanche on it but there is not enough --
    // -- space for all the soil                                            --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.5;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {0, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, first layer being lower --
    // -- and soil should partially avalanche on the second bucket layer   --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.5;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.4, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {2, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, first layer being lower --
    // -- and soil should fully avalanche on the second bucket layer       --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.5;
    sim_out->body_soil_[0][10][14] = 0.5;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.2;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {2, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil should partially avalanche on it   --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.3;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.5;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil should fully avalanche on it       --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.3;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = 0.2;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.5;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil should partially avalanche on it   --
    // -- but there is not enough space for all the soil                      --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.9;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = 0.2;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.5;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil should partially avalanche on the  --
    // -- second bucket layer                                                 --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.9;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.5;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil should fully avalanche on the      --
    // -- second bucket layer                                                 --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.7;
    sim_out->body_soil_[0][10][14] = 0.7;
    sim_out->body_soil_[1][10][14] = 0.9;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.5;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, second layer being lower --
    // -- and soil should partially avalanche on it                         --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.3;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {2, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, second layer being lower --
    // -- and soil should fully avalanche on it                             --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.3;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = 0.2;
    sim_out->body_[1][10][15] = 0.4;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.0, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.2, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {2, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, second layer being lower --
    // -- and soil should partially avalanche on it but there is not enough --
    // -- space for all the soil                                            --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = 0.2;
    sim_out->body_[1][10][15] = 0.4;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.2, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {2, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, second layer being lower --
    // -- and soil should partially avalanche on the first bucket layer     --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.4;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.6, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {0, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, second layer being lower --
    // -- and soil should fully avalanche on the first bucket layer         --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.6;
    sim_out->body_soil_[0][10][14] = 0.6;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.4, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {0, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there are two bucket layers with bucket soil --
    // -- second layer being lower and soil should partially avalanche on it  --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.6;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.3;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = -0.2;
    sim_out->body_soil_[3][10][15] = -0.1;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there are two bucket layers with bucket soil --
    // -- the second layer being lower and soil should fully avalanche on it  --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.1;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = -0.2;
    sim_out->body_soil_[3][10][15] = -0.1;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there are two bucket layers with bucket soil --
    // -- second layer being lower and soil should partially avalanche on it  --
    // -- but there is not enough space for all the soil                      --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = -0.2;
    sim_out->body_soil_[3][10][15] = -0.1;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there are two bucket layers with bucket soil --
    // -- second layer being lower and soil should partially avalanche on the --
    // -- first bucket layer                                                  --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.5;
    sim_out->body_soil_[2][10][15] = -0.2;
    sim_out->body_soil_[3][10][15] = -0.1;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there are two bucket layers with bucket soil --
    // -- the second layer being lower and soil should fully avalanche on the --
    // -- first bucket layer                                                  --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.6;
    sim_out->body_soil_[0][10][14] = 0.6;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.4;
    sim_out->body_soil_[2][10][15] = -0.2;
    sim_out->body_soil_[3][10][15] = -0.1;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil should partially avalanche on it   --
    // -- but there is no space at all                                        --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = 0.1;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.8;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.8, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case where there are two bucket layers with bucket soil --
    // -- second layer being lower and soil should partially avalanche on it  --
    // -- but there is no space at all                                        --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = -0.2;
    sim_out->body_soil_[3][10][15] = 0.1;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, first layer being lower --
    // -- and soil fully cover the space between the two layers, the soil  --
    // -- can partially avalanche to the second bucket layer               --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.2;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = 0.1;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {2, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, first layer being lower --
    // -- and soil fully cover the space between the two layers, the soil  --
    // -- can partially avalanche to the second bucket soil layer          --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.2;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = 0.1;
    sim_out->body_soil_[2][10][15] = 0.2;
    sim_out->body_soil_[3][10][15] = 0.4;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.6, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, second layer being lower --
    // -- and soil fully cover the space between the two layers, the soil   --
    // -- can partially avalanche to the first bucket layer                 --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = 0.4;
    sim_out->body_[1][10][15] = 0.5;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = -0.2;
    sim_out->body_soil_[3][10][15] = 0.4;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.4, 1e-5);
    EXPECT_TRUE(((*body_soil_pos)[0] == std::vector<int> {0, 10, 15}));
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing the case with two bucket layers, second layer being lower --
    // -- and soil fully cover the space between the two layers, the soil   --
    // -- can partially avalanche to the first bucket soil layer            --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.8;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = 0.4;
    sim_out->body_[1][10][15] = 0.5;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = 0.5;
    sim_out->body_soil_[1][10][15] = 0.6;
    sim_out->body_soil_[2][10][15] = -0.2;
    sim_out->body_soil_[3][10][15] = 0.4;
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 10, 14, 0, 10, 15, grid, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.4, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    sim_out->terrain_[10][14] = 0.0;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][14] = 0.0;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.0;
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    delete sim_out;
    delete body_soil_pos;
}

TEST(UnitTestRelax, RelaxBodySoil) {
}
