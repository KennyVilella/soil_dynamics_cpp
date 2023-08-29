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

    if (std::abs(total_volume - init_volume) > 0.5 * grid.cell_volume_) {
        LOG(WARNING) << "WARNING\nVolume is not conserved!\nInitial volume: " <<
            init_volume << "   Current volume: " << total_volume;
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
                LOG(WARNING) << "WARNING\nTerrain is above the bucket\n"
                    "Location: (" << ii << ", " << jj << ")\n"
                    "Terrain height: " << terrain << "\n"
                    "Bucket min height: " << body_0;
                return false;
            }

            if ((bucket_presence_2) && (terrain > body_2 + tol)) {
                LOG(WARNING) << "WARNING\nTerrain is above the bucket\n"
                    "Location: (" << ii << ", " << jj << ")\n"
                    "Terrain height: " << terrain << "\n"
                    "Bucket min height: " << body_2;
                return false;
            }

            if ((bucket_presence_0) && (body_0 > body_1 + tol)) {
                LOG(WARNING) << "WARNING\nMin height of the bucket is above its"
                    " max height\nLocation: (" << ii << ", " << jj << ")\n"
                    "Bucket min height: " << body_0 << "\n"
                    "Bucket max height: " << body_1;
                return false;
            }

            if ((bucket_presence_2) && (body_2 > body_3 + tol)) {
                LOG(WARNING) << "WARNING\nMin height of the bucket is above its"
                    " max height\nLocation: (" << ii << ", " << jj << ")\n"
                    "Bucket min height: " << body_2 << "\n"
                    "Bucket max height: " << body_3;
                return false;
            }

            if (
                (bucket_presence_0) && (bucket_presence_2) &&
                (body_1 + tol > body_2) && (body_3 + tol > body_0)) {
                LOG(WARNING) << "WARNING\nTwo bucket layers are intersecting\n"
                    "Location: (" << ii << ", " << jj << ")\n"
                    "Bucket 1 min height: " << body_0 << "\n"
                    "Bucket 1 max height: " << body_1 << "\n"
                    "Bucket 2 min height: " << body_2 << "\n"
                    "Bucket 2 max height: " << body_3;
                return false;
            }

            if (
                (bucket_presence_0) && (bucket_soil_presence_2) &&
                (body_1 - tol > body_soil_2) && (body_soil_3 - tol > body_0)) {
                LOG(WARNING) << "WARNING\nA bucket and bucket soil layer are"
                    "intersecting\nLocation: (" << ii << ", " << jj << ")\n"
                    "Bucket 1 min height: " << body_0 << "\n"
                    "Bucket 1 max height: " << body_1 << "\n"
                    "Bucket soil 2 min height: " << body_soil_2 << "\n"
                    "Bucket soil 2 max height: " << body_soil_3;
                return false;
            }

            if (
                (bucket_presence_2) && (bucket_soil_presence_0) &&
                (body_soil_1 - tol > body_2) && (body_3 - tol > body_soil_0)) {
                LOG(WARNING) << "WARNING\nA bucket and bucket soil layer are"
                    "intsecting\nLocation: (" << ii << ", " << jj << ")\n"
                    "Bucket soil 1 min height: " << body_soil_0 << "\n"
                    "Bucket soil 1 max height: " << body_soil_1 << "\n"
                    "Bucket 2 min height: " << body_2 << "\n"
                    "Bucket 2 max height: " << body_3;
                return false;
            }

            if ((bucket_soil_presence_0) && (body_soil_0 > body_soil_1 + tol)) {
                LOG(WARNING) << "WARNING\nMin height of the bucket soil is "
                    "above its max height\nLocation: (" << ii << ", " << jj <<
                     ")\nBucket soil 1 min height: " << body_soil_0 << "\n"
                    "Bucket soil 1 max height: " << body_soil_1;
                return false;
            }

            if ((bucket_soil_presence_2) && (body_soil_2 > body_soil_3 + tol)) {
                LOG(WARNING) << "WARNING\nMin height of the bucket soil is "
                    "above its max height\nLocation: (" << ii << ", " << jj <<
                    ")\nBucket soil 2 min height: " << body_soil_2 << "\n"
                    "Bucket soil 2 max height: " << body_soil_3;
                return false;
            }

            if ((bucket_soil_presence_0) && (body_1 > body_soil_0 + tol)) {
                LOG(WARNING) << "WARNING\nBucket is above the bucket soil\n"
                    "Location: (" << ii << ", " << jj << ")\n"
                    "Bucket 1 max height: " << body_1 << "\n"
                    "Bucket soil 1 min height: " << body_soil_0;
                return false;
            }

            if ((bucket_soil_presence_2) && (body_3 > body_soil_2 + tol)) {
                LOG(WARNING) << "WARNING\nBucket is above the bucket soil\n"
                    "Location: (" << ii << ", " << jj << ")\n"
                    "Bucket 2 max height: " << body_3 << "\n"
                    "Bucket soil 2 min height: " << body_soil_2;
                return false;
            }

            if ((bucket_soil_presence_0) && (body_soil_0 != body_1)) {
                LOG(WARNING) << "WARNING\nBucket soil is not resting on the "
                    "bucket\nLocation: (" << ii << ", " << jj << ")\n"
                    "Bucket 1 max height: " << body_1 << "\n"
                    "Bucket soil 1 min height: " << body_soil_0;
                return false;
            }

            if ((bucket_soil_presence_2) && (body_soil_2 != body_3)) {
                LOG(WARNING) << "WARNING\nBucket soil is not resting on the "
                    "bucket\nLocation: (" << ii << ", " << jj << ")\n"
                    "Bucket 2 max height: " << body_3 << "\n"
                    "Bucket soil 2 min height: " << body_soil_2;
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
            LOG(WARNING) << "WARNING\nBucket soil is present but there is no "
                "bucket\nLocation: (" << ii << ", " << jj << ")\n"
                "Bucket soil min height: " << body_soil_min << "\n"
                "Bucket soil max height: " << body_soil_max;
           return false;
       }
    }

    return true;
}

