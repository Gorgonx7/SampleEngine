#pragma once
#include "instance.hpp"
#include "device.hpp"
#include "surface.hpp"
#include <GLFW/glfw3.h>
// The state object stores the vulkan state for the entire application
struct vk_state
{
    instance *inst;
    physical_device *physdev;
    surface *surf;
    ~vk_state()
    {
        delete physdev;
        delete surf;
        delete inst;
    }
};

vk_state *create_state(GLFWwindow *window)
{
    vk_state *state = new vk_state();
    state->inst = new instance();
    state->surf = new surface(state->inst->get_instance(), window);
    state->physdev = new physical_device(state->inst->get_instance(), state->surf->get_surface());
    return state;
}
