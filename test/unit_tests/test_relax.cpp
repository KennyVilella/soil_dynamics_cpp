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
    
    // Test: RE-CUB-1
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

    // Test: RE-CUB-2
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

    // Test: RE-CUB-3
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

    // Test: RE-CUB-4
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

    // Test: RE-CUB-5
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

    // Test: RE-CUB-6
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

    // Test: RE-CUB-7
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

    // Test: RE-CUB-8
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

    // Test: RE-CUB-9
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

    // Test: RE-CUB-10
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

    // Test: RE-CUB-11
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

    // Test: RE-CUB-12
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

    // Test: RE-CUB-13
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

    // Test: RE-CUB-14
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

    // Test: RE-CUB-15
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

    // Test: RE-CUB-16
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

    // Test: RE-CUB-17
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

    // Test: RE-CUB-18
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

    // Test: RE-CUB-19
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

    // Test: RE-CUB-20
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

    // Test: RE-CUB-21
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

    // Test: RE-CUB-22
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

    // Test: RE-CUB-23
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

    // Test: RE-CUB-24
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

    // Test: RE-CUB-25
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

    // Test: RE-CUB-26
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

    // Test: RE-CUB-27
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

    // Test: RE-CUB-28
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

    // Test: RE-CUB-29
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

    // Test: RE-CUB-30
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

    // Test: RE-CUB-31
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

    // Test: RE-CUB-32
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

    // Test: RE-CUB-33
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

    // Test: RE-CUB-34
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

    // Test: RE-CUB-35
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

    // Test: RE-CUB-36
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

    // Test: RE-CUB-37
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

    // Test: RE-CUB-38
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

    // Test: RE-CUB-39
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

    // Test: RE-CUB-40
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

    // Test: RE-CUB-41
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

    // Test: RE-CUB-42
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
    
    // Test: RE-RUB-1
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

    // Test: RE-RUB-2
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

    // Test: RE-RUB-3
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

    // Test: RE-RUB-4
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

    // Test: RE-RUB-5
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

    // Test: RE-RUB-6
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

    // Test: RE-RUB-7
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

    // Test: RE-RUB-8
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

    // Test: RE-RUB-9
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

    // Test: RE-RUB-10
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

    // Test: RE-RUB-11
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

    // Test: RE-RUB-12
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

    // Test: RE-RUB-13
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

    // Test: RE-RUB-14
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

    // Test: RE-RUB-15
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

    // Test: RE-RUB-16
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

    // Test: RE-RUB-17
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

    // Test: RE-RUB-18
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

    // Test: RE-RUB-19
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

    // Test: RE-RUB-20
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

    // Test: RE-RUB-21
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

    // Test: RE-RUB-22
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

    // Test: RE-RUB-23
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

    // Test: RE-RUB-24
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

    // Test: RE-RUB-25
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

    // Test: RE-RUB-26
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

    // Test: RE-RUB-27
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

    // Test: RE-RUB-28
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

    // Test: RE-RUB-29
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

    // Test: RE-RUB-30
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

    // Test: RE-RUB-31
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

    // Test: RE-RUB-32
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

    // Test: RE-RUB-33
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

    // Test: RE-RUB-34
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

    // Test: RE-RUB-35
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

    // Test: RE-RUB-36
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

    // Test: RE-RUB-37
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

    // Test: RE-RUB-38
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

    // Test: RE-RUB-39
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

    // Test: RE-RUB-40
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

    // Test: RE-RUB-41
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

    // Test: RE-RUB-42
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

    // Test: RE-RUB-43
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

    // Test: RE-RUB-44
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

    // Test: RE-RUB-45
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

    // Test: RE-RUB-46
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

    // Test: RE-RUB-47
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

    // Test: RE-RUB-48
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

    // Test: RE-RUB-49
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

    // Test: RE-RUB-50
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

    // Test: RE-RUB-51
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

    // Test: RE-RBS-1
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

    // Test: RE-RBS-2
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

    // Test: RE-RBS-3
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

    // Test: RE-RBS-4
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

    // Test: RE-RBS-5
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

    // Test: RE-RBS-6
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

    // Test: RE-RBS-7
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

    // Test: RE-RBS-8
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

    // Test: RE-RBS-9
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

    // Test: RE-RBS-10
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

    // Test: RE-RBS-11
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

    // Test: RE-RBS-12
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

    // Test: RE-RBS-13
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

    // Test: RE-RBS-14
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

    // Test: RE-RBS-15
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

    // Test: RE-RBS-16
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

    // Test: RE-RBS-17
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

    // Test: RE-RBS-18
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

    // Test: RE-RBS-19
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

    // Test: RE-RBS-20
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

    // Test: RE-RBS-21
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

    // Test: RE-RBS-22
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

    // Test: RE-RBS-23
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

    // Test: RE-RBS-24
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

    // Test: RE-RBS-25
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

    // Test: RE-RBS-26
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

    // Test: RE-RBS-27
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

    // Test: RE-RBS-28
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

    // Test: RE-RBS-29
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

    // Test: RE-RBS-30
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

    // Test: RE-RBS-31
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

    // Test: RE-RBS-32
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

    // Test: RE-RBS-33
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

    // Test: RE-RBS-34
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

    // Test: RE-RBS-35
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

    // Test: RE-RBS-36
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

    // Test: RE-RBS-37
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

    // Test: RE-RBS-38
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

    // Test: RE-RBS-39
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

    // Test: RE-RBS-40
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

    // Test: RE-RBS-41
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

    // Test: RE-RBS-42
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

    // Test: RE-RBS-43
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

    // Test: RE-RBS-44
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

    // Test: RE-RBS-45
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

    // Test: RE-RBS-46
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

    // Test: RE-RBS-47
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

    // Test: RE-RBS-48
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

    // Test: RE-RBS-49
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

    // Test: RE-RBS-50
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

    // Test: RE-RBS-51
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

    // Test: RE-RBS-52
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

    // Test: RE-RBS-53
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

    // Test: RE-RBS-54
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

    // Test: RE-RBS-55
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

    // Test: RE-RBS-56
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

    // Test: RE-RBS-57
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

    // Test: RE-RBS-58
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

    // Test: RE-RBS-59
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

    // Test: RE-RBS-60
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

    // Test: RE-RBS-61
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

    // Test: RE-RBS-62
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

    // Test: RE-RBS-63
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

    // Test: RE-RBS-64
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

    // Test: RE-RBS-65
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

    // Test: RE-RBS-66
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

    // Test: RE-RBS-67
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

    // Test: RE-RBS-68
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

    // Test: RE-RBS-69
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

    // Test: RE-RBS-70
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

    // Test: RE-RBS-71
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

    // Test: RE-RBS-72
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

    // Test: RE-RBS-73
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

    // Test: RE-RBS-74
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

    // Test: RE-RBS-75
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

    // Test: RE-RBS-76
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

    // Test: RE-RBS-77
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
    sim_out->body_[3][10][15] = -0.2;
    sim_out->body_soil_[2][10][15] = -0.2;
    sim_out->body_soil_[3][10][15] = 0.0;
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

    // Test: RE-RBS-78
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

    // Test: RE-RBS-79
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
