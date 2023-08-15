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
     ///
     /// \param sim_out: Class that stores simulation outputs.
     /// \param pos: Cartesian coordinates of the bucket origin. [m]
     /// \param ori: Orientation of the bucket. [Quaternion]
     /// \param grid: Class that stores information related to the
     ///              simulation grid.
     /// \param bucket: Class that stores information related to the
     ///                bucket object.
     /// \param sim_param: Class that stores information related to
     ///                   the simulation.
     /// \param tol: Small number used to handle numerical approximation errors.
     void step(
         SimOut* sim_out, std::vector<float> pos, std::vector<float> ori,
         Grid grid, Bucket* bucket, SimParam sim_param, float tol);

     /// \brief Check the validity of the simulation outputs.
     ///
     /// \param sim_out: Class that stores simulation outputs.
     /// \param init_volume: Initial volume of soil in the terrain. [m^3]
     /// \param grid: Class that stores information related to the
     ///              simulation grid.
     /// \param tol: Small number used to handle numerical approximation errors.
     void check(SimOut* sim_out, float init_volume, Grid grid, float tol);

     /// \brief Write the simulation outputs into files.
     void WriteOutputs();
};
}  // namespace soil_simulator
