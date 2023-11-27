/*
This file declares an example script for the simulator.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

#include <tuple>
#include <vector>

namespace soil_simulator {

/// \brief This function provides an example script to run the simulator.
///
/// \param body: Class that stores information related to the body object.
/// \param set_rng: Indicates whether the RNG seed is set or not.
/// \param random_trajectory: Indicates whether the default trajectory or a
///                           randomized one is used.
/// \param logging: Indicates whether logging information are provided.
/// \param check_outputs: Indicates whether to run simulation outputs check
///                       at every step.
/// \param write_outputs: Indicates whether the simulation outputs are written
///                       into files at every step.
template <typename T>
void SoilEvolution(
    T* body, bool set_rng, bool random_trajectory, bool logging,
    bool check_outputs, bool write_outputs);

/// \brief This function calculates a parabolic trajectory given the starting
///        position (`x_i`, `z_i`) and the deepest position (`x_min`, `z_min`)
///        of the trajectory.
///
/// \param x_i: X coordinate of the starting position of the trajectory. [m]
/// \param z_i: Z coordinate of the starting position of the trajectory. [m]
/// \param x_min: X coordinate of the deepest position of the trajectory. [m]
/// \param z_min: Z coordinate of the deepest position of the trajectory. [m]
/// \param origin_angle: Angle to the horizontal plane of the bucket in
///                      its reference pose. [rad]
/// \param nn: Number of increments in the trajectory.
///
/// \return A tuple composed of a vector aggregating the position of the bucket
///         with time in meter, and a vector aggregating the orientation of the
///         bucket with time following the quaternion convention.
std::tuple<
    std::vector<std::vector<float>>, std::vector<std::vector<float>>
> CalcTrajectory(
    float x_i, float z_i, float x_min, float z_min, float origin_angle, int nn);

}  // namespace soil_simulator
