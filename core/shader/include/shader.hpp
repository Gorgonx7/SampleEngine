#pragma once
#include <shaderc/shaderc.hpp>
#include <vulkan/vulkan.hpp>

enum shader_type
{
    VERTEX_SHADER,
    FRAGMENT_SHADER
};

// TODO destroy the shader module in the deconstructor
class Shader
{
public:
    Shader(VkDevice device, shader_type type, std::string input);
    VkShaderModule get_shader_module();
    ~Shader();

private:
    VkShaderModule module;
    VkDevice vk_device;
    
    void createShaderModule(VkDevice device, std::vector<uint32_t> code);
    std::vector<uint32_t> buildShader(std::string input, shaderc_shader_kind kind, bool optimize);
};