/*
This file declares the utility functions used in this simulator.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <vector>
#include <tuple>
#include "src/types.hpp"

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

/// \brief This function converts Euler angles following the ZYX convention to
///        a quaternion.
///
/// \param ori: Orientation of the bucket. [Euler angles, ZYX sequence]
///
/// \return Orientation of the bucket. [Quaternion]
std::vector<float> AngleToQuat(std::vector<float> ori);

/// \brief This function calculates the product of two Quaternions.
///
/// \param q1: First quaternion. [Quaternion]
/// \param q2: Second quaternion. [Quaternion]
///
/// \return Product of the two inputs quaternions. [Quaternion]
std::vector<float> MultiplyQuaternion(
    std::vector<float> q1, std::vector<float> q2);

/// \brief This function checks that the volume of soil is conserved.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param init_volume: Initial volume of soil in the terrain. [m^3]
/// \param grid: Class that stores information related to the simulation grid.
///
/// \return Boolean indicating whether soil is conserved or not.
bool CheckVolume(SimOut* sim_out, float init_volume, Grid grid);

/// \brief This function checks that all the simulation outputs follow the
///        conventions of the simulator.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param tol: Small number used to handle numerical approximation errors.
///
/// \return Boolean indicating whether the simulation outputs are consistent.
bool CheckSoil(SimOut* sim_out, float tol);

/// \brief This function writes the terrain and the bucket soil into a csv
///        located in the `results` directory.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param grid: Class that stores information related to the simulation grid.
void WriteSoil(SimOut* sim_out, Grid grid);

/// \brief This function writes the position of all bucket faces into a csv
///        located in the `results` directory.
///
/// \param grid: Class that stores information related to the simulation grid.
void WriteBucket(Bucket* bucket);
}  // namespace soil_simulator