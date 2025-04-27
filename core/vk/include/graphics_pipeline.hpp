#pragma once
#include <vulkan/vulkan.hpp>
#include "descriptor_sets.hpp"

class GraphicsPipeline
{
public:
    GraphicsPipeline(VkDevice device, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, VkDescriptorSetLayout descriptor_set);
    ~GraphicsPipeline();
    VkPipeline get_pipeline();

    VkPipelineLayout get_pipeline_layout();

private:
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkDevice vk_device;
    void createGraphicsPipeline(VkDevice device, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass, VkDescriptorSetLayout descriptor_set);
};