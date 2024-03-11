#pragma once
#include <shaderc/shaderc.hpp>
#include <iostream>
#include <vulkan/vulkan.hpp>
#include "../system/fs/fs.hpp"
enum shader_type
{
    VERTEX_SHADER,
    FRAGMENT_SHADER
};

// TODO destroy the shader module in the deconstructor
class Shader
{
public:
    Shader(VkDevice device, shader_type type, std::string input)
    {
        auto vert_shader = fs::readFile(input);
        std::string s(vert_shader.begin(), vert_shader.end());
        std::vector<uint32_t> vertShaderCode;
        if (type == shader_type::VERTEX_SHADER)
        {
            vertShaderCode = Shader::buildShader(s, shaderc_vertex_shader, false);
        }
        else
        {
            vertShaderCode = Shader::buildShader(s, shaderc_fragment_shader, false);
        }
        createShaderModule(device, vertShaderCode);
    }
    VkShaderModule get_shader_module()
    {
        return module;
    }

private:
    VkShaderModule module;
    void createShaderModule(VkDevice device, std::vector<uint32_t> code)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size() * sizeof(unsigned int);
        createInfo.pCode = code.data();

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &module) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create shader module!");
        }
    }
    std::vector<uint32_t> buildShader(std::string input, shaderc_shader_kind kind, bool optimize)
    {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        if (optimize)
        {
            options.SetOptimizationLevel(shaderc_optimization_level_size);
        }
        shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(input, kind, "shader.vert", options);
        if (result.GetCompilationStatus() != shaderc_compilation_status_success)
        {
            std::cerr << result.GetErrorMessage() << std::endl;
            std::cerr << "error compiling shader" << std::endl;
        }
        std::vector<uint32_t> spirv = {result.cbegin(), result.cend()};
        return spirv;
    }
};