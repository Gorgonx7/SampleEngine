#include "state.hpp"
#include "instance.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "logical_device.hpp"
#include "swapchain.hpp"
#include "renderpass.hpp"
#include "graphics_pipeline.hpp"
#include <GLFW/glfw3.h>
vk_state::vk_state() {}
void vk_state::create_graphics_pipeline(DescriptorSet *descriptor_set)
{
    vk_graphics_pipeline = new GraphicsPipeline(vk_logical_device->get_device(), vk_physical_device->get_msaa_samples(), vk_render_pass->get_render_pass(), descriptor_set);
}

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