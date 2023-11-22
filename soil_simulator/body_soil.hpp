/*
This file declares the functions used to move the soil resting on the body
following its movement.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <vector>
#include "soil_simulator/types.hpp"

namespace soil_simulator {

/// \brief This function moves the soil resting on the body following
///        its movement.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param pos: Cartesian coordinates of the body origin. [m]
/// \param ori: Orientation of the body. [Quaternion]
/// \param grid: Class that stores information related to the simulation grid.
/// \param body: Class that stores information related to the body object.
/// \param tol: Small number used to handle numerical approximation errors.
void UpdateBodySoil(
    SimOut* sim_out, std::vector<float> pos, std::vector<float> ori, Grid grid,
    Body* body, float tol);

}  // namespace soil_simulator
