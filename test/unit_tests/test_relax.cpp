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
}

TEST(UnitTestRelax, RelaxTerrain) {
}

TEST(UnitTestRelax, CheckUnstableBodyCell) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    int status;

    // -- Testing case where there is no bucket and soil is unstable --

    // -- Testing case with first bucket layer and soil avalanche on it --

    // -- Testing case where there is the first bucket layer with bucket soil --
    // -- and soil should avalanche on it                                     --

    // -- Testing case with second bucket layer and soil avalanche on it --

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil should avalanche on it                            --

    // -- Testing case where there are two bucket layers, the first layer --
    // -- being lower and soil should avalanche on it                     --

    // -- Testing case with two bucket layers, the first layer with bucket --
    // -- soil being lower and soil should avalanche on it                 --

    // -- Testing case with two bucket layers, the first layer being lower --
    // -- and soil avalanche on it, while second layer is with bucket soil --

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- the first layer being lower and soil should avalanche on it      --

    // -- Testing case with two bucket layers, the first layer being lower --
    // -- and soil should avalanche on the second bucket layer             --

    // -- Testing case with two bucket layers, the first layer with bucket  --
    // -- soil being lower and soil should avalanche on second bucket layer --

    // -- Testing case with two bucket layers, the first layer being lower --
    // -- and soil should avalanche on the second bucket layer with soil   --

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- the first layer being lower and soil should avalanche on the     --
    // -- second bucket layer                                              --

    // -- Testing case with two bucket layers, the second layer being lower --
    // -- and soil should avalanche on it                                   --

    // -- Testing case with two bucket layers, the second layer with bucket --
    // -- soil being lower and soil should avalanche on it                  --

    // -- Testing case with two bucket layers, the second layer being lower --
    // -- and soil avalanche on it, while first layer is with bucket soil   --

    // -- Testing case where there are two bucket layers with bucket soil, --
    // -- the second layer being lower and soil should avalanche on it     --

    // -- Testing case with two bucket layers, the second layer being lower --
    // -- and soil should avalanche on the first bucket layer               --

    // -- Testing case with two bucket layers, the second layer with bucket --
    // -- soil being lower and soil should avalanche on first bucket layer  --

    // -- Testing case with two bucket layers, the second layer being lower --
    // -- and soil should avalanche on the first bucket layer with soil     --

    // -- Testing case where there are two bucket layers with bucket soil,  --
    // -- second layer being lower and soil avalanche on first bucket layer --

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil fully cover the space between the two layers, first bucket   --
    // -- layer is too high for soil to avalanche                           --

    // -- Testing case where there are two bucket layers with soil, second  --
    // -- layer being lower and soil fully cover the space between the two  --
    // -- layers, first bucket soil layer is too high for soil to avalanche --

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil fully cover the space between the two layers, second bucket --
    // -- layer is too high for soil to avalanche                          --

    // -- Testing case where there are two bucket layers with soil, first    --
    // -- layer being lower and soil fully cover the space between the two   --
    // -- layers, second bucket soil layer is too high for soil to avalanche --

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil fully cover the space between the two layers, but soil can   --
    // -- avalanche on the first bucket layer                               --

    // -- Testing case where there are two bucket layers with soil, second  --
    // -- layer being lower and soil fully cover the space between the two  --
    // -- layers, but the soil can avalanche on the first bucket soil layer --

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil fully cover the space between the two layers, but the soil  --
    // -- can avalanche on the second bucket layer                         --

    // -- Testing case where there are two bucket layers with soil, first    --
    // -- layer being lower and soil fully cover the space between the two   --
    // -- layers, but the soil can avalanche on the second bucket soil layer --

    // -- Testing case where there is no bucket and soil is not unstable --

    // -- Testing case with first bucket layer and soil is not unstable --

    // -- Testing case where there is the first bucket layer with bucket --
    // -- soil and soil is not unstable                                  --

    // -- Testing case with second bucket layer and soil is not unstable --

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil is not unstable                                   --

    // -- Testing case with two bucket layers and soil is not unstable (1) --

    // -- Testing case with two bucket layers and soil is not unstable (2) --

    // -- Testing case with two bucket layers and soil is not unstable (3) --

    // -- Testing case with two bucket layers and soil is not unstable (4) --



    delete sim_out;
}

TEST(UnitTestRelax, RelaxUnstableBodyCell) {
}

TEST(UnitTestRelax, RelaxBodySoil) {
}
