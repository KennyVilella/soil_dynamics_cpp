/*
This file declares the main class of this simulator.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <random>
#include <vector>
#include "soil_simulator/types.hpp"

namespace soil_simulator {

// Declaring RNG
extern std::mt19937 rng;

/// \brief Simulation class.
class SoilDynamics {
 public:
     /// \brief Initialize the simulator.
     ///
     /// \param sim_out: Class that stores simulation outputs.
     /// \param grid: Class that stores information related to the
     ///              simulation grid.
     /// \param amp_noise: Amplitude of the Simplex noise. [m]
     void Init(SimOut* sim_out, Grid grid, float amp_noise);

     /// \brief Step the simulation.
     ///
     /// \param sim_out: Class that stores simulation outputs.
     /// \param pos: Cartesian coordinates of the body origin. [m]
     /// \param ori: Orientation of the body. [Quaternion]
     /// \param grid: Class that stores information related to the
     ///              simulation grid.
     /// \param body: Class that stores information related to the body object.
     /// \param sim_param: Class that stores information related to
     ///                   the simulation.
     /// \param tol: Small number used to handle numerical approximation errors.
     ///
     /// \return A boolean indicating whether soil update has been done.
     template <typename T>
     bool Step(
         SimOut* sim_out, std::vector<float> pos, std::vector<float> ori,
         Grid grid, T* body, SimParam sim_param, float tol);

     /// \brief Check the validity of the simulation outputs.
     ///
     /// \param sim_out: Class that stores simulation outputs.
     /// \param init_volume: Initial number of soil cells.
     /// \param grid: Class that stores information related to the
     ///              simulation grid.
     /// \param tol: Small number used to handle numerical approximation errors.
     void Check(SimOut* sim_out, int init_volume, Grid grid, float tol);

     /// \brief Write the simulation outputs into files.
     ///
     /// \param sim_out: Class that stores simulation outputs.
     /// \param grid: Class that stores information related to the
     ///              simulation grid.
     /// \param body: Class that stores information related to the body object.
     template <typename T>
     void WriteOutputs(SimOut* sim_out, Grid grid, T* body);
};

}  // namespace soil_simulator
