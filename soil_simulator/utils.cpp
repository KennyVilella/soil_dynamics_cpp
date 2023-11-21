/*
This file implements the utility functions used in this simulator.

Copyright, 2023, Vilella Kenny.
*/
#include <glog/logging.h>
#include <source_location>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "soil_simulator/utils.hpp"
#include "soil_simulator/types.hpp"

std::tuple<
    std::vector<float>, std::vector<float>, std::vector<float>,
    std::vector<float>, std::vector<float>, std::vector<float>>
soil_simulator::CalcBucketCornerPos(
    std::vector<float> pos, std::vector<float> ori, Body* body
) {
    // Calculating position of the body vertices
    auto j_pos = soil_simulator::CalcRotationQuaternion(
        ori, body->j_pos_init_);
    auto b_pos = soil_simulator::CalcRotationQuaternion(
        ori, body->b_pos_init_);
    auto t_pos = soil_simulator::CalcRotationQuaternion(
        ori, body->t_pos_init_);

    // Unit vector normal to the side of the body
    auto normal_side = soil_simulator::CalcNormal(j_pos, b_pos, t_pos);

    // Declaring vectors for each vertex of the body
    std::vector<float> j_r_pos(3);
    std::vector<float> j_l_pos(3);
    std::vector<float> b_r_pos(3);
    std::vector<float> b_l_pos(3);
    std::vector<float> t_r_pos(3);
    std::vector<float> t_l_pos(3);

    for (auto ii = 0; ii < 3; ii++) {
        // Adding position of the body origin
        j_pos[ii] += pos[ii];
        b_pos[ii] += pos[ii];
        t_pos[ii] += pos[ii];

        // Position of each vertex of the body
        j_r_pos[ii] = j_pos[ii] + 0.5 * body->width_ * normal_side[ii];
        j_l_pos[ii] = j_pos[ii] - 0.5 * body->width_ * normal_side[ii];
        b_r_pos[ii] = b_pos[ii] + 0.5 * body->width_ * normal_side[ii];
        b_l_pos[ii] = b_pos[ii] - 0.5 * body->width_ * normal_side[ii];
        t_r_pos[ii] = t_pos[ii] + 0.5 * body->width_ * normal_side[ii];
        t_l_pos[ii] = t_pos[ii] - 0.5 * body->width_ * normal_side[ii];
    }

    return {j_r_pos, j_l_pos, b_r_pos, b_l_pos, t_r_pos, t_l_pos};
}

