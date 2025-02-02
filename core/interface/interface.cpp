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
static void check_vk_result(VkResult err)
{
    if (err == VK_SUCCESS)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}
Interface::Interface(GLFWwindow *window, vk_state *state, int MAX_FRAMES_IN_FLIGHT)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForVulkan(window, true);

    // create customer descriptor pool for imgui
    VkDescriptorPoolSize pool_sizes[] =
        {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 0;
    for (VkDescriptorPoolSize &pool_size : pool_sizes)
        pool_info.maxSets += pool_size.descriptorCount;
    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;
    VkDescriptorPool imgui_descriptor_pool;
    VkResult err = vkCreateDescriptorPool(state->vk_logical_device->get_device(), &pool_info, nullptr, &imgui_descriptor_pool);
    check_vk_result(err);

    ImGui_ImplVulkan_InitInfo init_info;
    init_info.DescriptorPool = imgui_descriptor_pool;
    init_info.RenderPass = state->vk_render_pass->get_render_pass();
    init_info.Instance = state->vk_instance->get_instance();
    init_info.Device = state->vk_logical_device->get_device();
    init_info.PhysicalDevice = state->vk_physical_device->get_device();
    init_info.MinImageCount = MAX_FRAMES_IN_FLIGHT;
    init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
    init_info.MSAASamples = state->vk_physical_device->get_msaa_samples();
    init_info.Queue = state->graphicsQueue;
    init_info.CheckVkResultFn = check_vk_result;
    init_info.UseDynamicRendering = false;
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.Subpass = 0;
    init_info.QueueFamily = findQueueFamilies(state->vk_physical_device->get_device(), state->vk_surface->get_surface()).graphicsFamily.value();
    init_info.Allocator = VK_NULL_HANDLE;
    init_info.MinAllocationSize = 0;
    // Debugging statements
    if (init_info.DescriptorPool == VK_NULL_HANDLE)
    {
        std::cerr << "DescriptorPool is null!" << std::endl;
    }
    if (init_info.RenderPass == VK_NULL_HANDLE)
    {
        std::cerr << "RenderPass is null!" << std::endl;
    }
    if (init_info.Instance == VK_NULL_HANDLE)
    {
        std::cerr << "Instance is null!" << std::endl;
    }
    if (init_info.Device == VK_NULL_HANDLE)
    {
        std::cerr << "Device is null!" << std::endl;
    }
    if (init_info.PhysicalDevice == VK_NULL_HANDLE)
    {
        std::cerr << "PhysicalDevice is null!" << std::endl;
    }

    ImGui_ImplVulkan_Init(&init_info);

    // ImGui_ImplVulkan_CreateFontsTexture();
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
