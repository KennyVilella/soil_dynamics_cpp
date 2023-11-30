/*
This file implements an example script for the simulator.

Copyright, 2023, Vilella Kenny.
*/
#include <glog/logging.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>
#include "test/example/soil_evolution.hpp"
#include "soil_simulator/soil_dynamics.hpp"
#include "soil_simulator/types.hpp"
#include "soil_simulator/body_pos.hpp"
#include "soil_simulator/body_soil.hpp"
#include "soil_simulator/intersecting_cells.hpp"
#include "soil_simulator/relax.hpp"
#include "soil_simulator/utils.hpp"

/// This example script simulates either a bucket performing a simple digging
/// scoop in the XZ plane following a parabolic trajectory, or a blade
/// performing a soil pushing in the XZ plane following a slightly parabolic
/// trajectory. There is an option to randomize the parabolic trajectory by
/// selecting the initial position (`x_i`, `z_i`) of the body and the deepest
/// point of the trajectory (`x_min`, `z_min`) within reasonable ranges.
///
/// Note that the parabolic trajectory assumes that the orientation of the
/// bucket follows the gradient of the trajectory. While it may not be fully
/// accurate, it provides a good approximation for testing the simulator.
/// By contrast, the blade keeps the same orientation throughout the trajectory.
template <typename T>
void soil_simulator::SoilEvolution(
    T* body, bool set_rng, bool random_trajectory, bool logging,
    bool check_outputs, bool write_outputs
) {
    // Initalizing the simulator
    soil_simulator::SoilDynamics sim;

    // Initalizing the simulation grid
    soil_simulator::Grid grid(4.0, 4.0, 4.0, 0.05, 0.01);

    // Initalizing the simulation parameter
    soil_simulator::SimParam sim_param(0.85, 3, 4);

    // Initalizing the simulation outputs class
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    if (set_rng)
        soil_simulator::rng.seed(1234);

    std::vector<std::vector<float>> pos;
    std::vector<std::vector<float>> ori;
    if (typeid(body) == typeid(soil_simulator::Bucket*)) {
        float origin_angle = std::atan2(
            body->t_pos_init_[2] - body->b_pos_init_[2],
            body->t_pos_init_[0] - body->b_pos_init_[0]);
        if (random_trajectory) {
            // Random parabolic trajectory
            // Calculating random parameters within a certain range
            std::uniform_real_distribution<float> dist(0.0, 1.0);
            float x_i = -3.0 + 2.0 * dist(soil_simulator::rng);
            float z_i = 0.5 + 1.5 * dist(soil_simulator::rng);
            float x_min = -0.5 * dist(soil_simulator::rng);
            float z_min = -0.25 + 0.5 * dist(soil_simulator::rng);

            // Creating the trajectory
            std::tie(pos, ori) = soil_simulator::CalcTrajectory(
                x_i, z_i, x_min, z_min, origin_angle, 10000);
        } else {
            // Default parabolic trajectory
            std::tie(pos, ori) = soil_simulator::CalcTrajectory(
                -2.0, 1.5, 0.1, 0.25, origin_angle, 10000);
        }
    } else if (typeid(body) == typeid(soil_simulator::Blade*)) {
        if (random_trajectory) {
            // Random parabolic trajectory
            // Calculating random parameters within a certain range
            std::uniform_real_distribution<float> dist(0.0, 1.0);
            float x_i = -3.0 + 2.0 * dist(soil_simulator::rng);
            float z_i = 0.1 + 0.1 * dist(soil_simulator::rng);
            float x_min = -0.5 * dist(soil_simulator::rng);
            float z_min = 0.1 + 0.1 * dist(soil_simulator::rng);

            // Creating the trajectory
            std::tie(pos, ori) = soil_simulator::CalcTrajectory(
                x_i, z_i, x_min, z_min, 0.0, 10000);
        } else {
            // Default parabolic trajectory
            std::tie(pos, ori) = soil_simulator::CalcTrajectory(
                -2.0, 0.1, 0.1, 0.0, 0.0, 10000);
        }
    } else {
        throw std::invalid_argument("Type of body class is not recognized.");
    }

    // Initializing body corner position vectors
    std::vector<std::vector<float>> j_r_pos;
    std::vector<std::vector<float>> j_l_pos;
    std::vector<std::vector<float>> b_r_pos;
    std::vector<std::vector<float>> b_l_pos;
    std::vector<std::vector<float>> t_r_pos;
    std::vector<std::vector<float>> t_l_pos;

    // Iterating over body trajectory
    for (auto ii = 0; ii < pos.size(); ii++) {
        // Converting orientation to quaternion
        auto ori_i = soil_simulator::AngleToQuat(
            {ori[ii][0], ori[ii][1], ori[ii][2]});

        // Calculating position of body points
        auto j_pos = soil_simulator::CalcRotationQuaternion(
            ori_i, body->j_pos_init_);
        auto b_pos = soil_simulator::CalcRotationQuaternion(
            ori_i, body->b_pos_init_);
        auto t_pos = soil_simulator::CalcRotationQuaternion(
            ori_i, body->t_pos_init_);
        for (auto jj = 0; jj < 3; jj++) {
            j_pos[jj] += pos[ii][jj];
            b_pos[jj] += pos[ii][jj];
            t_pos[jj] += pos[ii][jj];
        }

        // Calculating lateral vector of the body
        auto normal_side = soil_simulator::CalcNormal(j_pos, b_pos, t_pos);
        std::vector<float> half_width = {
            body->width_ * normal_side[0] / 2,
            body->width_ * normal_side[1] / 2,
            body->width_ * normal_side[2] / 2};

        // Populating position of the body corners
        j_r_pos.push_back(std::vector<float> {
            j_pos[0] + half_width[0], j_pos[1] + half_width[1],
            j_pos[2] + half_width[2]});
        j_l_pos.push_back(std::vector<float> {
            j_pos[0] - half_width[0], j_pos[1] - half_width[1],
            j_pos[2] - half_width[2]});
        b_r_pos.push_back(std::vector<float> {
            b_pos[0] + half_width[0], b_pos[1] + half_width[1],
            b_pos[2] + half_width[2]});
        b_l_pos.push_back(std::vector<float> {
            b_pos[0] - half_width[0], b_pos[1] - half_width[1],
            b_pos[2] - half_width[2]});
        t_r_pos.push_back(std::vector<float> {
            t_pos[0] + half_width[0], t_pos[1] + half_width[1],
            t_pos[2] + half_width[2]});
        t_l_pos.push_back(std::vector<float> {
            t_pos[0] - half_width[0], t_pos[1] - half_width[1],
            t_pos[2] - half_width[2]});
    }

    // Setting time
    float total_time = 8.0;
    float dt = 0.2;

    // Calculating time step for pos and ori vector
    float dt_int = total_time / (pos.size() - 1);

    // Initializing
    std::vector<std::vector<float>> pos_vec;
    std::vector<std::vector<float>> ori_vec;
    std::vector<float> time_vec;
    float dt_i = 0.1;
    float time = 0.1;
    pos_vec.push_back(pos[0]);
    auto ori_i = soil_simulator::AngleToQuat(
            {ori[0][0], ori[0][1], ori[0][2]});
    ori_vec.push_back(ori_i);

    // Creating time evolution
    while (time + dt_i < total_time) {
       // Adding time to time vector
       time_vec.push_back(time);

       // Searching time that is closest to the time where pos and ori
       // were calculated
       int kk = static_cast<int>(std::ceil(time / dt_int));

       // Calculating linear interpolation of pos and ori at time
       float a = ((kk + 1) * dt_int - time) / dt_int;
       float b = (time - kk * dt_int) / dt_int;
       pos_vec.push_back(std::vector<float> {
           pos[kk][0] * a + pos[kk+1][0] * b,
           pos[kk][1] * a + pos[kk+1][1] * b,
           pos[kk][2] * a + pos[kk+1][2] * b});

       ori_vec.push_back(soil_simulator::AngleToQuat({
            (ori[kk][0] * a + ori[kk+1][0] * b),
            (ori[kk][1] * a + ori[kk+1][1] * b),
            (ori[kk][2] * a + ori[kk+1][2] * b)}));

       // Calculating linear interpolation of body corners
       float time_plus = time + 0.5 * dt_i;
       int kk_plus = static_cast<int>(std::ceil(time_plus / dt_int));
       float a_plus = ((kk + 1) * dt_int - time_plus) / dt_int;
       float b_plus = (time_plus - kk * dt_int) / dt_int;
       std::vector<float> j_l_pos_plus = {
           j_l_pos[kk_plus][0] * a + j_l_pos[kk_plus+1][0] * b,
           j_l_pos[kk_plus][1] * a + j_l_pos[kk_plus+1][1] * b,
           j_l_pos[kk_plus][2] * a + j_l_pos[kk_plus+1][2] * b};
       std::vector<float> j_r_pos_plus = {
           j_r_pos[kk_plus][0] * a + j_r_pos[kk_plus+1][0] * b,
           j_r_pos[kk_plus][1] * a + j_r_pos[kk_plus+1][1] * b,
           j_r_pos[kk_plus][2] * a + j_r_pos[kk_plus+1][2] * b};
       std::vector<float> b_l_pos_plus = {
           b_l_pos[kk_plus][0] * a + b_l_pos[kk_plus+1][0] * b,
           b_l_pos[kk_plus][1] * a + b_l_pos[kk_plus+1][1] * b,
           b_l_pos[kk_plus][2] * a + b_l_pos[kk_plus+1][2] * b};
       std::vector<float> b_r_pos_plus = {
           b_r_pos[kk_plus][0] * a + b_r_pos[kk_plus+1][0] * b,
           b_r_pos[kk_plus][1] * a + b_r_pos[kk_plus+1][1] * b,
           b_r_pos[kk_plus][2] * a + b_r_pos[kk_plus+1][2] * b};
       std::vector<float> t_l_pos_plus = {
           t_l_pos[kk_plus][0] * a + t_l_pos[kk_plus+1][0] * b,
           t_l_pos[kk_plus][1] * a + t_l_pos[kk_plus+1][1] * b,
           t_l_pos[kk_plus][2] * a + t_l_pos[kk_plus+1][2] * b};
       std::vector<float> t_r_pos_plus = {
           t_r_pos[kk_plus][0] * a + t_r_pos[kk_plus+1][0] * b,
           t_r_pos[kk_plus][1] * a + t_r_pos[kk_plus+1][1] * b,
           t_r_pos[kk_plus][2] * a + t_r_pos[kk_plus+1][2] * b};
       float time_minus = time - 0.5 * dt_i;
       int kk_minus = static_cast<int>(std::ceil(time_minus / dt_int));
       float a_minus = ((kk + 1) * dt_int - time_minus) / dt_int;
       float b_minus = (time_minus - kk * dt_int) / dt_int;
       std::vector<float> j_l_pos_minus = {
           j_l_pos[kk_minus][0] * a + j_l_pos[kk_minus+1][0] * b,
           j_l_pos[kk_minus][1] * a + j_l_pos[kk_minus+1][1] * b,
           j_l_pos[kk_minus][2] * a + j_l_pos[kk_minus+1][2] * b};
       std::vector<float> j_r_pos_minus = {
           j_r_pos[kk_minus][0] * a + j_r_pos[kk_minus+1][0] * b,
           j_r_pos[kk_minus][1] * a + j_r_pos[kk_minus+1][1] * b,
           j_r_pos[kk_minus][2] * a + j_r_pos[kk_minus+1][2] * b};
       std::vector<float> b_l_pos_minus = {
           b_l_pos[kk_minus][0] * a + b_l_pos[kk_minus+1][0] * b,
           b_l_pos[kk_minus][1] * a + b_l_pos[kk_minus+1][1] * b,
           b_l_pos[kk_minus][2] * a + b_l_pos[kk_minus+1][2] * b};
       std::vector<float> b_r_pos_minus = {
           b_r_pos[kk_minus][0] * a + b_r_pos[kk_minus+1][0] * b,
           b_r_pos[kk_minus][1] * a + b_r_pos[kk_minus+1][1] * b,
           b_r_pos[kk_minus][2] * a + b_r_pos[kk_minus+1][2] * b};
       std::vector<float> t_l_pos_minus = {
           t_l_pos[kk_minus][0] * a + t_l_pos[kk_minus+1][0] * b,
           t_l_pos[kk_minus][1] * a + t_l_pos[kk_minus+1][1] * b,
           t_l_pos[kk_minus][2] * a + t_l_pos[kk_minus+1][2] * b};
       std::vector<float> t_r_pos_minus = {
           t_r_pos[kk_minus][0] * a + t_r_pos[kk_minus+1][0] * b,
           t_r_pos[kk_minus][1] * a + t_r_pos[kk_minus+1][1] * b,
           t_r_pos[kk_minus][2] * a + t_r_pos[kk_minus+1][2] * b};

       // Calculating velocity of body corners
       float j_l_vel = std::sqrt(
           std::pow(j_l_pos_plus[0] - j_l_pos_minus[0], 2) +
           std::pow(j_l_pos_plus[1] - j_l_pos_minus[1], 2) +
           std::pow(j_l_pos_plus[2] - j_l_pos_minus[2], 2)) / dt_i;
       float j_r_vel = std::sqrt(
           std::pow(j_r_pos_plus[0] - j_r_pos_minus[0], 2) +
           std::pow(j_r_pos_plus[1] - j_r_pos_minus[1], 2) +
           std::pow(j_r_pos_plus[2] - j_r_pos_minus[2], 2)) / dt_i;
       float b_l_vel = std::sqrt(
           std::pow(b_l_pos_plus[0] - b_l_pos_minus[0], 2) +
           std::pow(b_l_pos_plus[1] - b_l_pos_minus[1], 2) +
           std::pow(b_l_pos_plus[2] - b_l_pos_minus[2], 2)) / dt_i;
       float b_r_vel = std::sqrt(
           std::pow(b_r_pos_plus[0] - b_r_pos_minus[0], 2) +
           std::pow(b_r_pos_plus[1] - b_r_pos_minus[1], 2) +
           std::pow(b_r_pos_plus[2] - b_r_pos_minus[2], 2)) / dt_i;
       float t_l_vel = std::sqrt(
           std::pow(t_l_pos_plus[0] - t_l_pos_minus[0], 2) +
           std::pow(t_l_pos_plus[1] - t_l_pos_minus[1], 2) +
           std::pow(t_l_pos_plus[2] - t_l_pos_minus[2], 2)) / dt_i;
       float t_r_vel = std::sqrt(
           std::pow(t_r_pos_plus[0] - t_r_pos_minus[0], 2) +
           std::pow(t_r_pos_plus[1] - t_r_pos_minus[1], 2) +
           std::pow(t_r_pos_plus[2] - t_r_pos_minus[2], 2)) / dt_i;

       // Calculating the maximum velocity of the body
       float max_body_vel = 1.25 * std::max({
           j_l_vel, j_r_vel, b_l_vel, b_r_vel, t_l_vel, t_r_vel});

       if (max_body_vel != 0.0) {
           // Body is moving
           dt_i = std::min(grid.cell_size_xy_, grid.cell_size_z_) /
               max_body_vel;
       } else {
           // No body movement
           dt_i = dt;
       }

       if (dt_i > dt) {
           // Body is moving very slowly
           time += dt;
       } else {
           // Body is moving
           time += dt_i;
       }
    }

    // Adding final step
    int nn = pos.size() - 1;
    time_vec.push_back(total_time);
    pos_vec.push_back(pos[nn]);
    ori_vec.push_back(soil_simulator::AngleToQuat(
        {ori[nn][0], ori[nn][1], ori[nn][2]}));

    // Initializing the terrain
    sim.Init(sim_out, grid, 32.0);

    int init_volume = 0;
    if (check_outputs) {
        for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
            for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
                init_volume += round(
                    sim_out->terrain_[ii][jj] / grid.cell_size_z_);
    }

    // Simulation loop
    for (auto ii = 0; ii < time_vec.size(); ii++) {
        if (logging)
            LOG(INFO) << "Step " << ii << " / " << time_vec.size()-1;

        // Stepping the soil dynamics
        sim.Step(
            sim_out, pos_vec[ii], ori_vec[ii], grid, body, sim_param, 1e-5);

        // Checking consistency of simulation outputs
        if (check_outputs)
            sim.Check(sim_out, init_volume, grid, 1e-5);

        // Writing simulation outputs into csv files
        if (write_outputs)
            sim.WriteOutputs(sim_out, grid, body);
    }

    delete sim_out;
}
template void soil_simulator::SoilEvolution(
    Bucket* body, bool set_rng, bool random_trajectory, bool logging,
    bool check_outputs, bool write_outputs);
