if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

target_sources(${PROJECT_NAME}
    PRIVATE
        ./src/command.cpp
        ./src/buffer.cpp
        ./src/device.cpp
        ./src/graphics_pipeline.cpp
        ./src/image.cpp
        ./src/instance.cpp
        ./src/logical_device.cpp
        ./src/queue_family_indicies.cpp
        ./src/renderpass.cpp
        ./src/state.cpp
        ./src/surface.cpp
        ./src/swapchain_support_details.cpp
        ./src/swapchain.cpp)