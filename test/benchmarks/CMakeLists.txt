# Setup benchmarking
find_package(benchmark REQUIRED)

# Build benchmarking
add_executable(benchmarks)
target_sources(benchmarks
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/run_benchmarks.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmark_bucket_pos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src
)

# Link to Google benchmark
target_link_libraries(
  benchmarks
  benchmark::benchmark
)
