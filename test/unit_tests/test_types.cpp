/*

*/ 
#include "gtest/gtest.h"
#include "../../src/types.cpp"

using namespace soil_simulator;

TEST(UnitTestTypes, grid) {
    // Test member of the grid class
    grid grid(4.0, 4.0, 4.0, 0.05, 0.05);
    EXPECT_EQ(grid.half_length_x, 80);
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
