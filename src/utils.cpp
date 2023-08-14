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

/// The conventions that are checked by this function include:
/// - The terrain should not overlap with the bucket.
/// - The bucket should be properly defined, with its maximum height higher than
///   its minimum height.
/// - The bucket soil should be properly defined, with its maximum height higher
///   than its minimum height.
/// - The two bucket layers should not overlap or touch each other.
/// - One bucket layer should not overlap with all bucket soil layer.
/// - The bucket should not overlap with the corresponding bucket soil layer.
/// - The bucket soil layer should be resting on the corresponding bucket layer.
/// - The bucket should be present when there is bucket soil.
bool soil_simulator::CheckSoil(
    SimOut* sim_out, float tol
) {
    // Iterating over all cells in the bucket area
    int ii_min = sim_out->bucket_area_[0][0];
    int ii_max = sim_out->bucket_area_[0][1];
    int jj_min = sim_out->bucket_area_[1][0];
    int jj_max = sim_out->bucket_area_[1][1];
    for (auto ii = ii_min; ii < ii_max; ii++)
        for (auto jj = jj_min; jj < jj_max; jj++) {
            // Renaming for convenience
            float terrain = sim_out->terrain_[ii][jj];
            float body_0 = sim_out->body_[0][ii][jj];
            float body_1 = sim_out->body_[1][ii][jj];
            float body_2 = sim_out->body_[2][ii][jj];
            float body_3 = sim_out->body_[3][ii][jj];
            float body_soil_0 = sim_out->body_soil_[0][ii][jj];
            float body_soil_1 = sim_out->body_soil_[1][ii][jj];
            float body_soil_2 = sim_out->body_soil_[2][ii][jj];
            float body_soil_3 = sim_out->body_soil_[3][ii][jj];

            // Checking presence of bucket and soil
            bool bucket_presence_0 = ((body_0 != 0.0) || (body_1 != 0.0));
            bool bucket_presence_2 = ((body_2 != 0.0) || (body_3 != 0.0));
            bool bucket_soil_presence_0 = (
                (body_soil_0 != 0.0) || (body_soil_1 != 0.0));
            bool bucket_soil_presence_2 = (
                (body_soil_2 != 0.0) || (body_soil_3 != 0.0));

            if ((bucket_presence_0) && (terrain > body_0 + tol)) {
                std::cout << "Terrain is above the bucket\n" <<
                    "Location: (" << ii << ", " << jj << ")\n" <<
                    "Terrain height: " << terrain << "\n" <<
                    "Bucket min height: " << body_0 << "\n";
                return false;
            }

            if ((bucket_presence_2) && (terrain > body_2 + tol)) {
                std::cout << "Terrain is above the bucket\n" <<
                    "Location: (" << ii << ", " << jj << ")\n" <<
                    "Terrain height: " << terrain << "\n" <<
                    "Bucket min height: " << body_2 << "\n";
                return false;
            }

            if ((bucket_presence_0) && (body_0 > body_1 + tol)) {
                std::cout <<  "Min height of the bucket is above its max height"
                    << "\nLocation: (" << ii << ", " << jj << ")\n" <<
                    "Bucket min height: " << body_0 << "\n" <<
                    "Bucket max height: " << body_1 << "\n";
                return false;
            }

            if ((bucket_presence_2) && (body_2 > body_3 + tol)) {
                std::cout << "Min height of the bucket is above its max height"
                    << "\nLocation: (" << ii << ", " << jj << ")\n" <<
                    "Bucket min height: " << body_2 <<  "\n" <<
                    "Bucket max height: " << body_3 << "\n";
                return false;
            }

            if (
                (bucket_presence_0) && (bucket_presence_2) &&
                (body_1 + tol > body_2) && (body_3 + tol > body_0)) {
                std::cout << "The two bucket layers are intersecting\n" <<
                    "Location: (" << ii << ", " << jj << ")\n" <<
                    "Bucket 1 min height: " << body_0 << "\n" <<
                    "Bucket 1 max height: " << body_1 << "\n" <<
                    "Bucket 2 min height: " << body_2 << "\n" <<
                    "Bucket 2 max height: " << body_3 << "\n";
                return false;
            }

            if (
                (bucket_presence_0) && (bucket_soil_presence_2) &&
                (body_1 - tol > body_soil_2) && (body_soil_3 - tol > body_0)) {
                std::cout << "A bucket layer and a bucket soil layer are inter"
                    "secting\n Location: (" << ii << ", " << jj << ")\n" <<
                    "Bucket 1 min height: " << body_0 << "\n" <<
                    "Bucket 1 max height: " << body_1 << "\n" <<
                    "Bucket soil 2 min height: " << body_soil_2 << "\n" <<
                    "Bucket soil 2 max height: " << body_soil_3 << "\n";
                return false;
            }

            if (
                (bucket_presence_2) && (bucket_soil_presence_0) &&
                (body_soil_1 - tol > body_2) && (body_3 - tol > body_soil_0)) {
                std::cout << "A bucket layer and a bucket soil layer are inter"
                    "secting\n Location: (" << ii << ", " << jj << ")\n" <<
                    "Bucket soil 1 min height: " << body_soil_0 << "\n" <<
                    "Bucket soil 1 max height: " << body_soil_1 << "\n" <<
                    "Bucket 2 min height: " << body_2 << "\n" <<
                    "Bucket 2 max height: " << body_3 << "\n";
                return false;
            }

            if ((bucket_soil_presence_0) && (body_soil_0 > body_soil_1 + tol)) {
                std::cout << "Min height of the bucket soil is above its max "
                    << "height\n Location: (" << ii << ", " << jj << ")\n" <<
                    "Bucket soil 1 min height: " << body_soil_0 << "\n" <<
                    "Bucket soil 1 max height: " << body_soil_1 << "\n";
                return false;
            }

            if ((bucket_soil_presence_2) && (body_soil_2 > body_soil_3 + tol)) {
                std::cout << "Min height of the bucket soil is above its max "
                    << "height\n Location: (" << ii << ", " << jj << ")\n" <<
                    "Bucket soil 2 min height: " << body_soil_2 << "\n" <<
                    "Bucket soil 2 max height: " << body_soil_3 << "\n";
                return false;
            }

            if ((bucket_soil_presence_0) && (body_1 > body_soil_0 + tol)) {
                std::cout << "Bucket is above the bucket soil\n" <<
                    "Location: (" << ii << ", " << jj << ")\n" <<
                    "Bucket 1 max height: " << body_1 << "\n" <<
                    "Bucket soil 1 min height: " << body_soil_0 << "\n";
                return false;
            }

            if ((bucket_soil_presence_2) && (body_3 > body_soil_2 + tol)) {
                std::cout << "Bucket is above the bucket soil\n" <<
                    "Location: (" << ii << ", " << jj << ")\n" <<
                    "Bucket 2 max height: " << body_3 << "\n" <<
                    "Bucket soil 2 min height: " << body_soil_2 << "\n";
                return false;
            }

            if ((bucket_soil_presence_0) && (body_soil_0 != body_1)) {
                std::cout << "Bucket soil is not resting on the bucket\n" <<
                    "Location: (" << ii << ", " << jj << ")\n" <<
                    "Bucket 1 max height: " << body_1 << "\n" <<
                    "Bucket soil 1 min height: " << body_soil_0 << "\n";
                return false;
            }

            if ((bucket_soil_presence_2) && (body_soil_2 != body_3)) {
                std::cout << "Bucket soil is not resting on the bucket\n" <<
                    "Location: (" << ii << ", " << jj << ")\n" <<
                    "Bucket 2 max height: " << body_3 << "\n" <<
                    "Bucket soil 2 min height: " << body_soil_2 << "\n";
                return false;
            }
        }

    // Iterating over all cells where bucket soil is located
    for (auto nn = 0; nn < sim_out->body_soil_pos_.size(); nn++) {
        // Renaming for convenience
        int ii = sim_out->body_soil_pos_[nn][1];
        int jj = sim_out->body_soil_pos_[nn][2];
        int ind = sim_out->body_soil_pos_[nn][0];
        float body_min = sim_out->body_[ind][ii][jj];
        float body_max = sim_out->body_[ind+1][ii][jj];
        float body_soil_min = sim_out->body_soil_[ind][ii][jj];
        float body_soil_max = sim_out->body_soil_[ind+1][ii][jj];


        // Check that soil is actually present
        bool bucket_soil_presence = (
            (body_soil_min != 0.0) || (body_soil_max != 0.0));

       if ((bucket_soil_presence) && (body_min == 0.0) && (body_max == 0.0)) {
            std::cout << "Bucket soil is present but there is no bucket\n" <<
                "Location: (" << ii << ", " << jj << ")\n" <<
                "Bucket soil min height: " << body_soil_min << "\n" <<
                "Bucket soil max height: " << body_soil_max << "\n";
           return false;
       }
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
