/*

*/
#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include "soil_dynamics.hpp"

namespace soil_simulator
{   
    class grid : public soil_dynamics
    {
    public:
        int half_length_x;
        int half_length_y;
        int half_length_z;
        float cell_size_xy;
        float cell_size_z;
        float cell_area;
        float cell_volume;
        std::vector<float> vect_x;
        std::vector<float> vect_y;
        std::vector<float> vect_z;

        grid(
            float grid_size_x = 4.0, float grid_size_y = 4.0, float grid_size_z = 4.0,
            float cell_size_xy = 0.05, float cell_size_z = 0.01
        ); /* constructor */
    };

    class bucket : public soil_dynamics
    {};
    class sim_param : public soil_dynamics
    {};
    class sim_out : public soil_dynamics
    {};
}
#endif /*TYPES_H*/
