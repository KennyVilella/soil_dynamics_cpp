# Setup benchmarking
find_package(benchmark REQUIRED)

# Setup Logging
find_package (glog REQUIRED)

# Build benchmarking
add_executable(benchmarks)
target_sources(benchmarks
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/run_benchmarks.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmark_soil_evolution.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmark_bucket_pos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmark_body_soil.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmark_intersecting_cells.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmark_relax.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmark_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/soil_dynamics.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/soil_dynamics.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/types.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/types.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/bucket_pos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/bucket_pos.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/body_soil.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/body_soil.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/intersecting_cells.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/intersecting_cells.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/relax.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/relax.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/utils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../example/soil_evolution.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../example/soil_evolution.hpp
)

# Link to Google benchmark
target_link_libraries(
  benchmarks
  benchmark::benchmark
)

# Link to logging framework
target_link_libraries(
  benchmarks
  glog::glog
)
