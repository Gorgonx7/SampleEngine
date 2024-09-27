#pragma once
#include <glfw/glfw3.h>
#include "state.hpp"
namespace interface
{
    namespace config
    {
        void GenerateInstance(GLFWwindow *window, vk_state *state);

    }
    namespace draw
    {
        void Draw(GLFWwindow *window);
    }
}