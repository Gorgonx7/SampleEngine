#include "buffer.hpp"
#include <vulkan/vulkan.hpp>

void copyBuffer(CommandPool *commandPool, VkQueue graphicsQueue, VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = commandPool->beginSingleTimeCommands(device);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    commandPool->endSingleTimeCommands(graphicsQueue, device, commandBuffer);
}

uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void Buffer::createBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}
Buffer::Buffer()
{
}
Buffer::Buffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
    this->device = device;
    this->physicalDevice = physicalDevice;
    createBuffer(device, physicalDevice, size, usage, properties, buffer, bufferMemory);
}

Buffer::~Buffer()
{
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, bufferMemory, nullptr);
}

VkBuffer Buffer::get_buffer()
{
    return buffer;
}

VkDeviceMemory Buffer::get_buffer_memory()
{
    return bufferMemory;
}

void Buffer::copy_to_buffer(CommandPool *commandPool, VkQueue graphicsQueue, void *data, VkDeviceSize size)
{

    void *stagingData;
    vkMapMemory(device, bufferMemory, 0, size, 0, &stagingData);
    memcpy(stagingData, data, static_cast<size_t>(size));
    vkUnmapMemory(device, bufferMemory);
}

void Buffer::map_memory(void *mapped_memory)
{
    vkMapMemory(device, bufferMemory, 0, VK_WHOLE_SIZE, 0, &mapped_memory);
}
