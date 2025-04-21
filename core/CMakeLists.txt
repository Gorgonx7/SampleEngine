cmake_minimum_required(VERSION 3.30)
project("core")


if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top-level one")
endif()

find_package(Vulkan REQUIRED)


add_library(${PROJECT_NAME} STATIC)
add_subdirectory(system)
add_subdirectory(vk)
add_subdirectory(shader)
add_subdirectory(interface)
add_subdirectory(libs)

target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS} "./libs/imgui" "libs/imgui/backends" PUBLIC ./vk/include ./vk/models ./system ./shader/include ./config ./interface/include)
target_link_libraries(${PROJECT_NAME} PUBLIC
Vulkan::Vulkan
glfw
shaderc_combined
glm::glm
)

