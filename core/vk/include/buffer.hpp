#pragma once
#include <vulkan/vulkan.hpp>
#include "command.hpp"
void copyBuffer(CommandPool *commandPool, VkQueue graphicsQueue, VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

class Buffer
{
public:
    Buffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    ~Buffer();
    VkBuffer get_buffer();
    VkDeviceMemory get_buffer_memory();

private:
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    VkDevice device;

    void createBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
};