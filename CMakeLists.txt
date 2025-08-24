cmake_minimum_required(VERSION 3.30)
include(CMakePrintHelpers)
project(SampleEngine
VERSION 1.0
LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(core)
find_package(Vulkan REQUIRED)
add_executable(${PROJECT_NAME} "main.cpp")

if ((NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND
    (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))

configure_file(shaders/shader.vert Debug/shader.vert COPYONLY)
configure_file(shaders/shader.frag Debug/shader.frag COPYONLY)
configure_file(textures/texture.jpg Debug/texture.jpg COPYONLY)
configure_file(textures/viking_room.png Debug/viking_room.png COPYONLY)
configure_file(models/viking_room.obj Debug/viking_room.obj COPYONLY)
else()
configure_file(shaders/shader.vert shader.vert COPYONLY)
configure_file(shaders/shader.frag shader.frag COPYONLY)
configure_file(textures/texture.jpg texture.jpg COPYONLY)
configure_file(textures/viking_room.png viking_room.png COPYONLY)
configure_file(models/viking_room.obj viking_room.obj COPYONLY)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE include PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
Vulkan::Vulkan
glm::glm
core
)


project(tests
VERSION 1.0
LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
# Add test executable
file(GLOB_RECURSE CORE_TEST_SOURCES "${CMAKE_SOURCE_DIR}/test/core/**/*.cpp")
message(${CORE_TEST_SOURCES})
add_executable(tests test/test.cpp ${CORE_TEST_SOURCES})

# Set runtime output directory for tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set output directories for executables and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)


# Add Catch2
add_subdirectory(Catch2)
target_link_libraries(tests PRIVATE core Catch2::Catch2WithMain Vulkan::Vulkan)
target_include_directories(tests PRIVATE ./test/helpers)
# Enable testing and discover Catch2 tests
include(CTest)
include(Catch)
catch_discover_tests(tests)
