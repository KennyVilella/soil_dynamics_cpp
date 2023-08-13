/*
This file declares the main class of this simulator.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <random>
#include <vector>
#include "src/types.hpp"

namespace soil_simulator {

// Declaring RNG
extern std::mt19937 rng;

/// \brief Simulation class.
class SoilDynamics {
 public:
     /// \brief Initialize the simulator.
     void init();

     /// \brief Step the simulation.
     void step(
         SimOut* sim_out, std::vector<float> pos, std::vector<float> ori,
         Grid grid, Bucket* bucket, SimParam sim_param, float tol);

     /// \brief Check the validity of the simulation outputs.
     void check();

     /// \brief Write the simulation outputs into files.
     void WriteOutputs();
};
}  // namespace soil_simulator
