/*
Copyright, 2023,  Vilella Kenny.
*/
#pragma once

namespace soil_simulator {

class SoilDynamics {
 public:
     void init();
     void step();
     void check();
     void WriteOutputs();
};
}  // namespace soil_simulator
