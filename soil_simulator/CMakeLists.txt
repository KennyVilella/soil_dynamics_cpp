# Setup Logging
find_package (glog REQUIRED)

# Build simulator
add_executable(soil_dynamics)
target_sources(soil_dynamics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/soil_dynamics.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/soil_dynamics.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/types.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/types.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/bucket_pos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/bucket_pos.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/body_soil.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/body_soil.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/intersecting_cells.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/intersecting_cells.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/relax.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/relax.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp
)

# Link to logging framework
target_link_libraries(
  soil_dynamics
  glog::glog
)