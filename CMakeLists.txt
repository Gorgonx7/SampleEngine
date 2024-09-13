cmake_minimum_required(VERSION 3.30)
include(CMakePrintHelpers)
project(SampleEngine
VERSION 1.0
LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(glm)
add_subdirectory(core)
find_package(Vulkan REQUIRED)
message(STATUS "Vulkan_shaderc_combined_FOUND: ${Vulkan_shaderc_combined_FOUND}")
add_executable(${PROJECT_NAME} "main.cpp")



configure_file(shaders/frag.spv Debug/frag.spv COPYONLY)
configure_file(shaders/vert.spv Debug/vert.spv COPYONLY)
configure_file(textures/texture.jpg Debug/texture.jpg COPYONLY)
configure_file(textures/viking_room.png Debug/viking_room.png COPYONLY)
configure_file(models/viking_room.obj Debug/viking_room.obj COPYONLY)

target_include_directories(${PROJECT_NAME} PRIVATE include PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
Vulkan::Vulkan
glm::glm
core
)




# add_executable(tests test/test.cpp)
# add_library(test_sources OBJECT test/core/shader/shader.cpp)
# target_link_libraries(test_sources Catch2::Catch2)
# add_subdirectory(Catch2)

# target_link_libraries(tests PRIVATE Catch2::Catch2WithMain
# Vulkan::Vulkan
# glfw
# glm::glm
# test_sources
# Vulkan::shaderc_combined
# )
# list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
# include(CTest)
# include(Catch)
# catch_discover_tests(tests)
