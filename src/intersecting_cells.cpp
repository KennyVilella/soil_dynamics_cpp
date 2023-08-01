/*
This file implements the functions used to move the intersecting soil cells.

Copyright, 2023, Vilella Kenny.
*/
#include <iostream>
#include <tuple>
#include <vector>
#include "src/types.hpp"
#include "src/intersecting_cells.hpp"

void soil_simulator::MoveIntersectingCells(SimOut* sim_out, float tol) {
    // Moving bucket soil intersecting with the bucket
    soil_simulator::MoveIntersectingBodySoil!(sim_out, tol);

    // Moving terrain intersecting with the bucket
    soil_simulator::MoveIntersectingBody!(sim_out, tol);
}

void soil_simulator::MoveIntersectingBodySoil(SimOut* sim_out, float tol) {

}

void soil_simulator::MoveIntersectingBody(SimOut* sim_out, float tol) {

}

std::tuple<int, int, int, float, bool> soil_simulator::MoveBodySoil(
    SimOut* sim_out, int ind_p, int ii_p, int jj_p, float max_h, int ii_n,
    int jj_n, float h_soil, bool wall_presence, float tol) {

}

std::vector<std::vector<int>> soil_simulator::LocateIntersectingCells(
    SimOut* sim_out, float tol) {

}


