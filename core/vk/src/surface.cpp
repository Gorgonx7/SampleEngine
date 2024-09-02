#include "surface.hpp"
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
surface::surface(VkInstance vk_instance, GLFWwindow *window)
{
    createSurface(vk_instance, window);
    this->vk_instance = vk_instance;
}
VkSurfaceKHR surface::get_surface()
{
    return vk_surface;
}
surface::~surface()
{
    vkDestroySurfaceKHR(vk_instance, vk_surface, nullptr);
}

void surface::createSurface(VkInstance vk_instance, GLFWwindow *window)
{
    if (glfwCreateWindowSurface(vk_instance, window, nullptr, &vk_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}