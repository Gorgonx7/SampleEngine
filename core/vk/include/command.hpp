#pragma once
#include <vulkan/vulkan.hpp>
class CommandPool
{
    VkCommandPool commandPool;
    VkDevice device;

public:
    CommandPool(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurface, VkDevice vkDevice);
    ~CommandPool();

    void createCommandPool(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    VkCommandBuffer beginSingleTimeCommands(VkDevice device);

    void endSingleTimeCommands(VkQueue queue, VkDevice device, VkCommandBuffer commandBuffer);
    VkCommandPool get_command_pool();
};