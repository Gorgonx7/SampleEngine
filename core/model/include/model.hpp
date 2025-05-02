#pragma once
#include <vector>
#include <buffer.hpp>
#include <state.hpp>
#include <vertex.hpp>
#include <descriptor_sets.hpp>
#include <descriptor_pool.hpp>
#include <uniform_buffer.hpp>
#include <texture.hpp>
class Model
{
public:
    Model(const std::string MODEL_PATH, vk_state *state, UniformBuffer *uniform_buffer, DescriptorPool *descriptor_pool, Texture *texture);
    ~Model();
    void Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t currentFrame);
    VkDescriptorSetLayout get_descriptor_set_layout();

private:
    void LoadModel(std::string MODEL_PATH);
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    Buffer *vertexBuffer;
    Buffer *indexBuffer;
    DescriptorSet *descriptor_set;
    void createVertexBuffer(vk_state *VkState, VkDevice device, VkPhysicalDevice physicalDevice);
    void createIndexBuffer(vk_state *VkState, VkDevice device, VkPhysicalDevice physicalDevice);
};
