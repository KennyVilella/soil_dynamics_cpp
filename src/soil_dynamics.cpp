/*
Copyright, 2023,  Vilella Kenny.
*/
#include <iostream>
#include "src/soil_dynamics.hpp"
#include "src/types.hpp"

void soil_simulator::soil_dynamics::step() {
}

void soil_simulator::soil_dynamics::check() {
}

void soil_simulator::soil_dynamics::write_outputs() {
}

int main() {
    soil_simulator::soil_dynamics sim;
    soil_simulator::grid grid(4.0, 4.0, 4.0, 0.05, 0.05);
    std::cout << grid.vect_x[0];
    return 0;
}