template void soil_simulator::SoilEvolution(
    Blade* body, bool set_rng, bool random_trajectory, bool logging,
    bool check_outputs, bool write_outputs);

/// The parabolic trajectory is described by
///
///    z(x) = a * x * x + b * x + c.
///
/// Knowing that at the starting position
///
///    z(x_i) = z_i
///
/// and that at the deepest point of the trajectory
///
///    dz(x_min) / dx = 0.0
///    z(x_min) = z_min,
///
/// it is possible to calculate the three parameters (a, b, c) of the parabolic
/// equation. For the bucket, the orientation is assumed to be equal to the
/// gradient of the trajectory. This implies that the bucket teeth would follow
/// the movement, so that it can somewhat replicate an actual digging scoop.
/// For the blade, the orientation is constant throughout the trajectory.
std::tuple<
    std::vector<std::vector<float>>, std::vector<std::vector<float>>
> soil_simulator::CalcTrajectory(
    float x_i, float z_i, float x_min, float z_min, float origin_angle, int nn
) {
    // Calculating X vector of the trajectory
    std::vector<float> x_vec(nn);
    float delta_x = 2.0 * (x_min - x_i) / (nn - 1);
    for (auto ii = 0; ii < nn; ii++)
        x_vec[ii] = x_i + ii * delta_x;

    // Calculating factor of the parabolic function
    float a;
    float b;
    float c;
    if (x_min == 0) {
        a = (z_i - z_min) / (x_i * x_i);
        b = 0.0;
        c = z_min;
    } else {
        b = 2 * x_min * (z_min - z_i) / ((x_i - x_min) * (x_i - x_min));
        a = -b / (2 * x_min);
        c = z_min + b * b / (4 * a);
    }

    // Initializing trajectory vector
    std::vector<std::vector<float>> pos;
    std::vector<std::vector<float>> ori;

    // Creating trajectory
    for (auto ii = 0; ii < nn; ii++) {
        // Calculating the trajectory following a parabole
        float x = x_vec[ii];
        pos.push_back(std::vector<float> {x, 0.0, a * x * x + b * x + c});

        // Calculating orientation following the gradient of the trajectory
        ori.push_back(std::vector<float> {
            0.0, -origin_angle + std::atan(2 * a * x + b), 0.0});
    }

    return {pos, ori};
}
