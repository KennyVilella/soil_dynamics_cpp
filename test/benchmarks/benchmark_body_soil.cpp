/*
This file implements benchmarking for the functions in body_soil.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <benchmark/benchmark.h>
#include "src/body_soil.hpp"
#include "src/bucket_pos.hpp"

// -- UpdateBodySoil --
static void BM_UpdateBodySoil(benchmark::State& state) {
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

    // Setting previous bucket position
    std::vector<float> ori_1 = {0.707107, 0.707107, 0.0, 0.0};
    std::vector<float> pos_1 = {0.5, 0.0, 0.0};
    soil_simulator::CalcBucketPos(
        sim_out, pos_1, ori_1, grid, bucket, sim_param, 1e-5);
    for (auto ii = 90; ii < 105; ii++) {
        sim_out->body_soil_[0][ii][70] = sim_out->body_[1][ii][70];
        sim_out->body_soil_[1][ii][70] = sim_out->body_[1][ii][70] + 0.2;
    }
    for (auto ii = 90; ii < 104; ii++) {
        sim_out->body_soil_[0][ii][71] = sim_out->body_[1][ii][71];
        sim_out->body_soil_[1][ii][71] = sim_out->body_[1][ii][71] + 0.2;
    }
    for (auto ii = 90; ii < 103; ii++) {
        sim_out->body_soil_[0][ii][72] = sim_out->body_[1][ii][72];
        sim_out->body_soil_[1][ii][72] = sim_out->body_[1][ii][72] + 0.2;
    }
    for (auto ii = 90; ii < 101; ii++) {
        sim_out->body_soil_[0][ii][73] = sim_out->body_[1][ii][73];
        sim_out->body_soil_[1][ii][73] = sim_out->body_[1][ii][73] + 0.2;
    }
    for (auto ii = 90; ii < 100; ii++) {
        sim_out->body_soil_[0][ii][74] = sim_out->body_[1][ii][74];
        sim_out->body_soil_[1][ii][74] = sim_out->body_[1][ii][74] + 0.2;
    }
    for (auto ii = 90; ii < 99; ii++) {
        sim_out->body_soil_[0][ii][75] = sim_out->body_[1][ii][75];
        sim_out->body_soil_[1][ii][75] = sim_out->body_[1][ii][75] + 0.2;
    }
    for (auto ii = 90; ii < 97; ii++) {
        sim_out->body_soil_[0][ii][76] = sim_out->body_[1][ii][76];
        sim_out->body_soil_[1][ii][76] = sim_out->body_[1][ii][76] + 0.2;
    }
    for (auto ii = 90; ii < 96; ii++) {
        sim_out->body_soil_[0][ii][77] = sim_out->body_[1][ii][77];
        sim_out->body_soil_[1][ii][77] = sim_out->body_[1][ii][77] + 0.2;
    }
    for (auto ii = 90; ii < 94; ii++) {
        sim_out->body_soil_[0][ii][78] = sim_out->body_[1][ii][78];
        sim_out->body_soil_[1][ii][78] = sim_out->body_[1][ii][78] + 0.2;
    }
    for (auto ii = 90; ii < 93; ii++) {
        sim_out->body_soil_[0][ii][79] = sim_out->body_[1][ii][79];
        sim_out->body_soil_[1][ii][79] = sim_out->body_[1][ii][79] + 0.2;
    }
    for (auto ii = 90; ii < 92; ii++) {
        sim_out->body_soil_[0][ii][80] = sim_out->body_[1][ii][80];
        sim_out->body_soil_[1][ii][80] = sim_out->body_[1][ii][80] + 0.2;
    }

    // Setting new bucket position
    std::vector<float> pos_2 = {0.55, 0.0, 0.0};
    soil_simulator::CalcBucketPos(
        sim_out, pos_1, ori_1, grid, bucket, sim_param, 1e-5);

    for (auto _ : state)
        soil_simulator::UpdateBodySoil(
            sim_out, pos_2, ori_1, grid, bucket, 1.e-5);
    delete sim_out;
    delete bucket;
}
BENCHMARK(BM_UpdateBodySoil)->Unit(benchmark::kMillisecond);
