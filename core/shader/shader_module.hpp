#pragma once

#include "shader.hpp"
#include <vulkan/vulkan.hpp>
#include "uniform_buffer.hpp"
#include "../vk/image.hpp"

class ShaderModule
{
public:
    ShaderModule(VkDevice device, std::string vert_shader_location, std::string frag_shader_location)
    {
        vk_device = device;
        vertex_shader = new Shader(vk_device, shader_type::VERTEX_SHADER, vert_shader_location);
        fragment_shader = new Shader(vk_device, shader_type::FRAGMENT_SHADER, frag_shader_location);
        vertShaderModule = vertex_shader->get_shader_module();
        fragShaderModule = fragment_shader->get_shader_module();
    }
    ~ShaderModule()
    {
        delete vertex_shader;
        delete fragment_shader;
    }
    VkShaderModule get_vertex_shader_module()
    {
        return vertShaderModule;
    }
    VkShaderModule get_fragment_shader_module()
    {
        return fragShaderModule;
    }
    VkPipelineShaderStageCreateInfo *get_shader_info()
    {
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = get_vertex_shader_module();
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = get_fragment_shader_module();
        fragShaderStageInfo.pName = "main";

        shaderStages[0] = vertShaderStageInfo;
        shaderStages[1] = fragShaderStageInfo;
        return &shaderStages[0];
    }

private:
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    Shader *vertex_shader;
    Shader *fragment_shader;
    VkDevice vk_device;
    VkPipelineShaderStageCreateInfo shaderStages[2];
};