# Setup Logging
find_package(glog REQUIRED)

# Set source files
set(SOIL_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/soil_dynamics.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/soil_dynamics.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/types.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/types.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/body_pos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/body_pos.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/body_soil.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/body_soil.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/intersecting_cells.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/intersecting_cells.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/relax.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/relax.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp
)

# Set include files
set(SOIL_INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/soil_dynamics.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/types.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/body_pos.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/body_soil.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/intersecting_cells.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/relax.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp
)

# Build simulator
add_executable(soil_dynamics)
target_sources(soil_dynamics
    PUBLIC ${SOIL_SRCS}
)

# Link to logging framework
target_link_libraries(
  soil_dynamics
  glog::glog
)

# Build library
add_library(soil_simulator
    STATIC ${SOIL_SRCS}
)

set_target_properties(soil_simulator
    PROPERTIES PUBLIC_HEADER ${SOIL_INCLUDE}
)

target_include_directories(soil_simulator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(
    TARGETS soil_simulator
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT runtime
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT runtime
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT dev
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/soil_simulator" COMPONENT dev
)

# Link to logging framework
target_link_libraries(
  soil_simulator
  glog::glog
)
