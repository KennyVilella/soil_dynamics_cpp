/*
Copyright, 2023,  Vilella Kenny.
*/
#pragma once

#include <vector>
#include "src/soil_dynamics.hpp"

namespace soil_simulator {

class Grid {
 public:
     /* Class member */
     int half_length_x_;
     int half_length_y_;
     int half_length_z_;
     float cell_size_xy_;
     float cell_size_z_;
     float cell_area_;
     float cell_volume_;
     std::vector<float> vect_x_;
     std::vector<float> vect_y_;
     std::vector<float> vect_z_;

     /* Constructor */
     Grid(
         float grid_size_x = 4.0, float grid_size_y = 4.0,
         float grid_size_z = 4.0, float cell_size_xy = 0.05,
         float cell_size_z = 0.01);

     /* Destructor */
     ~Grid() {}
};

class Bucket {
 public:
     /* Class member */
     std::vector<float> j_pos_init_;
     std::vector<float> b_pos_init_;
     std::vector<float> t_pos_init_;
     float width_;
     std::vector<float> pos_;
     std::vector<float> ori_;

     /* Constructor */
     Bucket(
         std::vector<float> o_pos_init = {0.0, 0.0, 0.0},
         std::vector<float> j_pos_init = {0.0, 0.0, 0.0},
         std::vector<float> b_pos_init = {0.0, 0.0, -0.5},
         std::vector<float> t_pos_init = {0.7, 0.0, -0.5},
         float width = 0.5);

     /* Destructor */
     ~Bucket() {}
};

class SimParam {
 public:
     /* Class member */
     float repose_angle_;
     int max_iterations_;
     int cell_buffer_;

     /* Constructor */
     SimParam(
         float repose_angle = 0.85, int max_iterations = 10,
         int cell_buffer = 4);

     /* Destructor */
     ~SimParam() {}
};

class SimOut {
 public:
     /* Class member */
     bool equilibrium_;
     std::vector<std::vector<float>> terrain_;
     std::vector<std::vector<std::vector<float>>> body_;
     std::vector<std::vector<std::vector<float>>> body_soil_;
     std::vector<std::vector<int>> body_soil_pos_;
     int bucket_area_[2][2];
     int relax_area_[2][2];
     int impact_area_[2][2];

     /* Constructor */
     explicit SimOut(Grid grid);

    ~SimOut() {}
};
}  // namespace soil_simulator
