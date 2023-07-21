/*
Copyright, 2023,  Vilella Kenny.
*/
#pragma once

namespace soil_simulator {

class soil_dynamics {
 public:
     void init();
     void step();
     void check();
     void write_outputs();
};
}  // namespace soil_simulator
