/*
This file implements the functions used to move the intersecting soil cells.

Copyright, 2023, Vilella Kenny.
*/
#include <iostream>
#include <tuple>
#include <vector>
#include "src/types.hpp"
#include "src/intersecting_cells.hpp"

void soil_simulator::MoveIntersectingCells(SimOut* sim_out, float tol
) {
    // Moving bucket soil intersecting with the bucket
    soil_simulator::MoveIntersectingBodySoil(sim_out, tol);

    // Moving terrain intersecting with the bucket
    soil_simulator::MoveIntersectingBody(sim_out, tol);
}

void soil_simulator::MoveIntersectingBodySoil(SimOut* sim_out, float tol
) {
}

void soil_simulator::MoveIntersectingBody(SimOut* sim_out, float tol
) {
}

std::tuple<int, int, int, float, bool> soil_simulator::MoveBodySoil(
    SimOut* sim_out, int ind_p, int ii_p, int jj_p, float max_h, int ii_n,
    int jj_n, float h_soil, bool wall_presence, float tol
) {
    return {1, 1, 1, 0.0, true};
}

std::vector<std::vector<int>> soil_simulator::LocateIntersectingCells(
    SimOut* sim_out, float tol
) {
    // Initializing
    std::vector<std::vector<int>> intersecting_cells;

    // Iterating over all bucket position
    for (auto ii = sim_out->bucket_area_[0][0]; ii < sim_out->bucket_area_[0][1]; ii++)
        for (auto jj = sim_out->bucket_area_[1][0]; jj < sim_out->bucket_area_[1][1]; jj++) {
            if (
               ((sim_out->body_[0][ii][jj] != 0.0) ||
               (sim_out->body_[1][ii][jj] != 0.0)) &&
               (sim_out->terrain_[ii][jj] - tol > sim_out->body_[0][ii][jj])) {
                // Soil intersecting with the bucket
                intersecting_cells.push_back(std::vector<int> {0, ii, jj});
            }
            if (
               ((sim_out->body_[2][ii][jj] != 0.0) ||
               (sim_out->body_[3][ii][jj] != 0.0)) &&
                (sim_out->terrain_[ii][jj] - tol > sim_out->body_[2][ii][jj])) {
                // Soil intersecting with the bucket
                intersecting_cells.push_back(std::vector<int> {2, ii, jj});
            }
        }

    return intersecting_cells;
}
