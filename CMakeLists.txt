cmake_minimum_required(VERSION 3.12)
project(soil_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(soil_dynamics
    src/soil_dynamics.cpp
)
