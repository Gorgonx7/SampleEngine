#pragma once
#include <vulkan/vulkan.hpp>
class DescriptorPool
{
    VkDescriptorPool descriptorPool;

public:
    DescriptorPool(VkDevice device);
    ~DescriptorPool();
    VkDescriptorPool *get_descriptor_pool();
    const int MAX_FRAMES_IN_FLIGHT = 2; // TODO make this global in the config
private:
    VkDevice vk_device;
    void createDescriptorPool(VkDevice device);
};
