
# Add Catch2
add_subdirectory(Catch2)

# Add test executable
file(GLOB_RECURSE CORE_TEST_SOURCES "${CMAKE_SOURCE_DIR}/core/**/*.cpp")
add_executable(tests test.cpp ${CORE_TEST_SOURCES})
target_link_libraries(tests PRIVATE core Catch2::Catch2WithMain)
target_include_directories(tests PRIVATE ../core)

# Enable testing and discover Catch2 tests
include(CTest)
include(Catch)
catch_discover_tests(tests)