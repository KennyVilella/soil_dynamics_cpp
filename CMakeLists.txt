cmake_minimum_required(VERSION 3.12)
project(soil_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-O2 -fPIC")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(soil_simulator)
add_subdirectory(test/unit_tests)
add_subdirectory(test/example)
add_subdirectory(test/benchmarks)
