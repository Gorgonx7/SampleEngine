cmake_minimum_required(VERSION 3.25)

project( SampleEngine
VERSION 1.0
LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(SampleEngine "main.cpp")
configure_file(shaders/frag.spv frag.spv COPYONLY)
configure_file(shaders/vert.spv vert.spv COPYONLY)
file(COPY textures DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY models DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")


target_include_directories(SampleEngine PRIVATE include)
target_link_libraries(SampleEngine PRIVATE
Vulkan::Vulkan
${GLFW3_LIBRARY}
glm::glm
)