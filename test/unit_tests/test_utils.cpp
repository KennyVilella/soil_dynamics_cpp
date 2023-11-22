/*
This file implements unit tests for the functions in utils.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include <cmath>
#include <string>
#include "gtest/gtest.h"
#include "soil_simulator/utils.hpp"
#include "test/unit_tests/utility.hpp"

// To make the function call holds in a single line.
// It greatly improves readability.
using test_soil_simulator::SetHeight;
using test_soil_simulator::PushBodySoilPos;

TEST(UnitTestUtils, CalcBodyCornerPos) {
    // Setting up the environment
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);

    // Declaring variables
    std::vector<float> pos;
    std::vector<float> ori;
    std::vector<float> j_r_pos;
    std::vector<float> j_l_pos;
    std::vector<float> b_r_pos;
    std::vector<float> b_l_pos;
    std::vector<float> t_r_pos;
    std::vector<float> t_l_pos;

    // Test: UT-CBC-1
    pos = {0.0, 0.0, 0.0};
    ori = {1.0, 0.0, 0.0, 0.0};
    std::tie(j_r_pos, j_l_pos, b_r_pos, b_l_pos, t_r_pos, t_l_pos) =
        soil_simulator::CalcBodyCornerPos(pos, ori, bucket);
    EXPECT_TRUE((j_r_pos == std::vector<float> {0.0, -0.25, 0.0}));
    EXPECT_TRUE((j_l_pos == std::vector<float> {0.0, 0.25, 0.0}));
    EXPECT_TRUE((b_r_pos == std::vector<float> {0.0, -0.25, -0.5}));
    EXPECT_TRUE((b_l_pos == std::vector<float> {0.0, 0.25, -0.5}));
    EXPECT_TRUE((t_r_pos == std::vector<float> {0.7, -0.25, -0.5}));
    EXPECT_TRUE((t_l_pos == std::vector<float> {0.7, 0.25, -0.5}));

    // Test: UT-CBC-2
    pos = {0.1, -0.1, 0.2};
    ori = {1.0, 0.0, 0.0, 0.0};
    std::tie(j_r_pos, j_l_pos, b_r_pos, b_l_pos, t_r_pos, t_l_pos) =
        soil_simulator::CalcBodyCornerPos(pos, ori, bucket);
    EXPECT_TRUE((j_r_pos == std::vector<float> {0.1, -0.35, 0.2}));
    EXPECT_TRUE((j_l_pos == std::vector<float> {0.1, 0.15, 0.2}));
    EXPECT_TRUE((b_r_pos == std::vector<float> {0.1, -0.35, -0.3}));
    EXPECT_TRUE((b_l_pos == std::vector<float> {0.1, 0.15, -0.3}));
    EXPECT_TRUE((t_r_pos == std::vector<float> {0.8, -0.35, -0.3}));
    EXPECT_TRUE((t_l_pos == std::vector<float> {0.8, 0.15, -0.3}));

    // Test: UT-CBC-3
    pos = {0.0, 0.0, 0.0};
    ori = {0.707107, 0.0, 0.0, -0.707107};
    std::tie(j_r_pos, j_l_pos, b_r_pos, b_l_pos, t_r_pos, t_l_pos) =
        soil_simulator::CalcBodyCornerPos(pos, ori, bucket);
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

    // Test: UT-CBC-4
    pos = {0.1, -0.1, 0.2};
    ori = {0.707107, 0.0, 0.0, -0.707107};
    std::tie(j_r_pos, j_l_pos, b_r_pos, b_l_pos, t_r_pos, t_l_pos) =
        soil_simulator::CalcBodyCornerPos(pos, ori, bucket);
    EXPECT_TRUE((j_r_pos == std::vector<float> {0.35, -0.1, 0.2}));
    EXPECT_TRUE((j_l_pos == std::vector<float> {-0.15, -0.1, 0.2}));
    EXPECT_TRUE((b_r_pos == std::vector<float> {0.35, -0.1, -0.3}));
    EXPECT_TRUE((b_l_pos == std::vector<float> {-0.15, -0.1, -0.3}));
    EXPECT_TRUE((t_r_pos == std::vector<float> {0.35, 0.6, -0.3}));
    EXPECT_TRUE((t_l_pos == std::vector<float> {-0.15, 0.6, -0.3}));

    delete bucket;
}

TEST(UnitTestUtils, CheckBodyMovement) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};

    // Declaring variables
    std::vector<float> pos;
    std::vector<float> ori;
    bool status;

    // Test: UT-CBM-1
    pos = {0.1, 0.0, 0.0};
    ori = {1.0, 0.0, 0.0, 0.0};
    status = soil_simulator::CheckBodyMovement(pos, ori, grid, bucket);
    EXPECT_TRUE(status);

    // Test: UT-CBM-2
    pos = {0.05, 0.02, -0.05};
    ori = {1.0, 0.0, 0.0, 0.0};
    status = soil_simulator::CheckBodyMovement(pos, ori, grid, bucket);
    EXPECT_TRUE(status);

    // Test: UT-CBM-3
    pos = {0.0, 0.0, 0.0};
    ori = {0.997, 0.0, 0.07, 0.0};
    status = soil_simulator::CheckBodyMovement(pos, ori, grid, bucket);
    EXPECT_TRUE(status);

    // Test: UT-CBM-4
    pos = {0.05, 0.0, 0.0};
    ori = {0.997, 0.0, 0.07, 0.0};
    status = soil_simulator::CheckBodyMovement(pos, ori, grid, bucket);
    EXPECT_TRUE(status);

    // Test: UT-CBM-5
    pos = {0.005, 0.0, 0.0};
    ori = {1.0, 0.0, 0.0, 0.0};
    status = soil_simulator::CheckBodyMovement(pos, ori, grid, bucket);
    EXPECT_FALSE(status);

    // Test: UT-CBM-6
    pos = {0.001, 0.002, -0.003};
    ori = {1.0, 0.0, 0.0, 0.0};
    status = soil_simulator::CheckBodyMovement(pos, ori, grid, bucket);
    EXPECT_FALSE(status);

    // Test: UT-CBM-7
    pos = {0.0, 0.0, 0.0};
    ori = {0.999, 0.0, 0.0029, 0.0};
    status = soil_simulator::CheckBodyMovement(pos, ori, grid, bucket);
    EXPECT_FALSE(status);

    // Test: UT-CBM-8
    pos = {0.001, 0.0, 0.0};
    ori = {0.999, 0.0, 0.0029, 0.0};
    status = soil_simulator::CheckBodyMovement(pos, ori, grid, bucket);
    EXPECT_FALSE(status);

    // Test: UT-CBM-9
    pos = {0.3, 0.0, 0.0};
    ori = {1.0, 0.0, 0.0, 0.0};
    testing::internal::CaptureStdout();
    status = soil_simulator::CheckBodyMovement(pos, ori, grid, bucket);
    std::string warning_msg = testing::internal::GetCapturedStdout();
    std::string exp_msg = "Movement made by the body is larger than two"
        " cell size.";
    size_t string_loc = warning_msg.find(exp_msg);
    EXPECT_TRUE(string_loc != std::string::npos);
    EXPECT_TRUE(status);

    delete bucket;
}

TEST(UnitTestUtils, CalcNormal) {
    // Declaring variables
    std::vector<float> a;
    std::vector<float> b;
    std::vector<float> c;
    std::vector<float> normal;
    float cc;

    // Test: UT-CN-1
    a = {0.0, 0.0, 0.0};
    b = {2.3, 0.0, 0.0};
    c = {2.3, 2.45, 0.0};
    normal = soil_simulator::CalcNormal(a, b, c);
    EXPECT_TRUE((normal == std::vector<float> {0.0, 0.0, 1.0}));
    normal = soil_simulator::CalcNormal(a, c, b);
    EXPECT_TRUE((normal == std::vector<float> {0.0, 0.0, -1.0}));

    // Test: UT-CN-2
    a = {1.0, 0.0, -1.3};
    b = {0.3, 0.0, 4.2};
    c = {2.3, 0.0, 3.0};
    normal = soil_simulator::CalcNormal(a, b, c);
    EXPECT_TRUE((normal == std::vector<float> {0.0, 1.0, 0.0}));
    normal = soil_simulator::CalcNormal(a, c, b);
    EXPECT_TRUE((normal == std::vector<float> {0.0, -1.0, 0.0}));

    // Test: UT-CN-3
    a = {0.0, -4.7, 1.3};
    b = {0.0, 7.2, -0.6};
    c = {0.0, -1.0, 54.3};
    normal = soil_simulator::CalcNormal(a, b, c);
    EXPECT_TRUE((normal == std::vector<float> {1.0, 0.0, 0.0}));
    normal = soil_simulator::CalcNormal(a, c, b);
    EXPECT_TRUE((normal == std::vector<float> {-1.0, 0.0, 0.0}));

    // Test: UT-CN-4
    a = {1.0, 0.0, 0.0};
    b = {0.0, 1.0, 0.0};
    c = {0.0, 0.0, 1.0};
    cc = std::sqrt(1.0 / 3.0);
    normal = soil_simulator::CalcNormal(a, b, c);
    EXPECT_TRUE((normal == std::vector<float> {cc, cc, cc}));
    normal = soil_simulator::CalcNormal(a, c, b);
    EXPECT_TRUE((normal == std::vector<float> {-cc, -cc, -cc}));
}

TEST(UnitTestUtils, MultiplyQuaternion) {
    // Declaring variables
    std::vector<float> q1;
    std::vector<float> q2;
    std::vector<float> quat;

    // Test: UT-MQ-1
    q1 = {0.707107, 0.0, 0.0, -0.707107};
    quat = soil_simulator::MultiplyQuaternion(q1, q1);
    EXPECT_NEAR(quat[0], 0.0, 1e-5);
    EXPECT_NEAR(quat[1], 0.0, 1e-5);
    EXPECT_NEAR(quat[2], 0.0, 1e-5);
    EXPECT_NEAR(quat[3], -1.0, 1e-5);

    // Test: UT-MQ-2
    q1 = {0.707107, 0.0, -0.707107, 0.0};
    quat = soil_simulator::MultiplyQuaternion(q1, q1);
    EXPECT_NEAR(quat[0], 0.0, 1e-5);
    EXPECT_NEAR(quat[1], 0.0, 1e-5);
    EXPECT_NEAR(quat[2], -1.0, 1e-5);
    EXPECT_NEAR(quat[3], 0.0, 1e-5);

    // Test: UT-MQ-3
    q1 = {0.707107, -0.707107, 0.0, 0.0};
    quat = soil_simulator::MultiplyQuaternion(q1, q1);
    EXPECT_NEAR(quat[0], 0.0, 1e-5);
    EXPECT_NEAR(quat[1], -1.0, 1e-5);
    EXPECT_NEAR(quat[2], 0.0, 1e-5);
    EXPECT_NEAR(quat[3], 0.0, 1e-5);

    // Test: UT-MQ-4
    q1 = {0.8, -0.4, 0.2, 0.7};
    q2 = {0.2, 0.5, -0.7, -0.8};
    quat = soil_simulator::MultiplyQuaternion(q1, q2);
    EXPECT_NEAR(quat[0], 1.06, 1e-5);
    EXPECT_NEAR(quat[1], 0.65, 1e-5);
    EXPECT_NEAR(quat[2], -0.49, 1e-5);
    EXPECT_NEAR(quat[3], -0.32, 1e-5);
}

TEST(UnitTestUtils, CalcRotationQuaternion) {
    // Declaring variables
    std::vector<float> pos;
    std::vector<float> ori;
    std::vector<float> new_pos;

    // Test: UT-CRQ-1
    ori = {0.707107, 0.0, 0.0, -0.707107};
    pos = {0.1, 0.1, 0.3};
    new_pos = soil_simulator::CalcRotationQuaternion(ori, pos);
    EXPECT_NEAR(new_pos[0], -0.1, 1e-5);
    EXPECT_NEAR(new_pos[1], 0.1, 1e-5);
    EXPECT_NEAR(new_pos[2], 0.3, 1e-5);

    // Test: UT-CRQ-2
    ori = {0.707107, 0.0, -0.707107, 0.0};
    pos = {0.1, 0.5, 0.0};
    new_pos = soil_simulator::CalcRotationQuaternion(ori, pos);
    EXPECT_NEAR(new_pos[0], 0.0, 1e-5);
    EXPECT_NEAR(new_pos[1], 0.5, 1e-5);
    EXPECT_NEAR(new_pos[2], -0.1, 1e-5);

    // Test: UT-CRQ-3
    ori = {0.707107, -0.707107, 0.0, 0.0};
    pos = {-0.1, 0.3, -0.5};
    new_pos = soil_simulator::CalcRotationQuaternion(ori, pos);
    EXPECT_NEAR(new_pos[0], -0.1, 1e-5);
    EXPECT_NEAR(new_pos[1], 0.5, 1e-5);
    EXPECT_NEAR(new_pos[2], 0.3, 1e-5);

    // Test: UT-CRQ-4
    ori = {0.53, -0.21, 0.64, -0.33};
    pos = {-0.15, 0.67, -0.12};
    new_pos = soil_simulator::CalcRotationQuaternion(ori, pos);
    EXPECT_NEAR(new_pos[0], -0.380155, 1e-5);
    EXPECT_NEAR(new_pos[1], 0.504297, 1e-5);
    EXPECT_NEAR(new_pos[2], -0.29490, 1e-5);
}

TEST(UnitTestUtils, AngleToQuat) {
    // Declaring variables
    std::vector<float> ori;
    std::vector<float> quat;

    // Test: UT-AQ-1
    ori = {-1.570796327, 0.0, 0.0};
    quat = soil_simulator::AngleToQuat(ori);
    EXPECT_NEAR(quat[0], 0.707107, 1e-5);
    EXPECT_NEAR(quat[1], 0.0, 1e-5);
    EXPECT_NEAR(quat[2], 0.0, 1e-5);
    EXPECT_NEAR(quat[3], -0.707107, 1e-5);

    // Test: UT-AQ-2
    ori = {0.0, -1.570796327, 0.0};
    quat = soil_simulator::AngleToQuat(ori);
    EXPECT_NEAR(quat[0], 0.707107, 1e-5);
    EXPECT_NEAR(quat[1], 0.0, 1e-5);
    EXPECT_NEAR(quat[2], -0.707107, 1e-5);
    EXPECT_NEAR(quat[3], 0.0, 1e-5);

    // Test: UT-AQ-3
    ori = {0.0, 0.0, -1.570796327};
    quat = soil_simulator::AngleToQuat(ori);
    EXPECT_NEAR(quat[0], 0.707107, 1e-5);
    EXPECT_NEAR(quat[1], -0.707107, 1e-5);
    EXPECT_NEAR(quat[2], 0.0, 1e-5);
    EXPECT_NEAR(quat[3], 0.0, 1e-5);

    // Test: UT-AQ-4
    ori = {0.53, 1.2, -0.3};
    quat = soil_simulator::AngleToQuat(ori);
    EXPECT_NEAR(quat[0], 0.765481, 1e-5);
    EXPECT_NEAR(quat[1], -0.265256, 1e-5);
    EXPECT_NEAR(quat[2], 0.50651, 1e-5);
    EXPECT_NEAR(quat[3], 0.295169, 1e-5);
}

TEST(UnitTestUtils, CalcBodyFramePos) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    std::vector<float> o_pos = {0.0, 0.0, 0.0};
    std::vector<float> j_pos = {0.0, 0.0, 0.0};
    std::vector<float> b_pos = {0.0, 0.0, -0.5};
    std::vector<float> t_pos = {0.7, 0.0, -0.5};
    soil_simulator::Bucket *bucket = new soil_simulator::Bucket(
        o_pos, j_pos, b_pos, t_pos, 0.5);
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};

    // Declaring variables
    std::vector<float> pos;

    // Test: UT-CBF-1
    pos = soil_simulator::CalcBodyFramePos(11, 11, 0.2, grid, bucket);
    EXPECT_NEAR(pos[0], 0.1, 1e-5);
    EXPECT_NEAR(pos[1], 0.1, 1e-5);
    EXPECT_NEAR(pos[2], 0.2, 1e-5);

    // Test: UT-CBF-2
    bucket->pos_ = {-0.1, 0.2, 0.3};
    pos = soil_simulator::CalcBodyFramePos(10, 12, -0.2, grid, bucket);
    EXPECT_NEAR(pos[0], 0.1, 1e-5);
    EXPECT_NEAR(pos[1], 0.0, 1e-5);
    EXPECT_NEAR(pos[2], -0.5, 1e-5);
    bucket->pos_ = {0.0, 0.0, 0.0};

    // Test: UT-CBF-3
    bucket->ori_ = {0.707107, 0.0, 0.0, -0.707107};
    pos = soil_simulator::CalcBodyFramePos(11, 12, 0.3, grid, bucket);
    EXPECT_NEAR(pos[0], 0.2, 1e-5);
    EXPECT_NEAR(pos[1], -0.1, 1e-5);
    EXPECT_NEAR(pos[2], 0.3, 1e-5);
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};

    // Test: UT-CBF-4
    bucket->ori_ = {0.707107, 0.0, -0.707107, 0.0};
    pos = soil_simulator::CalcBodyFramePos(11, 12, 0.3, grid, bucket);
    EXPECT_NEAR(pos[0], -0.3, 1e-5);
    EXPECT_NEAR(pos[1], 0.2, 1e-5);
    EXPECT_NEAR(pos[2], 0.1, 1e-5);
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};

    // Test: UT-CBF-5
    bucket->ori_ = {0.707107, 0.707107, 0.0, 0.0};
    pos = soil_simulator::CalcBodyFramePos(11, 12, 0.3, grid, bucket);
    EXPECT_NEAR(pos[0], 0.1, 1e-5);
    EXPECT_NEAR(pos[1], -0.3, 1e-5);
    EXPECT_NEAR(pos[2], 0.2, 1e-5);
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};

    // Test: UT-CBF-6
    bucket->pos_ = {-0.1, 0.2, 0.3};
    bucket->ori_ = {0.707107, 0.0, 0.0, -0.707107};
    pos = soil_simulator::CalcBodyFramePos(10, 12, -0.2, grid, bucket);
    EXPECT_NEAR(pos[0], 0.0, 1e-5);
    EXPECT_NEAR(pos[1], -0.1, 1e-5);
    EXPECT_NEAR(pos[2], -0.5, 1e-5);
    bucket->pos_ = {0.0, 0.0, 0.0};
    bucket->ori_ = {1.0, 0.0, 0.0, 0.0};

    delete bucket;
}

TEST(UnitTestUtils, CheckVolume) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    // Declaring variables
    float init_volume;
    std::string warning_msg;
    std::string exp_msg;
    size_t string_loc;

    // Creating a lambda function to check that a warning has been issued
    auto CheckVolumeWarning = [&](float init_volume) {
        testing::internal::CaptureStdout();
        EXPECT_FALSE(
            soil_simulator::CheckVolume(sim_out, init_volume, grid, 1e-5));
        warning_msg = testing::internal::GetCapturedStdout();
        string_loc = warning_msg.find(exp_msg);
        EXPECT_TRUE(string_loc != std::string::npos);
    };

    // Test: UT-CV-1
    exp_msg = "Volume is not conserved!";
    EXPECT_TRUE(soil_simulator::CheckVolume(sim_out, 0.0, grid, 1e-5));
    CheckVolumeWarning(1.0);
    CheckVolumeWarning(-0.6 * grid.cell_volume_);
    CheckVolumeWarning(0.6 * grid.cell_volume_);

    // Test: UT-CV-2
    exp_msg = "Volume is not conserved!";
    sim_out->terrain_[1][2] = 0.2;
    init_volume =  0.2 * grid.cell_area_;
    EXPECT_TRUE(soil_simulator::CheckVolume(sim_out, init_volume, grid, 1e-5));
    CheckVolumeWarning(0.0);
    CheckVolumeWarning(init_volume - 0.6 * grid.cell_volume_);
    CheckVolumeWarning(init_volume + 0.6 * grid.cell_volume_);

    // Test: UT-CV-3
    exp_msg = "Volume is not conserved!";
    sim_out->terrain_[1][2] = 0.0;
    SetHeight(sim_out, 1, 1, NAN, NAN, NAN, 0.0, 0.08, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 2, 1, NAN, NAN, NAN, NAN, NAN, NAN, NAN, 0.0, 0.15);
    SetHeight(sim_out, 2, 2, NAN, NAN, NAN, -0.1, 0.0, NAN, NAN, 0.2, 0.27);
    PushBodySoilPos(sim_out, 0, 1, 1, {0.0, 0.0, 0.0}, 0.08);
    PushBodySoilPos(sim_out, 2, 2, 1, {0.0, 0.0, 0.0}, 0.15);
    PushBodySoilPos(sim_out, 0, 2, 2, {0.0, 0.0, 0.0}, 0.1);
    PushBodySoilPos(sim_out, 2, 2, 2, {0.0, 0.0, 0.0}, 0.07);
    init_volume =  0.4 * grid.cell_area_;
    EXPECT_TRUE(soil_simulator::CheckVolume(sim_out, init_volume, grid, 1e-5));
    CheckVolumeWarning(0.0);

    // Test: UT-CV-4
    exp_msg = "Volume of soil in body_soil_pos_ is not consistent";
    sim_out->body_soil_pos_[2].h_soil = 0.0;
    CheckVolumeWarning(init_volume);
    sim_out->body_soil_pos_[2].h_soil = 0.1;
    PushBodySoilPos(sim_out, 0, 2, 2, {0.0, 0.0, 0.0}, 0.05);
    CheckVolumeWarning(init_volume);
    sim_out->body_soil_[1][2][2] = 0.05;
    init_volume += 0.05 * grid.cell_area_;
    EXPECT_TRUE(soil_simulator::CheckVolume(sim_out, init_volume, grid, 1e-5));
    PushBodySoilPos(sim_out, 0, 5, 5, {0.0, 0.0, 0.0}, 0.05);
    CheckVolumeWarning(init_volume);

    delete sim_out;
}

TEST(UnitTestUtils, CheckSoil) {
    // Setting up the environment
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    // Declaring variables
    std::string warning_msg;
    std::string exp_msg;
    size_t string_loc;

    // Creating a lambda function to check that a warning has been issued
    auto CheckSoilWarning = [&]() {
        testing::internal::CaptureStdout();
        EXPECT_FALSE(soil_simulator::CheckSoil(sim_out, 1e-5));
        warning_msg = testing::internal::GetCapturedStdout();
        string_loc = warning_msg.find(exp_msg);
        EXPECT_TRUE(string_loc != std::string::npos);
    };

    // Test: UT-CS-1
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Test: UT-CS-2
    sim_out->terrain_[1][1] = -0.2;
    sim_out->terrain_[1][2] = -0.15;
    sim_out->terrain_[2][1] = 0.0;
    sim_out->terrain_[2][2] = 0.0;
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Test: UT-CS-3
    SetHeight(sim_out, 1, 1, NAN, -0.2, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 1, 2, NAN, -0.15, 0.0, NAN, NAN, 0.1, 0.2, NAN, NAN);
    SetHeight(sim_out, 2, 1, NAN, NAN, NAN, NAN, NAN, 0.0, 0.15, NAN, NAN);
    SetHeight(sim_out, 2, 2, NAN, 0.05, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Test: UT-CS-4
    SetHeight(sim_out, 1, 1, NAN, NAN, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 1, 2, NAN, NAN, NAN, 0.0, 0.1, NAN, NAN, 0.2, 0.3);
    SetHeight(sim_out, 2, 1, NAN, NAN, NAN, NAN, NAN, NAN, NAN, 0.15, 0.25);
    SetHeight(sim_out, 2, 2, NAN, NAN, NAN, 0.1, 0.15, NAN, NAN, NAN, NAN);
    PushBodySoilPos(sim_out, 0, 1, 1, {0.0, 0.0, 0.0}, 0.0);
    PushBodySoilPos(sim_out, 0, 1, 2, {0.0, 0.0, 0.0}, 0.0);
    PushBodySoilPos(sim_out, 2, 1, 2, {0.0, 0.0, 0.0}, 0.0);
    PushBodySoilPos(sim_out, 2, 2, 1, {0.0, 0.0, 0.0}, 0.0);
    PushBodySoilPos(sim_out, 0, 2, 2, {0.0, 0.0, 0.0}, 0.0);
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Test: UT-CS-5
    exp_msg = "Terrain is above the body";
    sim_out->terrain_[1][1] = 0.5;
    CheckSoilWarning();
    sim_out->terrain_[1][1] = -0.2;
    sim_out->terrain_[2][1] = 0.05;
    CheckSoilWarning();
    sim_out->terrain_[2][1] = 0.0;
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Test: UT-CS-6
    exp_msg = "Min height of the body is above its max height";
    SetHeight(sim_out, 1, 1, NAN, 0.0, -0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 1, NAN, 0.1, 0.1, NAN, NAN, NAN, NAN, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 1, NAN, 0.41, 0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 1, NAN, 0.41, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 1, NAN, 0.0, -0.4, NAN, NAN, NAN, NAN, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 1, NAN, -0.2, 0.0, 0.0, 0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 2, 1, NAN, NAN, NAN, NAN, NAN, 0.16, 0.15, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 2, 1, NAN, NAN, NAN, NAN, NAN, 0.0, 0.15, NAN, NAN);
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Test: UT-CS-7
    exp_msg = "Min height of the body soil is above its max height";
    SetHeight(sim_out, 1, 1, NAN, NAN, NAN, 0.0, -0.1, NAN, NAN, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 1, NAN, NAN, NAN, 0.2, 0.0, NAN, NAN, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 1, NAN, NAN, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 2, 1, NAN, NAN, NAN, NAN, NAN, NAN, NAN, 0.15, 0.14);
    CheckSoilWarning();
    SetHeight(sim_out, 2, 1, NAN, NAN, NAN, NAN, NAN, NAN, NAN, 0.15, 0.25);
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Test: UT-CS-8
    exp_msg = "Body is above the body soil";
    SetHeight(sim_out, 1, 1, NAN, -0.2, 0.05, NAN, NAN, NAN, NAN, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 1, NAN, -0.2, 0.00, NAN, NAN, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 1, 2, NAN, NAN, NAN, NAN, NAN, 0.1, 0.25, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 2, NAN, NAN, NAN, NAN, NAN, 0.1, 0.45, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 2, NAN, NAN, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN);
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Test: UT-CS-9
    exp_msg = "Body soil is not resting on the body";
    SetHeight(sim_out, 1, 1, NAN, NAN, NAN, 0.1, 0.1, NAN, NAN, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 1, NAN, NAN, NAN, 0.05, 0.1, NAN, NAN, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 1, NAN, NAN, NAN, 0.0, 0.1, NAN, NAN, NAN, NAN);
    SetHeight(sim_out, 2, 1, NAN, NAN, NAN, NAN, NAN, NAN, NAN, 0.2, 0.25);
    CheckSoilWarning();
    SetHeight(sim_out, 2, 1, NAN, NAN, NAN, NAN, NAN, NAN, NAN, 0.15, 0.25);
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Test: UT-CS-10
    exp_msg = "Body soil is present but there is no body";
    SetHeight(sim_out, 1, 2, NAN, NAN, NAN, NAN, NAN, 0.0, 0.0, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 2, NAN, NAN, NAN, NAN, NAN, 0.1, 0.2, NAN, NAN);
    SetHeight(sim_out, 1, 1, NAN, 0.0, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 1, 1, NAN, -0.2, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Test: UT-CS-11
    exp_msg = "Two body layers are intersecting";
    sim_out->terrain_[3][2] = -0.2;
    SetHeight(sim_out, 3, 2, NAN, -0.15, 0.1, NAN, NAN, 0.0, 0.2, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 3, 2, NAN, -0.15, 0.0, NAN, NAN, NAN, NAN, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 3, 2, NAN, 0.0, 0.2, NAN, NAN, -0.2, 0.1, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 3, 2, NAN, NAN, NAN, NAN, NAN, -0.2, 0.0, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 3, 2, NAN, 0.0, 0.0, NAN, NAN, 0.0, 0.0, NAN, NAN);
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    // Test: UT-CS-12
    exp_msg = "A body and body soil layer are intersecting";
    SetHeight(sim_out, 3, 2, NAN, -0.15, 0.0, 0.0, 0.15, 0.1, 0.2, NAN, NAN);
    CheckSoilWarning();
    SetHeight(sim_out, 3, 2, NAN, 0.1, 0.2, 0.0, 0.0, -0.15, 0.0, 0.0, 0.15);
    CheckSoilWarning();
    SetHeight(sim_out, 3, 2, NAN, NAN, NAN, NAN, NAN, NAN, NAN, 0.0, 0.1);
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));
    SetHeight(sim_out, 3, 2, NAN, 0.0, 0.0, NAN, NAN, 0.0, 0.0, 0.0, 0.0);
    EXPECT_TRUE(soil_simulator::CheckSoil(sim_out, 1e-5));

    delete sim_out;
}
