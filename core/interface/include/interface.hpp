#pragma once
#include <glfw/glfw3.h>
#include "state.hpp"
#include "renderpass.hpp"
#include "command.hpp"
#include "buffer.hpp"
namespace interface
{

    // void GenerateInstance(GLFWwindow *window, vk_state *state);
    class Interface
    {
        /// @brief VkDescriptorPool, VkRenderPass, VkCommandBuffer, and VkFramebuffer
    public:
        Interface(GLFWwindow *window, vk_state *state, int MAX_FRAMES_IN_FLIGHT);
        ~Interface();
        void Draw(VkCommandBuffer command_buffer, VkPipeline pipeline);
    };

}