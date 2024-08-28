#pragma once
#include "state.hpp"
#include <vulkan/vulkan.hpp>
#include <set>
#include "queue_family_indicies.hpp"
#include "../config/global_static_config.hpp"
class logical_device
{
public:
    logical_device(instance *vk_instance, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, bool validation_layers_enabled, VkQueue *graphicsQueue, VkQueue *presentQueue)
    {
        createLogicalDevice(vk_instance, physicalDevice, surface, validation_layers_enabled, graphicsQueue, presentQueue);
    }
    VkDevice get_device()
    {
        return vk_device;
    }
    ~logical_device()
    {
        vkDestroyDevice(vk_device, nullptr);
    }

private:
    VkDevice vk_device;
    void createLogicalDevice(instance *vk_instance, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, bool validation_layers_enabled, VkQueue *graphicsQueue, VkQueue *presentQueue)
    {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        deviceFeatures.sampleRateShading = VK_TRUE; // enable sample shading feature for the device

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
        createInfo.ppEnabledExtensionNames = device_extensions.data();

        if (validation_layers_enabled)
        {
            createInfo.enabledLayerCount = vk_instance->get_validation_layers_size();
            createInfo.ppEnabledLayerNames = vk_instance->get_validation_layers();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &vk_device) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(vk_device, indices.graphicsFamily.value(), 0, graphicsQueue);
        vkGetDeviceQueue(vk_device, indices.presentFamily.value(), 0, presentQueue);
    }
};