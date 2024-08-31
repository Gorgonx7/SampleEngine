cmake_minimum_required(VERSION 3.30)
project("shader_tools")


if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top-level one")
endif()

find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PRIVATE include PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
Vulkan::Vulkan
glm::glm
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ./src/descriptor_sets.cpp
        ./src/shader_module.cpp
        ./src/shader.cpp
        ./src/uniform_buffer.cpp)