/*
This file implements benchmarking for the functions in utils.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <benchmark/benchmark.h>
#include "soil_simulator/utils.hpp"

// -- CalcNormal --
static void BM_CalcNormal(benchmark::State& state) {
    // Defining inputs
    std::vector<float> a = {1.0, 0.0, 0.0};
    std::vector<float> b = {0.0, 1.0, 0.0};
    std::vector<float> c = {0.0, 0.0, 1.0};

    for (auto _ : state)
        soil_simulator::CalcNormal(a, b, c);
}
BENCHMARK(BM_CalcNormal);

// -- CalcRotationQuaternion --
static void BM_CalcRotationQuaternion(benchmark::State& state) {
    // Defining inputs
    std::vector<float> ori = {0.7, 0.0, 0.7, 0.0};
    std::vector<float> pos = {0.0, 1.0, 0.0};

    for (auto _ : state)
        soil_simulator::CalcRotationQuaternion(ori, pos);
}
BENCHMARK(BM_CalcRotationQuaternion);

// -- AngleToQuat --
static void BM_AngleToQuat(benchmark::State& state) {
    // Defining inputs
    std::vector<float> ori = {0.7, 0.0, 0.7, 0.0};

    for (auto _ : state)
        soil_simulator::AngleToQuat(ori);
}
BENCHMARK(BM_AngleToQuat);

// -- MultiplyQuaternion --
static void BM_MultiplyQuaternion(benchmark::State& state) {
    // Defining inputs
    std::vector<float> q1 = {0.7, 0.5, -0.7, 0.1};
    std::vector<float> q2 = {0.12, 0.32, 0.4, -0.3};

    for (auto _ : state)
        soil_simulator::MultiplyQuaternion(q1, q2);
}
BENCHMARK(BM_MultiplyQuaternion);
