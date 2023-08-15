/*
This file implements benchmarking for the functions in bucket_pos.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <iostream>
#include <benchmark/benchmark.h>
#include "src/bucket_pos.cpp"
#include "src/utils.cpp"
#include "src/types.cpp"

// -- CalcBucketPos --
static void BM_CalcBucketPos(benchmark::State& state) {
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
    std::vector<float> ori = {0.707107, 0.0, -0.707107, 0.0};
    std::vector<float> pos = {0.0, 0.0, -0.1};

    for (auto _ : state)
        soil_simulator::CalcBucketPos(
            sim_out, pos, ori, grid, bucket, sim_param, 1.e-5);
    delete sim_out;
    delete bucket;
}
BENCHMARK(BM_CalcBucketPos)->Unit(benchmark::kMillisecond);

// -- CalcRectanglePos --
static void BM_CalcRectanglePos(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    std::vector<float> a = {0.0, 0.0, 0.0};
    std::vector<float> b = {1.0, 0.0, 0.0};
    std::vector<float> c = {1.0, 0.5, 0.0};
    std::vector<float> d = {0.0, 0.5, 0.0};

    for (auto _ : state)
        soil_simulator::CalcRectanglePos(a, b, c, d, 0.01, grid, 1.e-5);
}
BENCHMARK(BM_CalcRectanglePos)->Unit(benchmark::kMillisecond);

// -- DecomposeVectorRectangle --
static void BM_DecomposeVectorRectangle(benchmark::State& state) {
    // Defining inputs
    std::vector<float> a_ind = {80.0, 80.0, 80.0};
    std::vector<float> ab_ind = {20.0, 3.0, 0.0};
    std::vector<float> ad_ind = {5.0, 19.0, 0.0};

    for (auto _ : state)
        soil_simulator::DecomposeVectorRectangle(
            ab_ind, ad_ind, a_ind, 75, 75, 25, 25, 1.e-5);
}
BENCHMARK(BM_DecomposeVectorRectangle)->Unit(benchmark::kMicrosecond);

// -- CalcTrianglePos --
static void BM_CalcTrianglePos(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    std::vector<float> a = {0.0, 0.0, 0.0};
    std::vector<float> b = {1.0, 0.0, 0.0};
    std::vector<float> c = {1.0, 0.5, 0.0};

    for (auto _ : state)
        soil_simulator::CalcTrianglePos(a, b, c, 0.01, grid, 1.e-5);
}
BENCHMARK(BM_CalcTrianglePos)->Unit(benchmark::kMillisecond);

// -- DecomposeVectorTriangle --
static void BM_DecomposeVectorTriangle(benchmark::State& state) {
    // Defining inputs
    std::vector<float> a_ind = {80.0, 80.0, 80.0};
    std::vector<float> ab_ind = {20.0, 3.0, 0.0};
    std::vector<float> ac_ind = {5.0, 19.0, 0.0};

    for (auto _ : state)
        soil_simulator::DecomposeVectorTriangle(
            ab_ind, ac_ind, a_ind, 75, 75, 25, 25, 1.e-5);
}
BENCHMARK(BM_DecomposeVectorTriangle)->Unit(benchmark::kMicrosecond);

// -- CalcLinePos --
static void BM_CalcLinePos(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    std::vector<float> a = {0.34, 0.56, 0.0};
    std::vector<float> b = {0.74, 0.97, 0.0};

    for (auto _ : state)
        soil_simulator::CalcLinePos(a, b, 0.01, grid);
}
BENCHMARK(BM_CalcLinePos)->Unit(benchmark::kMicrosecond);

// -- UpdateBody --
static void BM_UpdateBody(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);
    std::vector<float> a = {0.0, 0.0, 0.0};
    std::vector<float> b = {1.0, 0.0, 0.0};
    std::vector<float> c = {1.0, 0.5, 0.0};
    auto tri_pos = soil_simulator::CalcTrianglePos(a, b, c, 0.01, grid, 1.e-5);

    for (auto _ : state)
        soil_simulator::UpdateBody(tri_pos, sim_out, grid, 1.e-5);
    delete sim_out;
}
BENCHMARK(BM_UpdateBody)->Unit(benchmark::kMillisecond);

// -- IncludeNewBodyPos --
static void BM_IncludeNewBodyPos(benchmark::State& state) {
    // Defining inputs
    soil_simulator::Grid grid(4.0, 4.0, 3.0, 0.05, 0.01);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    for (auto _ : state)
        soil_simulator::IncludeNewBodyPos(sim_out, 10, 15, 0.5, 0.6, 1.e-5);
    delete sim_out;
}
BENCHMARK(BM_IncludeNewBodyPos)->Unit(benchmark::kMillisecond);
