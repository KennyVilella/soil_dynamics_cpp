/*
This file declares the functions used to move the intersecting soil cells.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <tuple>
#include <vector>
#include "src/types.hpp"

namespace soil_simulator {

void MoveIntersectingCells(SimOut* sim_out, float tol);

void MoveIntersectingBodySoil(SimOut* sim_out, float tol);

void MoveIntersectingBody(SimOut* sim_out, float tol);

std::tuple<int, int, int, float, bool> MoveBodySoil(
    SimOut* sim_out, int ind_p, int ii_p, int jj_p, float max_h, int ii_n,
    int jj_n, float h_soil, bool wall_presence, float tol);

std::vector<std::vector<int>> LocateIntersectingCells(
    SimOut* sim_out, float tol);

}  // namespace soil_simulator
