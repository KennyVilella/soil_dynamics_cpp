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
        /* Class member */
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

        /* Constructor */
        grid(
            float grid_size_x = 4.0, float grid_size_y = 4.0, float grid_size_z = 4.0,
            float cell_size_xy = 0.05, float cell_size_z = 0.01
        );
    };

    class bucket : public soil_dynamics
    {
    public:
        /* Class member */
        std::vector<float> j_pos_init;
        std::vector<float> b_pos_init;
        std::vector<float> t_pos_init;
        float width;
        std::vector<float> pos;
        std::vector<float> ori;

        /* Constructor */
        bucket(
            std::vector<float> o_pos_init = {0.0, 0.0, 0.0},
            std::vector<float> j_pos_init = {0.0, 0.0, 0.0},
            std::vector<float> b_pos_init = {0.0, 0.0, -0.5},
            std::vector<float> t_pos_init = {0.7, 0.0, -0.5},
            float width = 0.5
        );
    };

    class sim_param : public soil_dynamics
    {
    public:
        /* Class member */
        float repose_angle;
        int max_iterations;
        int cell_buffer;

        /* Constructor */
        sim_param(
            float repose_angle = 0.85, int max_iterations = 10, int cell_buffer = 4
        );
    };

    class sim_out : public soil_dynamics
    {
    public:
        /* Class member */
        bool equilibrium;
        float terrain;
        float body;
        float body_soil;
        std::vector<std::vector<int>> body_soil_pos;
        int bucket_area;
        int relax_area;
        int impact_area;

        /* Constructor */
        sim_out(int terrain, grid grid);
    };
}
#endif /*TYPES_H*/
