/*
This file declares the main class of this simulator.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

namespace soil_simulator {

void SoilEvolution(
    bool set_rng, bool random_trajectory, bool check_outputs,
    bool write_outputs);
}  // namespace soil_simulator
