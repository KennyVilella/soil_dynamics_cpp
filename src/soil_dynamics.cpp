/*

*/

#include "soil_dynamics.hpp"
#include <iostream>

using namespace soil_simulator;

void soil_dynamics::init() {
    std::cout << "Hello";
}

void soil_dynamics::step() {

}

void soil_dynamics::check() {

}

void soil_dynamics::write_outputs() {

}

int main() {
    soil_dynamics sim;
    sim.init();
    return 0;
}
