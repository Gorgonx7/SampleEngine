cmake_minimum_required(VERSION 3.30)
include(CMakePrintHelpers)
project( SampleEngine
VERSION 1.0
LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(glfw)
add_subdirectory(glm)

set(components
  glslang
  shaderc_combined
  SPIRV-Tools
  volk
  dxc
)

if(APPLE)
  list(APPEND components MoltenVK)
endif()
find_package(Vulkan REQUIRED
  COMPONENTS ${components})
find_library(Vulkan_shaderc_LIBRARY NAMES shaderc PATHS ${VULKAN_SDK}/Lib)
find_library(Vulkan_shaderc_shared_LIBRARY NAMES shaderc_shared PATHS ${VULKAN_SDK}/Lib)
add_executable(${PROJECT_NAME} "main.cpp")



configure_file(shaders/frag.spv frag.spv COPYONLY)
configure_file(shaders/vert.spv vert.spv COPYONLY)
configure_file(shaders/shader.frag shader.frag COPYONLY)
configure_file(shaders/shader.vert shader.vert COPYONLY)
file(COPY textures DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY models DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE include )
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
${Vulkan_LIBRARIES}
glfw
glm::glm
Vulkan::shaderc_combined
glfw
)

target_link_libraries(${PROJECT_NAME} INTERFACE 
${Vulkan_shaderc_LIBRARY}
${Vulkan_shaderc_shared_LIBRARY}
)

add_executable(tests test/test.cpp)
add_library(test_sources OBJECT test/core/shader/shader.cpp)
target_link_libraries(test_sources Catch2::Catch2)
add_subdirectory(Catch2)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain
Vulkan::Vulkan
glfw
glm::glm
test_sources
Vulkan::shaderc_combined
)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)
