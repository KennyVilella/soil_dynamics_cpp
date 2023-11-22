# Setup Logging
find_package (glog REQUIRED)

# Build example script
add_executable(soil_evolution)
target_sources(soil_evolution
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/soil_evolution.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/soil_evolution.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../soil_simulator/soil_dynamics.cpp
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

# Link to logging framework
target_link_libraries(
  soil_evolution
  glog::glog
)
