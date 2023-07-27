/*
This file declares the utility functions used in this simulator.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <vector>

namespace soil_simulator {

std::vector <float> CalcNormal(
    std::vector <float> a, std::vector <float> b, std::vector <float> c);

std::vector <float> CalcRotationQuaternion(
    std::vector <float> ori, std::vector <float> pos);

std::vector <float> MultiplyQuaternion(
    std::vector <float> q1, std::vector <float> q2);
}  // namespace soil_simulator
