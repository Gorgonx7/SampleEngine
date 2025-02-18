#include "uniform_buffer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <chrono>
#include "buffer.hpp"

UniformBuffer::UniformBuffer(VkDevice device, VkPhysicalDevice physicalDevice, const int MAX_FRAMES_IN_FLIGHT)
{
    vk_device = device;
    createUniformBuffers(device, physicalDevice, MAX_FRAMES_IN_FLIGHT);
}

UniformBuffer::~UniformBuffer()
{
}

VkBuffer UniformBuffer::get_buffer(const int buffer_number)
{
    return uniformBuffers[buffer_number].get_buffer();
}

void UniformBuffer::updateUniformBuffer(swapchain *vk_swapchain, uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), vk_swapchain->get_extent().width / (float)vk_swapchain->get_extent().height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void UniformBuffer::createUniformBuffers(VkDevice device, VkPhysicalDevice physicalDevice, const int MAX_FRAMES_IN_FLIGHT)
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        uniformBuffers[i] = Buffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        uniformBuffers[i].map_memory(uniformBuffersMapped[i]);
    }
}