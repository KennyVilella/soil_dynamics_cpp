/*
Copyright, 2023,  Vilella Kenny.
*/
#include <vector>
#include <exception>
#include <iostream>
#include "gtest/gtest.h"
#include "src/types.cpp"

TEST(UnitTestTypes, grid) {
    /* Testing member of the grid class */
    soil_simulator::grid grid(4.0, 4.0, 4.0, 0.05, 0.01);
    // Asserting values
    EXPECT_EQ(grid.half_length_x, 80);
    EXPECT_EQ(grid.half_length_y, 80);
    EXPECT_EQ(grid.half_length_z, 400);
    EXPECT_NEAR(grid.cell_size_xy, 0.05, 1e-8);
    EXPECT_NEAR(grid.cell_size_z, 0.01, 1e-8);
    EXPECT_NEAR(grid.cell_area, 0.0025, 1e-8);
    EXPECT_NEAR(grid.cell_volume, 0.000025, 1e-8);
    for (auto ii = 0 ; ii < grid.vect_x.size() ; ii++)
        EXPECT_NEAR(grid.vect_x[ii], -4.0 + ii * 0.05, 1e-5);
    for (auto ii = 0 ; ii < grid.vect_y.size() ; ii++)
        EXPECT_NEAR(grid.vect_y[ii], -4.0 + ii * 0.05, 1e-5);
    for (auto ii = 0 ; ii < grid.vect_z.size() ; ii++)
        EXPECT_NEAR(grid.vect_z[ii], -4.0 + ii * 0.01, 1e-5);

    /* Testing that exceptions are properly sent */
    EXPECT_THROW(
        soil_simulator::grid grid(4.0, 4.0, 4.0, 0.05, 0.0),
        std::invalid_argument);  // size_z = 0.0
    EXPECT_THROW(
        soil_simulator::grid grid(4.0, 4.0, 4.0, 0.05, -0.01),
        std::invalid_argument);  // size_z < 0.0
    EXPECT_THROW(
        soil_simulator::grid grid(4.0, 4.0, 4.0, 0.0, 0.01),
        std::invalid_argument);  // size_xy = 0.0
    EXPECT_THROW(
        soil_simulator::grid grid(4.0, 4.0, 4.0, -0.05, 0.01),
        std::invalid_argument);  // size_xy < 0.0
    EXPECT_THROW(
        soil_simulator::grid grid(0.0, 4.0, 4.0, 0.05, 0.01),
        std::invalid_argument);  // grid_size_x = 0.0
    EXPECT_THROW(
        soil_simulator::grid grid(-1.0, 4.0, 4.0, 0.05, 0.01),
        std::invalid_argument);  // grid_size_x < 0.0
    EXPECT_THROW(
        soil_simulator::grid grid(4.0, 0.0, 4.0, 0.05, 0.01),
        std::invalid_argument);  // grid_size_y = 0.0
    EXPECT_THROW(
        soil_simulator::grid grid(4.0, -2.0, 4.0, 0.05, 0.01),
        std::invalid_argument);  // grid_size_y < 0.0
    EXPECT_THROW(
        soil_simulator::grid grid(4.0, 4.0, 0.0, 0.05, 0.01),
        std::invalid_argument);  // grid_size_z = 0.0
    EXPECT_THROW(
        soil_simulator::grid grid(4.0, 4.0, -4.0, 0.05, 0.01),
        std::invalid_argument);  // grid_size_z < 0.0
    EXPECT_THROW(
        soil_simulator::grid grid(4.0, 4.0, 4.0, 0.05, 0.06),
        std::invalid_argument);  // size_z > size_xy
    // size_z = size_xy
    EXPECT_NO_THROW(soil_simulator::grid grid(4.0, 4.0, 4.0, 0.05, 0.05));
    EXPECT_THROW(
        soil_simulator::grid grid(0.04, 4.0, 4.0, 0.05, 0.05),
        std::invalid_argument);  // grid_size_x < size_xy
    // grid_size_x = size_xy
    EXPECT_NO_THROW(soil_simulator::grid grid(0.05, 4.0, 4.0, 0.05, 0.05));
    EXPECT_THROW(
        soil_simulator::grid grid(4.0, 0.01, 4.0, 0.05, 0.05),
        std::invalid_argument);  // grid_size_y < size_xy
    // grid_size_y = size_xy
    EXPECT_NO_THROW(soil_simulator::grid grid(4.0, 0.05, 4.0, 0.05, 0.05));
    EXPECT_THROW(
        soil_simulator::grid grid(4.0, 4.0, 0.01, 0.05, 0.03),
        std::invalid_argument);  // grid_size_z < size_z
    // grid_size_z = size_z
    EXPECT_NO_THROW(soil_simulator::grid grid(4.0, 4.0, 0.03, 0.05, 0.03));
}

