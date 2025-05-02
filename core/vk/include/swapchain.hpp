#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "image.hpp"
struct SwapchainImages
{
    std::vector<Image *> swapChainImages;
    Image *colourImage = nullptr;
    Image *depthImage = nullptr;

public:
    SwapchainImages()
    {
        swapChainImages = {};
    }
    ~SwapchainImages()
    {
        delete colourImage;
        delete depthImage;
        for (auto i : swapChainImages)
        {
            delete i;
        }
    }
};

class swapchain
{
public:
    swapchain(GLFWwindow *window, VkPhysicalDevice physical_device, VkDevice device, VkSurfaceKHR surface, VkSampleCountFlagBits msaaSamples);
    VkSwapchainKHR get_swapchain();
    ~swapchain();
    VkFormat get_format();
    VkFormat get_depth_format();
    VkExtent2D get_extent();
    VkFramebuffer get_framebuffer(int index);
    void createFramebuffers(VkRenderPass renderPass);
    void recreateSwapChain(GLFWwindow *window, VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass);

private:
    VkSwapchainKHR vk_swapchain;
    VkExtent2D swapChainExtent;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    SwapchainImages *images;
    VkFormat swapChainImageFormat;
    VkFormat depthFormat;
    VkDevice vk_device;

    void createSwapChain(GLFWwindow *window, VkPhysicalDevice physicalDevice, VkSurfaceKHR vk_surface);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

    VkExtent2D chooseSwapExtent(GLFWwindow *window, const VkSurfaceCapabilitiesKHR &capabilities);

    void cleanupSwapChain();
    void createColorResources(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits msaaSamples);
    void createDepthResources(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits msaaSamples);
    VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
    VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};