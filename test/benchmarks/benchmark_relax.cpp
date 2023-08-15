/*
This file implements benchmarking for the functions in relax.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <benchmark/benchmark.h>
#include <random>
#include "src/relax.hpp"

// -- RelaxTerrain --
static void BM_RelaxTerrain(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimParam sim_param(0.85, 3, 4);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    for (auto ii = 49; ii < 65; ii++)
        for (auto jj = 49; jj < 65; jj++) {
            sim_out->terrain_[ii][jj] = 0.4;
        }
    for (auto jj = 49; jj < 65; jj++) {
        sim_out->body_[0][48][jj] = 0.0;
    }
    for (auto jj = 49; jj < 60; jj++) {
        sim_out->body_[1][48][jj] = 0.1;
    }
    for (auto jj = 60; jj < 65; jj++) {
        sim_out->body_[1][48][jj] = 0.4;
    }
    sim_out->impact_area_[0][0] = 44;
    sim_out->impact_area_[1][0] = 45;
    sim_out->impact_area_[0][1] = 68;
    sim_out->impact_area_[1][1] = 68;

    for (auto _ : state)
        soil_simulator::RelaxTerrain(sim_out, grid, sim_param, 1.e-5);
    delete sim_out;
}
BENCHMARK(BM_RelaxTerrain)->Unit(benchmark::kMicrosecond);

// -- RelaxBodySoil --
static void BM_RelaxBodySoil(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimParam sim_param(0.85, 3, 4);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    for (auto ii = 49; ii < 65; ii++)
        for (auto jj = 49; jj < 65; jj++) {
            sim_out->terrain_[ii][jj] = 0.4;
            sim_out->body_[0][ii][jj] = 0.0;
        }
    for (auto ii = 49; ii < 65; ii++)
        for (auto jj = 49; jj < 60; jj++) {
            sim_out->body_[1][ii][jj] = 0.1;
            sim_out->body_soil_[0][ii][jj] = 0.1;
            sim_out->body_soil_[1][ii][jj] = 0.4;
            sim_out->body_soil_pos_.push_back(std::vector<int> {0, ii, jj});
        }
    for (auto ii = 49; ii < 65; ii++)
        for (auto jj = 60; jj < 65; jj++) {
            sim_out->body_[1][ii][jj] = 0.4;
            sim_out->body_soil_[0][ii][jj] = 0.4;
            sim_out->body_soil_[1][ii][jj] = 0.7;
            sim_out->body_soil_pos_.push_back(std::vector<int> {0, ii, jj});
        }
    sim_out->impact_area_[0][0] = 44;
    sim_out->impact_area_[1][0] = 45;
    sim_out->impact_area_[0][1] = 68;
    sim_out->impact_area_[1][1] = 68;

    for (auto _ : state)
        soil_simulator::RelaxBodySoil(sim_out, grid, sim_param, 1.e-5);
    delete sim_out;
}
BENCHMARK(BM_RelaxBodySoil)->Unit(benchmark::kMicrosecond);

// -- LocateUnstableTerrainCell --
static void BM_LocateUnstableTerrainCell(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimParam sim_param(0.85, 3, 4);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    for (auto ii = 49; ii < 65; ii++)
        for (auto jj = 49; jj < 65; jj++)
            sim_out->terrain_[ii][jj] = 0.4;
    sim_out->impact_area_[0][0] = 44;
    sim_out->impact_area_[1][0] = 45;
    sim_out->impact_area_[0][1] = 68;
    sim_out->impact_area_[1][1] = 68;

    for (auto _ : state)
        soil_simulator::LocateUnstableTerrainCell(sim_out, 0.1, 1.e-5);
    delete sim_out;
}
BENCHMARK(BM_LocateUnstableTerrainCell)->Unit(benchmark::kMicrosecond);

// -- CheckUnstableTerrainCell --
static void BM_CheckUnstableTerrainCell(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimParam sim_param(0.85, 3, 4);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    sim_out->terrain_[50][55] = 0.4;

    for (auto _ : state)
        soil_simulator::CheckUnstableTerrainCell(sim_out, 50, 55, 0.2, 1.e-5);
    delete sim_out;
}
BENCHMARK(BM_CheckUnstableTerrainCell);

// -- RelaxUnstableTerrainCell --
static void BM_RelaxUnstableTerrainCell(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimParam sim_param(0.85, 3, 4);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    sim_out->terrain_[50][55] = 0.4;
    sim_out->body_[0][49][55] = 0.0;
    sim_out->body_[1][49][55] = 0.1;

    for (auto _ : state)
        soil_simulator::RelaxUnstableTerrainCell(
            sim_out, 142, 0.1, 50, 55, 49, 55, grid, 1.e-5);
    delete sim_out;
}
BENCHMARK(BM_RelaxUnstableTerrainCell)->Unit(benchmark::kMicrosecond);

// -- CheckUnstableBodyCell --
static void BM_CheckUnstableBodyCell(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimParam sim_param(0.85, 3, 4);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    sim_out->terrain_[50][61] = 0.4;
    sim_out->body_[0][50][61] = 0.0;
    sim_out->body_[1][50][61] = 0.4;
    sim_out->body_soil_[0][50][61] = 0.4;
    sim_out->body_soil_[1][50][61] = 0.7;
    sim_out->terrain_[50][60] = 0.4;
    sim_out->body_[0][50][60] = 0.0;
    sim_out->body_[1][50][60] = 0.1;
    sim_out->body_soil_[0][50][60] = 0.1;
    sim_out->body_soil_[1][50][60] = 0.4;
    sim_out->body_soil_pos_.push_back(std::vector<int> {0, 50, 61});
    sim_out->body_soil_pos_.push_back(std::vector<int> {0, 50, 60});

    for (auto _ : state)
        soil_simulator::CheckUnstableBodyCell(
            sim_out, 50, 61, 0, 50, 60, 0.1, 1.e-5);
    delete sim_out;
}
BENCHMARK(BM_CheckUnstableBodyCell);

// -- RelaxUnstableBodyCell --
static void BM_RelaxUnstableBodyCell(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimParam sim_param(0.85, 3, 4);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    sim_out->terrain_[50][61] = 0.4;
    sim_out->body_[0][50][61] = 0.0;
    sim_out->body_[1][50][61] = 0.4;
    sim_out->body_soil_[0][50][61] = 0.4;
    sim_out->body_soil_[1][50][61] = 0.7;
    sim_out->terrain_[50][60] = 0.4;
    sim_out->body_[0][50][60] = 0.0;
    sim_out->body_[1][50][60] = 0.1;
    sim_out->body_soil_[0][50][60] = 0.1;
    sim_out->body_soil_[1][50][60] = 0.4;
    sim_out->body_soil_pos_.push_back(std::vector<int> {0, 50, 61});
    sim_out->body_soil_pos_.push_back(std::vector<int> {0, 50, 60});
    std::vector<std::vector<int>> *body_soil_pos = (
        new std::vector<std::vector<int>>);

    for (auto _ : state)
        soil_simulator::RelaxUnstableBodyCell(
            sim_out, 13, body_soil_pos, 0.1, 50, 61, 0, 50, 60, grid, 1.e-5);
    delete sim_out;
}
BENCHMARK(BM_RelaxUnstableBodyCell)->Unit(benchmark::kMicrosecond);
