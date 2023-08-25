# Setup Logging
find_package (glog REQUIRED)

set(CMAKE_CXX_FLAGS "-DGLOG_CUSTOM_PREFIX_SUPPORT")

# Build example script
add_executable(soil_evolution)
target_sources(soil_evolution
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/soil_evolution.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/soil_evolution.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/soil_dynamics.cpp
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

# Link to logging framework
target_link_libraries(
  soil_evolution
  glog::glog
)
