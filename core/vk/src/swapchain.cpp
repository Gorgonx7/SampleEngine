#include "swapchain.hpp"
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "swapchain_support_details.hpp"
#include "queue_family_indicies.hpp"
#include "image.hpp"
#include <limits>
swapchain::swapchain(GLFWwindow *window, VkPhysicalDevice physical_device, VkDevice device, VkSurfaceKHR surface, VkSampleCountFlagBits msaaSamples)
{
    images = new SwapchainImages();
    vk_device = device;
    createSwapChain(window, physical_device, surface);
    createColorResources(physical_device, msaaSamples);
    createDepthResources(physical_device, msaaSamples);
}
VkSwapchainKHR swapchain::get_swapchain()
{
    return vk_swapchain;
}
swapchain::~swapchain()
{
    cleanupSwapChain();
}
VkFormat swapchain::get_format()
{
    return swapChainImageFormat;
}
VkFormat swapchain::get_depth_format()
{
    return depthFormat;
}
VkExtent2D swapchain::get_extent()
{
    return swapChainExtent;
}
VkFramebuffer swapchain::get_framebuffer(int index)
{
    return swapChainFramebuffers[index];
}
void swapchain::createFramebuffers(VkRenderPass renderPass)
{
    swapChainFramebuffers.resize(images->swapChainImages.size());

    for (size_t i = 0; i < images->swapChainImages.size(); i++)
    {
        std::array<VkImageView, 3> attachments = {
            images->colourImage->get_view(),
            images->depthImage->get_view(),
            images->swapChainImages[i]->get_view()};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;
        VkFramebuffer *buffer = &swapChainFramebuffers[i];
        if (vkCreateFramebuffer(vk_device, &framebufferInfo, nullptr, buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}
void swapchain::recreateSwapChain(GLFWwindow *window, VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass)
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(vk_device);

    cleanupSwapChain();
    images = new SwapchainImages();
    createSwapChain(window, physical_device, surface);
    createColorResources(physical_device, msaaSamples);
    createDepthResources(physical_device, msaaSamples);
    createFramebuffers(renderPass);
}

void swapchain::createSwapChain(GLFWwindow *window, VkPhysicalDevice physicalDevice, VkSurfaceKHR vk_surface)
{

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, vk_surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(window, swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vk_surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, vk_surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(vk_device, &createInfo, nullptr, &vk_swapchain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }
    vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &imageCount, nullptr);
    std::vector<VkImage> swapChainImages = std::vector<VkImage>();
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &imageCount, swapChainImages.data());
    for (int i = 0; i < imageCount; i++)
    {
        Image *img = new Image(vk_device, swapChainImages[i], surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        images->swapChainImages.push_back(img);
    }
    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}
VkSurfaceFormatKHR swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    for (const auto &availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    for (const auto &availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D swapchain::chooseSwapExtent(GLFWwindow *window, const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void swapchain::cleanupSwapChain()
{

    for (auto framebuffer : swapChainFramebuffers)
    {
        vkDestroyFramebuffer(vk_device, framebuffer, nullptr);
    }

    delete images;

    vkDestroySwapchainKHR(vk_device, vk_swapchain, nullptr);
}
void swapchain::createColorResources(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits msaaSamples)
{
    VkFormat colorFormat = swapChainImageFormat;
    images->colourImage = new Image(vk_device, physicalDevice, swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, swapChainImageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
}
void swapchain::createDepthResources(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits msaaSamples)
{
    depthFormat = findDepthFormat(physicalDevice);
    images->depthImage = new Image(vk_device, physicalDevice, swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);
}
VkFormat swapchain::findDepthFormat(VkPhysicalDevice physicalDevice)
{
    return findSupportedFormat(physicalDevice,
                               {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                               VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}
VkFormat swapchain::findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}