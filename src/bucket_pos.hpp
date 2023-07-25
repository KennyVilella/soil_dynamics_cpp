/*
This file declares the functions used to calculate the bucket position.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <vector>
#include "src/types.hpp"

namespace soil_simulator {

void CalcBucketPos();
void CalcRectanglePos();
void DecomposeVectorRectangle();
void CalcTrianglePos();
void DecomposeVectorTriangle();

// This function determines all the cells that lie on a straight line between
// two Cartesian coordinates.
std::vector<std::vector<int>> CalcLinePos(
    std::vector<float> a, std::vector<float> b, float delta, Grid grid);

void UpdateBody();
void IncludeNewBodyPos();

}  // namespace soil_simulator
