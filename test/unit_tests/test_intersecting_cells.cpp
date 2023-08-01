/*
This file implements unit tests for the functions in intersecting_cells.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include "gtest/gtest.h"
#include "src/intersecting_cells.cpp"

TEST(UnitTestIntersectingCells, MoveBodySoil) {
}

TEST(UnitTestIntersectingCells, MoveIntersectingBodySoil) {
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
}
