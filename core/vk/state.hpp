#pragma once
#include "instance.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "logical_device.hpp"
#include "swapchain.hpp"
#include "renderpass.hpp"
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
    ~vk_state()
    {
        delete vk_swapchain;
        delete vk_render_pass;
        delete commandPool;
        delete vk_physical_device;
        delete vk_surface;
        delete vk_logical_device;
        delete vk_instance;
    }

public:
    vk_state() {}
};

vk_state *create_state(GLFWwindow *window)
{
    vk_state *state = new vk_state();
    state->vk_instance = new instance();
    state->vk_surface = new surface(state->vk_instance->get_instance(), window);
    state->vk_physical_device = new physical_device(state->vk_instance->get_instance(), state->vk_surface->get_surface());
    state->vk_logical_device = new logical_device(state->vk_instance, state->vk_physical_device->get_device(), state->vk_surface->get_surface(), state->vk_instance->are_validation_layers_enabled(), &state->graphicsQueue, &state->presentQueue);
    state->vk_swapchain = new swapchain(window, state->vk_physical_device->get_device(), state->vk_logical_device->get_device(), state->vk_surface->get_surface(), state->vk_physical_device->get_msaa_samples());
    state->commandPool = new CommandPool(state->vk_physical_device->get_device(), state->vk_surface->get_surface(), state->vk_logical_device->get_device());
    state->vk_render_pass = new RenderPass(state->vk_physical_device->get_device(), state->vk_physical_device->get_msaa_samples(), state->vk_logical_device->get_device(), state->vk_swapchain->get_format(), state->vk_swapchain->get_depth_format());
    state->vk_swapchain->createFramebuffers(state->vk_render_pass->get_render_pass());
    return state;
}
