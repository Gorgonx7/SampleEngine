cmake_minimum_required(VERSION 3.30)
include(CMakePrintHelpers)
if (POLICY CMP0091)
	cmake_policy(SET CMP0091 NEW)
endif()
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
project( SampleEngine
VERSION 1.0
LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SHADERC_ENABLE_SHARED_CRT True)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(glfw)
add_subdirectory(glm)

set(components
  shaderc_combined
  SPIRV-Tools
)

if(APPLE)
  list(APPEND components MoltenVK)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

add_subdirectory(shaderc/third_party/glslang)
find_package(Vulkan
  COMPONENTS ${components} REQUIRED)
message(STATUS "Vulkan_shaderc_combined_FOUND: ${Vulkan_shaderc_combined_FOUND}")
add_executable(${PROJECT_NAME} "main.cpp")



configure_file(shaders/frag.spv frag.spv COPYONLY)
configure_file(shaders/vert.spv vert.spv COPYONLY)
configure_file(shaders/shader.frag shader.frag COPYONLY)
configure_file(shaders/shader.vert shader.vert COPYONLY)
file(COPY textures DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY models DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(${PROJECT_NAME} PRIVATE include PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
Vulkan::Vulkan
Vulkan::shaderc_combined
Vulkan::SPIRV-Tools
glfw
glm::glm
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
