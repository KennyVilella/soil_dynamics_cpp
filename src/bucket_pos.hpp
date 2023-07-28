/*
This file declares the functions used to calculate the bucket position.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <tuple>
#include <vector>
#include "src/types.hpp"

namespace soil_simulator {

void CalcBucketPos(
    SimOut sim_out, std::vector<float> pos, std::vector<float> ori, Grid grid,
    Bucket bucket, SimParam sim_param, float tol);

// This function determines the cells where a rectangle surface is located.
std::vector<std::vector<int>> CalcRectanglePos(
    std::vector<float> a, std::vector<float> b, std::vector<float> c,
    std::vector<float> d, float delta, Grid grid, float tol);

// This function performs a vector decomposition on a portion of the horizontal
// plane where a rectangle ABCD is located.
std::tuple<
    std::vector<std::vector<float>>, std::vector<std::vector<float>>,
    std::vector<std::vector<bool>>, int>
DecomposeVectorRectangle(
    std::vector<float> ab_ind, std::vector<float> ad_ind,
    std::vector<float> a_ind, int area_min_x, int area_min_y,
    int area_length_x, int area_length_y, float tol);

// This function determines the cells where a triangle surface is located.
std::vector<std::vector<int>> CalcTrianglePos(
    std::vector<float> a, std::vector<float> b, std::vector<float> c,
    float delta, Grid grid, float tol);

// This function performs a vector decomposition on a portion of the horizontal
// plane where a tritangle ABC is located.
std::tuple<
    std::vector<std::vector<float>>, std::vector<std::vector<float>>,
    std::vector<std::vector<bool>>, int>
DecomposeVectorTriangle(
    std::vector<float> ab_ind, std::vector<float> ac_ind,
    std::vector<float> a_ind, int area_min_x, int area_min_y,
    int area_length_x, int area_length_y, float tol);

// This function determines all the cells that lie on a straight line between
// two Cartesian coordinates.
std::vector<std::vector<int>> CalcLinePos(
    std::vector<float> a, std::vector<float> b, float delta, Grid grid);

void UpdateBody(std::vector<std::vector<int>> area_pos, SimOut* sim_out,
    Grid grid, float tol);

void IncludeNewBodyPos(SimOut* sim_out, int ii, int jj, float min_h, float max_h,
    float tol);

}  // namespace soil_simulator
