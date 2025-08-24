#include <instance.hpp>
#include <surface.hpp>
#include <device.hpp>
#include <logical_device.hpp>
#include <swapchain.hpp>
#include <renderpass.hpp>
#include <image.hpp>
#include <graphics_pipeline.hpp>
#include <command.hpp>
#include <buffer.hpp>
#include <texture.hpp>
#include <shader_module.hpp>
#include <shader.hpp>
#include <uniform_buffer.hpp>
#include <descriptor_sets.hpp>
#include <descriptor_pool.hpp>
#include <model.hpp>
#include <GLFW/glfw3.h>

struct FakeVulkan
{
    GLFWwindow * window = nullptr;
    instance *vk_instance = nullptr;
    physical_device *vk_physical_device = nullptr;
    logical_device *vk_logical_device = nullptr;
    surface *vk_surface = nullptr;
    swapchain *vk_swapchain = nullptr;
    RenderPass *vk_renderpass = nullptr;
    GraphicsPipeline *vk_graphics_pipeline = nullptr;
    CommandPool *vk_command_pool = nullptr;
    Buffer *vk_buffer = nullptr;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;

    ~FakeVulkan()
    {
        if (vk_instance)
            delete vk_instance;
        if (vk_physical_device)
            delete vk_physical_device;
        if (vk_logical_device)
            delete vk_logical_device;
        if (vk_surface)
            delete vk_surface;
        if (vk_swapchain)
            delete vk_swapchain;
        if (vk_renderpass)
            delete vk_renderpass;
        if (vk_graphics_pipeline)
            delete vk_graphics_pipeline;
        if (vk_command_pool)
            delete vk_command_pool;
        if (vk_buffer)
            delete vk_buffer;
        if (window){
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    }
};
inline GLFWwindow* make_GLFW(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    return glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}
inline FakeVulkan * make_fake_instance_device()
{
    FakeVulkan * vk = new FakeVulkan();
    vk->window = make_GLFW();
    vk->vk_instance = new instance();
    vk->vk_surface = new surface(vk->vk_instance->get_instance(), vk->window);
    vk->vk_physical_device = new physical_device(vk->vk_instance->get_instance(), vk->vk_surface->get_surface());
    vk->vk_logical_device = new logical_device(
        vk->vk_instance,
        vk->vk_physical_device->get_device(),
        vk->vk_surface->get_surface(),
        false,
        &vk->graphicsQueue,
        &vk->presentQueue
    );
    return vk;
}

inline FakeVulkan *make_fake_swapchain_renderpass(GLFWwindow *window)
{
    FakeVulkan * vk = make_fake_instance_device();
    vk->vk_swapchain = new swapchain(window, vk->vk_physical_device->get_device(), vk->vk_logical_device->get_device(), vk->vk_surface->get_surface(), VK_SAMPLE_COUNT_1_BIT);
    vk->vk_renderpass = new RenderPass(vk->vk_physical_device->get_device(), VK_SAMPLE_COUNT_1_BIT, vk->vk_logical_device->get_device(), VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_D32_SFLOAT);
    return vk;
}

inline FakeVulkan * make_fake_full(GLFWwindow *window)
{
    FakeVulkan * vk = make_fake_swapchain_renderpass(window);
    vk->vk_graphics_pipeline = new GraphicsPipeline(
        vk->vk_logical_device->get_device(),

        VK_SAMPLE_COUNT_1_BIT,
        vk->vk_renderpass->get_render_pass(),
    VK_NULL_HANDLE);
    vk->vk_command_pool = new CommandPool(vk->vk_physical_device->get_device(), vk->vk_surface->get_surface(), vk->vk_logical_device->get_device());
    vk->vk_buffer = new Buffer(vk->vk_logical_device->get_device(), vk->vk_physical_device->get_device(), 1024, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    return vk;
}
