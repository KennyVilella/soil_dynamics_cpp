/*
This file implements the functions used for the terrain relaxation.

Copyright, 2023, Vilella Kenny.
*/
#include <vector>
#include "src/relax.hpp"
#include "src/types.hpp"

soil_simulator::RelaxTerrain(
    SimOut* sim_out, Grid grid, SimParam sim_param, float tol
) {
}

soil_simulator::RelaxBodySoil(
    SimOut* sim_out, Grid grid, SimParam sim_param, float tol
) {
}

soil_simulator::LocateUnstableTerrainCell(
    SimOut* sim_out, float dh_max, float tol
) {
    return std::vector<std::vector<int>>;
}

soil_simulator::CheckUnstableTerrainCell(
    SimOut* sim_out, int ii_c, int jj_c, float h_min, float tol
) {
    return 0;
}

soil_simulator::CheckUnstableBodyCell(
    SimOut* sim_out, int ii, int jj, int ind, int ii_c, int jj_c, float h_min,
    float tol
) {
    return 0;
}

soil_simulator::RelaxUnstableTerrainCell(
    SimOut* sim_out, int status, float dh_max, int ii, int jj, int ii_c,
    int jj_c, Grid grid, float tol
) {
}

soil_simulator::RelaxUnstableBodyCell(
    SimOut* sim_out, int status, std::vector<std::vector<int>> body_soil_pos,
    float dh_max, int ii, int jj, int ind, int ii_c, int jj_c, Grid grid,
    float tol
) {
}