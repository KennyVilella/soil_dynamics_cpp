/*
This file implements benchmarking for the example script.

Copyright, 2023, Vilella Kenny.
*/
#include <benchmark/benchmark.h>
#include <glog/logging.h>
#include "soil_simulator/soil_dynamics.hpp"
#include "test/example/soil_evolution.hpp"

// -- SoilEvolution --
static void BM_SoilEvolution(benchmark::State& state) {
    // Writing outputs to stderr instead of logfiles
    FLAGS_logtostderr = 1;

    // Initalizing the simulated bucket
    std::vector<float> o_pos_init = {0.0, 0.0, 0.0};
    std::vector<float> j_pos_init = {0.0, 0.0, 0.0};
    std::vector<float> b_pos_init = {0.0, 0.0, -0.5};
    std::vector<float> t_pos_init = {0.7, 0.0, -0.5};
    float bucket_width = 0.5;
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos_init, j_pos_init, b_pos_init, t_pos_init, bucket_width);

    for (auto _ : state)
        soil_simulator::SoilEvolution(bucket, true, false, false, false, false);

    delete bucket;
}
BENCHMARK(BM_SoilEvolution)->Unit(benchmark::kMillisecond);