/// `terrain` and `body_soil` are saved into files named `terrain` and
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

    // Setting filename for the bucket soil
    std::string new_body_soil_filename = (
        terrain_filename.substr(0, terrain_filename.find_last_of("/")) +
        "/body_soil_" + terrain_filename.substr(
            terrain_filename.find_last_of("_")+1, terrain_filename.size()));

    // Removing duplicates in body_soil_pos
    sort(sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end());
    sim_out->body_soil_pos_.erase(unique(
        sim_out->body_soil_pos_.begin(), sim_out->body_soil_pos_.end()),
        sim_out->body_soil_pos_.end());

    std::ofstream body_soil_file;
    body_soil_file.open(new_body_soil_filename);
    body_soil_file << "x,y,z\n";
    if (sim_out->body_soil_pos_.size() == 0) {
        // No soil is resting on the bucket
        // Writing a dummy position for paraview
        body_soil_file << grid.vect_x_[0] << "," << grid.vect_y_[0] << ","
                << grid.vect_z_[0] << "\n";
    } else {
        for (auto nn = 0; nn < sim_out->body_soil_pos_.size(); nn++) {
            int ii = sim_out->body_soil_pos_[nn][1];
            int jj = sim_out->body_soil_pos_[nn][2];
            int ind = sim_out->body_soil_pos_[nn][0];

            body_soil_file << grid.vect_x_[ii] << "," << grid.vect_y_[jj] << ","
                << sim_out->body_soil_[ind+1][ii][jj] << "\n";
        }
    }
}

/// The bucket corners are saved into a file named `bucket` followed by
/// the file number.
void soil_simulator::WriteBucket(
    Bucket* bucket
) {
    // Calculating position of the bucker points
    auto j_pos = soil_simulator::CalcRotationQuaternion(
        bucket->ori_, bucket->j_pos_init_);
    auto b_pos = soil_simulator::CalcRotationQuaternion(
        bucket->ori_, bucket->b_pos_init_);
    auto t_pos = soil_simulator::CalcRotationQuaternion(
        bucket->ori_, bucket->t_pos_init_);

    // Unit vector normal to the side of the bucket
    auto normal_side = soil_simulator::CalcNormal(j_pos, b_pos, t_pos);

    // Declaring vectors for each vertex of the bucket
    std::vector<float> j_r_pos(3);
    std::vector<float> j_l_pos(3);
    std::vector<float> b_r_pos(3);
    std::vector<float> b_l_pos(3);
    std::vector<float> t_r_pos(3);
    std::vector<float> t_l_pos(3);

    for (auto ii = 0; ii < 3; ii++) {
        // Adding position of the bucket origin
        j_pos[ii] += bucket->pos_[ii];
        b_pos[ii] += bucket->pos_[ii];
        t_pos[ii] += bucket->pos_[ii];

        // Position of each vertex of the bucket
        j_r_pos[ii] = j_pos[ii] + 0.5 * bucket->width_ * normal_side[ii];
        j_l_pos[ii] = j_pos[ii] - 0.5 * bucket->width_ * normal_side[ii];
        b_r_pos[ii] = b_pos[ii] + 0.5 * bucket->width_ * normal_side[ii];
        b_l_pos[ii] = b_pos[ii] - 0.5 * bucket->width_ * normal_side[ii];
        t_r_pos[ii] = t_pos[ii] + 0.5 * bucket->width_ * normal_side[ii];
        t_l_pos[ii] = t_pos[ii] - 0.5 * bucket->width_ * normal_side[ii];
    }

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