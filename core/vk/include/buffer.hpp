#pragma once
#include <vulkan/vulkan.hpp>
#include "command.hpp"
void copyBuffer(CommandPool *commandPool, VkQueue graphicsQueue, VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

class Buffer
{
public:
    Buffer();
    Buffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    ~Buffer();
    VkBuffer get_buffer();
    VkDeviceMemory get_buffer_memory();
    void copy_to_buffer(CommandPool *commandPool, VkQueue graphicsQueue, void *data, VkDeviceSize size);
    void map_memory(void *&mapped_memory);

private:
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    VkDevice device;
    VkPhysicalDevice physicalDevice;

    void createBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
};