# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Build testing
add_executable(unit_tests)
target_sources(unit_tests
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/run_tests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_types.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_bucket_pos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src
)

# Link to googletest
target_link_libraries(
  unit_tests
  GTest::gtest_main
)
