/*
This file implements unit tests for the functions in relax.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <cmath>
#include <random>
#include "gtest/gtest.h"
#include "soil_simulator/relax.hpp"
#include "soil_simulator/utils.hpp"
#include "test/unit_tests/utility.hpp"

// To make the function call holds in a single line.
// It greatly improves readability.
using test_soil_simulator::SetHeight;
using test_soil_simulator::PushBodySoilPos;
using test_soil_simulator::CheckHeight;
using test_soil_simulator::CheckBodySoilPos;
using test_soil_simulator::ResetValueAndTest;
using soil_simulator::LocateUnstableTerrainCell;
using soil_simulator::CheckUnstableTerrainCell;
using soil_simulator::CheckUnstableBodyCell;

TEST(UnitTestRelax, LocateUnstableTerrainCell) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    sim_out->impact_area_[0][0] = 2;
    sim_out->impact_area_[0][1] = 17;
    sim_out->impact_area_[1][0] = 2;
    sim_out->impact_area_[1][1] = 17;

    // Declaring variables
    std::vector<std::vector<int>> unstable_cells;

    // Test: RE-LUT-1
    unstable_cells = LocateUnstableTerrainCell(sim_out, 0.1, 1e-5);
    EXPECT_EQ(unstable_cells.size(), 0);

    // Test: RE-LUT-2
    sim_out->terrain_[7][13] = 0.1;
    unstable_cells = LocateUnstableTerrainCell(sim_out, 0.1, 1e-5);
    EXPECT_EQ(unstable_cells.size(), 0);
    unstable_cells = LocateUnstableTerrainCell(sim_out, 0.0, 1e-5);
    EXPECT_TRUE((unstable_cells[0] == std::vector<int> {7, 13}));
    EXPECT_EQ(unstable_cells.size(), 1);
    sim_out->terrain_[7][13] = 0.0;

    // Test: RE-LUT-3
    sim_out->terrain_[5][13] = 0.2;
    unstable_cells = LocateUnstableTerrainCell(sim_out, 0.1, 1e-5);
    EXPECT_TRUE((unstable_cells[0] == std::vector<int> {5, 13}));
    EXPECT_EQ(unstable_cells.size(), 1);
    unstable_cells = LocateUnstableTerrainCell(sim_out, 0.2, 1e-5);
    EXPECT_EQ(unstable_cells.size(), 0);
    sim_out->terrain_[5][13] = 0.0;

    // Test: RE-LUT-4
    sim_out->terrain_[9][9] = -0.1;
    unstable_cells = LocateUnstableTerrainCell(sim_out, 0.1, 1e-5);
    EXPECT_EQ(unstable_cells.size(), 0);
    unstable_cells = LocateUnstableTerrainCell(sim_out, 0.0, 1e-5);
    EXPECT_TRUE((unstable_cells[0] == std::vector<int> {8, 9}));
    EXPECT_TRUE((unstable_cells[1] == std::vector<int> {9, 8}));
    EXPECT_TRUE((unstable_cells[2] == std::vector<int> {9, 10}));
    EXPECT_TRUE((unstable_cells[3] == std::vector<int> {10, 9}));
    EXPECT_EQ(unstable_cells.size(), 4);
    sim_out->terrain_[9][9] = 0.0;

    // Test: RE-LUT-5
    sim_out->terrain_[11][13] = -0.2;
    unstable_cells = LocateUnstableTerrainCell(sim_out, 0.1, 1e-5);
    EXPECT_TRUE((unstable_cells[0] == std::vector<int> {10, 13}));
    EXPECT_TRUE((unstable_cells[1] == std::vector<int> {11, 12}));
    EXPECT_TRUE((unstable_cells[2] == std::vector<int> {11, 14}));
    EXPECT_TRUE((unstable_cells[3] == std::vector<int> {12, 13}));
    EXPECT_EQ(unstable_cells.size(), 4);
    unstable_cells = LocateUnstableTerrainCell(sim_out, 0.2, 1e-5);
    EXPECT_EQ(unstable_cells.size(), 0);
    sim_out->terrain_[11][13] = 0.0;

    // Test: RE-LUT-6
    sim_out->terrain_[15][5] = -0.4;
    sim_out->terrain_[15][6] = -0.2;
    unstable_cells = LocateUnstableTerrainCell(sim_out, 0.1, 1e-5);
    EXPECT_TRUE((unstable_cells[0] == std::vector<int> {14, 5}));
    EXPECT_TRUE((unstable_cells[1] == std::vector<int> {14, 6}));
    EXPECT_TRUE((unstable_cells[2] == std::vector<int> {15, 4}));
    EXPECT_TRUE((unstable_cells[3] == std::vector<int> {15, 6}));
    EXPECT_TRUE((unstable_cells[4] == std::vector<int> {15, 7}));
    EXPECT_TRUE((unstable_cells[5] == std::vector<int> {16, 5}));
    EXPECT_TRUE((unstable_cells[6] == std::vector<int> {16, 6}));
    EXPECT_EQ(unstable_cells.size(), 7);
    sim_out->terrain_[15][5] = 0.0;
    sim_out->terrain_[15][6] = 0.0;

    // Test: RE-LUT-7
    sim_out->terrain_[5][2] = -0.2;
    unstable_cells = LocateUnstableTerrainCell(sim_out, 0.1, 1e-5);
    EXPECT_TRUE((unstable_cells[0] == std::vector<int> {4, 2}));
    EXPECT_TRUE((unstable_cells[1] == std::vector<int> {5, 3}));
    EXPECT_TRUE((unstable_cells[2] == std::vector<int> {6, 2}));
    EXPECT_EQ(unstable_cells.size(), 3);
    sim_out->terrain_[5][2] = 0.0;

    delete sim_out;
}

TEST(UnitTestRelax, CheckUnstableTerrainCell) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    // Declaring variables
    int status;

    // Test: RE-CUT-1
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);

    // Test: RE-CUT-2
    sim_out->terrain_[10][15] = -0.2;
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 40);
    ResetValueAndTest(sim_out, {{10, 15}}, {}, {});

    // Test: RE-CUT-3
    SetHeight(sim_out, 10, 15, -0.2, -0.1, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 10);
    ResetValueAndTest(sim_out, {{10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-CUT-4
    SetHeight(sim_out, 10, 15, -0.4, -0.4, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(sim_out, {{10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-CUT-5
    SetHeight(sim_out, 10, 15, -0.4, -0.4, -0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 14);
    ResetValueAndTest(sim_out, {{10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-CUT-6
    SetHeight(sim_out, 9, 15, -0.8, -0.7, -0.5, -0.5, -0.3, NAN, NAN, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 9, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 10);
    ResetValueAndTest(sim_out, {{9, 15}}, {{0, 9, 15}}, {{0, 9, 15}});

    // Test: RE-CUT-7
    SetHeight(sim_out, 10, 15, -0.8, -0.8, -0.5, -0.5, 0.0, NAN, NAN, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(sim_out, {{10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-8
    SetHeight(sim_out, 9, 15, -0.8, -0.8, -0.5, -0.5, -0.3, NAN, NAN, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 9, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 13);
    ResetValueAndTest(sim_out, {{9, 15}}, {{0, 9, 15}}, {{0, 9, 15}});

    // Test: RE-CUT-9
    SetHeight(sim_out, 10, 15, -0.2, NAN, NAN, NAN, NAN, -0.1, 0.0, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 20);
    ResetValueAndTest(sim_out, {{10, 15}}, {{2, 10, 15}}, {});

    // Test: RE-CUT-10
    SetHeight(sim_out, 10, 15, -0.4, NAN, NAN, NAN, NAN, -0.4, 0.0, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(sim_out, {{10, 15}}, {{2, 10, 15}}, {});

    // Test: RE-CUT-11
    SetHeight(sim_out, 10, 15, -0.4, NAN, NAN, NAN, NAN, -0.4, -0.2, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 22);
    ResetValueAndTest(sim_out, {{10, 15}}, {{2, 10, 15}}, {});

    // Test: RE-CUT-12
    SetHeight(sim_out, 9, 15, -0.8, NAN, NAN, NAN, NAN, -0.7, -0.5, -0.5, -0.3);
    status = CheckUnstableTerrainCell(sim_out, 9, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 20);
    ResetValueAndTest(sim_out, {{9, 15}}, {{2, 9, 15}}, {{2, 9, 15}});

    // Test: RE-CUT-13
    SetHeight(sim_out, 10, 15, -0.8, NAN, NAN, NAN, NAN, -0.8, -0.5, -0.5, 0.0);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(sim_out, {{10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-14
    SetHeight(sim_out, 9, 15, -0.8, NAN, NAN, NAN, NAN, -0.8, -0.5, -0.5, -0.3);
    status = CheckUnstableTerrainCell(sim_out, 9, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 21);
    ResetValueAndTest(sim_out, {{9, 15}}, {{2, 9, 15}}, {{2, 9, 15}});

    // Test: RE-CUT-15
    SetHeight(sim_out, 9, 15, -0.8, -0.7, -0.6, NAN, NAN, -0.4, -0.3, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 9, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    ResetValueAndTest(sim_out, {{9, 15}}, {{0, 9, 15}, {2, 9, 15}}, {});

    // Test: RE-CUT-16
    SetHeight(sim_out, 10, 15, -0.8, -0.8, -0.6, NAN, NAN, 0.2, 0.4, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 34);
    ResetValueAndTest(sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});
    
    // Test: RE-CUT-17
    SetHeight(sim_out, 9, 15, -0.8, -0.8, -0.6, NAN, NAN, -0.4, -0.3, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 9, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 34);
    ResetValueAndTest(sim_out, {{9, 15}}, {{0, 9, 15}, {2, 9, 15}}, {});

    // Test: RE-CUT-18
    SetHeight(
        sim_out, 10, 15, -0.8, -0.7, -0.6, NAN, NAN, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-19
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, NAN, NAN, -0.4, -0.3, -0.3, 0.2);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 34);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-20
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, NAN, NAN, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 34);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-21
    SetHeight(
        sim_out, 10, 15, -0.8, -0.7, -0.6, -0.6, -0.5, -0.4, -0.3, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-22
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.5, -0.4, 0.0, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 33);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-23
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.5, -0.4, -0.3, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 33);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-24
    SetHeight(
        sim_out, 10, 15, -0.8, -0.7, -0.6, -0.6, -0.5, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-25
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.5, -0.4, -0.3, -0.3, 0.0);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 33);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-26
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.5, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 33);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-27
    SetHeight(
        sim_out, 10, 15, -0.8, -0.7, -0.6, -0.6, -0.4, -0.4, -0.3, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-28
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.4, -0.4, 0.0, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-29
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.4, -0.4, -0.3, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 32);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-30
    SetHeight(
        sim_out, 10, 15, -0.8, -0.7, -0.6, -0.6, -0.4, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-31
    SetHeight(
        sim_out, 10, 15, -0.7, -0.7, -0.6, -0.6, -0.4, -0.4, -0.3, -0.3, 0.2);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-32
    SetHeight(
        sim_out, 10, 15, -0.7, -0.7, -0.6, -0.6, -0.4, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 31);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-33
    SetHeight(sim_out, 9, 15, -0.8, -0.4, -0.3, NAN, NAN, -0.7, -0.6, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 9, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    ResetValueAndTest(
        sim_out, {{9, 15}}, {{0, 9, 15}, {2, 9, 15}}, {});

    // Test: RE-CUT-34
    SetHeight(sim_out, 10, 15, -0.8, -0.4, 0.0, NAN, NAN, -0.8, -0.6, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 32);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-CUT-35
    SetHeight(sim_out, 9, 15, -0.8, -0.4, -0.3, NAN, NAN, -0.8, -0.6, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 9, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 32);
    ResetValueAndTest(
        sim_out, {{9, 15}}, {{0, 9, 15}, {2, 9, 15}}, {});

    // Test: RE-CUT-36
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, -0.2, -0.7, -0.6, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-37
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, 0.0, -0.8, -0.6, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 32);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-38
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, -0.2, -0.8, -0.6, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 32);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{0, 10, 15}});

    // Test: RE-CUT-39
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, NAN, NAN, -0.7, -0.6, -0.6, -0.5);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-40
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, 0.0, NAN, NAN, -0.8, -0.6, -0.6, -0.5);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 31);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-41
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, NAN, NAN, -0.8, -0.6, -0.6, -0.5);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 31);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-42
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, -0.2, -0.7, -0.6, -0.6, -0.5);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-43
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, 0.0, -0.8, -0.6, -0.6, -0.5);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 31);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-44
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, -0.2, -0.8, -0.6, -0.6, -0.5);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 31);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-45
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, NAN, NAN, -0.7, -0.6, -0.6, -0.4);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-46
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, 0.0, NAN, NAN, -0.8, -0.6, -0.6, -0.4);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-47
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, NAN, NAN, -0.8, -0.6, -0.6, -0.4);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 34);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-CUT-48
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, -0.2, -0.7, -0.6, -0.6, -0.4);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 30);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-49
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, 0.0, -0.8, -0.6, -0.6, -0.4);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-50
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, -0.2, -0.8, -0.6, -0.6, -0.4);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 33);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUT-51
    SetHeight(sim_out, 10, 15, -1.0, -0.4, -0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.6, 1e-5);
    EXPECT_EQ(status, 10);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-CUT-52
    SetHeight(sim_out, 10, 15, -0.4, -0.4, -0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-CUT-53
    sim_out->terrain_[10][15] = -0.4;
    status = CheckUnstableTerrainCell(sim_out, 10, 15, -0.4, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(sim_out, {{10, 15}}, {}, {});

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
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};

    // Declaring variables
    std::vector<float> pos0;
    std::vector<float> pos2;
    std::vector<float> posA;

    // Test: RE-RUT-1
    sim_out->terrain_[10][14] = 0.4;
    sim_out->terrain_[10][15] = 0.1;
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 40, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], 0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.2, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(sim_out, {{10, 14}, {10, 15}}, {}, {});

    // Test: RE-RUT-2
    SetHeight(sim_out, 10, 15, -0.8, -0.5, -0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 10, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.5, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-RUT-3
    SetHeight(sim_out, 10, 15, -0.8, -0.3, -0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 10, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}}, {});

    // Test: RE-RUT-4
    SetHeight(sim_out, 10, 15, -0.4, -0.4, -0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 14, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.4, -0.2, -0.1, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[0], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RUT-5
    SetHeight(
        sim_out, 10, 15, -0.8, -0.7, -0.5, -0.5, -0.3, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 10, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.7, -0.5, -0.3, NAN, NAN);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RUT-6
    SetHeight(sim_out, 10, 15, -0.7, -0.2, -0.1, -0.1, 0.3, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.4);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 10, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.4, -0.1, 0.3, NAN, NAN);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RUT-7
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.5, -0.5, -0.3, NAN, NAN, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.2);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 13, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.5, -0.2, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RUT-8
    SetHeight(sim_out, 10, 15, -0.6, NAN, NAN, NAN, NAN, -0.4, 0.3, NAN, NAN);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 20, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(sim_out, {{10, 14}, {10, 15}}, {{2, 10, 15}}, {});

    // Test: RE-RUT-9
    SetHeight(sim_out, 10, 15, -0.6, NAN, NAN, NAN, NAN, 0.0, 0.3, NAN, NAN);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 20, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(sim_out, {{10, 14}, {10, 15}}, {{2, 10, 15}}, {});

    // Test: RE-RUT-10
    SetHeight(sim_out, 10, 15, -0.4, NAN, NAN, NAN, NAN, -0.4, -0.3, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 22, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.4, NAN, NAN, -0.3, -0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[0], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RUT-11
    SetHeight(
        sim_out, 10, 15, -0.8, NAN, NAN, NAN, NAN, -0.7, -0.5, -0.5, -0.3);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 20, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.7, NAN, NAN, -0.5, -0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RUT-12
    SetHeight(sim_out, 10, 15, -0.3, NAN, NAN, NAN, NAN, 0.0, 0.3, 0.3, 0.5);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 20, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.2, NAN, NAN, 0.3, 0.5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RUT-13
    SetHeight(
        sim_out, 10, 15, -0.8, NAN, NAN, NAN, NAN, -0.8, -0.5, -0.5, -0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.2);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 21, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, NAN, NAN, -0.5, -0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RUT-14
    SetHeight(
        sim_out, 10, 15, -0.8, -0.7, -0.6, NAN, NAN, -0.4, -0.3, NAN, NAN);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 30, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-RUT-15
    SetHeight(sim_out, 10, 15, -0.5, -0.1, 0.0, NAN, NAN, 0.2, 0.4, NAN, NAN);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 30, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-RUT-16
    SetHeight(sim_out, 10, 15, -0.8, -0.8, -0.6, NAN, NAN, 0.1, 0.3, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 34, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.3, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[0], 0, 10, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}});

    // Test: RE-RUT-17
    SetHeight(sim_out, 10, 15, -0.8, -0.8, -0.6, NAN, NAN, -0.4, 0.3, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 34, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.4, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[0], 0, 10, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}});

    // Test: RE-RUT-18
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.4, -0.4, -0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 32, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.4, -0.3, -0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-19
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.4, -0.4, -0.3, 0.4, 0.7, 0.7, 0.9);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 33, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.4, -0.2, 0.7, 0.9);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-20
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.5, -0.4, -0.3, -0.3, -0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 33, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.4, -0.3, -0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-21
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.4, -0.4, -0.3, -0.3, -0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 31, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.4, -0.3, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-22
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, NAN, NAN, -0.7, -0.6, NAN, NAN);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 30, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-RUT-23
    SetHeight(sim_out, 10, 15, -0.8, -0.1, 0.0, NAN, NAN, -0.3, -0.2, NAN, NAN);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 30, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-RUT-24
    SetHeight(sim_out, 10, 15, -0.8, -0.2, 0.3, NAN, NAN, -0.8, -0.6, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 32, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, NAN, NAN, -0.6, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[0], 2, 10, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{2, 10, 15}});

    // Test: RE-RUT-25
    SetHeight(sim_out, 10, 15, -0.8, -0.3, 0.3, NAN, NAN, -0.8, -0.4, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 32, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, NAN, NAN, -0.4, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[0], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{2, 10, 15}});

    // Test: RE-RUT-26
    SetHeight(
        sim_out, 10, 15, -0.8, -0.6, -0.4, NAN, NAN, -0.8, -0.7, -0.7, -0.6);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 34, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.2, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.4, -0.2, -0.7, -0.6);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-27
    SetHeight(
        sim_out, 10, 15, -0.8, -0.2, -0.1, -0.1, 0.5, -0.8, -0.7, -0.7, -0.6);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.6);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 31, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.3, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.1, 0.5, -0.7, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.3);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-28
    SetHeight(
        sim_out, 10, 15, -0.8, -0.5, -0.4, -0.4, 0.5, -0.8, -0.7, -0.7, -0.6);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.9);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 31, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.4, 0.5, -0.7, -0.5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RUT-29
    SetHeight(
        sim_out, 10, 15, -0.8, -0.5, -0.4, -0.4, -0.3, -0.8, -0.7, -0.7, -0.5);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxUnstableTerrainCell(
        sim_out, 33, 0.1, 10, 14, 10, 15, grid, bucket, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.4, -0.2, -0.7, -0.5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
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
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};

    // Declaring variables
    std::vector<float> pos0;
    std::vector<float> pos2;
    std::vector<float> posA;
    std::vector<float> posB;

    // Creating a lambda function to set relax_area_
    auto SetRelaxArea = [&]() {
        sim_out->relax_area_[0][0] = 10;
        sim_out->relax_area_[0][1] = 15;
        sim_out->relax_area_[1][0] = 10;
        sim_out->relax_area_[1][1] = 15;
    };

    // Creating a lambda function to check relax_area_
    auto CheckRelaxArea = [&](int a_00, int a_01, int a_10, int a_11) {
        EXPECT_EQ(sim_out->relax_area_[0][0], a_00);
        EXPECT_EQ(sim_out->relax_area_[0][1], a_01);
        EXPECT_EQ(sim_out->relax_area_[1][0], a_10);
        EXPECT_EQ(sim_out->relax_area_[1][1], a_11);
    };

    // Test: RE-RT-1
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    sim_out->terrain_[10][15] = -0.1;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(10, 15, 10, 15);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], 0.0, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(sim_out, {{10, 15}, {10, 16}}, {}, {});

    // Test: RE-RT-2
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    sim_out->terrain_[10][15] = -0.2;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(sim_out, {{10, 15}, {10, 16}}, {}, {});

    // Test: RE-RT-3
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.2, -0.1, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}}, {});

    // Test: RE-RT-4
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.4, -0.4, -0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.4, -0.2, -0.1, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[0], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RT-5
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.4, -0.4, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}}, {});

    // Test: RE-RT-6
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.8, -0.7, -0.5, -0.5, 0.0, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.7, -0.5, 0.0, NAN, NAN);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RT-7
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.5, -0.5, -0.3, NAN, NAN, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.2);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.5, -0.1, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RT-8
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.8, -0.8, -0.5, -0.5, 0.0, NAN, NAN, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.5);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    CheckHeight(sim_out, 10, 15, -0.8, -0.5, 0.0, NAN, NAN);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(sim_out, {{10, 15}}, {{0, 10, 15}}, {{0, 10, 15}});

    // Test: RE-RT-9
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.2, NAN, NAN, NAN, NAN, -0.1, 0.0, NAN, NAN);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(sim_out, {{10, 15}, {10, 16}}, {{2, 10, 15}}, {});

    // Test: RE-RT-10
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.4, NAN, NAN, NAN, NAN, -0.4, -0.2, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.4, NAN, NAN, -0.2, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[0], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RT-11
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.4, NAN, NAN, NAN, NAN, -0.4, 0.0, NAN, NAN);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(sim_out, {{10, 15}}, {{2, 10, 15}}, {});

    // Test: RE-RT-12
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, NAN, NAN, NAN, NAN, -0.7, -0.5, -0.5, -0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.4);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.7, NAN, NAN, -0.5, -0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RT-13
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, NAN, NAN, NAN, NAN, -0.8, -0.5, -0.5, -0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.3);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, NAN, NAN, -0.5, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RT-14
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.8, NAN, NAN, NAN, NAN, -0.8, -0.5, -0.5, 0.0);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    CheckHeight(sim_out, 10, 15, -0.8, NAN, NAN, -0.5, 0.0);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(sim_out, {{10, 15}}, {{2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RT-15
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.4, -0.2, 0.0, NAN, NAN, 0.2, 0.4, NAN, NAN);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-RT-16
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.8, -0.8, -0.6, NAN, NAN, -0.4, 0.0, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.4, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[0], 0, 10, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}});

    // Test: RE-RT-17
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, NAN, NAN, -0.4, -0.2, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.4, -0.2, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[0], 0, 10, 15, posA, 0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-18
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.4, -0.2, -0.1, NAN, NAN, 0.4, 0.5, 0.5, 0.7);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.5, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.2, NAN, NAN, 0.5, 0.7);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{2, 10, 15}});

    // Test: RE-RT-19
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, NAN, NAN, -0.4, -0.3, -0.3, 0.0);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.4, -0.3, 0.0);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-20
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, NAN, NAN, -0.4, -0.3, -0.3, -0.2);
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posB, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.4, -0.3, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-21
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.3, -0.1, 0.0, 0.0, 0.1, 0.2, 0.3, NAN, NAN);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.1, 0.0, 0.1, NAN, NAN);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}});

    // Test: RE-RT-22
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.5, -0.4, 0.0, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.4, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}});

    // Test: RE-RT-23
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.5, -0.4, -0.2, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.4, -0.2, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-24
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.6, -0.5, -0.4, -0.4, 0.0, 0.0, 0.3, 0.3, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.4);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.5, -0.4, 0.0, 0.3, 0.4);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-25
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.5, -0.4, -0.3, -0.3, 0.0);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.3);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.4, -0.3, 0.0);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-26
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.6, -0.6, -0.5, -0.4, -0.3, -0.3, -0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posB, 0.3);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.6, -0.4, -0.3, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-27
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.8, 0.4, 0.7, NAN, NAN, -0.7, -0.1, NAN, NAN);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.7, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}}, {});

    // Test: RE-RT-28
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.8, -0.4, 0.0, NAN, NAN, -0.8, -0.6, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, NAN, NAN, -0.6, -0.4);
    CheckBodySoilPos(sim_out->body_soil_pos_[0], 2, 10, 15, posA, 0.2);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{2, 10, 15}});

    // Test: RE-RT-29
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.2, NAN, NAN, -0.8, -0.6, NAN, NAN);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.2, -0.1, -0.6, -0.4);
    CheckBodySoilPos(sim_out->body_soil_pos_[0], 2, 10, 15, posA, 0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-30
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.2, 0.4, 0.5, 0.5, 0.6, 0.0, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.1, 0.5, 0.6, NAN, NAN);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}});

    // Test: RE-RT-31
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.8, 0.4, 0.5, 0.5, 0.6, -0.8, -0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, 0.5, 0.6, -0.2, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-32
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, -0.2, -0.8, -0.6, NAN, NAN);
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posB, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.3, -0.1, -0.6, -0.4);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-33
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.1, NAN, NAN, -0.7, -0.6, -0.6, -0.4);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.7, NAN, NAN, -0.6, -0.4);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{2, 10, 15}});

    // Test: RE-RT-34
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.1, 0.1, NAN, NAN, -0.8, -0.6, -0.6, -0.5);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, NAN, NAN, -0.6, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{9, 15}, {10, 14}, {10, 15}, {10, 16}},
        {{0, 10, 15}, {2, 10, 15}}, {{2, 10, 15}});

    // Test: RE-RT-35
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.2, NAN, NAN, -0.8, -0.6, -0.6, -0.5);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.2);
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.2, -0.1, -0.6, -0.4);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-36
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, -0.1, -0.7, -0.6, -0.6, -0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.7, -0.3, -0.1, -0.6, -0.4);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-37
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, 0.0, -0.8, -0.6, -0.6, -0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.3, 0.0, -0.6, -0.4);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-38
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, -0.2, -0.8, -0.6, -0.6, -0.5);
    posB = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posB, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    CheckHeight(sim_out, 10, 15, -0.8, -0.3, -0.1, -0.6, -0.4);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 10, 15, posB, 0.1);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{9, 15}, {10, 15}, {10, 16}}, {{0, 10, 15}, {2, 10, 15}},
        {{0, 10, 15}, {2, 10, 15}});

    // Test: RE-RT-39
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    SetHeight(sim_out, 10, 15, -0.6, 0.0, 0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][14], -0.1, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(
        sim_out, {{9, 15}, {10, 14}, {10, 15}, {10, 16}}, {{0, 10, 15}}, {});

    // Test: RE-RT-40
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    sim_out->terrain_[10][15] = -0.4;
    soil_simulator::RelaxTerrain(sim_out, grid, bucket, sim_param, 1e-5);
    CheckRelaxArea(5, 15, 10, 20);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][16], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[9][15], -0.1, 1e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 0);
    ResetValueAndTest(sim_out, {{9, 15}, {10, 15}, {10, 16}}, {}, {});

    // Test: RE-RT-41
    soil_simulator::rng.seed(200);
    SetRelaxArea();
    sim_out->terrain_[10][15] = -0.2;
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
    ResetValueAndTest(sim_out, {{10, 15}, {11, 15}}, {}, {});

    delete bucket;
    delete sim_out;
}

TEST(UnitTestRelax, CheckUnstableBodyCell) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    // Declaring variables
    int status;

    // Test: RE-CUB-1
    sim_out->terrain_[10][15] = -0.2;
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 40);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // Test: RE-CUB-2
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, 0.1, 0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 10);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-CUB-3
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, 0.3, 0.5, 0.5, 0.7, NAN, NAN, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 10);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-CUB-4
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 14);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-CUB-5
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, -0.1, -0.1, 0.0, NAN, NAN, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 13);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-CUB-6
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, NAN, NAN, NAN, NAN, 0.4, 0.5, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 20);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-CUB-7
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, NAN, NAN, NAN, NAN, 0.1, 0.2, 0.2, 0.3);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 20);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-CUB-8
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, NAN, NAN, NAN, NAN, -0.2, 0.0, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 22);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-CUB-9
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, NAN, NAN, NAN, NAN, -0.2, -0.1, -0.1, 0.0);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 21);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-CUB-10
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, -0.1, NAN, NAN, 0.1, 0.2, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 34);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-CUB-11
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, -0.1, -0.1, 0.0, 0.1, 0.2, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 33);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-CUB-12
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, -0.1, NAN, NAN, 0.1, 0.2, 0.2, 0.3);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 34);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-CUB-13
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, -0.1, -0.1, 0.0, 0.1, 0.2, 0.2, 0.3);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 33);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUB-14
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.4, -0.3, NAN, NAN, -0.2, -0.1, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 32);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-CUB-15
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.4, -0.3, -0.3, -0.2, -0.2, -0.1, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 32);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-CUB-16
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.4, -0.3, NAN, NAN, -0.2, -0.1, -0.1, 0.0);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 31);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-CUB-17
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.4, -0.3, -0.3, -0.2, -0.2, -0.1, -0.1, 0.0);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 31);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUB-18
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, 0.1, 0.2, NAN, NAN, -0.2, -0.1, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 32);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-CUB-19
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, 0.1, 0.2, NAN, NAN, -0.2, -0.1, -0.1, 0.0);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 31);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-CUB-20
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, 0.1, 0.2, 0.2, 0.3, -0.2, -0.1, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 32);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-CUB-21
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, 0.1, 0.2, 0.2, 0.3, -0.2, -0.1, -0.1, 0.0);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 31);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUB-22
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.2, -0.1, NAN, NAN, -0.4, -0.3, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 34);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-CUB-23
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.2, -0.1, NAN, NAN, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 34);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-CUB-24
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.2, -0.1, -0.1, 0.0, -0.4, -0.3, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 33);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-CUB-25
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.2, -0.1, -0.1, 0.0, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 33);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUB-26
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.2, 0.2, NAN, NAN, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-CUB-27
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.2, -0.1, -0.1, 0.2, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUB-28
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.4, -0.3, -0.3, -0.1, -0.1, 0.5, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-CUB-29
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.4, -0.3, -0.3, -0.1, -0.1, 0.0, 0.0, 0.1);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUB-30
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.2, -0.1, NAN, NAN, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 34);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-CUB-31
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.2, -0.1, -0.1, 0.0, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 33);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUB-32
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.4, -0.3, -0.3, -0.2, -0.2, -0.1, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 32);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-CUB-33
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.4, -0.3, -0.3, -0.2, -0.2, -0.1, -0.1, 0.0);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 31);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-CUB-34
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    sim_out->terrain_[10][15] = 0.1;
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // Test: RE-CUB-35
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-CUB-36
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, 0.0, 0.0, 0.1, NAN, NAN, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-CUB-37
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, NAN, NAN, NAN, NAN, -0.2, 0.1, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-CUB-38
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, NAN, NAN, NAN, NAN, -0.2, 0.0, 0.0, 0.1);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-CUB-39
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, 0.0, 0.1, NAN, NAN, -0.4, -0.3, -0.3, -0.2);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-CUB-40
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, 0.1, 0.2, NAN, NAN, -0.4, -0.3, -0.3, 0.1);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-CUB-41
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.4, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, -0.4, -0.3, NAN, NAN, 0.0, 0.1, 0.1, 0.3);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, 0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-CUB-42
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.4, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, -0.4, -0.1, NAN, NAN, 0.1, 0.2, NAN, NAN);
    status = CheckUnstableBodyCell(sim_out, 10, 14, 0, 10, 15, -0.1, 1e-5);
    EXPECT_EQ(status, 0);
    ResetValueAndTest(
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
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};

    // Declaring variables
    std::vector<float> pos0;
    std::vector<float> pos2;
    std::vector<float> posA;

    // Test: RE-RUB-1
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    sim_out->terrain_[10][15] = 0.0;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 40, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // Test: RE-RUB-2
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    sim_out->terrain_[10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 40, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.0, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // Test: RE-RUB-3
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    sim_out->terrain_[10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 40, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // Test: RE-RUB-4
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, 0.0, 0.3, 0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 10, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.1, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-RUB-5
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, 0.1, 0.4, 0.4, 0.5, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.4, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 10, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, 0.4, 0.5, NAN, NAN);
    EXPECT_EQ(body_soil_pos->size(), 0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RUB-6
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.5, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, 0.0, 0.2, 0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.5);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 10, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.3, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], 0.2, 1e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.3, 1.e-5);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-RUB-7
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 14, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, 0.0, 0.1, NAN, NAN);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-8
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, -0.3, -0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 14, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, 0.0, NAN, NAN);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-9
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, -0.3, -0.2, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 14, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, -0.1, NAN, NAN);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-10
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, -0.3, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.2);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 13, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, 0.1, NAN, NAN);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-11
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, -0.3, -0.2, -0.2, -0.1, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 13, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, 0.0, NAN, NAN);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-12
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.2, 0.2, 0.4, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, -0.3, -0.2, -0.2, -0.1, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 13, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.2, 0.3, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, 0.0, NAN, NAN);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-13
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, 0.0, NAN, NAN, NAN, NAN, 0.5, 0.6, 0.6, 0.7);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 20, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, 0.1, NAN, NAN, 0.6, 0.7);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RUB-14
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, NAN, NAN, NAN, NAN, 0.5, 0.6, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 20, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-RUB-15
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, NAN, NAN, NAN, NAN, -0.2, 0.0, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 22, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, NAN, NAN, 0.0, 0.1);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-16
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, NAN, NAN, NAN, NAN, -0.2, -0.1, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 22, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, NAN, NAN, -0.1, 0.0);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-17
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.3, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, NAN, NAN, NAN, NAN, -0.3, -0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 22, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.1, 0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, NAN, NAN, -0.2, -0.1);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-18
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, NAN, NAN, NAN, NAN, -0.2, -0.1, -0.1, 0.0);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 21, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, NAN, NAN, -0.1, 0.1);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-19
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, NAN, NAN, NAN, NAN, -0.2, -0.1, -0.1, 0.0);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 21, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, NAN, NAN, -0.1, 0.1);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-20
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.3, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, NAN, NAN, NAN, NAN, -0.3, -0.2, -0.2, -0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 21, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.1, 0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, NAN, NAN, -0.2, 0.0);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-21
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.3, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, -0.1, NAN, NAN, 0.1, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, -0.1, 0.1, NAN, NAN);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-22
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, -0.1, NAN, NAN, 0.1, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, -0.1, 0.0, NAN, NAN);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-23
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, -0.3, -0.2, NAN, NAN, 0.1, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, -0.1, NAN, NAN);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-24
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.5, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, -0.1, NAN, NAN, 0.1, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.5);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.3, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, -0.1, 0.1, NAN, NAN);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-25
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.5, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, -0.1, NAN, NAN, 0.0, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.5);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.4, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, NAN, NAN, 0.3, 0.4);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-26
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.5, 0.5, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, -0.2, -0.1, NAN, NAN, 0.0, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, NAN, NAN, 0.2, 0.5);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.3);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-27
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.3, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, -0.3, -0.2, -0.2, -0.1, 0.1, 0.3, 0.3, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, 0.1, 0.3, 0.5);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-28
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.3, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, -0.3, -0.2, -0.2, -0.1, 0.2, 0.3, 0.3, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, 0.1, 0.3, 0.5);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-29
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.3, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, -0.3, -0.2, -0.2, -0.1, 0.2, 0.3, 0.3, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.1, 0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, 0.0, 0.3, 0.5);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-30
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.9, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, -0.3, -0.2, -0.2, -0.1, 0.2, 0.3, 0.3, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.8);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.1, 0.6, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, 0.2, 0.3, 0.5);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.3);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-31
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.9, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, -0.3, -0.2, -0.2, -0.1, 0.1, 0.3, 0.3, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.8);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.2);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.1, 0.7, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, -0.1, 0.3, 0.7);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-32
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.7, 0.7, 0.9, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, -0.3, -0.2, -0.2, -0.1, 0.1, 0.3, 0.3, 0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.2);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, -0.1, 0.3, 0.7);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-33
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.3, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, 0.1, 0.3, NAN, NAN, -0.2, -0.1, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, NAN, NAN, -0.1, 0.1);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-34
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.3, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, 0.2, 0.4, NAN, NAN, -0.2, 0.0, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.0, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, NAN, NAN, 0.0, 0.2);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-35
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.3, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, 0.2, 0.4, NAN, NAN, -0.2, 0.0, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.0, 1, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.1, 0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, NAN, NAN, 0.0, 0.1);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-36
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, 0.2, 0.4, NAN, NAN, -0.2, 0.0, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.1, 0.6, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, NAN, NAN, 0.0, 0.2);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-37
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, 0.1, 0.4, NAN, NAN, -0.2, 0.0, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.4, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.1, 0.6, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, 0.4, 0.6, NAN, NAN);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-38
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.6, 0.6, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.2, 0.1, 0.2, NAN, NAN, -0.2, 0.0, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.2, 0.2, 0.4, NAN, NAN);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-39
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.3, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, 0.1, 0.3, 0.3, 0.8, -0.3, -0.2, -0.2, -0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, 0.3, 0.8, -0.2, 0.1);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-40
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, 0.1, 0.3, 0.3, 0.8, -0.3, -0.2, -0.2, -0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, 0.3, 0.8, -0.2, 0.0);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-41
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, 0.1, 0.3, 0.3, 0.8, -0.4, -0.3, -0.3, -0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, 0.3, 0.8, -0.3, -0.1);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-42
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, 0.1, 0.3, 0.3, 0.8, -0.3, -0.2, -0.2, -0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.8);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.6, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, 0.3, 0.8, -0.2, 0.1);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-43
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.1, 0.1, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, 0.1, 0.3, 0.3, 0.5, -0.3, -0.2, -0.2, -0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.2);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.1, 0.7, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, 0.3, 0.6, -0.2, -0.1);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-44
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.6, 0.6, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.3, 0.1, 0.3, 0.3, 0.4, -0.3, -0.2, -0.2, -0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, 0.3, 0.6, -0.2, -0.1);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-45
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, -0.3, -0.2, -0.2, 0.1, 0.1, 0.3, 0.3, 0.8);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.8);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.3);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.5);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.8, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, 0.1, 0.3, 0.8);
    EXPECT_EQ(body_soil_pos->size(), 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-46
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, 0.1, 0.3, 0.3, 0.8, -0.3, -0.2, -0.2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.8);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.5);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.3);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.8, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, 0.3, 0.8, -0.2, 0.1);
    EXPECT_EQ(body_soil_pos->size(), 0);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-47
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, -0.3, -0.2, -0.2, 0.1, 0.1, 0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.8);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.2, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 32, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.5, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, 0.1, 0.2, 0.5);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.3);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-48
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, -0.3, -0.2, -0.2, 0.1, 0.1, 0.2, 0.2, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.8);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.2, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.2);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 31, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.6, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, -0.2, 0.1, 0.2, 0.6);
    CheckBodySoilPos((*body_soil_pos)[0], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-49
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, 0.4, 0.5, NAN, NAN, -0.3, -0.2, -0.2, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.8);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.6);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.5, grid, bucket);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 34, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.7, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, 0.5, 0.6, -0.2, 0.4);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-50
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.8, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, 0.4, 0.5, 0.5, 0.6, -0.3, -0.2, -0.2, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.8);
    posA = soil_simulator::CalcBucketFramePos(10, 15, 0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.6);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 33, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.7, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.3, 0.5, 0.7, -0.2, 0.4);
    CheckBodySoilPos((*body_soil_pos)[0], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});
    body_soil_pos->erase(body_soil_pos->begin(), body_soil_pos->end());

    // Test: RE-RUB-51
    SetHeight(sim_out, 10, 14, -0.2, -0.2, 0.0, 0.0, 0.5, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, 0.0, NAN, NAN, NAN, NAN, 0.2, 0.3, 0.3, 0.7);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.5);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.4);
    soil_simulator::RelaxUnstableBodyCell(
        sim_out, 20, body_soil_pos, 0.1, 0, 10, 14, 0, 10, 15, grid, bucket,
        1e-5);
    CheckHeight(sim_out, 10, 14, -0.2, 0.0, 0.3, NAN, NAN);
    CheckHeight(sim_out, 10, 15, 0.2, NAN, NAN, 0.3, 0.7);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.3, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(body_soil_pos->size(), 0);
    ResetValueAndTest(
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
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};

    // Declaring variables
    std::vector<float> pos0;
    std::vector<float> pos2;
    std::vector<float> posA;
    std::vector<std::vector<int>> terrain_pos;

    // Test: RE-RBS-1
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.3, -0.3, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    sim_out->terrain_[10][15] = -0.2;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.3, -0.2, -0.1, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.1, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // Test: RE-RBS-2
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.3, -0.3, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    sim_out->terrain_[10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.3, 0.0, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // Test: RE-RBS-3
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.3, -0.3, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    sim_out->terrain_[10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.3, 0.0, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // Test: RE-RBS-4
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, -0.1, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.2, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-RBS-5
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, 0.0, 0.2, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.6, -0.1, 0.0, 0.0, 0.1, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, 0.0, 0.1, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-6
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, -0.2, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.4);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-RBS-7
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, -0.4, -0.3, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, -0.3, -0.2, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-8
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.6, -0.6, -0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.5, -0.3, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-9
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.6, -0.6, -0.5, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.5, -0.3, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-10
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.4, -0.3, -0.3, -0.2, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, -0.3, -0.1, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-11
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.7, -0.7, -0.6, -0.6, -0.5, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.7, -0.6, -0.3, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-12
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.7, -0.7, -0.6, -0.6, -0.5, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.7, -0.6, -0.3, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-13
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, NAN, NAN, NAN, NAN, 0.0, 0.1, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.2, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-RBS-14
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.6, NAN, NAN, NAN, NAN, 0.0, 0.1, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-RBS-15
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.3, NAN, NAN, NAN, NAN, -0.2, 0.1, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.4);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-RBS-16
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, NAN, NAN, NAN, NAN, -0.4, -0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, NAN, NAN, -0.3, -0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-17
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.6, NAN, NAN, NAN, NAN, -0.6, -0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.4);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, NAN, NAN, -0.5, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-18
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.6, NAN, NAN, NAN, NAN, -0.6, -0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, NAN, NAN, -0.5, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-19
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, NAN, NAN, NAN, NAN, -0.4, -0.3, -0.3, -0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, NAN, NAN, -0.3, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-20
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.7, NAN, NAN, NAN, NAN, -0.7, -0.6, -0.6, -0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.7, NAN, NAN, -0.6, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-21
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.7, NAN, NAN, NAN, NAN, -0.7, -0.6, -0.6, -0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.7, NAN, NAN, -0.6, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-22
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, -0.4, -0.3, NAN, NAN, 0.1, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, -0.3, -0.2, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-23
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.6, -0.6, -0.5, NAN, NAN, 0.1, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.5, -0.3, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-24
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.6, -0.6, -0.5, NAN, NAN, 0.1, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.5, -0.3, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-25
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.6, -0.5, NAN, NAN, -0.4, -0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.2, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, NAN, NAN, -0.2, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-26
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.7, NAN, NAN, -0.6, -0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, NAN, NAN, -0.5, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-27
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.6, -0.5, -0.5, -0.3, 0.1, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.5, -0.2, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-28
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.6, -0.5, -0.5, -0.4, 0.1, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.5, -0.3, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-29
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.1, -0.1, 0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.6, -0.5, -0.5, -0.4, 0.1, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.5, -0.2, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-30
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.7, -0.7, -0.6, -0.4, -0.2, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.2, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.7, -0.6, -0.2, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-31
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.7, -0.7, -0.6, -0.5, -0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.7, -0.6, -0.4, -0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-32
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, -0.4, -0.2, NAN, NAN, -0.1, 0.0, 0.0, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, -0.2, -0.1, 0.0, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-33
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.5, -0.5, -0.4, NAN, NAN, -0.1, 0.0, 0.0, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.5, -0.4, -0.2, 0.0, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-34
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.3, -0.3, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.6, -0.5, NAN, NAN, -0.4, -0.3, -0.3, -0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.3, -0.2, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, NAN, NAN, -0.3, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-35
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.3, -0.3, -0.1, -0.1, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.6, -0.5, NAN, NAN, -0.4, -0.3, -0.3, -0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.1, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.3, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, NAN, NAN, -0.3, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-36
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.6, -0.5, -0.5, -0.4, -0.2, -0.1, -0.1, 0.0);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.5, -0.2, -0.1, 0.0);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[2].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-37
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.7, -0.7, -0.6, -0.6, -0.5, -0.2, -0.1, -0.1, 0.0);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.1, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.7, -0.6, -0.3, -0.1, 0.0);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-38
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.6, -0.5, -0.5, -0.4, -0.3, -0.2, -0.2, -0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.2, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.5, -0.4, -0.2, 0.0);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-39
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.7, -0.7, -0.6, -0.6, -0.5, -0.5, -0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.7, -0.6, -0.5, -0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-40
    soil_simulator::rng.seed(1236);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.3, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.6, -0.6, -0.4, NAN, NAN, -0.2, 0.0, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.6);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.4, -0.2, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-41
    soil_simulator::rng.seed(1236);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.6, -0.5, -0.5, -0.3, -0.2, 0.0, 0.0, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.5);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.2);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.5, -0.2, 0.0, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-42
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.6, -0.4, -0.4, -0.2, -0.2, 0.0, 0.0, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, 0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.4, -0.2, 0.0, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.4, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-43
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, -0.1, 0.0, NAN, NAN, -0.4, -0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, NAN, NAN, -0.3, -0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-44
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.6, -0.1, 0.0, NAN, NAN, -0.6, -0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, NAN, NAN, -0.5, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-45
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.6, -0.1, 0.0, NAN, NAN, -0.6, -0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, NAN, NAN, -0.5, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-46
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.3, -0.2, NAN, NAN, -0.6, -0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.2, -0.1, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-47
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.2, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.5, -0.4, NAN, NAN, -0.8, -0.7, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.4, -0.3, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-48
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.1, 0.0, NAN, NAN, -0.6, -0.5, -0.5, -0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, NAN, NAN, -0.5, -0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-49
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.7, -0.1, 0.0, NAN, NAN, -0.7, -0.6, -0.6, -0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.7, NAN, NAN, -0.6, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-50
    soil_simulator::rng.seed(1234);
    SetHeight(
        sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, -0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.7, -0.1, 0.0, NAN, NAN, -0.7, -0.6, -0.6, -0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.7, NAN, NAN, -0.6, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[4], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 5);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-51
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.4, -0.3, NAN, NAN, -0.6, -0.5, -0.5, -0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.3, -0.2, -0.5, -0.4);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-52
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.7, -0.5, -0.4, NAN, NAN, -0.7, -0.6, -0.6, -0.5);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.6, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.7, -0.4, -0.2, -0.6, -0.5);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-53
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, -0.1, 0.0, 0.0, 0.1, -0.4, -0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, 0.0, 0.1, -0.3, -0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-54
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.6, -0.1, 0.0, 0.0, 0.1, -0.6, -0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, 0.0, 0.1, -0.5, -0.3);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-55
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.6, -0.4, -0.3, -0.3, -0.2, -0.6, -0.5, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.2, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.6, -0.3, -0.1, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-56
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.6, -0.5, -0.5, -0.4, -0.8, -0.7, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.5, -0.2, NAN, NAN);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-57
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.1, 0.0, 0.0, 0.1, -0.8, -0.7, -0.7, -0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.3);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, -0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, 0.0, 0.1, -0.7, -0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.1, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-58
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.1, 0.0, 0.0, 0.1, -0.8, -0.7, -0.7, -0.6);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, 0.0, 0.1, -0.7, -0.4);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[1].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-59
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.3, -0.3, 0.1, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.3, -0.2, -0.2, -0.1, -0.8, -0.7, -0.7, -0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.4);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.3);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, -0.3, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.2, 0.0, -0.7, -0.4);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.3, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-60
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.6, -0.5, -0.5, -0.4, -0.8, -0.7, -0.7, -0.6);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.4, 0.0, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.5, -0.2, -0.7, -0.6);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.0, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-61
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.5, -0.3, NAN, NAN, -0.8, -0.7, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, -0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, NAN, NAN, -0.7, -0.5);
    CheckBodySoilPos(sim_out->body_soil_pos_[1], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-62
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, -0.2, -0.8, -0.7, -0.7, -0.6);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, -0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.3, -0.2, -0.7, -0.4);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-63
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, -0.1, -0.8, -0.7, -0.7, -0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.3);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.3, -0.1, -0.7, -0.4);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-64
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.7, -0.7, -0.5, -0.5, -0.4, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, -0.2, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.7, -0.5, -0.4, -0.2);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 2, 10, 15, posA, 0.2);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.5, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-65
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.8, -0.7, -0.7, -0.5, -0.5, -0.4, -0.4, -0.2);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.4, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, posA, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.7, -0.5, -0.4, -0.1);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 2, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-66
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, NAN, NAN, -0.8, -0.7, -0.7, -0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.3);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.3, -0.2, -0.7, -0.4);
    CheckBodySoilPos(sim_out->body_soil_pos_[2], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-67
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.8, -0.4, -0.3, -0.3, -0.2, -0.8, -0.7, -0.7, -0.4);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    posA = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, posA, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.3);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, -0.1, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.8, -0.3, -0.1, -0.7, -0.4);
    CheckBodySoilPos(sim_out->body_soil_pos_[3], 0, 10, 15, posA, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.6, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 4);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-68
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    // Test: RE-RBS-69
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, -0.4, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-RBS-70
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, -0.4, -0.2, -0.2, 0.0, NAN, NAN, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.2, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, -0.2, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-71
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, NAN, NAN, NAN, NAN, -0.4, 0.1, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-RBS-72
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, NAN, NAN, NAN, NAN, -0.4, -0.3, -0.3, -0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, NAN, NAN, -0.3, -0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-73
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, -0.4, -0.1, NAN, NAN, 0.1, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-RBS-74
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.4, -0.3, -0.3, -0.1, 0.1, 0.3, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, -0.3, -0.1, NAN, NAN);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}});

    // Test: RE-RBS-75
    soil_simulator::rng.seed(1234);
    for (auto ii = 9; ii < 12;  ii++)
        for (auto jj = 13; jj < 16;  jj++)
            sim_out->terrain_[ii][jj] = 0.2;
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.4, -0.3, -0.3, -0.1, -0.1, 0.0, 0.0, 0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.2);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.1);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, -0.3, -0.1, 0.0, 0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    terrain_pos = {
        {9, 13}, {9, 14}, {9, 15}, {10, 13}, {10, 14}, {10, 15}, {11, 13},
        {11, 14}, {11, 15}};
    ResetValueAndTest(
        sim_out, terrain_pos, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-76
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, 0.0, 0.1, NAN, NAN, -0.4, -0.1, NAN, NAN);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, 0.0, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.4, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}});

    // Test: RE-RBS-77
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 10, 15, -0.4, 0.0, 0.1, NAN, NAN, -0.4, -0.2, -0.2, 0.0);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, NAN, NAN, -0.2, 0.0);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 2);
    ResetValueAndTest(
        sim_out, {{10, 14}, {10, 15}}, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {2, 10, 15}});

    // Test: RE-RBS-78
    soil_simulator::rng.seed(1234);
    for (auto ii = 9; ii < 12;  ii++)
        for (auto jj = 13; jj < 16;  jj++)
            sim_out->terrain_[ii][jj] = 0.2;
    SetHeight(sim_out, 10, 14, -0.8, -0.8, -0.7, -0.7, 0.0, NAN, NAN, NAN, NAN);
    SetHeight(
        sim_out, 10, 15, -0.4, -0.1, 0.0, 0.0, 0.1, -0.4, -0.3, -0.3, -0.1);
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.7, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.7);
    pos0 = soil_simulator::CalcBucketFramePos(10, 15, 0.0, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 15, pos0, 0.1);
    pos2 = soil_simulator::CalcBucketFramePos(10, 15, -0.3, grid, bucket);
    PushBodySoilPos(sim_out, 2, 10, 15, pos2, 0.2);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.8, -0.7, 0.0, NAN, NAN);
    CheckHeight(sim_out, 10, 15, -0.4, 0.0, 0.1, -0.3, -0.1);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.7, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 3);
    terrain_pos = {
        {9, 13}, {9, 14}, {9, 15}, {10, 13}, {10, 14}, {10, 15}, {11, 13},
        {11, 14}, {11, 15}};
    ResetValueAndTest(
        sim_out, terrain_pos, {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}},
        {{0, 10, 14}, {0, 10, 15}, {2, 10, 15}});

    // Test: RE-RBS-79
    soil_simulator::rng.seed(1234);
    SetHeight(sim_out, 10, 14, -0.6, -0.6, -0.5, -0.5, 0.0, NAN, NAN, NAN, NAN);
    sim_out->terrain_[10][13] = -0.4;
    sim_out->terrain_[10][15] = -0.4;
    pos0 = soil_simulator::CalcBucketFramePos(10, 14, -0.5, grid, bucket);
    PushBodySoilPos(sim_out, 0, 10, 14, pos0, 0.5);
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.6, -0.5, -0.3, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][13], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    soil_simulator::rng.seed(1235);
    sim_out->body_soil_[1][10][14] = 0.0;
    sim_out->terrain_[10][13] = -0.4;
    sim_out->terrain_[10][15] = -0.4;
    sim_out->body_soil_pos_[0].h_soil = 0.5;
    soil_simulator::RelaxBodySoil(sim_out, grid, bucket, sim_param, 1e-5);
    CheckHeight(sim_out, 10, 14, -0.6, -0.5, -0.3, NAN, NAN);
    EXPECT_NEAR(sim_out->terrain_[10][13], -0.3, 1e-5);
    EXPECT_NEAR(sim_out->terrain_[10][15], -0.2, 1e-5);
    EXPECT_NEAR(sim_out->body_soil_pos_[0].h_soil, 0.2, 1.e-5);
    EXPECT_EQ(sim_out->body_soil_pos_.size(), 1);
    ResetValueAndTest(
        sim_out, {{10, 13}, {10, 14}, {10, 15}}, {{0, 10, 14}}, {{0, 10, 14}});

    delete bucket;
    delete sim_out;
}
