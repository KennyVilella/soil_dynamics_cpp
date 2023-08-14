/*
This file implements unit tests for the functions in utils.cpp.

Copyright, 2023, Vilella Kenny.
*/
#include "gtest/gtest.h"
#include "src/utils.cpp"

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

TEST(UnitTestUtils, CheckVolume) {
    // Setting dummy classes
    soil_simulator::Grid grid(1.0, 1.0, 1.0, 0.1, 0.1);
    soil_simulator::SimOut *sim_out = new soil_simulator::SimOut(grid);

    // -- Testing that no warning is sent for correct initial volume --
    EXPECT_TRUE(soil_simulator::CheckVolume(sim_out, 0.0, grid));

    // -- Testing that warning is sent for incorrect initial volume --
    EXPECT_FALSE(soil_simulator::CheckVolume(sim_out, 1.0, grid));
    EXPECT_FALSE(soil_simulator::CheckVolume(
        sim_out, -0.6 * grid.cell_volume_, grid));
    EXPECT_FALSE(soil_simulator::CheckVolume(
        sim_out, 0.6 * grid.cell_volume_, grid));

    // Setting non-zero terrain
    sim_out->terrain_[1][2] = 0.2;
    float init_volume =  0.2 * grid.cell_area_;

    // -- Testing that no warning is sent for correct initial volume --
    EXPECT_TRUE(soil_simulator::CheckVolume(sim_out, init_volume, grid));

    // -- Testing that warning is sent for incorrect initial volume --
    EXPECT_FALSE(soil_simulator::CheckVolume(sim_out, 0.0, grid));
    EXPECT_FALSE(soil_simulator::CheckVolume(
        sim_out, init_volume - 0.6 * grid.cell_volume_, grid));
    EXPECT_FALSE(soil_simulator::CheckVolume(
        sim_out, init_volume + 0.6 * grid.cell_volume_, grid));

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
    init_volume =  0.4 * grid.cell_area_;
    sim_out->body_soil_pos_.resize(4, std::vector<int>(3, 0));
    sim_out->body_soil_pos_[0] = std::vector<int> {0, 2, 2};
    sim_out->body_soil_pos_[1] = std::vector<int> {2, 2, 2};
    sim_out->body_soil_pos_[2] = std::vector<int> {0, 1, 1};
    sim_out->body_soil_pos_[3] = std::vector<int> {2, 2, 1};

    // -- Testing that no warning is sent for correct initial volume --
    EXPECT_TRUE(soil_simulator::CheckVolume(sim_out, init_volume, grid));

    // -- Testing that warning is sent for incorrect initial volume --
    EXPECT_FALSE(soil_simulator::CheckVolume(sim_out, 0.0, grid));
}

