#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

class instance
{
public:
    instance();
    VkInstance get_instance();
    bool are_validation_layers_enabled();
    uint32_t get_validation_layers_size();
    const char *const *get_validation_layers();
    ~instance();

private:
    VkInstance vk_instance;
    VkDebugUtilsMessengerEXT debugMessenger;
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};
    void createInstance(bool enableValidationLayers);
    bool checkValidationLayerSupport();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    std::vector<const char *> getRequiredExtensions(bool enableValidationLayers);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
    void setupDebugMessenger(bool enableValidationLayers);
};
