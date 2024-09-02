#pragma once

#include "shader.hpp"
#include <vulkan/vulkan.hpp>


class ShaderModule
{
public:
    ShaderModule(VkDevice device, std::string vert_shader_location, std::string frag_shader_location);
    ~ShaderModule();
    VkShaderModule get_vertex_shader_module();
    VkShaderModule get_fragment_shader_module();
    VkPipelineShaderStageCreateInfo *get_shader_info();

private:
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    Shader *vertex_shader;
    Shader *fragment_shader;
    VkDevice vk_device;
    VkPipelineShaderStageCreateInfo shaderStages[2];
};