/// This function calculates the maximum distance travelled by any part of the
/// body since the last soil update. The position of the body during the
/// last soil update is stored in the `body` class.
///
/// If the maximum distance travelled is lower than 50% of the cell size,
/// the function returns `false` otherwise it returns `true`.
/// Note that if the distance travelled exceeds twice the cell size, a warning
/// is issued to indicate a potential problem with the soil update.
bool soil_simulator::CheckBodyMovement(
    std::vector<float> pos, std::vector<float> ori, Grid grid, Body* body
) {
    // Calculating new position of body corners
    auto [j_r_pos_n, j_l_pos_n, b_r_pos_n, b_l_pos_n, t_r_pos_n, t_l_pos_n] =
        soil_simulator::CalcBucketCornerPos(pos, ori, body);

    // Calculating former position of body corners
    auto [j_r_pos_f, j_l_pos_f, b_r_pos_f, b_l_pos_f, t_r_pos_f, t_l_pos_f] =
        soil_simulator::CalcBucketCornerPos(body->pos_, body->ori_, body);

    // Calculating distance travelled
    float j_r_dist = std::sqrt(
        (j_r_pos_f[0] - j_r_pos_n[0]) * (j_r_pos_f[0] - j_r_pos_n[0]) +
        (j_r_pos_f[1] - j_r_pos_n[1]) * (j_r_pos_f[1] - j_r_pos_n[1])+
        (j_r_pos_f[2] - j_r_pos_n[2]) * (j_r_pos_f[2] - j_r_pos_n[2]));
    float j_l_dist = std::sqrt(
        (j_l_pos_f[0] - j_l_pos_n[0]) * (j_l_pos_f[0] - j_l_pos_n[0]) +
        (j_l_pos_f[1] - j_l_pos_n[1]) * (j_l_pos_f[1] - j_l_pos_n[1]) +
        (j_l_pos_f[2] - j_l_pos_n[2]) * (j_l_pos_f[2] - j_l_pos_n[2]));
    float b_r_dist = std::sqrt(
        (b_r_pos_f[0] - b_r_pos_n[0]) * (b_r_pos_f[0] - b_r_pos_n[0]) +
        (b_r_pos_f[1] - b_r_pos_n[1]) * (b_r_pos_f[1] - b_r_pos_n[1]) +
        (b_r_pos_f[2] - b_r_pos_n[2]) * (b_r_pos_f[2] - b_r_pos_n[2]));
    float b_l_dist = std::sqrt(
        (b_l_pos_f[0] - b_l_pos_n[0]) * (b_l_pos_f[0] - b_l_pos_n[0]) +
        (b_l_pos_f[1] - b_l_pos_n[1]) * (b_l_pos_f[1] - b_l_pos_n[1]) +
        (b_l_pos_f[2] - b_l_pos_n[2]) * (b_l_pos_f[2] - b_l_pos_n[2]));
    float t_r_dist = std::sqrt(
        (t_r_pos_f[0] - t_r_pos_n[0]) * (t_r_pos_f[0] - t_r_pos_n[0]) +
        (t_r_pos_f[1] - t_r_pos_n[1]) * (t_r_pos_f[1] - t_r_pos_n[1]) +
        (t_r_pos_f[2] - t_r_pos_n[2]) * (t_r_pos_f[2] - t_r_pos_n[2]));
    float t_l_dist = std::sqrt(
        (t_l_pos_f[0] - t_l_pos_n[0]) * (t_l_pos_f[0] - t_l_pos_n[0]) +
        (t_l_pos_f[1] - t_l_pos_n[1]) * (t_l_pos_f[1] - t_l_pos_n[1]) +
        (t_l_pos_f[2] - t_l_pos_n[2]) * (t_l_pos_f[2] - t_l_pos_n[2]));

    // Calculating max distance travelled
    float max_dist = std::max(
        {j_r_dist, j_l_dist, b_r_dist, b_l_dist, t_r_dist, t_l_dist});

    // Calculating min cell size
    float min_cell_size = std::min(grid.cell_size_xy_, grid.cell_size_z_);

    if (max_dist < 0.5 * min_cell_size) {
        // Body has only slightly moved since last update
        return false;
    } else if (max_dist > 2 * min_cell_size) {
        LOG(WARNING) << "WARNING\nMovement made by the body is larger than "
            "two cell size.\nThe validity of the soil update is not ensured.";
    }

    return true;
}

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

std::vector<float> soil_simulator::CalcBucketFramePos(
    int ii, int jj, float z, Grid grid, Body* body
) {
    // Calculating cell's position in body frame
    std::vector<float> cell_pos = {
        grid.vect_x_[ii] - body->pos_[0],
        grid.vect_y_[jj] - body->pos_[1],
        z - body->pos_[2]};

    // Inversing rotation
    std::vector<float> inv_ori = {
        body->ori_[0], -body->ori_[1], -body->ori_[2], -body->ori_[3]};

    // Calculating reference position of cell in body frame
    auto cell_local_pos = soil_simulator::CalcRotationQuaternion(
        inv_ori, cell_pos);

    return cell_local_pos;
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
    SimOut* sim_out, float init_volume, Grid grid, float tol
) {
    // Copying body_soil location
    auto old_body_soil = sim_out->body_soil_;

    // Iterating over all XY positions where body_soil is present
    for (auto nn = 0; nn < sim_out->body_soil_pos_.size(); nn++) {
        int ind = sim_out->body_soil_pos_[nn].ind;
        int ii = sim_out->body_soil_pos_[nn].ii;
        int jj = sim_out->body_soil_pos_[nn].jj;
        float h_soil = sim_out->body_soil_pos_[nn].h_soil;

        // Removing soil from old_body_soil
        old_body_soil[ind+1][ii][jj] -= h_soil;
    }

    // Checking that volume of soil in body_soil_pos_ corresponds to soil
    // in body_soil
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++) {
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++) {
            float dh_1 = std::abs(
                old_body_soil[0][ii][jj] - old_body_soil[1][ii][jj]);
            float dh_2 = std::abs(
                old_body_soil[2][ii][jj] - old_body_soil[3][ii][jj]);
            if ((dh_1 > tol) || (dh_2 > tol)) {
                // Soil in body_soil_pos_ does not correspond to amount of soil
                // in body_soil
                LOG(WARNING) << "WARNING\nVolume of soil in body_soil_pos_ " <<
                    "is not consistent with the amount of soil in body_soil.\n";
                return false;
            }
        }
    }

    // Calculating volume of soil in the terrain
    float terrain_volume = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            terrain_volume += sim_out->terrain_[ii][jj];

    terrain_volume = grid.cell_area_ * terrain_volume;

    // Calculating volume of body soil
    float body_soil_volume = 0.0;
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++) {
            if (
                (sim_out->body_soil_[0][ii][jj] != 0.0) ||
                (sim_out->body_soil_[1][ii][jj] != 0.0)) {
                // Body soil is present
                body_soil_volume += (
                    sim_out->body_soil_[1][ii][jj] -
                    sim_out->body_soil_[0][ii][jj]);
            }
            if (
                (sim_out->body_soil_[2][ii][jj] != 0.0) ||
                (sim_out->body_soil_[3][ii][jj] != 0.0)) {
                // Body soil is present
                body_soil_volume += (
                    sim_out->body_soil_[3][ii][jj] -
                    sim_out->body_soil_[2][ii][jj]);
            }
        }
    body_soil_volume = grid.cell_area_ * body_soil_volume;

    // Calculating total volume of soil
    float total_volume = terrain_volume + body_soil_volume;

    if (std::abs(total_volume - init_volume) > 0.5 * grid.cell_volume_) {
        LOG(WARNING) << "WARNING\nVolume is not conserved!\nInitial volume: " <<
            init_volume << "   Current volume: " << total_volume;
        return false;
    }
    return true;
}

