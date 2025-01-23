#pragma once
#include <glfw/glfw3.h>
#include "state.hpp"
#include "renderpass.hpp"
#include "command.hpp"
#include "buffer.hpp"
namespace interface
{
    namespace config
    {
        void GenerateInstance(GLFWwindow *window, vk_state *state);
        class Interface
        {
            /// @brief VkDescriptorPool, VkRenderPass, VkCommandBuffer, and VkFramebuffer

            Interface();
        };
    }
    namespace draw
    {
        void Draw(GLFWwindow *window);
    }
}