TEST(UnitTestUtils, CalcTrajectory) {
    // -- Testing for a simple flat trajectory --
    auto [pos, ori] = soil_simulator::CalcTrajectory(-1.0, 0.0, 0.0, 0.0, 3);
    EXPECT_EQ(pos.size(), 3);
    EXPECT_EQ(ori.size(), 3);
    EXPECT_TRUE((pos[0] == std::vector<float> {-1.0, 0.0, 0.0}));
    EXPECT_TRUE((pos[1] == std::vector<float> {0.0, 0.0, 0.0}));
    EXPECT_TRUE((pos[2] == std::vector<float> {1.0, 0.0, 0.0}));
    EXPECT_TRUE((ori[0] == std::vector<float> {0.0, 0.0, 0.0}));
    EXPECT_TRUE((ori[1] == std::vector<float> {0.0, 0.0, 0.0}));
    EXPECT_TRUE((ori[2] == std::vector<float> {0.0, 0.0, 0.0}));

    // -- Testing for a simple trajectory --
    std::tie(pos, ori) = soil_simulator::CalcTrajectory(
        -1.0, 0.0, 0.0, -1.0, 3);
    EXPECT_EQ(pos.size(), 3);
    EXPECT_EQ(ori.size(), 3);
    EXPECT_TRUE((pos[0] == std::vector<float> {-1.0, 0.0, 0.0}));
    EXPECT_TRUE((pos[1] == std::vector<float> {0.0, 0.0, -1.0}));
    EXPECT_TRUE((pos[2] == std::vector<float> {1.0, 0.0, 0.0}));
    for (auto ii = 0; ii < 3; ii++) {
        EXPECT_NEAR(ori[ii][0], 0.0, 1e-5);
        EXPECT_NEAR(ori[ii][2], 0.0, 1e-5);
    }
    EXPECT_NEAR(ori[0][1], -1.10715, 1e-5);
    EXPECT_NEAR(ori[1][1], 0.0, 1e-5);
    EXPECT_NEAR(ori[2][1], 1.10715, 1e-5);

    // -- Testing for a simple trajectory translated in the Z axis --
    std::tie(pos, ori) = soil_simulator::CalcTrajectory(-1.0, 2.5, 0.0, 1.5, 3);
    EXPECT_EQ(pos.size(), 3);
    EXPECT_EQ(ori.size(), 3);
    EXPECT_TRUE((pos[0] == std::vector<float> {-1.0, 0.0, 2.5}));
    EXPECT_TRUE((pos[1] == std::vector<float> {0.0, 0.0, 1.5}));
    EXPECT_TRUE((pos[2] == std::vector<float> {1.0, 0.0, 2.5}));
    for (auto ii = 0; ii < 3; ii++) {
        EXPECT_NEAR(ori[ii][0], 0.0, 1e-5);
        EXPECT_NEAR(ori[ii][2], 0.0, 1e-5);
    }
    EXPECT_NEAR(ori[0][1], -1.10715, 1e-5);
    EXPECT_NEAR(ori[1][1], 0.0, 1e-5);
    EXPECT_NEAR(ori[2][1], 1.10715, 1e-5);

    // -- Testing for a simple trajectory translated in the X axis --
    std::tie(pos, ori) = soil_simulator::CalcTrajectory(2.0, 0.0, 3.0, -1.0, 3);
    EXPECT_EQ(pos.size(), 3);
    EXPECT_EQ(ori.size(), 3);
    EXPECT_TRUE((pos[0] == std::vector<float> {2.0, 0.0, 0.0}));
    EXPECT_TRUE((pos[1] == std::vector<float> {3.0, 0.0, -1.0}));
    EXPECT_TRUE((pos[2] == std::vector<float> {4.0, 0.0, 0.0}));
    for (auto ii = 0; ii < 3; ii++) {
        EXPECT_NEAR(ori[ii][0], 0.0, 1e-5);
        EXPECT_NEAR(ori[ii][2], 0.0, 1e-5);
    }
    EXPECT_NEAR(ori[0][1], -1.10715, 1e-5);
    EXPECT_NEAR(ori[1][1], 0.0, 1e-5);
    EXPECT_NEAR(ori[2][1], 1.10715, 1e-5);

    // -- Testing for a simple trajectory with more points --
    std::tie(pos, ori) = soil_simulator::CalcTrajectory(
        -1.0, 0.0, 0.0, -1.0, 5);
    EXPECT_EQ(pos.size(), 5);
    EXPECT_EQ(ori.size(), 5);
    EXPECT_TRUE((pos[0] == std::vector<float> {-1.0, 0.0, 0.0}));
    EXPECT_TRUE((pos[1] == std::vector<float> {-0.5, 0.0, -0.75}));
    EXPECT_TRUE((pos[2] == std::vector<float> {0.0, 0.0, -1.0}));
    EXPECT_TRUE((pos[3] == std::vector<float> {0.5, 0.0, -0.75}));
    EXPECT_TRUE((pos[4] == std::vector<float> {1.0, 0.0, 0.0}));
    for (auto ii = 0; ii < 5; ii++) {
        EXPECT_NEAR(ori[ii][0], 0.0, 1e-5);
        EXPECT_NEAR(ori[ii][2], 0.0, 1e-5);
    }
    EXPECT_NEAR(ori[0][1], -1.10715, 1e-5);
    EXPECT_NEAR(ori[2][1], 0.0, 1e-5);
    EXPECT_NEAR(ori[4][1], 1.10715, 1e-5);
}