/// The conventions that are checked by this function include:
/// - The terrain should not overlap with the body.
/// - The body should be properly defined, with its maximum height higher than
///   its minimum height.
/// - The body soil should be properly defined, with its maximum height higher
///   than its minimum height.
/// - The two body layers should not overlap or touch each other.
/// - One body layer should not overlap with all body soil layer.
/// - The body should not overlap with the corresponding body soil layer.
/// - The body soil layer should be resting on the corresponding body layer.
/// - The body should be present when there is body soil.
bool soil_simulator::CheckSoil(
    SimOut* sim_out, float tol
) {
    // Iterating over all cells in the body area
    int ii_min = sim_out->body_area_[0][0];
    int ii_max = sim_out->body_area_[0][1];
    int jj_min = sim_out->body_area_[1][0];
    int jj_max = sim_out->body_area_[1][1];
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

            // Checking presence of body and soil
            bool body_presence_0 = ((body_0 != 0.0) || (body_1 != 0.0));
            bool body_presence_2 = ((body_2 != 0.0) || (body_3 != 0.0));
            bool body_soil_presence_0 = (
                (body_soil_0 != 0.0) || (body_soil_1 != 0.0));
            bool body_soil_presence_2 = (
                (body_soil_2 != 0.0) || (body_soil_3 != 0.0));

            if ((body_presence_0) && (terrain > body_0 + tol)) {
                LOG(WARNING) << "WARNING\nTerrain is above the body.\n"
                    "Location: (" << ii << ", " << jj << ")\n"
                    "Terrain height: " << terrain << "\n"
                    "Body min height: " << body_0;
                return false;
            }

            if ((body_presence_2) && (terrain > body_2 + tol)) {
                LOG(WARNING) << "WARNING\nTerrain is above the body.\n"
                    "Location: (" << ii << ", " << jj << ")\n"
                    "Terrain height: " << terrain << "\n"
                    "Body min height: " << body_2;
                return false;
            }

            if ((body_presence_0) && (body_0 > body_1 - tol)) {
                LOG(WARNING) << "WARNING\nMin height of the body is above its"
                    " max height.\nLocation: (" << ii << ", " << jj << ")\n"
                    "Body min height: " << body_0 << "\n"
                    "Body max height: " << body_1;
                return false;
            }

            if ((body_presence_2) && (body_2 > body_3 - tol)) {
                LOG(WARNING) << "WARNING\nMin height of the body is above its"
                    " max height.\nLocation: (" << ii << ", " << jj << ")\n"
                    "Body min height: " << body_2 << "\n"
                    "Body max height: " << body_3;
                return false;
            }

            if (
                (body_presence_0) && (body_presence_2) &&
                (body_1 + tol > body_2) && (body_3 + tol > body_0)) {
                LOG(WARNING) << "WARNING\nTwo body layers are intersecting.\n"
                    "Location: (" << ii << ", " << jj << ")\n"
                    "Body 1 min height: " << body_0 << "\n"
                    "Body 1 max height: " << body_1 << "\n"
                    "Body 2 min height: " << body_2 << "\n"
                    "Body 2 max height: " << body_3;
                return false;
            }

            if (
                (body_presence_0) && (body_soil_presence_2) &&
                (body_1 - tol > body_soil_2) && (body_soil_3 - tol > body_0)) {
                LOG(WARNING) << "WARNING\nA body and body soil layer are "
                    "intersecting.\nLocation: (" << ii << ", " << jj << ")\n"
                    "Body 1 min height: " << body_0 << "\n"
                    "Body 1 max height: " << body_1 << "\n"
                    "Body soil 2 min height: " << body_soil_2 << "\n"
                    "Body soil 2 max height: " << body_soil_3;
                return false;
            }

            if (
                (body_presence_2) && (body_soil_presence_0) &&
                (body_soil_1 - tol > body_2) && (body_3 - tol > body_soil_0)) {
                LOG(WARNING) << "WARNING\nA body and body soil layer are "
                    "intersecting.\nLocation: (" << ii << ", " << jj << ")\n"
                    "Body soil 1 min height: " << body_soil_0 << "\n"
                    "Body soil 1 max height: " << body_soil_1 << "\n"
                    "Body 2 min height: " << body_2 << "\n"
                    "Body 2 max height: " << body_3;
                return false;
            }

            if ((body_soil_presence_0) && (body_soil_0 > body_soil_1 + tol)) {
                LOG(WARNING) << "WARNING\nMin height of the body soil is "
                    "above its max height.\nLocation: (" << ii << ", " << jj <<
                     ")\nBody soil 1 min height: " << body_soil_0 << "\n"
                    "Body soil 1 max height: " << body_soil_1;
                return false;
            }

            if ((body_soil_presence_2) && (body_soil_2 > body_soil_3 + tol)) {
                LOG(WARNING) << "WARNING\nMin height of the body soil is "
                    "above its max height.\nLocation: (" << ii << ", " << jj <<
                    ")\nBody soil 2 min height: " << body_soil_2 << "\n"
                    "Body soil 2 max height: " << body_soil_3;
                return false;
            }

            if ((body_soil_presence_0) && (body_1 > body_soil_0 + tol)) {
                LOG(WARNING) << "WARNING\nBody is above the body soil.\n"
                    "Location: (" << ii << ", " << jj << ")\n"
                    "Body 1 max height: " << body_1 << "\n"
                    "Body soil 1 min height: " << body_soil_0;
                return false;
            }

            if ((body_soil_presence_2) && (body_3 > body_soil_2 + tol)) {
                LOG(WARNING) << "WARNING\nBody is above the body soil.\n"
                    "Location: (" << ii << ", " << jj << ")\n"
                    "Body 2 max height: " << body_3 << "\n"
                    "Body soil 2 min height: " << body_soil_2;
                return false;
            }

            if ((body_soil_presence_0) && !(body_presence_0)) {
                LOG(WARNING) << "WARNING\nBody soil is present but there is "
                    "no body.\nLocation: (" << ii << ", " << jj << ")\n"
                    "Body soil 1 min height: " << body_soil_0 << "\n"
                    "Body soil 1 max height: " << body_soil_1;
                return false;
            }

            if ((body_soil_presence_2) && !(body_presence_2)) {
                LOG(WARNING) << "WARNING\nBody soil is present but there is "
                    "no body.\nLocation: (" << ii << ", " << jj << ")\n"
                    "Body soil 2 min height: " << body_soil_2 << "\n"
                    "Body soil 2 max height: " << body_soil_3;
                return false;
            }

            if ((body_soil_presence_0) && (body_soil_0 != body_1)) {
                LOG(WARNING) << "WARNING\nBody soil is not resting on the "
                    "body.\nLocation: (" << ii << ", " << jj << ")\n"
                    "Body 1 max height: " << body_1 << "\n"
                    "Body soil 1 min height: " << body_soil_0;
                return false;
            }

            if ((body_soil_presence_2) && (body_soil_2 != body_3)) {
                LOG(WARNING) << "WARNING\nBody soil is not resting on the "
                    "body.\nLocation: (" << ii << ", " << jj << ")\n"
                    "Body 2 max height: " << body_3 << "\n"
                    "Body soil 2 min height: " << body_soil_2;
                return false;
            }
        }

    return true;
}

