#pragma once

#include <vulkan/vulkan.hpp>
#include "buffer.hpp"
#include "command.hpp"

class Image
{
public:
    Image(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags flags);
    Image(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags flags, uint32_t mipLevels);
    ~Image();
    VkImage *get_image();
    VkImageView get_view();
    void transitionImageLayout(CommandPool *pool, VkQueue queue, VkDevice device, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
    void copyBufferToImage(CommandPool *pool, VkQueue queue, VkDevice device, Buffer *buffer, uint32_t width, uint32_t height);
    void generateMipmaps(CommandPool *pool, VkQueue queue, VkDevice device, VkPhysicalDevice physicalDevice, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

private:
    VkDevice device;
    VkImage image;
    VkFormat image_format;
    VkImageView view;
    uint32_t mipLevels;
    VkDeviceMemory imageMemory;
    bool destroyable_image = true;

    void create_image(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceMemory &imageMemory);

    void createImageView(VkDevice device, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
};