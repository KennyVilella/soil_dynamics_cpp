/*
This file implements unit tests for the functions in intersecting_cells.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include "gtest/gtest.h"
#include "src/intersecting_cells.cpp"

// Setting RNG
std::mt19937 rng;

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
    EXPECT_NEAR(sim_out->terrain_[8][17], 0.25, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[12][17], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][17], 0.05, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[13][19], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[14][20], 0.1, 1e-5);
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
    EXPECT_NEAR(sim_out->terrain_[7][21], 0.2, 1e-5);
    sim_out->terrain_[11][17] = 0.0;
    sim_out->terrain_[10][17] = 0.0;
    sim_out->terrain_[8][17] = 0.0;
    sim_out->terrain_[12][17] = 0.0;
    sim_out->terrain_[13][17] = 0.0;
    sim_out->terrain_[13][19] = 0.0;
    sim_out->terrain_[14][20] = 0.0;
    sim_out->terrain_[7][21] = 0.0;
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
