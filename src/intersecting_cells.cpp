/*
This file implements the functions used to move the intersecting soil cells.

Copyright, 2023, Vilella Kenny.
*/
#include <algorithm>
#include <iostream>
#include <random>
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

/// This function checks the eight lateral directions surrounding the
/// intersecting soil column and moves the soil to available spaces. If there is
/// insufficient space for all the soil, it incrementally checks the eight
/// directions farther from the intersecting soil column until all the soil has
/// been moved. The process can be illustrated as follows
///
///                 ↖   ↑   ↗
///                   ↖ ↑ ↗
///                 ← ← O → →
///                   ↙ ↓ ↘
///                 ↙   ↓   ↘
///
/// Note that the order in which the directions are checked is randomized in
/// order to avoid asymmetrical results.
void soil_simulator::MoveIntersectingBody(SimOut* sim_out, float tol
) {
    // Locating soil cells intersecting with the bucket
    auto intersecting_cells = soil_simulator::LocateIntersectingCells(
        sim_out, tol);

    if (intersecting_cells.size() == 0) {
        // No intersecting cells
        return;
    }

    // Storing all possible directions
    std::vector<std::vector<int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    // Iterating over intersecting cells
    for (auto nn = 0; nn < intersecting_cells.size(); nn++) {
        int ind = intersecting_cells[nn][0];
        int ii = intersecting_cells[nn][1];
        int jj = intersecting_cells[nn][2];

        if (sim_out->terrain_[ii][jj] - tol < sim_out->body_[ind][ii][jj]) {
            // Intersecting soil column has already been moved
            continue;
        }

        // Randomizing direction to avoid asymmetry
        std::random_shuffle(directions.begin(), directions.end());

        // Calculating vertical extension of intersecting soil column
        float h_soil = sim_out->terrain_[ii][jj] - sim_out->body_[ind][ii][jj];

        int pp = 0;
        // Investigating farther and farther until all the soil has been moved
        while (h_soil > tol) {
            pp += 1;
            // Iterating over the eight lateral directions
            for (auto xy = 0; xy < directions.size(); xy++) {
                // Calculating considered position
                int ii_p = ii + directions[xy][0] * pp;
                int jj_p = jj + directions[xy][1] * pp;

                // Determining presence of bucket
                bool bucket_absence_1 = (
                    (sim_out->body_[0][ii_p][jj_p] == 0.0) &&
                    (sim_out->body_[1][ii_p][jj_p] == 0.0));
                bool bucket_absence_3 = (
                    (sim_out->body_[2][ii_p][jj_p] == 0.0) &&
                    (sim_out->body_[3][ii_p][jj_p] == 0.0));

                if (bucket_absence_1 && bucket_absence_3) {
                    // No bucket
                    sim_out->terrain_[ii_p][jj_p] += h_soil;
                    h_soil = 0.0;
                    break;
                } else {
                    // Bucket is present
                    // Calculating minimum height of bucket
                    float bucket_bot;
                    if (bucket_absence_1)
                        bucket_bot = sim_out->body_[2][ii_p][jj_p];
                    else if (bucket_absence_3)
                        bucket_bot = sim_out->body_[0][ii_p][jj_p];
                    else
                        bucket_bot = std::min(
                            sim_out->body_[0][ii_p][jj_p],
                            sim_out->body_[2][ii_p][jj_p]);

                    if (sim_out->terrain_[ii_p][jj_p] + tol < bucket_bot) {
                        // Space under the bucket
                        // Calculating available space
                        float delta_h = (
                            bucket_bot - sim_out->terrain_[ii_p][jj_p]);

                        if (delta_h < h_soil) {
                            // Not enough space
                            sim_out->terrain_[ii_p][jj_p] = bucket_bot;
                            h_soil -= delta_h;
                        } else {
                            // More space than soil
                            sim_out->terrain_[ii_p][jj_p] += h_soil;
                            h_soil = 0.0;
                            break;
                        }
                    }
                }
            }
        }

        // Removing intersecting soil
        sim_out->terrain_[ii][jj] = sim_out->body_[ind][ii][jj];
    }
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
    for (auto ii = sim_out->bucket_area_[0][0];
        ii < sim_out->bucket_area_[0][1]; ii++
    )
        for (auto jj = sim_out->bucket_area_[1][0];
            jj < sim_out->bucket_area_[1][1]; jj++
        ) {
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
