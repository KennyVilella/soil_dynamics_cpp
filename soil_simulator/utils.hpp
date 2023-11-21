/*
This file declares the utility functions used in this simulator.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <tuple>
#include <vector>
#include "soil_simulator/types.hpp"

namespace soil_simulator {

/// \brief This function calculates the global position of the six corners of
///        the body.
///
/// \param pos: Cartesian coordinates of the body origin. [m]
/// \param ori: Orientation of the body. [Quaternion]
/// \param body: Class that stores information related to the body object.
///
/// \return A tuple composed of six vectors giving the Cartesian coordinates
///         of the body corners in that order: right side of the body joint,
///         left side of the body joint, right side of the body base, left
///         side of the body base, right side of the body teeth, left side
///         of the body teeth. [m]
std::tuple<
    std::vector<float>, std::vector<float>, std::vector<float>,
    std::vector<float>, std::vector<float>, std::vector<float>>
CalcBodyCornerPos(
    std::vector<float> pos, std::vector<float> ori, Body* body);

/// \brief This function calculates how far the body has travelled since the
///        last soil update and checks whether it is necessary to update the
///        soil.
///
/// \param pos: Cartesian coordinates of the body origin. [m]
/// \param ori: Orientation of the body. [Quaternion]
/// \param grid: Class that stores information related to the simulation grid.
/// \param body: Class that stores information related to the body object.
///
/// \return A boolean indicating whether the soil should be updated.
bool CheckBodyMovement(
    std::vector<float> pos, std::vector<float> ori, Grid grid, Body* body);

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

/// \brief This function calculates the position of a considered cell in the
///        body frame assuming that the body is in its reference position.
///
/// \param ii: Index of the considered cell in the X direction.
/// \param jj: Index of the considered cell in the Y direction.
/// \param z: Height of the considered position. [m]
/// \param grid: Class that stores information related to the simulation grid.
/// \param body: Class that stores information related to the body object.
///
/// \return Cartesian coordinates of the considered position in the reference
///         body frame.
std::vector<float> CalcBodyFramePos(
    int ii, int jj, float z, Grid grid, Body* body);

/// \brief This function applies a rotation `ori` to the Cartesian
///        coordinates `pos`.
///
/// The Quaternion convention is used for 3D rotation.
///
/// \param ori: Orientation of the body. [Quaternion]
/// \param pos: Cartesian coordinates of one point of the body in its
///             reference pose relative to the body origin. [m]
///
/// \return Rotated Cartesian coordinates of the input `pos` relative to the
///         body origin. [m]
std::vector<float> CalcRotationQuaternion(
    std::vector<float> ori, std::vector<float> pos);

/// \brief This function converts Euler angles following the ZYX convention to
///        a quaternion.
///
/// \param ori: Orientation of the body. [Euler angles, ZYX sequence]
///
/// \return Orientation of the body. [Quaternion]
std::vector<float> AngleToQuat(std::vector<float> ori);

/// \brief This function calculates the product of two quaternions.
///
/// \param q1: First quaternion. [Quaternion]
/// \param q2: Second quaternion. [Quaternion]
///
/// \return Product of the two inputs quaternions. [Quaternion]
std::vector<float> MultiplyQuaternion(
    std::vector<float> q1, std::vector<float> q2);

/// \brief This function checks that the volume of soil is conserved and that
///        the content of `body_soil_pos_` and `body_soil_` is consistent.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param init_volume: Initial volume of soil in the terrain. [m^3]
/// \param grid: Class that stores information related to the simulation grid.
/// \param tol: Small number used to handle numerical approximation errors.
///
/// \return Boolean indicating whether soil is conserved or not.
bool CheckVolume(SimOut* sim_out, float init_volume, Grid grid, float tol);

/// \brief This function checks that all the simulation outputs follow the
///        conventions of the simulator.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param tol: Small number used to handle numerical approximation errors.
///
/// \return Boolean indicating whether the simulation outputs are consistent.
bool CheckSoil(SimOut* sim_out, float tol);

/// \brief This function writes the terrain and the body soil into a csv
///        located in the `results` directory.
///
/// \param sim_out: Class that stores simulation outputs.
/// \param grid: Class that stores information related to the simulation grid.
void WriteSoil(SimOut* sim_out, Grid grid);

/// \brief This function writes the position of all body faces into a csv
///        located in the `results` directory.
///
/// \param body: Class that stores information related to the body object.
void WriteBody(Body* body);

/// \brief This function creates simplex noise for more realistic terrain
///        generation.
///
/// \param x: Rescaled Cartesian coordinate in the X direction.
/// \param y: Rescaled Cartesian coordinate in the Y direction.
/// \param perm_table: Pre-calculated permutation table to generate
///                    pseudo-random noise.
///
/// \return A float representing the Simplex noise.
float simplex_noise(float x, float y, std::vector<int> perm_table);

}  // namespace soil_simulator
