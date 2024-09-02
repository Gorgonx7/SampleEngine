#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
class surface
{
public:
    surface(VkInstance vk_instance, GLFWwindow *window);
    VkSurfaceKHR get_surface();
    ~surface();

private:
    VkSurfaceKHR vk_surface;
    VkInstance vk_instance;
    void createSurface(VkInstance vk_instance, GLFWwindow *window);
};