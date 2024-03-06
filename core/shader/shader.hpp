#pragma once
#include <shaderc/shaderc.hpp>
class Shader
{
    static void buildShader(std::string input)
    {
        shaderc::Compiler compiler;
        shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(input, shaderc_glsl_default_vertex_shader, nullptr);
    }
};