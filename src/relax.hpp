/*
This file declares the functions used for the terrain relaxation.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <vector>
#include "src/types.hpp"

namespace soil_simulator {

void RelaxTerrain(SimOut* sim_out, Grid grid, SimParam sim_param, float tol);

void RelaxBodySoil(SimOut* sim_out, Grid grid, SimParam sim_param, float tol);

/// \brief This function locates all the cells in `terrain_` that have a height
///        difference larger than `dh_max` with at least one neighboring cell.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param dh_max: Maximum height difference allowed between two neighboring
///                cells. [m]
/// \param tol: Small number used to handle numerical approximation errors.
///
/// \return Collection of cells indices that are possibly unstable.
std::vector<std::vector<int>> LocateUnstableTerrainCell(
    SimOut* sim_out, float dh_max, float tol);

int CheckUnstableTerrainCell(
    SimOut* sim_out, int ii_c, int jj_c, float h_min, float tol);

int CheckUnstableBodyCell(
    SimOut* sim_out, int ii, int jj, int ind, int ii_c, int jj_c, float h_min,
    float tol);

void RelaxUnstableTerrainCell(
    SimOut* sim_out, int status, float dh_max, int ii, int jj, int ii_c,
    int jj_c, Grid grid, float tol);

void RelaxUnstableBodyCell(
    SimOut* sim_out, int status, std::vector<std::vector<int>> body_soil_pos,
    float dh_max, int ii, int jj, int ind, int ii_c, int jj_c, Grid grid,
    float tol);

}  // namespace soil_simulator
