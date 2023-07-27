/*
This file declares the utility functions used in this simulator.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <vector>

namespace soil_simulator {

// This function calculates the unit normal vector of a plane formed by three
// points using the right-hand rule.
std::vector <float> CalcNormal(
    std::vector <float> a, std::vector <float> b, std::vector <float> c);

// This function applies a rotation `ori` to the Cartesian coordinates `pos`.
// The Quaternion convention is used for 3D rotation.
std::vector <float> CalcRotationQuaternion(
    std::vector <float> ori, std::vector <float> pos);

// This function calculates the product of two Quaternions.
std::vector <float> MultiplyQuaternion(
    std::vector <float> q1, std::vector <float> q2);
}  // namespace soil_simulator
