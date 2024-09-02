#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include "swapchain.hpp"

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};
class UniformBuffer
{
public:
    UniformBuffer(VkDevice device, VkPhysicalDevice physicalDevice, const int MAX_FRAMES_IN_FLIGHT);
    VkBuffer get_buffer(const int buffer_number);
    ~UniformBuffer();
    void updateUniformBuffer(swapchain *vk_swapchain, uint32_t currentImage);

private:
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void *> uniformBuffersMapped;
    VkDevice vk_device;
    void createUniformBuffers(VkDevice device, VkPhysicalDevice physicalDevice, const int MAX_FRAMES_IN_FLIGHT);
};