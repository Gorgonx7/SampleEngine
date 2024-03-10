cmake_minimum_required(VERSION 3.25)
include(CMakePrintHelpers)
project( SampleEngine
VERSION 1.0
LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(SampleEngine "main.cpp")
configure_file(shaders/frag.spv frag.spv COPYONLY)
configure_file(shaders/vert.spv vert.spv COPYONLY)
configure_file(shaders/shader.frag shader.frag COPYONLY)
configure_file(shaders/shader.vert shader.vert COPYONLY)
file(COPY textures DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY models DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
cmake_print_variables(shaderc_combined)

target_include_directories(SampleEngine PRIVATE include)
target_link_libraries(SampleEngine PRIVATE
Vulkan::Vulkan
${GLFW3_LIBRARY}
glm::glm
${Vulkan_shaderc_combined_LIBRARY}

)


add_executable(tests test/test.cpp)
add_library(test_sources OBJECT test/core/shader/shader.cpp)
target_link_libraries(test_sources Catch2::Catch2)
find_package(Catch2 3 REQUIRED)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain
Vulkan::Vulkan
${GLFW3_LIBRARY}
glm::glm
test_sources
${Vulkan_shaderc_combined_LIBRARY}
)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)
