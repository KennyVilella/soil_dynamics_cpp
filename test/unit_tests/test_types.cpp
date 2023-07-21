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
    soil_simulator::grid grid_1(4.0, 4.0, 4.0, 0.05, 0.01);
    // Asserting values
    EXPECT_EQ(grid_1.half_length_x, 80);
    EXPECT_EQ(grid_1.half_length_y, 80);
    EXPECT_EQ(grid_1.half_length_z, 400);
    EXPECT_NEAR(grid_1.cell_size_xy, 0.05, 1e-8);
    EXPECT_NEAR(grid_1.cell_size_z, 0.01, 1e-8);
    EXPECT_NEAR(grid_1.cell_area, 0.0025, 1e-8);
    EXPECT_NEAR(grid_1.cell_volume, 0.000025, 1e-8);
    for (auto ii = 0 ; ii <  grid_1.vect_x.size() ; ii++)
        EXPECT_NEAR(grid_1.vect_x[ii], -4.0 + ii * 0.05, 1e-5);
    for (auto ii = 0 ; ii <  grid_1.vect_y.size() ; ii++) 
        EXPECT_NEAR(grid_1.vect_y[ii], -4.0 + ii * 0.05, 1e-5);
    for (auto ii = 0 ; ii <  grid_1.vect_z.size() ; ii++) 
        EXPECT_NEAR(grid_1.vect_z[ii], -4.0 + ii * 0.01, 1e-5);

    /* Testing that exceptions are properly sent */
    EXPECT_THROW(
        soil_simulator::grid grid_2(0.0, 4.0, 4.0, 0.05, 0.01),
        std::invalid_argument
    );
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
