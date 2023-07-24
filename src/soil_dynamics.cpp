/*
This file implements the main functions of the simulator.

Copyright, 2023,  Vilella Kenny.
*/
#include <iostream>
#include "src/soil_dynamics.hpp"
#include "src/types.hpp"

void soil_simulator::SoilDynamics::step() {
}

void soil_simulator::SoilDynamics::check() {
}

void soil_simulator::SoilDynamics::WriteOutputs() {
}

int main() {
    soil_simulator::SoilDynamics sim;
    soil_simulator::Grid grid(4.0, 4.0, 4.0, 0.05, 0.05);
    std::cout << grid.vect_x_[0];
    return 0;
}