/// `terrain_` and `body_soil_` are saved into files named `terrain` and
/// `body_soil`, respectively, followed by the file number.
void soil_simulator::WriteSoil(
    SimOut* sim_out, Grid grid
) {
    // Finding next filename for the terrain file
    std::source_location location = std::source_location::current();
    std::string filename = location.file_name();
    std::string path = filename.substr(
        0, filename.find_last_of("/")) + "/../results/";
    std::string terrain_filename;

    // Iterating until finding a filename that does not exist
    for (auto ii = 0; ii < 100000; ii++) {
        std::string file_number = std::to_string(ii);
        size_t n = 5;  // Number of digit
        int nn = n - std::min(n, file_number.size());  // Number of leading 0

        // Setting next filename
        terrain_filename = (
            path + "terrain_" + std::string(nn, '0').append(file_number)
            + ".csv");

        // Checking if file exists
        std::ifstream infile(terrain_filename);
        if (!infile.good()) {
            // File does not exist
            break;
        }
    }

    std::ofstream terrain_file;
    terrain_file.open(terrain_filename);
    terrain_file << "x,y,z\n";
    for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
        for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++)
            terrain_file << grid.vect_x_[ii] << "," << grid.vect_y_[jj] << ","
                << sim_out->terrain_[ii][jj] << "\n";
    terrain_file.close();

    // Setting filename for the body soil
    std::string new_body_soil_filename = (
        terrain_filename.substr(0, terrain_filename.find_last_of("/")) +
        "/body_soil_" + terrain_filename.substr(
            terrain_filename.find_last_of("_")+1, terrain_filename.size()));

    std::ofstream body_soil_file;
    body_soil_file.open(new_body_soil_filename);
    body_soil_file << "x,y,z\n";
    if (sim_out->body_soil_pos_.size() == 0) {
        // No soil is resting on the body
        // Writing a dummy position for paraview
        body_soil_file << grid.vect_x_[0] << "," << grid.vect_y_[0] << ","
                << grid.vect_z_[0] << "\n";
    } else {
        for (auto ii = 0; ii < sim_out->terrain_.size(); ii++)
            for (auto jj = 0; jj < sim_out->terrain_[0].size(); jj++) {
                if (
                    (sim_out->body_soil_[0][ii][jj] != 0.0) ||
                    (sim_out->body_soil_[1][ii][jj] != 0.0)) {
                    // Body soil is present
                    body_soil_file << grid.vect_x_[ii] << "," <<
                        grid.vect_y_[jj] << "," <<
                        sim_out->body_soil_[1][ii][jj] << "\n";
                }
                if (
                    (sim_out->body_soil_[2][ii][jj] != 0.0) ||
                    (sim_out->body_soil_[3][ii][jj] != 0.0)) {
                    // Body soil is present
                    body_soil_file << grid.vect_x_[ii] << "," <<
                        grid.vect_y_[jj] << "," <<
                        sim_out->body_soil_[3][ii][jj] << "\n";
                }
            }
    }
}

