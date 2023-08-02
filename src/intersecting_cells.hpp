/*
This file declares the functions used to move the intersecting soil cells.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <random>
#include <tuple>
#include <vector>
#include "src/types.hpp"

// Setting RNG
std::mt19937 rng;

namespace soil_simulator {

void MoveIntersectingCells(SimOut* sim_out, float tol);

void MoveIntersectingBodySoil(SimOut* sim_out, float tol);

/// \brief This function moves the soil cells in the `terrain_` that intersect
///        with a bucket.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param tol: Small number used to handle numerical approximation errors.
void MoveIntersectingBody(SimOut* sim_out, float tol);

std::tuple<int, int, int, float, bool> MoveBodySoil(
    SimOut* sim_out, int ind_p, int ii_p, int jj_p, float max_h, int ii_n,
    int jj_n, float h_soil, bool wall_presence, float tol);

/// \brief This function identifies all the soil cells in the `terrain_` that
///        intersect with the bucket.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param tol: Small number used to handle numerical approximation errors.
std::vector<std::vector<int>> LocateIntersectingCells(
    SimOut* sim_out, float tol);

}  // namespace soil_simulator
