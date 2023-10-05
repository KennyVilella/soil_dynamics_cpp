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

}  // namespace test_soil_simulator
