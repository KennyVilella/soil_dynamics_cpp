/*

*/
#include "soil_dynamics.hpp"
#include "types.hpp"
#include <iostream>

using namespace soil_simulator;

void soil_dynamics::step() {

};

void soil_dynamics::check() {

};

void soil_dynamics::write_outputs() {

};

int main() {
    soil_dynamics sim;
    grid grid(4.0, 4.0, 4.0, 0.05, 0.05);
    std::cout << grid.half_length_x;
    return 0;
};
