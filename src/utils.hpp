/*
This file declares the utility functions used in this simulator.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <vector>
#include <tuple>

namespace soil_simulator {

/// \brief This function calculates the unit normal vector of a plane formed by
///        three points using the right-hand rule.
///
/// \param a: Cartesian coordinates of the first point of the plane. [m]
/// \param b: Cartesian coordinates of the second point of the plane. [m]
/// \param c: Cartesian coordinates of the third point of the plane. [m]
///
/// \return Unit normal vector of the provided plane. [m]
std::vector<float> CalcNormal(
    std::vector<float> a, std::vector<float> b, std::vector<float> c);

/// \brief This function applies a rotation `ori` to the Cartesian
///        coordinates `pos`.
///
/// The Quaternion convention is used for 3D rotation.
///
/// \param ori: Orientation of the bucket. [Quaternion]
/// \param pos: Cartesian coordinates of one point of the bucket in its
///             reference pose relative to the bucket origin. [m]
///
/// \return Rotated Cartesian coordinates of the input `pos` relative to the
///         bucket origin. [m]
std::vector<float> CalcRotationQuaternion(
    std::vector<float> ori, std::vector<float> pos);

/// \brief This function calculates the product of two Quaternions.
///
/// \param q1: First quaternion. [Quaternion]
/// \param q2: Second quaternion. [Quaternion]
///
/// \return Product of the two inputs quaternions. [Quaternion]
std::vector<float> MultiplyQuaternion(
    std::vector<float> q1, std::vector<float> q2);

std::tuple<
    std::vector<std::vector<float>>, std::vector<std::vector<float>>
> CalcTrajectory(float x_i, float z_i, float x_min, float z_min, int nn);
}  // namespace soil_simulator
