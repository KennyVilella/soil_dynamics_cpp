/*
This file declares the utility functions for unit testing.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <vector>
#include "soil_simulator/types.hpp"

namespace test_soil_simulator {

/// \brief This function resets the requested outputs and checks that all
///        terrain, body and body_soil is properly reset. This can be used to
///        catch potential unexpected modifications of the outputs.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param terrain_pos: Collection of terrain cells that should be reset.
/// \param body_pos: Collection of body cells that should be reset.
/// \param body_soil_pos: Collection of body soil cells that should be reset.
void ResetValueAndTest(
    soil_simulator::SimOut* sim_out, std::vector<std::vector<int>> terrain_pos,
    std::vector<std::vector<int>> body_pos,
    std::vector<std::vector<int>> body_soil_pos);

/// \brief This function checks the values of an inputted `body_soil` struct
///        against provided values.
///
/// \param body_soil_pos: `body_soil` struct to be checked.
/// \param ind: Expected index of the soil layer.
/// \param ii: Expected index of the body soil in the X direction.
/// \param jj: Expected index of the body soil in the Y direction.
/// \param pos: Expected Cartesian coordinates of the body soil in the
///             reference bucket frame. [m]
/// \param h_soil: Expected height of the soil column. [m]
void CheckBodySoilPos(
    soil_simulator::body_soil body_soil_pos, int ind, int, ii, int jj,
    std::vector<float> pos, float h_soil);

}  // namespace test_soil_simulator
