/*
This file implements unit tests for the functions in relax.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <random>
#include "gtest/gtest.h"
#include "soil_simulator/relax.hpp"
#include "soil_simulator/utils.hpp"
#include "test/unit_tests/utility.hpp"

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

    // Test: RE-CUT-1
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);

    // Test: RE-CUT-2
    sim_out->terrain_[10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 40);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(sim_out, {{10, 15}}, {}, {});

    // Test: RE-CUT-3
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 10);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-CUT-4
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 14);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-CUT-5
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-CUT-6
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.3;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 10);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-7
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.3;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 13);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-8
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-9
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.1;
    sim_out->body_[3][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 20);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{2, 10, 15}}, {});

    // Test: RE-CUT-10
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 22);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{2, 10, 15}}, {});

    // Test: RE-CUT-11
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{2, 10, 15}}, {});

    // Test: RE-CUT-12
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.3;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 20);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-13
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.3;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 21);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-14
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-15
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-CUT-16
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 34);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-CUT-17
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = 0.2;
    sim_out->body_[3][10][15] = 0.4;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 34);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-CUT-18
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-19
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 34);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-20
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = 0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 34);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-21
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-22
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 33);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-23
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 33);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-24
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.4;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-25
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.4;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 32);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-26
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.4;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-27
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
    EXPECT_EQ(status, 30);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-28
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
    EXPECT_EQ(status, 33);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-29
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
    EXPECT_EQ(status, 33);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-30
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.4;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-31
    sim_out->terrain_[10][15] = -0.7;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.4;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 31);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-32
    sim_out->terrain_[10][15] = -0.7;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.4;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = 0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-33
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-CUT-34
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 32);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-CUT-35
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 32);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-CUT-36
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-37
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 32);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-38
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = 0.0;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 32);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-39
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-40
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 31);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-41
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 31);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-42
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.4;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-43
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.4;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 34);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-44
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.4;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-45
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
    EXPECT_EQ(status, 30);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-46
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
    EXPECT_EQ(status, 31);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-47
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
    EXPECT_EQ(status, 31);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-48
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.4;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-49
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.4;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 33);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-50
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.4;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-51
    sim_out->terrain_[10][15] = -1.0;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.2;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.6, 1e-5);
    EXPECT_EQ(status, 10);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-CUT-52
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.1;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-CUT-53
    sim_out->terrain_[10][15] = -0.4;
    status = soil_simulator::CheckUnstableTerrainCell(
        sim_out, 10, 15, -0.4, 1e-5);
    EXPECT_EQ(status, 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(sim_out, {{10, 15}}, {}, {});

    delete sim_out;
}

TEST(UnitTestRelax, RelaxUnstableTerrainCell) {
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
    
    // Test: RE-RUT-1
    sim_out->terrain_[10][14] = 0.4;
    sim_out->terrain_[10][15] = 0.1;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 40, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.2, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {}, {});

    // Test: RE-RUT-2
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.5;
    sim_out->body_[1][10][15] = -0.2;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 10, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.5, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-RUT-3
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.1;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 10, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-RUT-4
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.2;
    auto posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 14, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RUT-5
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.3;
    auto  pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 10, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RUT-6
    sim_out->terrain_[10][15] = -0.7;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_soil_[0][10][15] = -0.1;
    sim_out->body_soil_[1][10][15] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.4});
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 10, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.3, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RUT-7
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.3;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.2});
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 13, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RUT-8
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.3;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 20, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{2, 10, 15}}, {});

    // Test: RE-RUT-9
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 20, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{2, 10, 15}}, {});

    // Test: RE-RUT-10
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 22, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RUT-11
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.3;
    auto pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 20, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RUT-12
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.5;
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 20, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RUT-13
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.3;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.2});
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 21, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RUT-14
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 30, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-RUT-15
    sim_out->terrain_[10][15] = -0.5;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.2;
    sim_out->body_[3][10][15] = 0.4;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 30, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-RUT-16
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 34, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}});

    // Test: RE-RUT-17
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.3;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 34, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}});

    // Test: RE-RUT-18
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 32, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-19
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.4;
    sim_out->body_[2][10][15] = 0.4;
    sim_out->body_[3][10][15] = 0.7;
    sim_out->body_soil_[0][10][15] = -0.4;
    sim_out->body_soil_[1][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = 0.7;
    sim_out->body_soil_[3][10][15] = 0.9;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 33, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.9, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-20
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 33, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-21
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.4;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 31, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-22
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 30, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-RUT-23
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 30, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-RUT-24
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 32, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 2, 10, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{2, 10, 15}});

    // Test: RE-RUT-25
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.4;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 32, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{2, 10, 15}});

    // Test: RE-RUT-26
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[2][10][15] = -0.7;
    sim_out->body_soil_[3][10][15] = -0.6;
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 34, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.6, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-27
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[0][10][15] = -0.1;
    sim_out->body_soil_[1][10][15] = 0.5;
    sim_out->body_soil_[2][10][15] = -0.7;
    sim_out->body_soil_[3][10][15] = -0.6;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.6});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 31, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-28
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.5;
    sim_out->body_[1][10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[0][10][15] = -0.4;
    sim_out->body_soil_[1][10][15] = 0.5;
    sim_out->body_soil_[2][10][15] = -0.7;
    sim_out->body_soil_[3][10][15] = -0.6;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.9});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 31, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.5, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-29
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.5;
    sim_out->body_[1][10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[0][10][15] = -0.4;
    sim_out->body_soil_[1][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.7;
    sim_out->body_soil_[3][10][15] = -0.5;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 33, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.5, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    delete bucket;
    delete sim_out;
}

TEST(UnitTestRelax, RelaxTerrain) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    soil_simulator::SimParam sim_param(0.785, 3, 4);
    sim_out->impact_area_[0][0] = 4;
    sim_out->impact_area_[0][1] = 16;
    sim_out->impact_area_[1][0] = 9;
    sim_out->impact_area_[1][1] = 20;
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};

    // Test: RE-RT-1
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.1;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 10);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 15);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {}, {});

    // Test: RE-RT-2
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {}, {});

    // Test: RE-RT-3
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}}, {});

    // Test: RE-RT-4
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    auto posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RT-5
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}}, {});

    // Test: RE-RT-6
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
    auto pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RT-7
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
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.2});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RT-8
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
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.5});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RT-9
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.1;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{2, 10, 15}}, {});

    // Test: RE-RT-10
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RT-11
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{2, 10, 15}}, {});

    // Test: RE-RT-12
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
    auto pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.4});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RT-13
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
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.3});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RT-14
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
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RT-15
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.2;
    sim_out->body_[3][10][15] = 0.4;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-RT-16
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
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}});

    // Test: RE-RT-17
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
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    auto posB = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 0, 10, 15, posA, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-18
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.4;
    sim_out->body_[3][10][15] = 0.5;
    sim_out->body_soil_[2][10][15] = 0.5;
    sim_out->body_soil_[3][10][15] = 0.7;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{2, 10, 15}});

    // Test: RE-RT-19
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
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-20
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
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posB[0], posB[1], posB[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-21
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.2;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.1;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}});

    // Test: RE-RT-22
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
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}});

    // Test: RE-RT-23
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
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-24
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.5;
    sim_out->body_[1][10][15] = -0.4;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = -0.4;
    sim_out->body_soil_[1][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.4;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.4});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-25
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
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.3});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-26
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
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posB[0], posB[1], posB[2], 0.3});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-27
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = 0.4;
    sim_out->body_[1][10][15] = 0.7;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-RT-28
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
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 2, 10, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{2, 10, 15}});

    // Test: RE-RT-29
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
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[0], 2, 10, 15, posA, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-30
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = 0.4;
    sim_out->body_[1][10][15] = 0.5;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.2;
    sim_out->body_soil_[0][10][15] = 0.5;
    sim_out->body_soil_[1][10][15] = 0.6;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.6, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}});

    // Test: RE-RT-31
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = 0.4;
    sim_out->body_[1][10][15] = 0.5;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = 0.5;
    sim_out->body_soil_[1][10][15] = 0.6;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-32
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
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posB[0], posB[1], posB[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-33
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.4;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{2, 10, 15}});

    // Test: RE-RT-34
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.1;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.2);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 14}, {10, 15}, {10, 16}},
        {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RT-35
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
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-36
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.4;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-37
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-38
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
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posB[0], posB[1], posB[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-39
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.2;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 14}, {10, 15}, {10, 16}}, {{0, 10, 15}}, {});

    // Test: RE-RT-40
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.4;
    sim_out->relax_area_[0][0] = 10;
    sim_out->relax_area_[0][1] = 15;
    sim_out->relax_area_[1][0] = 10;
    sim_out->relax_area_[1][1] = 15;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->relax_area_[0][0], 5);
    EXPECT_EQ(sim_out->relax_area_[0][1], 15);
    EXPECT_EQ(sim_out->relax_area_[1][0], 10);
    EXPECT_EQ(sim_out->relax_area_[1][1], 20);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {}, {});

    // Test: RE-RT-41
    soil_simulator::rng.seed(200);
    sim_out->terrain_[10][15] = -0.2;
    sim_out->relax_area_[0][0] = 2;
    sim_out->relax_area_[0][1] = 20;
    sim_out->relax_area_[1][0] = 2;
    sim_out->relax_area_[1][1] = 20;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    soil_simulator::rng.seed(201);
    sim_out->terrain_[10][15] = -0.2;
    sim_out->terrain_[10][16] = 0.0;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[11][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 15}, {11, 15}}, {}, {});

    delete bucket;
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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // -- Testing case with first bucket layer and soil should avalanche --
    // -- on terrain                                                     --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.2;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 10);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case where there is the first bucket layer with bucket soil --
    // -- and soil should avalanche on the terrain                            --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = 0.3;
    sim_out->body_[1][10][15] = 0.5;
    sim_out->body_soil_[0][10][15] = 0.5;
    sim_out->body_soil_[1][10][15] = 0.7;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 10);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with second bucket layer and soil should avalanche --
    // -- on terrain                                                      --
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = 0.4;
    sim_out->body_[3][10][15] = 0.5;
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    status = soil_simulator::CheckUnstableBodyCell(
        sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 20);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil should avalanche on the terrain                   --
    sim_out->terrain_[10][15] = -0.2;
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
    EXPECT_EQ(status, 20);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

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
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

    delete sim_out;
}

TEST(UnitTestRelax, RelaxUnstableBodyCell) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    std::vector<soil_simulator::body_soil> *body_soil_pos = (
        new std::vector<soil_simulator::body_soil>);
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};

    // -- Testing case with no bucket and soil should partially avalanche --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = 0.0;
    auto pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 40, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // -- Testing case with no bucket and soil should fully avalanche --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 40, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // -- Testing case with no bucket, soil should fully avalanche but not --
    // -- enough soil in body_soil_pos_                                    --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 40, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // -- Testing case with first bucket layer and soil should partially --
    // -- avalanche on the terrain                                       --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.3;
    sim_out->body_[1][10][15] = 0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 10, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case with first bucket layer and soil should fully --
    // -- avalanche on the terrain                                   --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.4;
    sim_out->body_soil_[0][10][15] = 0.4;
    sim_out->body_soil_[1][10][15] = 0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 10, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.5, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with first bucket layer and soil should partially      --
    // -- avalanche on the terrain but there is not enough space for all soil --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.5;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[0][10][15] = 0.2;
    sim_out->body_[1][10][15] = 0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.5});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 10, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.3, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.3, 1.e-5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case with first bucket layer and soil should partially --
    // -- avalanche on it                                                --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = 0.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    auto posA = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 14, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_EQ((*body_soil_pos)[0].ind, 0);
    EXPECT_EQ((*body_soil_pos)[0].ii, 10);
    EXPECT_EQ((*body_soil_pos)[0].jj, 15);
    EXPECT_NEAR((*body_soil_pos)[0].x_b, posA[0], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].y_b, posA[1], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].z_b, posA[2], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with first bucket layer and soil should fully --
    // -- avalanche on it                                            --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 14, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_EQ((*body_soil_pos)[0].ind, 0);
    EXPECT_EQ((*body_soil_pos)[0].ii, 10);
    EXPECT_EQ((*body_soil_pos)[0].jj, 15);
    EXPECT_NEAR((*body_soil_pos)[0].x_b, posA[0], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].y_b, posA[1], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].z_b, posA[2], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].h_soil, 0.2, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with first bucket layer and soil should fully --
    // -- avalanche on it, but not enough soil in body_soil_pos_     --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 14, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_EQ((*body_soil_pos)[0].ind, 0);
    EXPECT_EQ((*body_soil_pos)[0].ii, 10);
    EXPECT_EQ((*body_soil_pos)[0].jj, 15);
    EXPECT_NEAR((*body_soil_pos)[0].x_b, posA[0], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].y_b, posA[1], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].z_b, posA[2], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there is the first bucket layer with bucket --
    // -- soil and soil should partially avalanche on it                 --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.2});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 13, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_EQ((*body_soil_pos)[0].ind, 0);
    EXPECT_EQ((*body_soil_pos)[0].ii, 10);
    EXPECT_EQ((*body_soil_pos)[0].jj, 15);
    EXPECT_NEAR((*body_soil_pos)[0].x_b, posA[0], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].y_b, posA[1], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].z_b, posA[2], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there is the first bucket layer with bucket --
    // -- soil and soil should fully avalanche on it                     --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 13, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_EQ((*body_soil_pos)[0].ind, 0);
    EXPECT_EQ((*body_soil_pos)[0].ii, 10);
    EXPECT_EQ((*body_soil_pos)[0].jj, 15);
    EXPECT_NEAR((*body_soil_pos)[0].x_b, posA[0], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].y_b, posA[1], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].z_b, posA[2], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there is the first bucket layer with bucket  --
    // -- soil and soil should fully avalanche on it, but not enough soil --
    // -- in body_soil_pos_                                               --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.2;
    sim_out->body_soil_[0][10][14] = 0.2;
    sim_out->body_soil_[1][10][14] = 0.4;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = -0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 13, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_EQ((*body_soil_pos)[0].ind, 0);
    EXPECT_EQ((*body_soil_pos)[0].ii, 10);
    EXPECT_EQ((*body_soil_pos)[0].jj, 15);
    EXPECT_NEAR((*body_soil_pos)[0].x_b, posA[0], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].y_b, posA[1], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].z_b, posA[2], 1.e-5);
    EXPECT_NEAR((*body_soil_pos)[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with second bucket layer and soil should partially --
    // -- avalanche on the terrain                                        --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    sim_out->body_soil_[2][10][15] = 0.6;
    sim_out->body_soil_[3][10][15] = 0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    auto pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 20, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with second bucket layer and soil should fully --
    // -- avalanche on the terrain                                    --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = 0.5;
    sim_out->body_[3][10][15] = 0.6;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 20, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case with second bucket layer and soil should partially     --
    // -- avalanche on the terrain but there is not enough space for all soil --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.5;
    sim_out->terrain_[10][15] = 0.0;
    sim_out->body_[2][10][15] = 0.2;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[2][10][15] = 0.3;
    sim_out->body_soil_[3][10][15] = 0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.5});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.4});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 20, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with second bucket layer and soil should --
    // -- partially avalanche on it                             --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 22, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with second bucket layer and soil should fully --
    // -- avalanche on it                                             --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 22, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with second bucket layer and soil should fully --
    // -- avalanche on it, but not enough soil in body_soil_pos_      --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.3;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 22, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil should partially avalanche on it                  --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 21, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil should fully avalanche on it                      --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 21, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil should fully avalanche on it, but not enough soil --
    // -- in body_soil_pos_                                               --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.1;
    sim_out->body_soil_[0][10][14] = 0.1;
    sim_out->body_soil_[1][10][14] = 0.3;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = -0.2;
    sim_out->body_soil_[3][10][15] = -0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 21, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, first layer being lower --
    // -- and soil should partially avalanche on it                    --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, first layer being lower --
    // -- and soil should fully avalanche on it                        --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, first layer being lower  --
    // -- and soil should fully avalanche on it, but not enough soil in --
    // -- body_soil_pos_                                                --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case whith two bucket layers, first layer being lower     --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, first layer being lower   --
    // -- and soil should partially avalanche on the second bucket layer --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, first layer being lower --
    // -- and soil should fully avalanche on the second bucket layer   --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.3);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil   --
    // -- first layer being lower and soil should partially avalanche on it --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil should fully avalanche on it   --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil    --
    // -- first layer being lower and soil should fully avalanche on it, but --
    // -- not enough soil in body_soil_pos_                                  --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil   --
    // -- first layer being lower and soil should partially avalanche on it --
    // -- but there is not enough space for all the soil                    --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.8});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.3);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil    --
    // -- first layer being lower and soil should partially avalanche on the --
    // -- second bucket layer                                                --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.8});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.2});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil should fully avalanche on the  --
    // -- second bucket layer                                             --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.2});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.7, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, second layer being lower --
    // -- and soil should partially avalanche on it                     --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, second layer being lower --
    // -- and soil should fully avalanche on it                         --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.0, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, second layer being lower --
    // -- and soil should fully avalanche on it, but not enough soil in --
    // -- body_soil_pos_                                                --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.0, 1, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, second layer being lower     --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, second layer being lower --
    // -- and soil should partially avalanche on the first bucket layer --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.4, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.6, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, second layer being lower --
    // -- and soil should fully avalanche on the first bucket layer     --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil    --
    // -- second layer being lower and soil should partially avalanche on it --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil    --
    // -- the second layer being lower and soil should fully avalanche on it --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil     --
    // -- the second layer being lower and soil should fully avalanche on it, --
    // -- but not enough soil in body_soil_pos_                               --
    sim_out->terrain_[10][14] = -0.2;
    sim_out->body_[0][10][14] = -0.2;
    sim_out->body_[1][10][14] = 0.0;
    sim_out->body_soil_[0][10][14] = 0.0;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = 0.1;
    sim_out->body_[1][10][15] = 0.3;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = 0.3;
    sim_out->body_soil_[1][10][15] = 0.8;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil    --
    // -- second layer being lower and soil should partially avalanche on it --
    // -- but there is not enough space for all the soil                     --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.8});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil     --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.2});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil     --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil   --
    // -- first layer being lower and soil should partially avalanche on it --
    // -- but there is no space at all                                      --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.8});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.3});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.5});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.8, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case where there are two bucket layers with bucket soil    --
    // -- second layer being lower and soil should partially avalanche on it --
    // -- but there is no space at all                                       --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.8});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.5});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.3});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, first layer being lower    --
    // -- and soil fully cover the space between the two layers, the soil --
    // -- can partially avalanche to the second bucket layer              --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.8});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.5, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.3);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, first layer being lower    --
    // -- and soil fully cover the space between the two layers, the soil --
    // -- can partially avalanche to the second bucket soil layer         --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.8});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.2});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.6, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, second layer being lower   --
    // -- and soil fully cover the space between the two layers, the soil --
    // -- can partially avalanche to the first bucket layer               --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.8});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.6});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.5, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // -- Testing case with two bucket layers, second layer being lower   --
    // -- and soil fully cover the space between the two layers, the soil --
    // -- can partially avalanche to the first bucket soil layer          --
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
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.8});
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.6});
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        (*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    delete bucket;
    delete sim_out;
    delete body_soil_pos;
}

TEST(UnitTestRelax, RelaxBodySoil) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    soil_simulator::SimParam sim_param(0.785, 3, 4);
    sim_out->impact_area_[0][0] = 2;
    sim_out->impact_area_[0][1] = 20;
    sim_out->impact_area_[1][0] = 2;
    sim_out->impact_area_[1][1] = 20;
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};

    // -- Testing case with no bucket and soil should partially avalanche --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.3;
    sim_out->body_[0][10][14] = -0.3;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.2;
    auto pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // -- Testing case with no bucket and soil should fully avalanche --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.3;
    sim_out->body_[0][10][14] = -0.3;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // -- Testing case with no bucket and soil should fully avalanche, --
    // -- soil should avalanche in several steps                       --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.3;
    sim_out->body_[0][10][14] = -0.3;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // -- Testing case with first bucket layer and soil should partially --
    // -- avalanche on the terrain                                       --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case with first bucket layer and soil should fully --
    // -- avalanche on the terrain                                   --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->body_[2][10][14] = 0.0;
    sim_out->body_[3][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with first bucket layer and soil should partially      --
    // -- avalanche on the terrain but there is not enough space for all soil --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.1;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[0][10][15] = -0.2;
    sim_out->body_[1][10][15] = 0.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.4});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case with first bucket layer and soil should partially --
    // -- avalanche on it                                                --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    auto posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with first bucket layer and soil should fully --
    // -- avalanche on it                                            --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with first bucket layer and soil should fully --
    // -- avalanche on it in several steps                           --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case where there is the first bucket layer with bucket soil --
    // -- and soil should partially avalanche on it                           --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case where there is the first bucket layer with bucket soil --
    // -- and soil should fully avalanche on it                               --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.7;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case where there is the first bucket layer with bucket soil --
    // -- and soil should fully avalanche on it in several steps              --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.7;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[4], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with second bucket layer and soil should partially --
    // -- avalanche on the terrain                                        --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case with second bucket layer and soil should fully --
    // -- avalanche on the terrain                                    --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[2][10][15] = 0.0;
    sim_out->body_[3][10][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case with second bucket layer and soil should partially     --
    // -- avalanche on the terrain but there is not enough space for all soil --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.1;
    sim_out->terrain_[10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.4});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case with second bucket layer and soil should partially --
    // -- avalanche on it                                                 --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with second bucket layer and soil should fully --
    // -- avalanche on it                                             --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.6;
    sim_out->body_[3][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.4});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with second bucket layer and soil should fully --
    // -- avalanche on it in several steps                            --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.6;
    sim_out->body_[3][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil should partially avalanche on it                  --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil should fully avalanche on it                      --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.7;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil should fully avalanche on it in several steps     --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.7;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[4], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should partially avalanche on it                            --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should fully avalanche on it                                --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should fully avalanche on it in several steps               --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should partially avalanche on the second bucket layer       --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should fully avalanche on the second bucket layer           --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.7;
    sim_out->body_[2][10][15] = -0.6;
    sim_out->body_[3][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with two bucket layers, first layer with soil being --
    // -- lower and soil should partially avalanche on it                  --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with two bucket layers, first layer with soil being --
    // -- lower and soil should fully avalanche on it                      --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.2;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with two bucket layers, first layer with soil being --
    // -- lower and soil should fully avalanche on it in several steps     --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.1;
    sim_out->body_soil_[0][10][14] = -0.1;
    sim_out->body_soil_[1][10][14] = 0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[4], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with two bucket layers, first layer with soil being --
    // -- lower and soil should partially avalanche on second bucket layer --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.7;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = -0.7;
    sim_out->body_soil_[1][10][15] = -0.6;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    auto pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, first layer with soil being --
    // -- lower and soil should fully avalanche on the second bucket layer --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.7;
    sim_out->body_[2][10][15] = -0.5;
    sim_out->body_[3][10][15] = -0.4;
    sim_out->body_soil_[0][10][15] = -0.7;
    sim_out->body_soil_[1][10][15] = -0.6;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should partially avalanche on it, while the second layer is --
    // -- with bucket soil                                                 --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.1;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.1;
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should fully avalanche on it, while the second layer is     --
    // -- with bucket soil                                                 --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.5;
    sim_out->body_[0][10][15] = -0.5;
    sim_out->body_[1][10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.1;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.1;
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should partially avalanche on second bucket layer with soil --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.3;
    sim_out->body_[0][10][14] = -0.3;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should fully avalanche on the second bucket layer with soil --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.3;
    sim_out->body_[0][10][14] = -0.3;
    sim_out->body_[1][10][14] = -0.1;
    sim_out->body_soil_[0][10][14] = -0.1;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil   --
    // -- first layer being lower and soil should partially avalanche on it --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.4;
    sim_out->body_soil_[2][10][15] = -0.1;
    sim_out->body_soil_[3][10][15] = 0.0;
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil should fully avalanche on it   --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.7;
    sim_out->body_[0][10][15] = -0.7;
    sim_out->body_[1][10][15] = -0.6;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[0][10][15] = -0.6;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.1;
    sim_out->body_soil_[3][10][15] = 0.0;
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil should partially avalanche on  --
    // -- the second bucket layer                                         --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = -0.3;
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.4;
    sim_out->body_soil_[2][10][15] = -0.2;
    sim_out->body_soil_[3][10][15] = -0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.0, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil should fully avalanche on      --
    // -- the second bucket layer                                         --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.7;
    sim_out->body_[2][10][15] = -0.6;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.7;
    sim_out->body_soil_[1][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil should partially avalanche on it but there is not enough    --
    // -- space for all the soil                                           --
    soil_simulator::rng.seed(1236);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.3;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.6});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil should partially avalanche on  --
    // -- it but there is not enough space for all the soil               --
    soil_simulator::rng.seed(1236);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.2;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.5});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.2});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil   --
    // -- first layer being lower and soil should partially avalanche on it --
    // -- but there is no space at all                                      --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.2;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = -0.4;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.4});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil should partially avalanche on it                             --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil should fully avalanche on it                                 --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.6;
    sim_out->body_[3][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil should fully avalanche on it in several steps                --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.6;
    sim_out->body_[3][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil should partially avalanche on the first bucket layer         --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.6;
    sim_out->body_[3][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil should fully avalanche on the first bucket layer             --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.2;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.5;
    sim_out->body_[1][10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with two bucket layers, second layer with soil being --
    // -- lower and soil should partially avalanche on it                   --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.6;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer with soil being --
    // -- lower and soil should fully avalanche on it                       --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.7;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer with soil being --
    // -- lower and soil should fully avalanche on it in several steps      --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = -0.1;
    sim_out->terrain_[10][15] = -0.7;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[4], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer with soil being   --
    // -- lower and soil should partially avalanche on the first bucket layer --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.6;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.5;
    sim_out->body_soil_[3][10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer with soil being --
    // -- lower and soil should fully avalanche on the first bucket layer   --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.7;
    sim_out->body_[0][10][15] = -0.5;
    sim_out->body_[1][10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.7;
    sim_out->body_[3][10][15] = -0.6;
    sim_out->body_soil_[2][10][15] = -0.6;
    sim_out->body_soil_[3][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.5, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil should partially avalanche on it, while the first layer is   --
    // -- with bucket soil                                                  --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil should fully avalanche on it, while the first layer is with  --
    // -- bucket soil                                                       --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.6;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.3, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and   --
    // -- soil should partially avalanche on the first bucket layer with soil --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.6;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.6;
    sim_out->body_[3][10][15] = -0.5;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil should fully avalanche on the first bucket layer with soil   --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil    --
    // -- second layer being lower and soil should partially avalanche on it --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.1;
    sim_out->body_soil_[2][10][15] = -0.7;
    sim_out->body_soil_[3][10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.3});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil --
    // -- second layer being lower and soil should fully avalanche on it  --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.1;
    sim_out->body_soil_[2][10][15] = -0.7;
    sim_out->body_soil_[3][10][15] = -0.6;
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil  --
    // -- the second layer being lower and soil should partially avalanche --
    // -- on the first bucket layer                                        --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.3;
    sim_out->body_soil_[0][10][14] = -0.3;
    sim_out->body_soil_[1][10][14] = 0.1;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.3;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = -0.7;
    sim_out->body_soil_[3][10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.4});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.3});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil --
    // -- second layer being lower and soil should fully avalanche on the --
    // -- first bucket layer                                              --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.4;
    sim_out->body_[0][10][14] = -0.4;
    sim_out->body_[1][10][14] = -0.2;
    sim_out->body_soil_[0][10][14] = -0.2;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.6;
    sim_out->body_[1][10][15] = -0.5;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[0][10][15] = -0.5;
    sim_out->body_soil_[1][10][15] = -0.4;
    sim_out->body_soil_[2][10][15] = -0.7;
    sim_out->body_soil_[3][10][15] = -0.6;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.6, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and   --
    // -- soil should partially avalanche on it but there is not enough space --
    // -- for all the soil                                                    --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.5;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.5, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil --
    // -- second layer being lower and soil should partially avalanche on --
    // -- it but there is not enough space for all the soil               --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = -0.7;
    sim_out->body_soil_[3][10][15] = -0.6;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil --
    // -- second layer being lower and soil should partially avalanche on --
    // -- it but there is no space at all                                 --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = -0.7;
    sim_out->body_soil_[3][10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.3});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil fully cover the space between the two layers, the soil can  --
    // -- partially avalanche to the second bucket layer                   --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.7;
    sim_out->body_[2][10][15] = -0.5;
    sim_out->body_[3][10][15] = -0.4;
    sim_out->body_soil_[0][10][15] = -0.7;
    sim_out->body_soil_[1][10][15] = -0.5;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.2, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil fully cover the space between the two layers, the soil can  --
    // -- partially avalanche to the second bucket soil layer              --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.8;
    sim_out->body_[1][10][15] = -0.7;
    sim_out->body_[2][10][15] = -0.5;
    sim_out->body_[3][10][15] = -0.4;
    sim_out->body_soil_[0][10][15] = -0.7;
    sim_out->body_soil_[1][10][15] = -0.5;
    sim_out->body_soil_[2][10][15] = -0.4;
    sim_out->body_soil_[3][10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, posA[0], posA[1], posA[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil fully cover the space between the two layers, the soil can   --
    // -- partially avalanche to the first bucket layer                     --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[2][10][15] = -0.7;
    sim_out->body_soil_[3][10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.3});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil fully cover the space between the two layers, the soil can   --
    // -- partially avalanche to the first bucket soil layer                --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.8;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.8;
    sim_out->body_[3][10][15] = -0.7;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = -0.7;
    sim_out->body_soil_[3][10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, posA[0], posA[1], posA[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.3});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.4, 1e-5);
    test_soil_simulator::CheckBodySoilPos(
        sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with no bucket and soil is not unstable --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // -- Testing case with first bucket layer and soil is not unstable --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = 0.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case with first bucket layer with bucket soil and soil --
    // -- is not unstable                                                --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.2;
    sim_out->body_soil_[0][10][15] = -0.2;
    sim_out->body_soil_[1][10][15] = 0.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case with second bucket layer and soil is not unstable --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case where there is the second bucket layer with bucket --
    // -- soil and soil is not unstable                                   --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case with two bucket layers, first layer being lower and --
    // -- soil is not unstable                                             --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.1;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case with two bucket layers, first layer with bucket soil --
    // -- being lower and soil is not unstable                              --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = 0.1;
    sim_out->body_[3][10][15] = 0.3;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil --
    // -- first layer being lower and soil is not unstable                --
    soil_simulator::rng.seed(1234);
    for (auto ii = 9; ii < 12;  ii++)
        for (auto jj = 13; jj < 16;  jj++)
            sim_out->terrain_[ii][jj] = 0.2;
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.4;
    sim_out->body_[1][10][15] = -0.3;
    sim_out->body_[2][10][15] = -0.1;
    sim_out->body_[3][10][15] = 0.0;
    sim_out->body_soil_[0][10][15] = -0.3;
    sim_out->body_soil_[1][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = 0.0;
    sim_out->body_soil_[3][10][15] = 0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.2});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.1});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    std::vector<std::vector<int>> terrain_pos = {
        {9, 13}, {9, 14}, {9, 15}, {10, 13}, {10, 14}, {10, 15}, {11, 13},
        {11, 14}, {11, 15}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, terrain_pos, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing case with two bucket layers, second layer being lower and --
    // -- soil is not unstable                                              --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.1;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

    // -- Testing case with two bucket layers, second layer with bucket soil --
    // -- being lower and soil is not unstable                               --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = 0.0;
    sim_out->body_[1][10][15] = 0.1;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.1;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // -- Testing case where there are two bucket layers with bucket soil --
    // -- second layer being lower and soil is not unstable               --
    soil_simulator::rng.seed(1234);
    for (auto ii = 9; ii < 12;  ii++)
        for (auto jj = 13; jj < 16;  jj++)
            sim_out->terrain_[ii][jj] = 0.2;
    sim_out->terrain_[10][14] = -0.8;
    sim_out->body_[0][10][14] = -0.8;
    sim_out->body_[1][10][14] = -0.7;
    sim_out->body_soil_[0][10][14] = -0.7;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_[0][10][15] = -0.1;
    sim_out->body_[1][10][15] = 0.0;
    sim_out->body_[2][10][15] = -0.4;
    sim_out->body_[3][10][15] = -0.3;
    sim_out->body_soil_[0][10][15] = 0.0;
    sim_out->body_soil_[1][10][15] = 0.1;
    sim_out->body_soil_[2][10][15] = -0.3;
    sim_out->body_soil_[3][10][15] = -0.1;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.7});
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 15, pos0[0], pos0[1], pos0[2], 0.1});
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 10, 15, pos2[0], pos2[1], pos2[2], 0.2});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.8, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][15], 0.0, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][15], 0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[2][10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[3][10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    // Resetting values
    terrain_pos = {
        {9, 13}, {9, 14}, {9, 15}, {10, 13}, {10, 14}, {10, 15}, {11, 13},
        {11, 14}, {11, 15}};
    test_soil_simulator::ResetValueAndTest(
        sim_out, terrain_pos, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // -- Testing randomization --
    soil_simulator::rng.seed(1234);
    sim_out->terrain_[10][14] = -0.6;
    sim_out->body_[0][10][14] = -0.6;
    sim_out->body_[1][10][14] = -0.5;
    sim_out->body_soil_[0][10][14] = -0.5;
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][13] = -0.4;
    sim_out->terrain_[10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.5, grid, bucket);
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 10, 14, pos0[0], pos0[1], pos0[2], 0.5});
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][13], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    soil_simulator::rng.seed(1235);
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][13] = -0.4;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_soil_pos_[0].h_soil = 0.5;
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.6, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][13], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[0][10][14], -0.5, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_[1][10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    // Resetting values
    test_soil_simulator::ResetValueAndTest(
        sim_out, {{10, 13}, {10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    delete bucket;
    delete sim_out;
}
