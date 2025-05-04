#pragma once
#include <state.hpp>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <vector>
#include <model.hpp>
#include <swapchain.hpp>

class Engine
{
public:
    Engine();
    ~Engine();
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t currentFrame) = 0;
    virtual void Cleanup() {};

    GLFWwindow *window;
    vk_state *state;
    bool framebufferResized = false;
    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t currentFrame = 0;
    Model *model; // TODO: move to child class
    void Run();

private:
    void mainLoop();
    void initVulkan();
    void initWindow();
    void createSyncObjects(VkDevice device);
    void createCommandBuffers(VkDevice device);
    void recordCommandBuffer(VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout, swapchain *vk_swapchain, VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass);
    void drawFrame(vk_state *state);
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
};