/// The body corners are saved into a file named `bucket` followed by
/// the file number.
void soil_simulator::WriteBucket(
    Body* body
) {
    // Calculating position of body corners
    auto [j_r_pos, j_l_pos, b_r_pos, b_l_pos, t_r_pos, t_l_pos] =
        soil_simulator::CalcBucketCornerPos(body->pos_, body->ori_, body);

    // Finding next filename for the bucket file
    std::source_location location = std::source_location::current();
    std::string filename = location.file_name();
    std::string path = filename.substr(
        0, filename.find_last_of("/")) + "/../results/";
    std::string bucket_filename;

    // Iterating until finding a filename that does not exist
    for (auto ii = 0; ii < 100000; ii++) {
        std::string file_number = std::to_string(ii);
        size_t n = 5;  // Number of digit
        int nn = n - std::min(n, file_number.size());  // Number of leading 0

        // Setting next filename
        bucket_filename = (
            path + "bucket_" + std::string(nn, '0').append(file_number)
            + ".csv");

        // Checking if file exists
        std::ifstream infile(bucket_filename);
        if (!infile.good()) {
            // File does not exist
            break;
        }
    }

    std::ofstream bucket_file;
    bucket_file.open(bucket_filename);
    bucket_file << "x,y,z\n";
    // Writing bucket right side
    bucket_file << b_r_pos[0] << "," << b_r_pos[1] << ","
                <<  b_r_pos[2] << "\n";
    bucket_file << t_r_pos[0] << "," << t_r_pos[1] << ","
                <<  t_r_pos[2] << "\n";
    bucket_file << j_r_pos[0] << "," << j_r_pos[1] << ","
                <<  j_r_pos[2] << "\n";
    // Writing bucket back
    bucket_file << j_r_pos[0] << "," << j_r_pos[1] << ","
                <<  j_r_pos[2] << "\n";
    bucket_file << j_l_pos[0] << "," << j_l_pos[1] << ","
                <<  j_l_pos[2] << "\n";
    bucket_file << b_l_pos[0] << "," << b_l_pos[1] << ","
                <<  b_l_pos[2] << "\n";
    bucket_file << b_r_pos[0] << "," << b_r_pos[1] << ","
                <<  b_r_pos[2] << "\n";
    // Writing bucket base
    bucket_file << b_r_pos[0] << "," << b_r_pos[1] << ","
                <<  b_r_pos[2] << "\n";
    bucket_file << t_r_pos[0] << "," << t_r_pos[1] << ","
                <<  t_r_pos[2] << "\n";
    bucket_file << t_l_pos[0] << "," << t_l_pos[1] << ","
                <<  t_l_pos[2] << "\n";
    bucket_file << b_l_pos[0] << "," << b_l_pos[1] << ","
                <<  b_l_pos[2] << "\n";
    // Writing bucket left side
    bucket_file << b_l_pos[0] << "," << b_l_pos[1] << ","
                <<  b_l_pos[2] << "\n";
    bucket_file << t_l_pos[0] << "," << t_l_pos[1] << ","
                <<  t_l_pos[2] << "\n";
    bucket_file << j_l_pos[0] << "," << j_l_pos[1] << ","
                <<  j_l_pos[2] << "\n";
    bucket_file.close();
}

