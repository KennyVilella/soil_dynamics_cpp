/*
This file declares the functions used to move the intersecting soil cells.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <tuple>
#include <vector>
#include "soil_simulator/types.hpp"

namespace soil_simulator {

// Declaring RNG
extern std::mt19937 rng;

/// \brief This function moves all soil cells in `terrain_` and in `body_soil_`
///        that intersect with the body or with another soil cell.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param grid: Class that stores information related to the simulation grid.
/// \param body: Class that stores information related to the body object.
/// \param tol: Small number used to handle numerical approximation errors.
void MoveIntersectingCells(
    SimOut* sim_out, Grid grid, Body* body, float tol);

/// \brief This function moves the soil cells resting on the body that
///        intersect with another body layer.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param grid: Class that stores information related to the simulation grid.
/// \param body: Class that stores information related to the body object.
/// \param tol: Small number used to handle numerical approximation errors.
void MoveIntersectingBodySoil(
    SimOut* sim_out, Grid grid, Body* body, float tol);

/// \brief This function moves the soil cells in the `terrain_` that intersect
///        with a body.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param tol: Small number used to handle numerical approximation errors.
void MoveIntersectingBody(SimOut* sim_out, float tol);

/// \brief This function tries to move the soil cells resting on the body
///        layer `ind_p` at the location (`ii_p`, `jj_p`) to a new location
///        at (`ii_n`, `jj_n`).
///
/// \param sim_out: Class that stores simulation outputs.
/// \param ind_p: Index of the previous considered body layer.
/// \param ii_p: Index of the previous considered position in the X direction.
/// \param jj_p: Index of the previous considered position in the Y direction.
/// \param max_h: Maximum height authorized for the movement. [m]
/// \param ii_n: Index of the new considered position in the X direction.
/// \param jj_n: Index of the new considered position in the Y direction.
/// \param h_soil: Height of the soil column left to be moved. [m]
/// \param wall_presence: Indicates whether a wall is blocking the movement.
/// \param grid: Class that stores information related to the simulation grid.
/// \param body: Class that stores information related to the body object.
/// \param tol: Small number used to handle numerical approximation errors.
///
/// \return A tuple composed of the index of the new considered body layer,
///         the index of the new considered position in the X direction, the
///         index of the new considered position in the Y direction, the height
///         of the soil column left to be moved, and a boolean indicating
///         whether a body wall is blocking the movement.
std::tuple<int, int, int, float, bool> MoveBodySoil(
    SimOut* sim_out, int ind_p, int ii_p, int jj_p, float max_h, int ii_n,
    int jj_n, float h_soil, bool wall_presence, Grid grid, Body* body,
    float tol);

/// \brief This function identifies all the soil cells in the `terrain_` that
///        intersect with the body.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param tol: Small number used to handle numerical approximation errors.
std::vector<std::vector<int>> LocateIntersectingCells(
    SimOut* sim_out, float tol);

}  // namespace soil_simulator
