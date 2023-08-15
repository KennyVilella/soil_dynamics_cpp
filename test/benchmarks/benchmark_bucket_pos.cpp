/*
This file implements benchmarking for the functions in bucket_pos.cpp.

Copyright, 2023, Vilella Kenny.
*/
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
}
BENCHMARK(BM_CalcBucketPos)->Unit(benchmark::kMillisecond);

