#pragma once
#include <vulkan/vulkan.hpp>

class physical_device
{
public:
    physical_device(VkInstance instance, VkSurfaceKHR surface);
    VkPhysicalDevice get_device();
    VkSampleCountFlagBits get_msaa_samples();
    ~physical_device();

private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

    void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
    VkSampleCountFlagBits getMaxUsableSampleCount();

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
};