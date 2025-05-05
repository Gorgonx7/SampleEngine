#pragma once
#include "instance.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "logical_device.hpp"
#include "swapchain.hpp"
#include "renderpass.hpp"
#include "graphics_pipeline.hpp"
#include <GLFW/glfw3.h>
// The state object stores the vulkan state for the entire application
struct vk_state
{
    instance *vk_instance;
    physical_device *vk_physical_device;
    logical_device *vk_logical_device;
    surface *vk_surface;
    swapchain *vk_swapchain;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;
    CommandPool *commandPool;
    RenderPass *vk_render_pass;
    GraphicsPipeline *vk_graphics_pipeline;
    ~vk_state()
    {
        delete vk_swapchain;
        delete vk_graphics_pipeline;

        delete vk_render_pass;
        delete commandPool;
        delete vk_physical_device;
        delete vk_surface;
        delete vk_logical_device;
        delete vk_instance;
    }

public:
    vk_state();
    void create_graphics_pipeline();
};

vk_state *create_state(GLFWwindow *window);
