/*
This file implements the main functions of the simulator.

Copyright, 2023, Vilella Kenny.
*/
#include <algorithm>
#include <iostream>
#include <vector>
#include "src/soil_dynamics.hpp"
#include "src/types.hpp"
#include "src/bucket_pos.hpp"
#include "src/body_soil.hpp"
#include "src/intersecting_cells.hpp"
#include "src/relax.hpp"
#include "src/utils.hpp"

void soil_simulator::SoilDynamics::step(
    SimOut* sim_out, std::vector<float> pos, std::vector<float> ori,
    Grid grid, Bucket* bucket, SimParam sim_param, float tol
) {
    // Updating bucket position
    soil_simulator::CalcBucketPos(
        sim_out, pos, ori, grid, bucket, sim_param, tol);

    // Updating position of soil resting on the bucket
    soil_simulator::UpdateBodySoil(sim_out, pos, ori, grid, bucket, tol);

    // Moving intersecting soil cells
    soil_simulator::MoveIntersectingCells(sim_out, tol);

    // Assuming that the terrain is not at equilibrium
    sim_out->equilibrium_ = false;

    // Iterating until equilibrium or maximum number of iterations is reached
    int it = 0;
    while (!sim_out->equilibrium_ && it < sim_param.max_iterations_) {
        it++;

        // Updating impact_area
        sim_out->impact_area_[0][0] = std::min(
            sim_out->bucket_area_[0][0], sim_out->relax_area_[0][0]);
        sim_out->impact_area_[1][0] = std::min(
            sim_out->bucket_area_[1][0], sim_out->relax_area_[1][0]);
        sim_out->impact_area_[0][1] = std::max(
            sim_out->bucket_area_[0][1], sim_out->relax_area_[0][1]);
        sim_out->impact_area_[1][1] = std::max(
            sim_out->bucket_area_[1][1], sim_out->relax_area_[1][1]);

        // Relaxing the terrain
        RelaxTerrain(sim_out, grid, sim_param, tol);

        // Relaxing the soil resting on the bucket
        RelaxBodySoil(sim_out, grid, sim_param, tol);
    }
}

void soil_simulator::SoilDynamics::check(
    SimOut* sim_out, float init_volume, Grid grid, float tol
) {
    // Checking mass conservation
    soil_simulator::CheckVolume(sim_out, init_volume, grid);

    // Checking consistency of simulation outputs
    soil_simulator::CheckSoil(sim_out, tol);
}

void soil_simulator::SoilDynamics::WriteOutputs(
        SimOut* sim_out, Grid grid, Bucket* bucket
) {
    // Writing terrain_ and body_soil_
    soil_simulator::WriteSoil(sim_out, grid);

    // Writing bucket corners
    soil_simulator::WriteBucket(bucket);
}