TEST(UnitTestTypes, bucket) {
    /* Testing member of the grid class */
    std::vector<float> o_pos = {0.0, 0.1, 0.0};
    std::vector<float> j_pos = {0.0, 0.5, 0.0};
    std::vector<float> b_pos = {0.0, 0.5, 0.5};
    std::vector<float> t_pos = {0.5, 0.5, 0.0};
    soil_simulator::bucket bucket(o_pos, j_pos, b_pos, t_pos, 0.5);
    // Asserting values
    EXPECT_NEAR(bucket.j_pos_init[0], 0.0, 1e-8);
    EXPECT_NEAR(bucket.j_pos_init[1], 0.4, 1e-8);
    EXPECT_NEAR(bucket.j_pos_init[2], 0.0, 1e-8);
    EXPECT_NEAR(bucket.b_pos_init[0], 0.0, 1e-8);
    EXPECT_NEAR(bucket.b_pos_init[1], 0.4, 1e-8);
    EXPECT_NEAR(bucket.b_pos_init[2], 0.5, 1e-8);
    EXPECT_NEAR(bucket.t_pos_init[0], 0.5, 1e-8);
    EXPECT_NEAR(bucket.t_pos_init[1], 0.4, 1e-8);
    EXPECT_NEAR(bucket.t_pos_init[2], 0.0, 1e-8);
    EXPECT_EQ(bucket.width, 0.5);
    for (auto ii = 0 ; ii < 3 ; ii++)
        EXPECT_EQ(bucket.pos[ii], 0.0);
    for (auto ii = 0 ; ii < 4 ; ii++)
        EXPECT_EQ(bucket.ori[ii], 0.0);

    /* Testing that exceptions are properly sent */
    std::vector<float> vect_1 = {0.0};
    std::vector<float> vect_2 = {0.0, 0.1};
    std::vector<float> vect_4 = {0.0, 0.1, 0.0, 0.3};
    EXPECT_THROW(
        soil_simulator::bucket bucket(vect_1, j_pos, b_pos, t_pos, 0.5),
        std::invalid_argument);  // o_pos.size() != 3
    EXPECT_THROW(
        soil_simulator::bucket bucket(o_pos, vect_2, b_pos, t_pos, 0.5),
        std::invalid_argument);  // j_pos.size() != 3
    EXPECT_THROW(
        soil_simulator::bucket bucket(o_pos, j_pos, vect_4, t_pos, 0.5),
        std::invalid_argument);  // b_pos.size() != 3
    EXPECT_THROW(
        soil_simulator::bucket bucket(o_pos, j_pos, b_pos, vect_2, 0.5),
        std::invalid_argument);  // t_pos.size() != 3
    EXPECT_THROW(
        soil_simulator::bucket bucket(o_pos, b_pos, b_pos, t_pos, 0.5),
        std::invalid_argument);  // j_pos == b_pos
    EXPECT_THROW(
        soil_simulator::bucket bucket(o_pos, t_pos, b_pos, t_pos, 0.5),
        std::invalid_argument);  // j_pos == t_pos
    EXPECT_THROW(
        soil_simulator::bucket bucket(o_pos, j_pos, b_pos, b_pos, 0.5),
        std::invalid_argument);  // b_pos == t_pos
    EXPECT_THROW(
        soil_simulator::bucket bucket(o_pos, j_pos, b_pos, t_pos, 0.0),
        std::invalid_argument);  // bucket_width = 0.0
    EXPECT_THROW(
        soil_simulator::bucket bucket(o_pos, j_pos, b_pos, t_pos, -0.5),
        std::invalid_argument);  // bucket_width < 0.0
}

TEST(UnitTestTypes, simParam) {
    /* Testing member of the grid class */
    soil_simulator::sim_param sim_param(0.85, 5, 4);
    // Asserting values
    EXPECT_NEAR(sim_param.repose_angle, 0.85, 1e-7);
    EXPECT_EQ(sim_param.max_iterations, 5);
    EXPECT_EQ(sim_param.cell_buffer, 4);

    /* Testing that exceptions are properly sent */
    EXPECT_THROW(
        soil_simulator::sim_param sim_param(3.14, 5, 4),
        std::invalid_argument);  // repose_angle > pi/2
    EXPECT_THROW(
        soil_simulator::sim_param sim_param(-0.85, 5, 4),
        std::invalid_argument);  // repose_angle < 0.0
    // max_iterations = 0.0
    EXPECT_NO_THROW(soil_simulator::sim_param sim_param(0.85, 0.0, 4));
    EXPECT_THROW(
        soil_simulator::sim_param sim_param(0.85, -5, 4),
        std::invalid_argument);  // max_iterations < 0.0
    EXPECT_THROW(
        soil_simulator::sim_param sim_param(0.85, 5, 1),
        std::invalid_argument);  // cell_buffer < 2.0
}

TEST(UnitTestTypes, simOut) {
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
