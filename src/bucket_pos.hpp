/*
This file declares the functions used to calculate the bucket position.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <tuple>
#include <vector>
#include "src/types.hpp"

namespace soil_simulator {

void CalcBucketPos();

void CalcRectanglePos();

// This function performs a vector decomposition on a portion of the horizontal
// plane where a rectangle ABCD is located.
std::tuple<
    std::vector<std::vector<float>>, std::vector<std::vector<float>>,
    std::vector<std::vector<bool>>, int>
DecomposeVectorRectangle(
    std::vector<float> ab_ind, std::vector<float> ad_ind,
    std::vector<float> a_ind, int area_min_x, int area_min_y,
    int area_length_x, int area_length_y, float tol);

void CalcTrianglePos();

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

void UpdateBody();

void IncludeNewBodyPos();

}  // namespace soil_simulator