int main() {
    // Flags for the simulation
    bool set_rng = true;
    bool random_trajectory = true;
    bool check_outputs = true;
    bool write_outputs = true;

    // Initalizing the simulator
    soil_simulator::SoilDynamics sim;

    // Initalizing the simulation grid
    soil_simulator::Grid grid(4.0, 4.0, 4.0, 0.05, 0.05);

    // Initalizing the simulated bucket
    std::vector<float> o_pos_init = {0.0, 0.0, 0.0};
    std::vector<float> j_pos_init = {0.0, 0.0, 0.0};
    std::vector<float> b_pos_init = {0.0, 0.0, -0.5};
    std::vector<float> t_pos_init = {0.7, 0.0, -0.5};
    float bucket_width = 0.5;
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos_init, j_pos_init, b_pos_init, t_pos_init, bucket_width);

    // Initalizing the simulation parameter
    soil_simulator::SimParam sim_param(0.85, 10, 4);

    // Initalizing the simulation outputs class
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    if (set_rng)
        soil_simulator::rng.seed(1234);

    std::vector<std::vector<float>> pos;
    std::vector<std::vector<float>> ori;
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
            x_i, z_i, x_min, z_min, 100);
    } else {
        // Default parabolic trajectory
        std::tie(pos, ori) = soil_simulator::CalcTrajectory(
            -2.0, 1.5, 0.1, 0.25, 100);
    }

    // Initializing bucket corner position vectors
    std::vector<std::vector<float>> j_r_pos;
    std::vector<std::vector<float>> j_l_pos;
    std::vector<std::vector<float>> b_r_pos;
    std::vector<std::vector<float>> b_l_pos;
    std::vector<std::vector<float>> t_r_pos;
    std::vector<std::vector<float>> t_l_pos;

    // Iterating over bucket trajectory
    for (auto ii = 0; ii < pos.size(); ii++) {
        // Converting orientation to quaternion
        auto ori_i = soil_simulator::AngleToQuat(
            {-ori[ii][0], -ori[ii][1], -ori[ii][2]});

        // Calculating position of bucket points
        auto j_pos = soil_simulator::CalcRotationQuaternion(ori_i, j_pos_init);
        auto b_pos = soil_simulator::CalcRotationQuaternion(ori_i, b_pos_init);
        auto t_pos = soil_simulator::CalcRotationQuaternion(ori_i, t_pos_init);
        for (auto jj = 0; jj < 3; jj++) {
            j_pos[jj] += pos[ii][jj];
            b_pos[jj] += pos[ii][jj];
            t_pos[jj] += pos[ii][jj];
        }

        // Calculating lateral vector of the bucket
        auto normal_side = soil_simulator::CalcNormal(j_pos, b_pos, t_pos);
        std::vector<float> half_width = {
            bucket_width * normal_side[0] / 2,
            bucket_width * normal_side[1] / 2,
            bucket_width * normal_side[2] / 2};

        // Populating position of the bucket corners
        j_r_pos.push_back(std::vector<float> {
            j_pos[0] + half_width[0], j_pos[1] + half_width[1],
            j_pos[2] + half_width[2]});
        j_l_pos.push_back(std::vector<float> {
            j_pos[0] + half_width[0], j_pos[1] - half_width[1],
            j_pos[2] + half_width[2]});
        b_r_pos.push_back(std::vector<float> {
            b_pos[0] + half_width[0], b_pos[1] + half_width[1],
            b_pos[2] + half_width[2]});
        b_l_pos.push_back(std::vector<float> {
            b_pos[0] + half_width[0], b_pos[1] - half_width[1],
            b_pos[2] + half_width[2]});
        t_r_pos.push_back(std::vector<float> {
            t_pos[0] + half_width[0], t_pos[1] + half_width[1],
            t_pos[2] + half_width[2]});
        t_l_pos.push_back(std::vector<float> {
            t_pos[0] + half_width[0], t_pos[1] - half_width[1],
            t_pos[2] + half_width[2]});
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
    float dt_i = dt;
    float time = dt;
    pos_vec.push_back(pos[0]);
    auto ori_i = soil_simulator::AngleToQuat(
            {-ori[0][0], -ori[0][1], -ori[0][2]});
    ori_vec.push_back(ori_i);

    // Creating time evolution
    while (time + dt_i < total_time) {
       // Adding time to time vector
       time_vec.push_back(time);

       // Searching time that is closest to the time where pos and ori
       // were calculated
       int kk = static_cast<int>(std::floor(time / dt_int));

       // Calculating linear interpolation of pos and ori at time
       float a = ((kk + 1) * dt_int - time) / dt_int;
       float b = (time - kk * dt_int) / dt_int;
       pos_vec.push_back(std::vector<float> {
           pos[kk][0] * a + pos[kk+1][0] * b,
           pos[kk][1] * a + pos[kk+1][1] * b,
           pos[kk][2] * a + pos[kk+1][2] * b});

       ori_vec.push_back(soil_simulator::AngleToQuat({
            -(ori[kk][0] * a + ori[kk+1][0] * b),
            -(ori[kk][1] * a + ori[kk+1][1] * b),
            -(ori[kk][2] * a + ori[kk+1][2] * b)}));

       // Calculating linear interpolation of bucket corners
       float time_plus = time + 0.5 * dt_i;
       int kk_plus = static_cast<int>(std::floor(time_plus / dt_int));
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
       int kk_minus = static_cast<int>(std::floor(time_minus / dt_int));
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

       // Calculating velocity of bucket corners
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

       // Calculating the maximum velocity of the bucket
       float max_bucket_vel = std::max({
           j_l_vel, j_r_vel, b_l_vel, b_r_vel, t_l_vel, t_r_vel});

       if (max_bucket_vel != 0.0) {
           // Bucket is moving
           dt_i = grid.cell_size_xy_ / max_bucket_vel;
       } else {
           // No bucket movement
           dt_i = dt;
       }

       if (dt_i > dt) {
           // Bucket is moving very slowly
           time += dt;
       } else {
           // Bucket is mobing
           time += dt_i;
       }
    }

    // Adding final step
    int nn = pos.size() - 1;
    time_vec.push_back(total_time);
    pos_vec.push_back(pos[nn]);
    ori_vec.push_back(soil_simulator::AngleToQuat(
        {-ori[nn][0], -ori[nn][1], -ori[nn][2]}));

    float init_volume = 0.0;
    if (check_outputs) {
        for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
            for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
                init_volume += sim_out->terrain_[ii][jj];
        init_volume = grid.cell_area_ * init_volume;
    }

    // Simulation loop
    for (auto ii = 0; ii < time_vec.size(); ii++) {
        std::cout << "Step " << ii << " / " << time_vec.size()-1 << "\n";

        // Stepping the soil dynamics
        sim.step(
            sim_out, pos_vec[ii], ori_vec[ii], grid, bucket, sim_param, 1e-5);

        // Checking consistency of simulation outputs
        if (check_outputs)
            sim.check(sim_out, init_volume, grid, 1e-5);

        // Writing simulation outputs into csv files
        if (write_outputs)
            sim.WriteOutputs(sim_out, grid, bucket);
    }

    delete bucket;
    delete sim_out;

    return 0;
>>>>>>> origin/main
}
