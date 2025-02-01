#include "include/interface.hpp"
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "state.hpp"
#include "queue_family_indicies.hpp"
using namespace interface;
/// @brief VkDescriptorPool, VkRenderPass, VkCommandBuffer, and VkFramebuffer

Interface::Interface(GLFWwindow *window, vk_state *state, DescriptorSet *descriptor_set, int MAX_FRAMES_IN_FLIGHT)
{
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForVulkan(window, true);

    ImGui_ImplVulkan_InitInfo init_info;
    init_info.DescriptorPool = *descriptor_set->get_descriptor_pool();
    init_info.RenderPass = state->vk_render_pass->get_render_pass();
    init_info.Instance = state->vk_instance->get_instance();
    init_info.Device = state->vk_logical_device->get_device();
    init_info.PhysicalDevice = state->vk_physical_device->get_device();
    init_info.MinImageCount = MAX_FRAMES_IN_FLIGHT;
    init_info.MSAASamples = state->vk_physical_device->get_msaa_samples();
    ImGui_ImplVulkan_Init(&init_info);

    ImGui_ImplVulkan_CreateFontsTexture();
    vkDeviceWaitIdle(state->vk_logical_device->get_device());
};

Interface::~Interface()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
};
void Interface::Draw(VkCommandBuffer command_buffer, VkPipeline pipeline)
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), command_buffer, pipeline);
}
