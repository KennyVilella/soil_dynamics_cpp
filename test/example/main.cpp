/*
This file implements the main function.

Copyright, 2023, Vilella Kenny.
*/
#include <glog/logging.h>
#include <random>
#include "soil_simulator/soil_dynamics.hpp"
#include "test/example/soil_evolution.hpp"

// Defining RNG
std::mt19937 soil_simulator::rng;

/// This function removes the prefix of glog message.
void EmptyPrefix(std::ostream &s, const google::LogMessageInfo &l, void*) {
    s;
}

int main(int argc, char* argv[]) {
    // Initialize Google’s logging library.
    google::InitGoogleLogging(argv[0], &EmptyPrefix);

    // Writing outputs to stderr instead of logfiles
    FLAGS_logtostderr = 1;

    // Selecting the simulated body object
    if (false) {
        // Simulating a bucket
        // Initalizing the simulated bucket
        std::vector<float> o_pos_init = {0.0, 0.0, 0.0};
        std::vector<float> j_pos_init = {0.0, 0.0, 0.0};
        std::vector<float> b_pos_init = {0.0, 0.0, -0.5};
        std::vector<float> t_pos_init = {0.7, 0.0, -0.5};
        float bucket_width = 0.5;
        soil_simulator::Bucket *body = new soil_simulator::Bucket(
            o_pos_init, j_pos_init, b_pos_init, t_pos_init, bucket_width);

        // Launching a simulation
        soil_simulator::SoilEvolution(body, true, false, true, true, false);
        delete body;
    } else {
        // Simulating a blade
        // Initalizing the simulated blade
        std::vector<float> o_pos_init = {0.0, 0.0, 0.0};
        std::vector<float> j_pos_init = {0.9, 0.0, 0.2};
        std::vector<float> b_pos_init = {0.75, 0.0, 1.5};
        std::vector<float> t_pos_init = {1.6, 0.0, -0.4};
        float blade_width = 4.5;
        soil_simulator::Blade *body = new soil_simulator::Blade(
            o_pos_init, j_pos_init, b_pos_init, t_pos_init, blade_width);

        // Launching a simulation
        soil_simulator::SoilEvolution(body, true, false, true, true, false);
        delete body;
    }

    return 0;
}
