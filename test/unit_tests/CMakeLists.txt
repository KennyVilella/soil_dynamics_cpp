# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Setup Logging
find_package (glog REQUIRED)

# Build testing
add_executable(unit_tests)
target_sources(unit_tests
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/run_tests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_types.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bucket_pos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_body_soil.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_intersecting_cells.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_relax.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/soil_dynamics.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/types.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/types.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/bucket_pos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/bucket_pos.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/body_soil.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/body_soil.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/intersecting_cells.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/intersecting_cells.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/relax.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/relax.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/utils.hpp
)

# Link to googletest
target_link_libraries(
  unit_tests
  GTest::gtest_main
)

# Link to logging framework
target_link_libraries(
  unit_tests
  glog::glog
)