/// This function implements 2D Simplex noise. A lot of material can be found
/// online concerning this implementation so that the details would not be
/// explicited here.
float soil_simulator::simplex_noise(
    float x, float y, std::vector<int> perm_table
) {
    float grad_0, grad_1, grad_2;

    // Applying coordinates skewing
    float s = (x + y) * (std::sqrt(3) - 1) / 2;
    int xi = std::floor(x + s);
    int yi = std::floor(y + s);

    // Reversing skewing
    s = (xi + yi) * (3.0 - std::sqrt(3.0)) / 6.0;
    float x0 = x - (xi - s);
    float y0 = y - (yi - s);

    int i1, j1;
    if (x0 > y0) {
        // Setting the lower triangle
        i1 = 1;
        j1 = 0;
    } else {
        // Setting the upper triangle
        i1 = 0;
        j1 = 1;
    }

    // Calculating coordinates of two other corners
    float x1 = x0 - i1 + (3 - std::sqrt(3)) / 6;
    float y1 = y0 - j1 + (3 - std::sqrt(3)) / 6;
    float x2 = x0 - 1.0 + 2.0 * (3 - std::sqrt(3)) / 6;
    float y2 = y0 - 1.0 + 2.0 * (3 - std::sqrt(3)) / 6;

    // Setting directions corners
    std::vector<std::vector<int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    // Calculating the contribution from the first corner
    float t0 = 0.5 - x0 * x0 - y0 * y0;
    if (t0 < 0.0) {
        grad_0 = 0.0;
    } else {
        t0 *= t0;
        int ii = perm_table[xi + perm_table[yi] & 255] & 7;
        grad_0 = t0 * t0 * (x0 * directions[ii][0] + y0 * directions[ii][1]);
    }

    // Calculating the contribution from the second corner
    float t1 = 0.5 - x1 * x1 - y1 * y1;
    if (t1 < 0.0) {
        grad_1 = 0.0;
    } else {
        t1 *= t1;
        int ii = perm_table[xi + i1 + perm_table[yi + j1] & 255] & 7;
        grad_1 = t1 * t1 * (x1 * directions[ii][0] + y1 * directions[ii][1]);
    }

    // Calculating the contribution from the third corner
    float t2 = 0.5 - x2 * x2 - y2 * y2;
    if (t2 < 0.0) {
        grad_2 = 0.0;
    } else {
        t2 *= t2;
        int ii = perm_table[xi + 1 + perm_table[yi + 1] & 255] & 7;
        grad_2 = t2 * t2 * (x2 * directions[ii][0] + y2 * directions[ii][1]);
    }

    return grad_0 + grad_1 + grad_2;
}
