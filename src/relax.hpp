/*
This file declares the functions used for the terrain relaxation.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <vector>
#include "src/types.hpp"

namespace soil_simulator {

// Declaring RNG
extern std::mt19937 rng;

/// \brief This function moves the soil in `terrain_` towards a state closer
///        to equilibrium.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param grid: Class that stores information related to the simulation grid.
/// \param sim_param: Class that stores information related to the simulation.
/// \param tol: Small number used to handle numerical approximation errors.
void RelaxTerrain(SimOut* sim_out, Grid grid, SimParam sim_param, float tol);

/// \brief This function moves the soil in `body_soil_` towards a state closer
///        to equilibrium.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param grid: Class that stores information related to the simulation grid.
/// \param sim_param: Class that stores information related to the simulation.
/// \param tol: Small number used to handle numerical approximation errors.
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

/// \brief This function checks the stability of a soil column in `terrain_`
///        compared to one of its neighbor (`ii_c`, `jj_c`)
///
/// \param sim_out: Class that stores simulation outputs.
/// \param ii_c: Index of the neighboring cell in the X direction.
/// \param jj_c: Index of the neighboring cell in the Y direction.
/// \param h_min: Minimum allowed height for a stable configuration. [m]
/// \param tol: Small number used to handle numerical approximation errors.
///
/// \return Two-digit number indicating how the soil should avalanche.
///         `0` is returned if the soil column is stable.
int CheckUnstableTerrainCell(
    SimOut* sim_out, int ii_c, int jj_c, float h_min, float tol);

/// \brief This function checks the stability of a soil column in the soil
///        layer `ind` of `body_soil_` at (`ii`, `jj`) compared to one of its
///        neighbor at (`ii_c`, `jj_c`).
///
/// \param sim_out: Class that stores simulation outputs.
/// \param ii: Index of the considered cell in the X direction.
/// \param jj: Index of the considered cell in the Y direction.
/// \param ind: Index of the considered soil layer.
/// \param ii_c: Index of the neighboring cell in the X direction.
/// \param jj_c: Index of the neighboring cell in the Y direction.
/// \param h_min: Minimum allowed height for a stable configuration. [m]
/// \param tol: Small number used to handle numerical approximation errors.
///
/// \return Two-digit number indicating how the soil should avalanche.
///         `0` is returned if the soil column is stable.
int CheckUnstableBodyCell(
    SimOut* sim_out, int ii, int jj, int ind, int ii_c, int jj_c, float h_min,
    float tol);

/// \brief This function moves the soil from the `terrain_` at (`ii`, `jj`) to
///        the soil column in (`ii_c`, `jj_c`).
///
/// \param sim_out: Class that stores simulation outputs.
/// \param status: Two-digit number indicating how the soil should avalanche.
/// \param dh_max: Maximum height difference allowed between two neighboring
///                cells. [m]
/// \param ii: Index of the considered cell in the X direction.
/// \param jj: Index of the considered cell in the Y direction.
/// \param ii_c: Index of the neighboring cell in the X direction.
/// \param jj_c: Index of the neighboring cell in the Y direction.
/// \param grid: Class that stores information related to the simulation grid.
/// \param tol: Small number used to handle numerical approximation errors.
void RelaxUnstableTerrainCell(
    SimOut* sim_out, int status, float dh_max, int ii, int jj, int ii_c,
    int jj_c, Grid grid, float tol);

/// \brief This function moves the soil from the soil layer `ind` of
///        `body_soil_` at (`ii`, `jj`) to the soil column in (`ii_c`, `jj_c`).
///
/// \param sim_out: Class that stores simulation outputs.
/// \param status: Two-digit number indicating how the soil should avalanche.
/// \param body_soil_pos: Queue to append new body_soil_pos.
/// \param dh_max: Maximum height difference allowed between two neighboring
///                cells. [m]
/// \param ii: Index of the considered cell in the X direction.
/// \param jj: Index of the considered cell in the Y direction.
/// \param ind: Index of the considered soil layer.
/// \param ii_c: Index of the neighboring cell in the X direction.
/// \param jj_c: Index of the neighboring cell in the Y direction.
/// \param grid: Class that stores information related to the simulation grid.
/// \param tol: Small number used to handle numerical approximation errors.
void RelaxUnstableBodyCell(
    SimOut* sim_out, int status, std::vector<std::vector<int>>* body_soil_pos,
    float dh_max, int ii, int jj, int ind, int ii_c, int jj_c, Grid grid,
    float tol);

}  // namespace soil_simulator
