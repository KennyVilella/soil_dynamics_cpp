/*
This file benchmarks all the simulator functions.

Copyright, 2023, Vilella Kenny.
*/
#include <benchmark/benchmark.h>
#include <random>
#include "src/soil_dynamics.hpp"

// Defining RNG
std::mt19937 soil_simulator::rng;

BENCHMARK_MAIN();
