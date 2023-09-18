/*
This file implements unit tests for the functions in utils.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <cmath>
#include "gtest/gtest.h"
#include "soil_simulator/utils.hpp"

TEST(UnitTestUtils, CalcBucketCornerPos) {
    // Setting up the environment
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);

    // -- Testing for a bucket in its reference pose --
    auto pos = std::vector<float> {0.0, 0.0, 0.0};
    auto ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    auto [j_r_pos, j_l_pos, b_r_pos, b_l_pos, t_r_pos, t_l_pos] =
        soil_simulator::CalcBucketCornerPos(pos, ori, bucket);
    EXPECT_TRUE((j_r_pos == std::vector<float> {0.0, -0.25, 0.0}));
    EXPECT_TRUE((j_l_pos == std::vector<float> {0.0, 0.25, 0.0}));
    EXPECT_TRUE((b_r_pos == std::vector<float> {0.0, -0.25, -0.5}));
    EXPECT_TRUE((b_l_pos == std::vector<float> {0.0, 0.25, -0.5}));
    EXPECT_TRUE((t_r_pos == std::vector<float> {0.7, -0.25, -0.5}));
    EXPECT_TRUE((t_l_pos == std::vector<float> {0.7, 0.25, -0.5}));

    // -- Testing for a bucket translated compared to its reference pose --
    pos = std::vector<float> {0.1, -0.1, 0.2};
    ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    std::tie(j_r_pos, j_l_pos, b_r_pos, b_l_pos, t_r_pos, t_l_pos) =
        soil_simulator::CalcBucketCornerPos(pos, ori, bucket);
    EXPECT_TRUE((j_r_pos == std::vector<float> {0.1, -0.35, 0.2}));
    EXPECT_TRUE((j_l_pos == std::vector<float> {0.1, 0.15, 0.2}));
    EXPECT_TRUE((b_r_pos == std::vector<float> {0.1, -0.35, -0.3}));
    EXPECT_TRUE((b_l_pos == std::vector<float> {0.1, 0.15, -0.3}));
    EXPECT_TRUE((t_r_pos == std::vector<float> {0.8, -0.35, -0.3}));
    EXPECT_TRUE((t_l_pos == std::vector<float> {0.8, 0.15, -0.3}));

    // -- Testing for a bucket rotated compared to its reference pose --
    pos = std::vector<float> {0.0, 0.0, 0.0};
    ori = std::vector<float> {0.707107, 0.0, 0.0, -0.707107};
    std::tie(j_r_pos, j_l_pos, b_r_pos, b_l_pos, t_r_pos, t_l_pos) =
        soil_simulator::CalcBucketCornerPos(pos, ori, bucket);
    EXPECT_TRUE((j_r_pos == std::vector<float> {0.25, 0.0, 0.0}));
    EXPECT_TRUE((j_l_pos == std::vector<float> {-0.25, 0.0, 0.0}));
    EXPECT_TRUE((b_r_pos == std::vector<float> {0.25, 0.0, -0.5}));
    EXPECT_TRUE((b_l_pos == std::vector<float> {-0.25, 0.0, -0.5}));
    // Following ones requires a different check due to numerical approximation
    EXPECT_NEAR(t_r_pos[0], 0.25, 1e-5);
    EXPECT_NEAR(t_r_pos[1], 0.7, 1e-5);
    EXPECT_NEAR(t_r_pos[2], -0.5, 1e-5);
    EXPECT_NEAR(t_l_pos[0], -0.25, 1e-5);
    EXPECT_NEAR(t_l_pos[1], 0.7, 1e-5);
    EXPECT_NEAR(t_l_pos[2], -0.5, 1e-5);

    // -- Testing for a bucket rotated and translated compared to its --
    // -- reference pose                                              --
    pos = std::vector<float> {0.1, -0.1, 0.2};
    ori = std::vector<float> {0.707107, 0.0, 0.0, -0.707107};
    std::tie(j_r_pos, j_l_pos, b_r_pos, b_l_pos, t_r_pos, t_l_pos) =
        soil_simulator::CalcBucketCornerPos(pos, ori, bucket);
    EXPECT_TRUE((j_r_pos == std::vector<float> {0.35, -0.1, 0.2}));
    EXPECT_TRUE((j_l_pos == std::vector<float> {-0.15, -0.1, 0.2}));
    EXPECT_TRUE((b_r_pos == std::vector<float> {0.35, -0.1, -0.3}));
    EXPECT_TRUE((b_l_pos == std::vector<float> {-0.15, -0.1, -0.3}));
    EXPECT_TRUE((t_r_pos == std::vector<float> {0.35, 0.6, -0.3}));
    EXPECT_TRUE((t_l_pos == std::vector<float> {-0.15, 0.6, -0.3}));

    delete bucket;
}

TEST(UnitTestUtils, CheckBucketMovement) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};

    // -- Testing for a one cell translation --
    auto pos = std::vector<float> {0.1, 0.0, 0.0};
    auto ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    bool status = soil_simulator::CheckBucketMovement(pos, ori, grid, bucket);
    EXPECT_TRUE(status);

    // -- Testing for an arbitrary translation --
    pos = std::vector<float> {0.05, 0.02, -0.05};
    ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    status = soil_simulator::CheckBucketMovement(pos, ori, grid, bucket);
    EXPECT_TRUE(status);

    // -- Testing for a slight rotation --
    pos = std::vector<float> {0.0, 0.0, 0.0};
    ori = std::vector<float> {0.997, 0.0, 0.07, 0.0};
    status = soil_simulator::CheckBucketMovement(pos, ori, grid, bucket);
    EXPECT_TRUE(status);

    // -- Testing for a slight rotation and translation --
    pos = std::vector<float> {0.05, 0.0, 0.0};
    ori = std::vector<float> {0.997, 0.0, 0.07, 0.0};
    status = soil_simulator::CheckBucketMovement(pos, ori, grid, bucket);
    EXPECT_TRUE(status);

    // -- Testing for a short translation --
    pos = std::vector<float> {0.005, 0.0, 0.0};
    ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    status = soil_simulator::CheckBucketMovement(pos, ori, grid, bucket);
    EXPECT_FALSE(status);

    // -- Testing for a short arbitrary translation --
    pos = std::vector<float> {0.001, 0.002, -0.003};
    ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    status = soil_simulator::CheckBucketMovement(pos, ori, grid, bucket);
    EXPECT_FALSE(status);

    // -- Testing for a very slight rotation --
    pos = std::vector<float> {0.0, 0.0, 0.0};
    ori = std::vector<float> {0.999, 0.0, 0.0029, 0.0};
    status = soil_simulator::CheckBucketMovement(pos, ori, grid, bucket);
    EXPECT_FALSE(status);

    // -- Testing for a very slight rotation and translation --
    pos = std::vector<float> {0.001, 0.0, 0.0};
    ori = std::vector<float> {0.999, 0.0, 0.0029, 0.0};
    status = soil_simulator::CheckBucketMovement(pos, ori, grid, bucket);
    EXPECT_FALSE(status);

    // -- Testing for a three cell translation --
    pos = std::vector<float> {0.005, 0.0, 0.0};
    ori = std::vector<float> {1.0, 0.0, 0.0, 0.0};
    status = soil_simulator::CheckBucketMovement(pos, ori, grid, bucket);
    EXPECT_FALSE(status);

    delete bucket;
}

TEST(UnitTestUtils, CalcNormal) {
    // Setting dummy coordinates forming a triangle in the XY plane
    std::vector<float> a = {0.0, 0.0, 0.0};
    std::vector<float> b = {2.3, 0.0, 0.0};
    std::vector<float> c = {2.3, 2.45, 0.0};

    // -- Testing that the unit normal vector follows the Z direction --
    auto normal = soil_simulator::CalcNormal(a, b, c);
    EXPECT_TRUE((normal == std::vector<float> {0.0, 0.0, 1.0}));

    // -- Testing the opposite direction --
    normal = soil_simulator::CalcNormal(a, c, b);
    EXPECT_TRUE((normal == std::vector<float> {0.0, 0.0, -1.0}));

    // Setting dummy coordinates forming a triangle in the XZ plane
    a = {1.0, 0.0, -1.3};
    b = {0.3, 0.0, 4.2};
    c = {2.3, 0.0, 3.0};

    // -- Testing that the unit normal vector follows the Y direction --
    normal = soil_simulator::CalcNormal(a, b, c);
    EXPECT_TRUE((normal == std::vector<float> {0.0, 1.0, 0.0}));

    // -- Testing the opposite direction --
    normal = soil_simulator::CalcNormal(a, c, b);
    EXPECT_TRUE((normal == std::vector<float> {0.0, -1.0, 0.0}));

    // Setting dummy coordinates forming a triangle in the YZ plane
    a = {0.0, -4.7, 1.3};
    b = {0.0, 7.2, -0.6};
    c = {0.0, -1.0, 54.3};

    // -- Testing that the unit normal vector follows the X direction --
    normal = soil_simulator::CalcNormal(a, b, c);
    EXPECT_TRUE((normal == std::vector<float> {1.0, 0.0, 0.0}));

    // -- Testing the opposite direction --
    normal = soil_simulator::CalcNormal(a, c, b);
    EXPECT_TRUE((normal == std::vector<float> {-1.0, 0.0, 0.0}));

    // Setting dummy coordinates following a 45 degrees inclined plane
    a = {1.0, 0.0, 0.0};
    b = {0.0, 1.0, 0.0};
    c = {0.0, 0.0, 1.0};

    // -- Testing that the unit normal vector follows the inclined plane --
    float cc = std::sqrt(1.0 / 3.0);
    normal = soil_simulator::CalcNormal(a, b, c);
    EXPECT_TRUE((normal == std::vector<float> {cc, cc, cc}));

    // -- Testing the opposite direction --
    normal = soil_simulator::CalcNormal(a, c, b);
    EXPECT_TRUE((normal == std::vector<float> {-cc, -cc, -cc}));
}

TEST(UnitTestUtils, MultiplyQuaternion) {
    // -- Testing multiplying two half rotation around the Z axis --
    std::vector<float> q1 = {0.707107, 0.0, 0.0, -0.707107};
    std::vector<float> quat = soil_simulator::MultiplyQuaternion(q1, q1);
    EXPECT_NEAR(quat[0], 0.0, 1e-5);
    EXPECT_NEAR(quat[1], 0.0, 1e-5);
    EXPECT_NEAR(quat[2], 0.0, 1e-5);
    EXPECT_NEAR(quat[3], -1.0, 1e-5);

    // -- Testing multiplying two half rotation around the Y axis --
    q1 = {0.707107, 0.0, -0.707107, 0.0};
    quat = soil_simulator::MultiplyQuaternion(q1, q1);
    EXPECT_NEAR(quat[0], 0.0, 1e-5);
    EXPECT_NEAR(quat[1], 0.0, 1e-5);
    EXPECT_NEAR(quat[2], -1.0, 1e-5);
    EXPECT_NEAR(quat[3], 0.0, 1e-5);

    // -- Testing multiplying two half rotation around the X axis --
    q1 = {0.707107, -0.707107, 0.0, 0.0};
    quat = soil_simulator::MultiplyQuaternion(q1, q1);
    EXPECT_NEAR(quat[0], 0.0, 1e-5);
    EXPECT_NEAR(quat[1], -1.0, 1e-5);
    EXPECT_NEAR(quat[2], 0.0, 1e-5);
    EXPECT_NEAR(quat[3], 0.0, 1e-5);

    // -- Testing multiplying two arbitrary quaternions --
    // Results checked against ReferenceFrameRotations library in Julia
    q1 = {0.8, -0.4, 0.2, 0.7};
    std::vector<float> q2 = {0.2, 0.5, -0.7, -0.8};
    quat = soil_simulator::MultiplyQuaternion(q1, q2);
    EXPECT_NEAR(quat[0], 1.06, 1e-5);
    EXPECT_NEAR(quat[1], 0.65, 1e-5);
    EXPECT_NEAR(quat[2], -0.49, 1e-5);
    EXPECT_NEAR(quat[3], -0.32, 1e-5);
}

TEST(UnitTestUtils, CalcRotationQuaternion) {
    // -- Testing applying a pi/2 rotation around the Z axis --
    std::vector<float> ori = {0.707107, 0.0, 0.0, -0.707107};
    std::vector<float> pos = {0.1, 0.1, 0.3};
    auto new_pos = soil_simulator::CalcRotationQuaternion(ori, pos);
    EXPECT_NEAR(new_pos[0], -0.1, 1e-5);
    EXPECT_NEAR(new_pos[1], 0.1, 1e-5);
    EXPECT_NEAR(new_pos[2], 0.3, 1e-5);

    // -- Testing applying a pi/2 rotation around the Y axis --
    ori = {0.707107, 0.0, -0.707107, 0.0};
    pos = {0.1, 0.5, 0.0};
    new_pos = soil_simulator::CalcRotationQuaternion(ori, pos);
    EXPECT_NEAR(new_pos[0], 0.0, 1e-5);
    EXPECT_NEAR(new_pos[1], 0.5, 1e-5);
    EXPECT_NEAR(new_pos[2], -0.1, 1e-5);

    // -- Testing applying a pi/2 rotation around the X axis --
    ori = {0.707107, -0.707107, 0.0, 0.0};
    pos = {-0.1, 0.3, -0.5};
    new_pos = soil_simulator::CalcRotationQuaternion(ori, pos);
    EXPECT_NEAR(new_pos[0], -0.1, 1e-5);
    EXPECT_NEAR(new_pos[1], 0.5, 1e-5);
    EXPECT_NEAR(new_pos[2], 0.3, 1e-5);

    // -- Testing applying an arbitrary rotation --
    // Results checked against ReferenceFrameRotations library in Julia
    ori = {0.53, -0.21, 0.64, -0.33};
    pos = {-0.15, 0.67, -0.12};
    new_pos = soil_simulator::CalcRotationQuaternion(ori, pos);
    EXPECT_NEAR(new_pos[0], -0.380155, 1e-5);
    EXPECT_NEAR(new_pos[1], 0.504297, 1e-5);
    EXPECT_NEAR(new_pos[2], -0.29490, 1e-5);
}

TEST(UnitTestUtils, AngleToQuat) {
    // -- Testing applying a pi/2 rotation around the Z axis --
    std::vector<float> ori = {-1.570796327, 0.0, 0.0};
    auto quat = soil_simulator::AngleToQuat(ori);
    EXPECT_NEAR(quat[0], 0.707107, 1e-5);
    EXPECT_NEAR(quat[1], 0.0, 1e-5);
    EXPECT_NEAR(quat[2], 0.0, 1e-5);
    EXPECT_NEAR(quat[3], -0.707107, 1e-5);

    // -- Testing applying a pi/2 rotation around the Y axis --
    ori = {0.0, -1.570796327, 0.0};
    quat = soil_simulator::AngleToQuat(ori);
    EXPECT_NEAR(quat[0], 0.707107, 1e-5);
    EXPECT_NEAR(quat[1], 0.0, 1e-5);
    EXPECT_NEAR(quat[2], -0.707107, 1e-5);
    EXPECT_NEAR(quat[3], 0.0, 1e-5);

    // -- Testing applying a pi/2 rotation around the X axis --
    ori = {0.0, 0.0, -1.570796327};
    quat = soil_simulator::AngleToQuat(ori);
    EXPECT_NEAR(quat[0], 0.707107, 1e-5);
    EXPECT_NEAR(quat[1], -0.707107, 1e-5);
    EXPECT_NEAR(quat[2], 0.0, 1e-5);
    EXPECT_NEAR(quat[3], 0.0, 1e-5);

    // -- Testing applying an arbitrary rotation --
    // Results checked against ReferenceFrameRotations library in Julia
    ori = {0.53, 1.2, -0.3};
    quat = soil_simulator::AngleToQuat(ori);
    EXPECT_NEAR(quat[0], 0.765481, 1e-5);
    EXPECT_NEAR(quat[1], -0.265256, 1e-5);
    EXPECT_NEAR(quat[2], 0.50651, 1e-5);
    EXPECT_NEAR(quat[3], 0.295169, 1e-5);
}

TEST(UnitTestUtils, CalcBucketFramePos) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};

    // -- Testing for simple translation (1) --
    auto pos = soil_simulator::CalcBucketFramePos(
        11, 11, 0.2, grid, bucket);
    EXPECT_NEAR(pos[0], 0.1, 1e-5);
    EXPECT_NEAR(pos[1], 0.1, 1e-5);
    EXPECT_NEAR(pos[2], 0.2, 1e-5);

    // -- Testing for simple translation (2) --
    pos = soil_simulator::CalcBucketFramePos(
        10, 12, -0.2, grid, bucket);
    EXPECT_NEAR(pos[0], 0.0, 1e-5);
    EXPECT_NEAR(pos[1], 0.2, 1e-5);
    EXPECT_NEAR(pos[2], -0.2, 1e-5);

    // -- Testing for simple translation (3) --
    bucket->pos_ = std::vector<float> {-0.1, 0.2, 0.3};
    pos = soil_simulator::CalcBucketFramePos(
        10, 12, -0.2, grid, bucket);
    EXPECT_NEAR(pos[0], 0.1, 1e-5);
    EXPECT_NEAR(pos[1], 0.0, 1e-5);
    EXPECT_NEAR(pos[2], -0.5, 1e-5);
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};

    // -- Testing for rotation by pi/2 around the Z axis --
    bucket->ori_ = std::vector<float> {0.707107, 0.0, 0.0, -0.707107};
    pos = soil_simulator::CalcBucketFramePos(
        11, 12, 0.3, grid, bucket);
    EXPECT_NEAR(pos[0], 0.2, 1e-5);
    EXPECT_NEAR(pos[1], -0.1, 1e-5);
    EXPECT_NEAR(pos[2], 0.3, 1e-5);
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};

    // -- Testing for rotation by pi/2 around the Y axis --
    bucket->ori_ = std::vector<float> {0.707107, 0.0, -0.707107, 0.0};
    pos = soil_simulator::CalcBucketFramePos(
        11, 12, 0.3, grid, bucket);
    EXPECT_NEAR(pos[0], -0.3, 1e-5);
    EXPECT_NEAR(pos[1], 0.2, 1e-5);
    EXPECT_NEAR(pos[2], 0.1, 1e-5);
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};

    // -- Testing for rotation by pi/2 around the X axis --
    bucket->ori_ = std::vector<float> {0.707107, 0.707107, 0.0, 0.0};
    pos = soil_simulator::CalcBucketFramePos(
        11, 12, 0.3, grid, bucket);
    EXPECT_NEAR(pos[0], 0.1, 1e-5);
    EXPECT_NEAR(pos[1], -0.3, 1e-5);
    EXPECT_NEAR(pos[2], 0.2, 1e-5);
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};

    // -- Testing for rotation + translation  --
    bucket->pos_ = std::vector<float> {-0.1, 0.2, 0.3};
    bucket->ori_ = std::vector<float> {0.707107, 0.0, 0.0, -0.707107};
    pos = soil_simulator::CalcBucketFramePos(
        10, 12, -0.2, grid, bucket);
    EXPECT_NEAR(pos[0], 0.0, 1e-5);
    EXPECT_NEAR(pos[1], -0.1, 1e-5);
    EXPECT_NEAR(pos[2], -0.5, 1e-5);
    bucket->pos_ = std::vector<float> {0.0, 0.0, 0.0};
    bucket->ori_ = std::vector<float> {1.0, 0.0, 0.0, 0.0};

    delete bucket;
}

TEST(UnitTestUtils, CheckVolume) {
    // Setting dummy classes
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    // -- Testing that no warning is sent for correct initial volume --
    EXPECT_TRUE(soil_simulator::CheckVolume(sim_out, 0.0, grid, 1e-5));

    // -- Testing that warning is sent for incorrect initial volume --
    EXPECT_FALSE(soil_simulator::CheckVolume(sim_out, 1.0, grid, 1e-5));
    EXPECT_FALSE(soil_simulator::CheckVolume(
        sim_out, -0.6 * grid.cell_volume_, grid, 1e-5));
    EXPECT_FALSE(soil_simulator::CheckVolume(
        sim_out, 0.6 * grid.cell_volume_, grid, 1e-5));

    // Setting non-zero terrain
    sim_out->terrain_[1][2] = 0.2;
    float init_volume =  0.2 * grid.cell_area_;

    // -- Testing that no warning is sent for correct initial volume --
    EXPECT_TRUE(soil_simulator::CheckVolume(sim_out, init_volume, grid, 1e-5));

    // -- Testing that warning is sent for incorrect initial volume --
    EXPECT_FALSE(soil_simulator::CheckVolume(sim_out, 0.0, grid, 1e-5));
    EXPECT_FALSE(soil_simulator::CheckVolume(
        sim_out, init_volume - 0.6 * grid.cell_volume_, grid, 1e-5));
    EXPECT_FALSE(soil_simulator::CheckVolume(
        sim_out, init_volume + 0.6 * grid.cell_volume_, grid, 1e-5));

    // Setting non-zero body soil
    sim_out->terrain_[1][2] = 0.0;
    sim_out->body_soil_[0][2][2] = -0.1;
    sim_out->body_soil_[1][2][2] = 0.0;
    sim_out->body_soil_[2][2][2] = 0.2;
    sim_out->body_soil_[3][2][2] = 0.27;
    sim_out->body_soil_[0][1][1] = 0.0;
    sim_out->body_soil_[1][1][1] = 0.08;
    sim_out->body_soil_[2][2][1] = 0.0;
    sim_out->body_soil_[3][2][1] = 0.15;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 2, 2, 0., 0., 0., 0.1});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 2, 2, 0., 0., 0., 0.07});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 1, 1, 0., 0., 0., 0.08});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 2, 1, 0., 0., 0., 0.15});
    init_volume =  0.4 * grid.cell_area_;

    // -- Testing that no warning is sent for correct initial volume --
    EXPECT_TRUE(soil_simulator::CheckVolume(sim_out, init_volume, grid, 1e-5));

    // -- Testing that warning is sent for incorrect initial volume --
    EXPECT_FALSE(soil_simulator::CheckVolume(sim_out, 0.0, grid, 1e-5));

    delete sim_out;
}

TEST(UnitTestUtils, CheckSoil) {
    // Setting dummy classes
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    // -- Testing that no warning is sent when everything is at zero --
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Changing terrain to an arbitrary shape
    sim_out->terrain_[1][1] = -0.2;
    sim_out->terrain_[1][2] = -0.15;
    sim_out->terrain_[2][1] = 0.0;
    sim_out->terrain_[2][2] = 0.0;

    // -- Testing that no warning is sent --
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Setting the bucket
    sim_out->body_[0][1][1] = -0.2;
    sim_out->body_[1][1][1] = 0.0;
    sim_out->body_[0][1][2] = -0.15;
    sim_out->body_[1][1][2] = 0.0;
    sim_out->body_[2][1][2] = 0.1;
    sim_out->body_[3][1][2] = 0.2;
    sim_out->body_[2][2][1] = 0.0;
    sim_out->body_[3][2][1] = 0.15;
    sim_out->body_[0][2][2] = 0.1;
    sim_out->body_[1][2][2] = 0.1;

    // -- Testing that no warning is sent --
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Setting the bucket soil
    sim_out->body_soil_[0][1][1] = 0.0;
    sim_out->body_soil_[1][1][1] = 0.1;
    sim_out->body_soil_[0][1][2] = 0.0;
    sim_out->body_soil_[1][1][2] = 0.1;
    sim_out->body_soil_[2][1][2] = 0.2;
    sim_out->body_soil_[3][1][2] = 0.3;
    sim_out->body_soil_[2][2][1] = 0.15;
    sim_out->body_soil_[3][2][1] = 0.25;
    sim_out->body_soil_[0][2][2] = 0.1;
    sim_out->body_soil_[1][2][2] = 0.1;
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 1, 1, 0.0, 0.0, 0.0, 0.0});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 1, 2, 0.0, 0.0, 0.0, 0.0});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 1, 2, 0.0, 0.0, 0.0, 0.0});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {2, 2, 1, 0.0, 0.0, 0.0, 0.0});
    sim_out->body_soil_pos_.push_back(
        soil_simulator::body_soil {0, 2, 2, 0.0, 0.0, 0.0, 0.0});

    // -- Testing that no warning is sent --
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // -- Testing that warning is sent when terrain is above the bucket --
    sim_out->terrain_[1][1] = 0.5;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->terrain_[1][1] = -0.2;
    sim_out->terrain_[2][1] = 0.05;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->terrain_[2][1] = 0.0;

    // -- Testing that no warning is sent --
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // -- Testing that warning is sent when body is not set properly --
    sim_out->body_[0][1][1] = 0.0;
    sim_out->body_[1][1][1] = -0.1;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_soil_[0][1][1] = 0.0;
    sim_out->body_soil_[1][1][1] = 0.0;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[0][1][1] = 0.41;
    sim_out->body_[1][1][1] = 0.4;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[1][1][1] = 0.0;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[0][1][1] = 0.0;
    sim_out->body_[1][1][1] = -0.4;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[0][1][1] = -0.2;
    sim_out->body_[1][1][1] = 0.0;
    sim_out->body_soil_[0][1][1] = 0.0;
    sim_out->body_soil_[1][1][1] = 0.1;
    sim_out->body_[2][2][1] = 0.16;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[2][2][1] = 0.0;

    // -- Testing that no warning is sent --
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // -- Testing that warning is sent when bucket soil is not set properly --
    sim_out->body_soil_[0][1][1] = 0.0;
    sim_out->body_soil_[1][1][1] = -0.1;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_soil_[0][1][1] = 0.2;
    sim_out->body_soil_[1][1][1] = 0.0;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_soil_[0][1][1] = 0.0;
    sim_out->body_soil_[1][1][1] = 0.1;
    sim_out->body_soil_[2][2][1] = 0.15;
    sim_out->body_soil_[3][2][1] = 0.14;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_soil_[2][2][1] = 0.15;
    sim_out->body_soil_[3][2][1] = 0.25;

    // -- Testing that no warning is sent --
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // -- Testing that warning is sent when bucket is above bucket soil --
    sim_out->body_[1][1][1] = 0.05;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[1][1][1] = 0.0;
    sim_out->body_[3][1][2] = 0.25;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[3][1][2] = 0.45;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[3][1][2] = 0.2;

    // -- Testing that no warning is sent --
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // -- Testing that warning is sent when there is a gap between bucket --
    // -- and bucket soil                                                 --
    sim_out->body_soil_[0][1][1] = 0.1;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_soil_[0][1][1] = 0.05;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_soil_[0][1][1] = 0.0;
    sim_out->body_soil_[2][2][1] = 0.20;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_soil_[2][2][1] = 0.15;

    // -- Testing that no warning is sent --
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // -- Testing that warning is sent when there is bucket soil but --
    // -- no bucket                                                  --
    sim_out->body_[2][1][2] = 0.0;
    sim_out->body_[3][1][2] = 0.0;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[2][1][2] = 0.1;
    sim_out->body_[3][1][2] = 0.2;
    sim_out->body_[0][1][1] = 0.0;
    sim_out->body_[1][1][1] = 0.0;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[0][1][1] = -0.2;
    sim_out->body_[1][1][1] = 0.0;

    // -- Testing that no warning is sent --
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // -- Testing that warning is sent when two bucket layers are --
    // -- intersecting                                            --
    sim_out->terrain_[3][2] = -0.2;
    sim_out->body_[0][3][2] = -0.15;
    sim_out->body_[1][3][2] = 0.1;
    sim_out->body_[2][3][2] = 0.0;
    sim_out->body_[3][3][2] = 0.2;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[1][3][2] = 0.0;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[0][3][2] = 0.0;
    sim_out->body_[1][3][2] = 0.2;
    sim_out->body_[2][3][2] = -0.2;
    sim_out->body_[3][3][2] = 0.1;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[3][3][2] = 0.0;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[0][3][2] = 0.0;
    sim_out->body_[1][3][2] = 0.0;
    sim_out->body_[2][3][2] = 0.0;
    sim_out->body_[3][3][2] = 0.0;

    // -- Testing that no warning is sent --
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // -- Testing that warning is sent when a bucket layer and a bucket --
    // -- soil layer are intersecting                                   --
    sim_out->body_[0][3][2] = -0.15;
    sim_out->body_[1][3][2] = 0.0;
    sim_out->body_[2][3][2] = 0.1;
    sim_out->body_[3][3][2] = 0.2;
    sim_out->body_soil_[0][3][2] = 0.0;
    sim_out->body_soil_[1][3][2] = 0.15;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_soil_[0][3][2] = 0.0;
    sim_out->body_soil_[1][3][2] = 0.0;
    sim_out->body_[2][3][2] = -0.15;
    sim_out->body_[3][3][2] = 0.0;
    sim_out->body_[0][3][2] = 0.1;
    sim_out->body_[1][3][2] = 0.2;
    sim_out->body_soil_[2][3][2] = 0.0;
    sim_out->body_soil_[3][3][2] = 0.15;
    EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_soil_[3][3][2] = 0.1;
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));
    sim_out->body_[0][3][2] = 0.0;
    sim_out->body_[1][3][2] = 0.0;
    sim_out->body_[2][3][2] = 0.0;
    sim_out->body_[3][3][2] = 0.0;
    sim_out->body_soil_[2][3][2] = 0.0;
    sim_out->body_soil_[3][3][2] = 0.0;

    // -- Testing that no warning is sent --
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    delete sim_out;
}
