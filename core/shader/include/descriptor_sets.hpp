#pragma once
#include <vulkan/vulkan.hpp>
#include "uniform_buffer.hpp"
#include "image.hpp"

class DescriptorSet
{
public:
    DescriptorSet(VkDevice device, UniformBuffer *buffer, Image *textureImage, VkSampler sampler);

    VkDescriptorSetLayout *get_descriptor_set_layout();
    VkDescriptorSet *get_descriptor_set(const int index);
    VkDescriptorPool *get_descriptor_pool();
    ~DescriptorSet();

private:
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    const int MAX_FRAMES_IN_FLIGHT = 2; // TODO make this global in the config
    VkDevice vk_device;

    void createDescriptorSetLayout();

    void createDescriptorPool(VkDevice device);

    void createDescriptorSets(VkDevice device, UniformBuffer *buffer, Image *textureImage, VkSampler sampler);
};