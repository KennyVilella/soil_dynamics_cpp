/*
This file implements the utility functions used in this simulator.

Copyright, 2023, Vilella Kenny.
*/
#include <cmath>
#include <iostream>
#include <vector>
#include "src/utils.hpp"
#include "src/types.hpp"

std::vector<float> soil_simulator::CalcNormal(
    std::vector<float> a, std::vector<float> b, std::vector<float> c
) {
    // Declaring vectors
    std::vector<float> normal(3);
    std::vector<float> cross(3);

    // Calculating cross product
    cross[0] = (b[1] - a[1]) * (c[2] - a[2]) - (b[2] - a[2]) * (c[1] - a[1]);
    cross[1] = (b[2] - a[2]) * (c[0] - a[0]) - (b[0] - a[0]) * (c[2] - a[2]);
    cross[2] = (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);

    // Calculating normal
    float norm_cross = std::sqrt(
        cross[0] * cross[0] + cross[1] * cross[1] + cross[2] * cross[2]);
    normal[0] = cross[0] / norm_cross;
    normal[1] = cross[1] / norm_cross;
    normal[2] = cross[2] / norm_cross;

    return normal;
}

/// The mathematical reasoning behind this implementation can be easily found
/// in the Wiki page of Quaternion or elsewhere.
std::vector<float> soil_simulator::CalcRotationQuaternion(
    std::vector<float> ori, std::vector<float> pos
) {
    // Declaring vectors
    float norm_ori = (
        ori[0] * ori[0] + ori[1] * ori[1] + ori[2] * ori[2] + ori[3] * ori[3]);
    std::vector<float> conj_ori = {
        ori[0] / norm_ori,
        -ori[1] / norm_ori,
        -ori[2] / norm_ori,
        -ori[3] / norm_ori};
    std::vector<float> temp_pos = {0.0, pos[0], pos[1], pos[2]};

    // Calculating rotation
    auto temp_quat = soil_simulator::MultiplyQuaternion(conj_ori, temp_pos);
    auto quat = soil_simulator::MultiplyQuaternion(temp_quat, ori);
    return {quat[1], quat[2], quat[3]};
}

/// The mathematical reasoning behind this implementation can be easily found
/// in the Wiki page of Quaternion or elsewhere.
///
/// Note that this function only works if the Euler angles follow the
/// ZYX convention.
std::vector<float> soil_simulator::AngleToQuat(
    std::vector<float> ori
) {
    // Compute the sines and cosines of half angle.
    float sin_0 = std::sin(ori[0] / 2.0);
    float sin_1 = std::sin(ori[1] / 2.0);
    float sin_2 = std::sin(ori[2] / 2.0);
    float cos_0 = std::cos(ori[0] / 2.0);
    float cos_1 = std::cos(ori[1] / 2.0);
    float cos_2 = std::cos(ori[2] / 2.0);

    float q_0 = cos_0 * cos_1 * cos_2 + sin_0 * sin_1 * sin_2;
    std::vector<float> quat(4);
    if (q_0 > 0) {
        quat[0] = q_0;
        quat[1] = cos_0 * cos_1 * sin_2 - sin_0 * sin_1 * cos_2;
        quat[2] = cos_0 * sin_1 * cos_2 + sin_0 * cos_1 * sin_2;
        quat[3] = sin_0 * cos_1 * cos_2 - cos_0 * sin_1 * sin_2;
    } else {
        quat[0] = -q_0;
        quat[1] = -cos_0 * cos_1 * sin_2 + sin_0 * sin_1 * cos_2;
        quat[2] = -cos_0 * sin_1 * cos_2 - sin_0 * cos_1 * sin_2;
        quat[3] = -sin_0 * cos_1 * cos_2 + cos_0 * sin_1 * sin_2;
    }

    return quat;
}

/// The mathematical reasoning behind this implementation can be easily found
/// in the Wiki page of Quaternion or elsewhere.
std::vector<float> soil_simulator::MultiplyQuaternion(
    std::vector<float> q1, std::vector<float> q2
) {
    // Declaring vectors
    std::vector<float> quat(4);

    // Calculating quaternion multiplication
    quat[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
    quat[1] = q1[0] * q2[1] + q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2];
    quat[2] = q1[0] * q2[2] - q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1];
    quat[3] = q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0];

    return quat;
}

/// The initial volume of soil (`init_volume`) has to be provided.
bool soil_simulator::CheckVolume(
    SimOut* sim_out, float init_volume, Grid grid
) {
    // Calculating volume of soil in the terrain
    float terrain_volume = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            terrain_volume += sim_out->terrain_[ii][jj];

    terrain_volume = grid.cell_area_ * terrain_volume;

    // Removing duplicates in body_soil_pos
    sort(sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());
    sim_out->body_soil_pos_.erase(unique(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end()),
        sim_out->body_soil_pos_.end());

    // Calculating volume of bucket soil
    float body_soil_volume = 0.0;
    for (auto nn = 0; nn < sim_out->body_soil_pos_.size(); nn++) {
        int ii = sim_out->body_soil_pos_[nn][1];
        int jj = sim_out->body_soil_pos_[nn][2];
        int ind = sim_out->body_soil_pos_[nn][0];
        body_soil_volume += (
            sim_out->body_soil_[ind+1][ii][jj] -
            sim_out->body_soil_[ind][ii][jj]);
    }
    body_soil_volume = grid.cell_area_ * body_soil_volume;

    // Calculating total volume of soil
    float total_volume = terrain_volume + body_soil_volume;

    if (abs(total_volume - init_volume) > 0.5 * grid.cell_volume_) {
        std::cout << "Volume is not conserved! \n" <<
            "Initial volume: " << init_volume << "   Current volume: " <<
            total_volume << "\n";
        return false;
    }
    return true;
}


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
/// equation. The orientation is assumed to be equal to the gradient of the
/// trajectory. This implies that the bucket teeth would follow the movement, so
/// that it can somewhat replicate an actual digging scoop.
std::tuple<
    std::vector<std::vector<float>>, std::vector<std::vector<float>>
> soil_simulator::CalcTrajectory(
    float x_i, float z_i, float x_min, float z_min, int nn
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
        ori.push_back(std::vector<float> {0.0, std::atan(2 * a * x + b), 0.0});
    }

    return {pos, ori};
}
