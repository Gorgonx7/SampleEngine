#pragma once
#include <vulkan/vulkan.hpp>
#include "instance.hpp"
class logical_device
{
public:
    logical_device(instance *vk_instance, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, bool validation_layers_enabled, VkQueue *graphicsQueue, VkQueue *presentQueue);
    VkDevice get_device();
    ~logical_device();

private:
    VkDevice vk_device;
    void createLogicalDevice(instance *vk_instance, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, bool validation_layers_enabled, VkQueue *graphicsQueue, VkQueue *presentQueue);
};