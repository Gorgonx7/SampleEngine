#include "model.hpp"
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <fs/fs.hpp>
#include <unordered_map>
#include <vertex.hpp>
#include <buffer.hpp>
#include <descriptor_sets.hpp>
#include <descriptor_pool.hpp>
#include <uniform_buffer.hpp>
#include <vulkan/vulkan.hpp>
#include <texture.hpp>
Model::Model(const std::string MODEL_PATH, vk_state *state, UniformBuffer *uniform_buffer, DescriptorPool *descriptor_pool, Texture *texture)
{
    descriptor_set = new DescriptorSet(state->vk_logical_device->get_device(), uniform_buffer, descriptor_pool, texture->get_texture_image(), texture->get_texture_sampler());
    LoadModel(MODEL_PATH);
    createVertexBuffer(state, state->vk_logical_device->get_device(), state->vk_physical_device->get_device());
    createIndexBuffer(state, state->vk_logical_device->get_device(), state->vk_physical_device->get_device());
}

Model::~Model()
{
    delete descriptor_set;
    delete indexBuffer;
    delete vertexBuffer;
}

VkDescriptorSetLayout Model::get_descriptor_set_layout()
{
    return descriptor_set->get_descriptor_set_layout();
}

void Model::Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t currentFrame)
{

    VkBuffer vertexBuffers[] = {vertexBuffer->get_buffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer->get_buffer(), 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, descriptor_set->get_descriptor_set(currentFrame), 0, nullptr);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void Model::LoadModel(std::string MODEL_PATH)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    fs::exists_test0(MODEL_PATH.c_str());

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str()))
    {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]};

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
}

void Model::createVertexBuffer(vk_state *VkState, VkDevice device, VkPhysicalDevice physicalDevice)
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    Buffer *buffer = new Buffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    buffer->copy_to_buffer(VkState->commandPool, VkState->graphicsQueue, vertices.data(), bufferSize);
    vertexBuffer = new Buffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    copyBuffer(VkState->commandPool, VkState->graphicsQueue, device, buffer->get_buffer(), vertexBuffer->get_buffer(), bufferSize);
    delete buffer;
}

void Model::createIndexBuffer(vk_state *VkState, VkDevice device, VkPhysicalDevice physicalDevice)
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
    Buffer *buffer = new Buffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    buffer->copy_to_buffer(VkState->commandPool, VkState->graphicsQueue, indices.data(), bufferSize);
    indexBuffer = new Buffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    copyBuffer(VkState->commandPool, VkState->graphicsQueue, device, buffer->get_buffer(), indexBuffer->get_buffer(), bufferSize);
    delete buffer;
}