/*
This file declares the main class of this simulator.

Copyright, 2023, Vilella Kenny.
*/
#pragma once

namespace soil_simulator {

void SoilEvolution(
    bool set_rng, bool random_trajectory, bool logging, bool check_outputs,
    bool write_outputs);

/// \brief This function calculates a parabolic trajectory given the starting
///        position (`x_i`, `z_i`) and the deepest position (`x_min`, `z_min`)
///        of the trajectory.
///
/// \param x_i: X coordinate of the starting position of the trajectory. [m]
/// \param z_i: Z coordinate of the starting position of the trajectory. [m]
/// \param x_min: X coordinate of the deepest position of the trajectory. [m]
/// \param z_min: Z coordinate of the deepest position of the trajectory. [m]
/// \param nn: Number of increments in the trajectory.
///
/// \return A tuple composed of a vector aggregating the position of the bucket
///         with time in meter, and a vector aggregating the orientation of the
///         bucket with time following the quaternion convention.
std::tuple<
    std::vector<std::vector<float>>, std::vector<std::vector<float>>
> CalcTrajectory(float x_i, float z_i, float x_min, float z_min, int nn);
}  // namespace soil_simulator
