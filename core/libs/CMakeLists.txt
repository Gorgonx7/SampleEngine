add_subdirectory(glfw)
add_subdirectory(glm)

target_sources(${PROJECT_NAME} PRIVATE ./imgui/imgui.cpp
./imgui/imgui_demo.cpp
./imgui/imgui_draw.cpp
./imgui/imgui_tables.cpp
./imgui/imgui_widgets.cpp
./imgui/backends/imgui_impl_glfw.cpp
./imgui/backends/imgui_impl_vulkan.cpp)


## shaderc

set(SHADERC_SKIP_TESTS ON)

set(SHADERC_SKIP_EXAMPLES ON)
set(SHADERC_SKIP_COPYRIGHT_CHECK ON)
set(SHADERC_SPIRV_TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SPIRV-Tools)
set(SHADERC_GLSLANG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glslang)
set(SHADERC_ENABLE_SHARED_CRT ON)
set(SPIRV-Headers_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SPIRV-Headers)
add_subdirectory(shaderc)
