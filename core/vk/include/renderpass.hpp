#pragma once

#include <vulkan/vulkan.hpp>

class RenderPass
{
public:
    RenderPass(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits msaaSamples, VkDevice device, VkFormat swapChainImageFormat, VkFormat depthFormat);
    ~RenderPass();
    VkRenderPass get_render_pass();

private:
    VkRenderPass renderPass;
    VkDevice device;
    void createRenderPass(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits msaaSamples, VkDevice device, VkFormat swapChainImageFormat, VkFormat depthFormat);
};