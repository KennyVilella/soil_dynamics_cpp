/*
This file implements benchmarking for the functions in intersecting_cells.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <benchmark/benchmark.h>
#include <glog/logging.h>
#include <random>
#include "soil_simulator/intersecting_cells.hpp"
#include "soil_simulator/bucket_pos.hpp"

// -- MoveIntersectingCells --
static void BM_MoveIntersectingCells(benchmark::State& state) {
    // Writting outputs to stderr instead of logfiles
    FLAGS_logtostderr = 1;

    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimParam sim_param(0.85, 3, 4);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    std::vector<float> ori = {0.707107, 0.707107, 0.0, 0.0};
    std::vector<float> pos = {0.0, 0.0, 0.0};
    soil_simulator::CalcBucketPos(
        sim_out, pos, ori, grid, bucket, sim_param, 1e-5);

    for (auto _ : state)
        soil_simulator::MoveIntersectingCells(sim_out, grid, bucket, 1.e-5);
    delete sim_out;
    delete bucket;
}
BENCHMARK(BM_MoveIntersectingCells)->Unit(benchmark::kMicrosecond);

// -- MoveIntersectingBody --
static void BM_MoveIntersectingBody(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimParam sim_param(0.85, 3, 4);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    std::vector<float> ori = {0.707107, 0.707107, 0.0, 0.0};
    std::vector<float> pos = {0.0, 0.0, 0.0};
    soil_simulator::CalcBucketPos(
        sim_out, pos, ori, grid, bucket, sim_param, 1e-5);

    for (auto _ : state)
        soil_simulator::MoveIntersectingBody(sim_out, 1.e-5);
    delete sim_out;
    delete bucket;
}
BENCHMARK(BM_MoveIntersectingBody)->Unit(benchmark::kMicrosecond);

// -- MoveIntersectingBodySoil --
static void BM_MoveIntersectingBodySoil(benchmark::State& state) {
    // Writting outputs to stderr instead of logfiles
    FLAGS_logtostderr = 1;

    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    for (auto ii = 19; ii < 30; ii++)
        for (auto jj = 23; jj < 41; jj++) {
            sim_out->body_[0][ii][jj] = 0.1;
            sim_out->body_[1][ii][jj] = 0.3;
            sim_out->body_[2][ii][jj] = 0.6;
            sim_out->body_[3][ii][jj] = 0.8;
            sim_out->body_soil_[0][ii][jj] = 0.3;
            sim_out->body_soil_[1][ii][jj] = 0.4;
            sim_out->body_soil_[2][ii][jj] = 0.8;
            sim_out->body_soil_[3][ii][jj] = 0.9;
        }
    for (auto jj = 23; jj < 41; jj++) {
        sim_out->body_soil_[1][19][jj] = 0.8;
        sim_out->body_soil_[1][24][jj] = 0.9;
    }
    for (auto ii = 19; ii < 38; ii++)
        sim_out->body_soil_[1][ii][39] = 0.7;

    for (auto _ : state)
        soil_simulator::MoveIntersectingBodySoil(sim_out, grid, bucket, 1.e-5);
    delete sim_out;
    delete bucket;
}
BENCHMARK(BM_MoveIntersectingBodySoil);

// -- LocateIntersectingCells --
static void BM_LocateIntersectingCells(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimParam sim_param(0.85, 3, 4);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    std::vector<float> ori = {0.707107, 0.707107, 0.0, 0.0};
    std::vector<float> pos = {0.0, 0.0, 0.0};
    soil_simulator::CalcBucketPos(
        sim_out, pos, ori, grid, bucket, sim_param, 1e-5);

    for (auto _ : state)
        soil_simulator::LocateIntersectingCells(sim_out, 1.e-5);
    delete sim_out;
    delete bucket;
}
BENCHMARK(BM_LocateIntersectingCells)->Unit(benchmark::kMicrosecond);

// -- MoveBodySoil --
static void BM_MoveBodySoil(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    sim_out->body_[0][5][7] = 0.1;
    sim_out->body_[1][5][7] = 0.3;
    sim_out->body_[2][5][7] = 0.6;
    sim_out->body_[3][5][7] = 0.8;
    sim_out->body_soil_[0][5][7] = 0.3;
    sim_out->body_soil_[1][5][7] = 1.1;
    sim_out->body_soil_[2][5][7] = 0.8;
    sim_out->body_soil_[3][5][7] = 0.9;
    sim_out->body_[0][5][11] = 0.0;
    sim_out->body_[1][5][11] = 0.2;
    sim_out->body_[2][5][11] = 0.4;
    sim_out->body_[3][5][11] = 0.8;
    sim_out->body_soil_[0][5][11] = 0.2;
    sim_out->body_soil_[1][5][11] = 1.3;

    for (auto _ : state)
        soil_simulator::MoveBodySoil(
            sim_out, 0, 5, 7, 0.4, 5, 11, 0.5, true, grid, bucket, 1.e-5);
    delete sim_out;
    delete bucket;
}
BENCHMARK(BM_MoveBodySoil);
