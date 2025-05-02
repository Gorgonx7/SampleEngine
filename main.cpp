//
//  main.cpp
//  SampleEngine
//
//  Created by James Gordon on 12/01/2024.
//

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <cstdlib>
#include <state.hpp>
#include <interface.hpp>
#include <model.hpp>
#include <texture.hpp>
#include <engine.hpp>

const std::string MODEL_PATH = "viking_room.obj";
const std::string TEXTURE_PATH = "viking_room.png";

class HelloTriangleApplication : public Engine
{
private:
    interface::Interface *interface;

    UniformBuffer *uniform_buffer;
    DescriptorPool *descriptor_pool;

    Texture *texture;
    void Init() override
    {
        uniform_buffer = new UniformBuffer(state->vk_logical_device->get_device(), state->vk_physical_device->get_device(), MAX_FRAMES_IN_FLIGHT);
        descriptor_pool = new DescriptorPool(state->vk_logical_device->get_device());
        texture = new Texture(state, TEXTURE_PATH);
        model = new Model(MODEL_PATH, state, uniform_buffer, descriptor_pool, texture);
        interface = new interface::Interface(window, state, MAX_FRAMES_IN_FLIGHT);
    }
    void Cleanup() override
    {
        delete descriptor_pool;
        delete model;
        delete texture;
        delete uniform_buffer;
        delete interface;
    }

    void Update() override
    {
        interface->Render(window);
        uniform_buffer->updateUniformBuffer(state->vk_swapchain, currentFrame);
    }
    void Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t currentFrame) override
    {
        model->Draw(commandBuffer, pipelineLayout, currentFrame);
        interface->Draw(commandBuffer);
    }
};

int main(int argc, char *argv[])
{
    std::cout << argv[0] << std::endl;
    HelloTriangleApplication app = HelloTriangleApplication();

    try
    {
        app.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};
