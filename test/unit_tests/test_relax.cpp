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
}

TEST(UnitTestRelax, CheckUnstableBodyCell) {
}

TEST(UnitTestRelax, RelaxUnstableBodyCell) {
}

TEST(UnitTestRelax, RelaxBodySoil) {
}
