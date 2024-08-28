#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
class surface
{
public:
    surface(VkInstance vk_instance, GLFWwindow *window)
    {
        createSurface(vk_instance, window);
        this->vk_instance = vk_instance;
    }
    VkSurfaceKHR get_surface()
    {
        return vk_surface;
    }
    ~surface()
    {
        vkDestroySurfaceKHR(vk_instance, vk_surface, nullptr);
    }

private:
    VkSurfaceKHR vk_surface;
    VkInstance vk_instance;
    void createSurface(VkInstance vk_instance, GLFWwindow *window)
    {
        if (glfwCreateWindowSurface(vk_instance, window, nullptr, &vk_surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }
};