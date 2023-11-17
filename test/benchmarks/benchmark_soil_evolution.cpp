/*
This file implements benchmarking for the example script.

Copyright, 2023, Vilella Kenny.
*/
#include <benchmark/benchmark.h>
#include <glog/logging.h>
#include "test/example/soil_evolution.hpp"

// -- SoilEvolution --
static void BM_SoilEvolution(benchmark::State& state) {
    // Writing outputs to stderr instead of logfiles
    FLAGS_logtostderr = 1;

    for (auto _ : state)
        soil_simulator::SoilEvolution(true, false, false, false, false);
}
BENCHMARK(BM_SoilEvolution)->Unit(benchmark::kMillisecond);
