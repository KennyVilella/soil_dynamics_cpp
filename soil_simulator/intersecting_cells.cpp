/*
This file implements the functions used to move the intersecting soil cells.

Copyright, 2023, Vilella Kenny.
*/
#include <glog/logging.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <tuple>
#include <utility>
#include <vector>
#include "soil_simulator/types.hpp"
#include "soil_simulator/intersecting_cells.hpp"
#include "soil_simulator/utils.hpp"

/// Note that `MoveIntersectingBodySoil` must be called before
/// `MoveIntersectingBody`, otherwise some intersecting soil cells may remain.
void soil_simulator::MoveIntersectingCells(
    SimOut* sim_out, Grid grid, Body* body, float tol
) {
    // Moving body soil intersecting with the body
    soil_simulator::MoveIntersectingBodySoil(sim_out, grid, body, tol);

    // Moving terrain intersecting with the body
    soil_simulator::MoveIntersectingBody(sim_out, tol);
}

/// This function checks the eight lateral directions surrounding the
/// intersecting soil column and moves the soil to available spaces.
///
/// The algorithm follows an incremental approach, checking directions farther
/// from the intersecting soil column until it reaches a body wall blocking
/// the movement or until all the soil has been moved. If the movement is
/// blocked by a body wall, the algorithm explores another direction.
///
/// In cases where the soil should be moved to the terrain, all soil is moved
/// regardless of the available space. If this movement induces intersecting
/// soil cells, it will be resolved by the `MoveIntersectingBody` function.
///
/// In rare situations where there is insufficient space to accommodate all the
/// intersecting soil, the algorithm currently handles it by allowing the excess
/// soil to simply disappear. This compromise seems to be reasonable as long as
/// the amount of soil disappearing remains negligible.
///
/// Note that the order in which the directions are checked is randomized in
/// order to avoid asymmetrical results.
void soil_simulator::MoveIntersectingBodySoil(
    SimOut* sim_out, Grid grid, Body* body, float tol
) {
    // Storing all possible directions
    std::vector<std::vector<int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    // Iterating over body soil cells
    for (auto nn = 0; nn < sim_out->body_soil_pos_.size(); nn++) {
        int ind = sim_out->body_soil_pos_[nn].ind;
        int ii = sim_out->body_soil_pos_[nn].ii;
        int jj = sim_out->body_soil_pos_[nn].jj;

        int ind_t;
        if (ind == 0) {
            // Soil is on the first body soil layer
            ind_t = 2;
        } else {
            // Soil is on the second body soil layer
            ind_t = 0;
        }

        if (
            (sim_out->body_[ind_t][ii][jj] == 0.0) &&
            (sim_out->body_[ind_t+1][ii][jj] == 0.0)) {
            // No additionnal body layer, soil cannot be intersecting
            continue;
        }

        float h_soil = 0.0;
        if (
            (sim_out->body_[ind_t][ii][jj] > sim_out->body_[ind][ii][jj]) &&
            (sim_out->body_soil_[ind+1][ii][jj] - tol >
                sim_out->body_[ind_t][ii][jj])) {
            // Body soil intersects with body
            h_soil = (
                sim_out->body_soil_[ind+1][ii][jj] -
                sim_out->body_[ind_t][ii][jj]);

            // Only the intersecting soil within this body_soil_pos is moved
            if (h_soil > sim_out->body_soil_pos_[nn].h_soil) {
                // All the soil would be moved
                h_soil = sim_out->body_soil_pos_[nn].h_soil;
                sim_out->body_soil_pos_[nn].h_soil = 0.0;
            } else {
                // Soil would be partially moved
                sim_out->body_soil_pos_[nn].h_soil -= h_soil;
            }
        } else {
            // No intersection between body soil and body
            continue;
        }

        // Updating body soil
        sim_out->body_soil_[ind+1][ii][jj] -= h_soil;

        // Randomizing direction to avoid asymmetry
        // random_suffle is not used because it is machine dependent,
        // which makes unit testing difficult
        for (int aa = directions.size() - 1; aa > 0; aa--) {
            std::uniform_int_distribution<int> dist(0, aa);
            int bb = dist(rng);
            std::swap(directions[aa], directions[bb]);
        }

        // Iterating over the eight lateral directions
        for (auto xy = 0; xy < directions.size(); xy++) {
            // Initializing loop properties
            int pp = 0;
            bool wall_presence = false;
            int ii_p = ii;
            int jj_p = jj;
            int ind_p = ind;
            float max_h = sim_out->body_[ind_t][ii][jj];

            // Exploring the direction until reaching a wall or
            // all soil has been moved
            while ((wall_presence == false) && (h_soil > tol)) {
                // Calculating considered position
                pp += 1;
                int ii_n = ii + pp * directions[xy][0];
                int jj_n = jj + pp * directions[xy][1];

                std::tie(ind_p, ii_p, jj_p, h_soil, wall_presence) = (
                    soil_simulator::MoveBodySoil(
                        sim_out, ind_p, ii_p, jj_p, max_h, ii_n, jj_n, h_soil,
                        wall_presence, grid, body, tol));

                // Updating the value used for the detection of body wall
                // This is working because this value will be used only in cases
                // where two body layers are present. Note however that the
                // value is incorrect when it will not be used.
                max_h = std::max(
                    sim_out->body_[0][ii_p][jj_p],
                    sim_out->body_[2][ii_p][jj_p]);
            }
            if (h_soil < tol) {
                // No more soil to move
                break;
            }
        }

        if (h_soil > tol) {
            // For cases where the soil cannot be moved
            // For instance, this happens when the body is going straight
            // underground with soil trapped inside.
            // This should not happen when soil reaction force is considered.
            LOG(WARNING) << "WARNING\nNot all soil intersecting with a body "
                "layer could be moved.\nThe extra soil has been arbitrarily "
                "removed.";
        }
    }
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
    // Locating soil cells intersecting with the body
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
        // random_suffle is not used because it is machine dependent,
        // which makes unit testing difficult
        for (int aa = directions.size() - 1; aa > 0; aa--) {
            std::uniform_int_distribution<int> dist(0, aa);
            int bb = dist(rng);
            std::swap(directions[aa], directions[bb]);
        }

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

                // Determining presence of body
                bool body_absence_1 = (
                    (sim_out->body_[0][ii_p][jj_p] == 0.0) &&
                    (sim_out->body_[1][ii_p][jj_p] == 0.0));
                bool body_absence_3 = (
                    (sim_out->body_[2][ii_p][jj_p] == 0.0) &&
                    (sim_out->body_[3][ii_p][jj_p] == 0.0));

                if (body_absence_1 && body_absence_3) {
                    // No body
                    sim_out->terrain_[ii_p][jj_p] += h_soil;
                    h_soil = 0.0;
                    break;
                } else {
                    // Body is present
                    // Calculating minimum height of body
                    float body_bot;
                    if (body_absence_1)
                        body_bot = sim_out->body_[2][ii_p][jj_p];
                    else if (body_absence_3)
                        body_bot = sim_out->body_[0][ii_p][jj_p];
                    else
                        body_bot = std::min(
                            sim_out->body_[0][ii_p][jj_p],
                            sim_out->body_[2][ii_p][jj_p]);

                    if (sim_out->terrain_[ii_p][jj_p] + tol < body_bot) {
                        // Space under the body
                        // Calculating available space
                        float delta_h = (
                            body_bot - sim_out->terrain_[ii_p][jj_p]);

                        if (delta_h < h_soil) {
                            // Not enough space
                            sim_out->terrain_[ii_p][jj_p] = body_bot;
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

/// This function can be separated into three main scenarios:
/// - If all the soil can be moved to the new location (either on the terrain
///   or on the body), the soil is moved and the value of `h_soil` is
///   set to zero.
/// - If a body wall is blocking the movement, the `wall_presence` parameter
///   is set to `true`.
/// - If there is insufficient space to move all the soil but no body wall is
///   blocking the movement, the function updates the values for the new
///   location and adjusts `h_soil` accordingly.
///
/// This function is designed to be used iteratively by the function
/// `MoveIntersectingBodySoil` until all intersecting soil cells are moved.
///
/// Note that, by convention, the soil can be moved from the body to the
/// terrain even if the body is underground.
/// Moreover, in cases where the soil should be moved to the terrain, all soil
/// is moved regardless of the available space. If this movement induces
/// intersecting soil cells, it will be resolved by `MoveIntersectingBody`.
std::tuple<int, int, int, float, bool> soil_simulator::MoveBodySoil(
    SimOut* sim_out, int ind_p, int ii_p, int jj_p, float max_h, int ii_n,
    int jj_n, float h_soil, bool wall_presence, Grid grid, Body* body,
    float tol
) {
    // Determining presence of body
    bool body_absence_1 = (
        (sim_out->body_[0][ii_n][jj_n] == 0.0) &&
        (sim_out->body_[1][ii_n][jj_n] == 0.0));
    bool body_absence_3 = (
        (sim_out->body_[2][ii_n][jj_n] == 0.0) &&
        (sim_out->body_[3][ii_n][jj_n] == 0.0));

    if (body_absence_1 && body_absence_3) {
        // No body
        sim_out->terrain_[ii_n][jj_n] += h_soil;
        return {ind_p, ii_p, jj_p, 0.0, wall_presence};
    } else if (body_absence_1) {
        // Only the second body layer
        if (
            sim_out->body_[2][ii_n][jj_n] - tol >
            sim_out->body_[ind_p+1][ii_p][jj_p]) {
            // Soil avalanche below the second body layer to the terrain
            // Note that all soil is going to the terrain without considering
            // the space available. If there is not enough space available, the
            // soil would intersect with the body and later be moved by
            // the MoveIntersectingBody function
            sim_out->terrain_[ii_n][jj_n] += h_soil;
            return {ind_p, ii_p, jj_p, 0.0, wall_presence};
        } else if (sim_out->body_[3][ii_n][jj_n] + tol > max_h) {
            // Body is blocking the movement
            return {ind_p, ii_p, jj_p, h_soil, true};
        }

        bool body_soil_presence_3 = (
            (sim_out->body_soil_[2][ii_n][jj_n] != 0.0) ||
            (sim_out->body_soil_[3][ii_n][jj_n] != 0.0));

        // The only option left is that there is space for the intersecting soil
        // Note that there is necessarily enough space for all the soil.
        if (body_soil_presence_3) {
            // Soil should go into the existing body soil layer
            sim_out->body_soil_[3][ii_n][jj_n] += h_soil;
        } else {
            // Soil should create a new body soil layer
            sim_out->body_soil_[2][ii_n][jj_n] = sim_out->body_[3][ii_n][jj_n];
            sim_out->body_soil_[3][ii_n][jj_n] = (
                sim_out->body_[3][ii_n][jj_n] + h_soil);
        }

        // Calculating pos of cell in body frame
        auto pos = soil_simulator::CalcBodyFramePos(
            ii_n, jj_n, sim_out->body_[3][ii_n][jj_n], grid, body);

        // Adding new body soil position to body_soil_pos
        sim_out->body_soil_pos_.push_back(
            soil_simulator::body_soil
            {2, ii_n, jj_n, pos[0], pos[1], pos[2], h_soil});
        h_soil = 0.0;
    } else if (body_absence_3) {
        // Only the first body layer
        if (
            sim_out->body_[0][ii_n][jj_n] - tol >
            sim_out->body_[ind_p+1][ii_p][jj_p]) {
            // Soil avalanche below the first body layer to the terrain
            // Note that all soil is going to the terrain without considering
            // the space available. If there is not enough space available, the
            // soil would intersect with the body and later be moved by the
            // MoveIntersectingBody function
            sim_out->terrain_[ii_n][jj_n] += h_soil;
            return {ind_p, ii_p, jj_p, 0.0, wall_presence};
        } else if (sim_out->body_[1][ii_n][jj_n] + tol > max_h) {
            // Body is blocking the movement
            return {ind_p, ii_p, jj_p, h_soil, true};
        }

        bool body_soil_presence_1 = (
            (sim_out->body_soil_[0][ii_n][jj_n] != 0.0) ||
            (sim_out->body_soil_[1][ii_n][jj_n] != 0.0));

        // The only option left is that there is space for the intersecting soil
        // Note that there is necessarily enough space for all the soil
        if (body_soil_presence_1) {
            // Soil should go into the existing body soil layer
            sim_out->body_soil_[1][ii_n][jj_n] += h_soil;
        } else {
            // Soil should create a new body soil layer
            sim_out->body_soil_[0][ii_n][jj_n] = sim_out->body_[1][ii_n][jj_n];
            sim_out->body_soil_[1][ii_n][jj_n] = (
                sim_out->body_[1][ii_n][jj_n] + h_soil);
        }

        // Calculating pos of cell in body frame
        auto pos = soil_simulator::CalcBodyFramePos(
            ii_n, jj_n, sim_out->body_[1][ii_n][jj_n], grid, body);

        // Adding new body soil position to body_soil_pos
        sim_out->body_soil_pos_.push_back(
            soil_simulator::body_soil
            {0, ii_n, jj_n, pos[0], pos[1], pos[2], h_soil});
        h_soil = 0.0;
    } else {
        // Both body layers are present
        int ind_b_n;
        int ind_t_n;
        if (sim_out->body_[0][ii_n][jj_n] < sim_out->body_[2][ii_n][jj_n]) {
            // First layer at bottom
            ind_b_n = 0;
            ind_t_n = 2;
        } else {
            // Second layer at bottom
            ind_b_n = 2;
            ind_t_n = 0;
        }

        bool body_soil_presence = (
            (sim_out->body_soil_[ind_b_n][ii_n][jj_n] != 0.0) ||
            (sim_out->body_soil_[ind_b_n+1][ii_n][jj_n] != 0.0));

        if (body_soil_presence) {
            // Body soil is present between the two body layers
            if (
                sim_out->body_soil_[ind_b_n+1][ii_n][jj_n] + tol >
                sim_out->body_[ind_t_n][ii_n][jj_n]) {
                // Body and soil blocking the movement
                return {ind_b_n, ii_n, jj_n, h_soil, wall_presence};
            }
        }

        // Calculating pos of cell in body frame
        auto pos = soil_simulator::CalcBodyFramePos(
            ii_n, jj_n, sim_out->body_[ind_b_n+1][ii_n][jj_n], grid, body);

        // Only option left is that there is space for the intersecting soil
        if (body_soil_presence) {
            // Soil should go into the existing body soil layer
            // Calculating available space
            float delta_h = (
                sim_out->body_[ind_t_n][ii_n][jj_n] -
                sim_out->body_soil_[ind_b_n+1][ii_n][jj_n]);

            if (delta_h < h_soil) {
                // Not enough space
                h_soil -= delta_h;

                // Adding soil to the body soil layer
                sim_out->body_soil_[ind_b_n+1][ii_n][jj_n] += delta_h;

                // Adding new body soil position to body_soil_pos
                sim_out->body_soil_pos_.push_back(
                    soil_simulator::body_soil
                    {ind_b_n, ii_n, jj_n, pos[0], pos[1], pos[2], delta_h});

                // Updating previous position
                ii_p = ii_n;
                jj_p = jj_n;
                ind_p = ind_b_n;
            } else {
                // More space than soil
                // Adding soil to the body soil layer
                sim_out->body_soil_[ind_b_n+1][ii_n][jj_n] += h_soil;

                // Adding new body soil position to body_soil_pos
                sim_out->body_soil_pos_.push_back(
                    soil_simulator::body_soil
                    {ind_b_n, ii_n, jj_n, pos[0], pos[1], pos[2], h_soil});
                h_soil = 0.0;
            }
        } else {
            // Soil should create a new body soil layer
            // Calculating available space
            float delta_h = (
                sim_out->body_[ind_t_n][ii_n][jj_n] -
                sim_out->body_[ind_b_n+1][ii_n][jj_n]);

            if (delta_h < h_soil) {
                // Not enough space
                h_soil -= delta_h;

                // Creating a new body soil layer
                sim_out->body_soil_[ind_b_n][ii_n][jj_n] = (
                    sim_out->body_[ind_b_n+1][ii_n][jj_n]);
                sim_out->body_soil_[ind_b_n+1][ii_n][jj_n] = (
                    sim_out->body_[ind_b_n+1][ii_n][jj_n] + delta_h);

                // Adding new body soil position to body_soil_pos
                sim_out->body_soil_pos_.push_back(
                    soil_simulator::body_soil
                    {ind_b_n, ii_n, jj_n, pos[0], pos[1], pos[2], delta_h});

                // Updating previous position
                ii_p = ii_n;
                jj_p = jj_n;
                ind_p = ind_b_n;
            } else {
                // More space than soil
                // Creating a new body soil layer
                sim_out->body_soil_[ind_b_n][ii_n][jj_n] = (
                    sim_out->body_[ind_b_n+1][ii_n][jj_n]);
                sim_out->body_soil_[ind_b_n+1][ii_n][jj_n] = (
                    sim_out->body_[ind_b_n+1][ii_n][jj_n] + h_soil);

                // Adding new body soil position to body_soil_pos
                sim_out->body_soil_pos_.push_back(
                    soil_simulator::body_soil
                    {ind_b_n, ii_n, jj_n, pos[0], pos[1], pos[2], h_soil});
                h_soil = 0.0;
            }
        }
    }

    return {ind_p, ii_p, jj_p, h_soil, wall_presence};
}

std::vector<std::vector<int>> soil_simulator::LocateIntersectingCells(
    SimOut* sim_out, float tol
) {
    // Initializing
    std::vector<std::vector<int>> intersecting_cells;

    // Iterating over all body position
    for (auto ii = sim_out->body_area_[0][0];
        ii < sim_out->body_area_[0][1]; ii++
    )
        for (auto jj = sim_out->body_area_[1][0];
            jj < sim_out->body_area_[1][1]; jj++
        ) {
            if (
               ((sim_out->body_[0][ii][jj] != 0.0) ||
               (sim_out->body_[1][ii][jj] != 0.0)) &&
               (sim_out->terrain_[ii][jj] - tol > sim_out->body_[0][ii][jj])) {
                // Soil intersecting with the body
                intersecting_cells.push_back(std::vector<int> {0, ii, jj});
            }
            if (
               ((sim_out->body_[2][ii][jj] != 0.0) ||
               (sim_out->body_[3][ii][jj] != 0.0)) &&
                (sim_out->terrain_[ii][jj] - tol > sim_out->body_[2][ii][jj])) {
                // Soil intersecting with the body
                intersecting_cells.push_back(std::vector<int> {2, ii, jj});
            }
        }

    return intersecting_cells;
}
