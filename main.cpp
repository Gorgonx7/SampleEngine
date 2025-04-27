//
//  main.cpp
//  SampleEngine
//
//  Created by James Gordon on 12/01/2024.
//

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <array>
#include <unordered_map>
#include <state.hpp>
#include <vertex.hpp>
#include <fstream>
#include <interface.hpp>
#include <fs/fs.hpp>
#include <model.hpp>
#include <texture.hpp>
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::string MODEL_PATH = "viking_room.obj";
const std::string TEXTURE_PATH = "viking_room.png";

const int MAX_FRAMES_IN_FLIGHT = 2;

class HelloTriangleApplication
{
public:
    void run()
    {
        initWindow();
        initVulkan();
        interface = new interface::Interface(window, state, MAX_FRAMES_IN_FLIGHT);
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow *window;

    interface::Interface *interface;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    bool framebufferResized = false;

    vk_state *state;
    UniformBuffer *uniform_buffer;
    DescriptorPool *descriptor_pool;
    Model *model;
    Texture *texture;
    void initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    static void framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto app = reinterpret_cast<HelloTriangleApplication *>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }

    void initVulkan()
    {
        state = create_state(window);

        uniform_buffer = new UniformBuffer(state->vk_logical_device->get_device(), state->vk_physical_device->get_device(), MAX_FRAMES_IN_FLIGHT);
        descriptor_pool = new DescriptorPool(state->vk_logical_device->get_device());
        texture = new Texture(state, TEXTURE_PATH);
        model = new Model(MODEL_PATH, state, uniform_buffer, descriptor_pool, texture);
        state->create_graphics_pipeline(model->get_descriptor_set_layout());
        createCommandBuffers(state->vk_logical_device->get_device());
        createSyncObjects(state->vk_logical_device->get_device());
    }

    void mainLoop()
    {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            drawFrame(state);
        }

        vkDeviceWaitIdle(state->vk_logical_device->get_device());
    }

    void cleanup()
    {

        delete descriptor_pool;
        delete model;
        delete uniform_buffer;
        delete interface;
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(state->vk_logical_device->get_device(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(state->vk_logical_device->get_device(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(state->vk_logical_device->get_device(), inFlightFences[i], nullptr);
        }
        delete state;

        glfwDestroyWindow(window);

        glfwTerminate();
    }

    bool hasStencilComponent(VkFormat format)
    {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    void createCommandBuffers(VkDevice device)
    {
        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = state->commandPool->get_command_pool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

        if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void recordCommandBuffer(VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout, swapchain *vk_swapchain, VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = vk_swapchain->get_framebuffer(imageIndex);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = vk_swapchain->get_extent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)vk_swapchain->get_extent().width;
        viewport.height = (float)vk_swapchain->get_extent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = vk_swapchain->get_extent();
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        model->Draw(commandBuffer, pipelineLayout, currentFrame);
        interface->Draw(commandBuffer);
        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void createSyncObjects(VkDevice device)
    {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
    }

    void drawFrame(vk_state *state)
    {
        // interface::draw::Draw(window);
        VkDevice device = state->vk_logical_device->get_device();
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device, state->vk_swapchain->get_swapchain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            state->vk_swapchain->recreateSwapChain(window, state->vk_physical_device->get_device(), state->vk_surface->get_surface(), state->vk_physical_device->get_msaa_samples(), state->vk_render_pass->get_render_pass());
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        vkResetFences(device, 1, &inFlightFences[currentFrame]);
        vkResetCommandBuffer(commandBuffers[currentFrame], 0);
        interface->Render(window);
        uniform_buffer->updateUniformBuffer(state->vk_swapchain, currentFrame);
        recordCommandBuffer(state->vk_graphics_pipeline->get_pipeline(), state->vk_graphics_pipeline->get_pipeline_layout(), state->vk_swapchain, commandBuffers[currentFrame], imageIndex, state->vk_render_pass->get_render_pass());

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(state->graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {state->vk_swapchain->get_swapchain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(state->presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
        {
            framebufferResized = false;
            state->vk_swapchain->recreateSwapChain(window, state->vk_physical_device->get_device(), state->vk_surface->get_surface(), state->vk_physical_device->get_msaa_samples(), state->vk_render_pass->get_render_pass());
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
};

int main(int argc, char *argv[])
{
    std::cout << argv[0] << std::endl;
    HelloTriangleApplication app;

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
