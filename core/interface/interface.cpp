#include "include/interface.hpp"
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "state.hpp"
#include "queue_family_indicies.hpp"
namespace interface
{
    namespace config
    {
        // void GenerateInstance(GLFWwindow *window, vk_state *state)
        // {
        //     // 1: create descriptor pool for IMGUI
        //     // the size of the pool is very oversize, but it's copied from imgui demo itself.
        //     VkDescriptorPoolSize pool_sizes[] =
        //         {
        //             {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
        //             {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
        //             {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
        //             {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
        //             {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
        //             {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
        //             {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
        //             {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
        //             {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
        //             {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
        //             {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

        //     VkDescriptorPoolCreateInfo pool_info = {};
        //     pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        //     pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        //     pool_info.maxSets = 1000;
        //     pool_info.poolSizeCount = std::size(pool_sizes);
        //     pool_info.pPoolSizes = pool_sizes;

        //     VkDescriptorPool imguiPool;
        //     vkCreateDescriptorPool(state->vk_logical_device->get_device(), &pool_info, nullptr, &imguiPool);

        //     IMGUI_CHECKVERSION();
        //     ImGui::CreateContext();
        //     ImGuiIO &io = ImGui::GetIO();
        //     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        //     io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        //     // Setup Platform/Renderer backends
        //     ImGui_ImplGlfw_InitForVulkan(window, true);
        //     ImGui_ImplVulkan_InitInfo init_info = {};
        //     init_info.Instance = state->vk_instance->get_instance();
        //     init_info.PhysicalDevice = state->vk_physical_device->get_device();
        //     init_info.Device = state->vk_logical_device->get_device();
        //     QueueFamilyIndices indicies = findQueueFamilies(state->vk_physical_device->get_device(), state->vk_surface->get_surface());
        //     init_info.QueueFamily = indicies.graphicsFamily.emplace();
        //     init_info.Queue = state->graphicsQueue;
        //     init_info.DescriptorPool = imguiPool;
        //     init_info.MinImageCount = 3;
        //     init_info.ImageCount = 3;
        //     init_info.MSAASamples = VK_SAMPLE_COUNT_8_BIT;
        //     init_info.RenderPass = state->vk_render_pass->get_render_pass();
        //     ImGui_ImplVulkan_Init(&init_info);
        // }

    }
    namespace draw
    {
        void Draw(GLFWwindow *window)
        {

            // imgui new frame
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // imgui commands
            ImGui::ShowDemoWindow();
            ImGui::Render();
        }
    }
}