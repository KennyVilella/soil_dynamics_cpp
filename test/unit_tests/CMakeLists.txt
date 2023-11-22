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
        ${CMAKE_CURRENT_SOURCE_DIR}/test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_body_pos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_body_soil.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_intersecting_cells.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_relax.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utility.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utility.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/soil_dynamics.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/types.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/types.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/body_pos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/body_pos.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/body_soil.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/body_soil.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/intersecting_cells.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/intersecting_cells.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/relax.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/relax.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/utils.hpp
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
