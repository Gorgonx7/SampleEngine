#include <shader.hpp>
#include <vk_fake.hpp>

#include <catch2/catch_test_macros.hpp>
TEST_CASE("Shader abstraction can create a shader from shader.vert using vk_fake", "[vulkan][shader]")
{
    // Setup fake Vulkan environment
    FakeVulkan vk = make_fake_instance_device();
    REQUIRE(vk.vk_logical_device != nullptr);

    // Create a Shader from shader.vert
    Shader shader(vk.vk_logical_device->get_device(), VERTEX_SHADER, "shaders/shader.vert");
    REQUIRE(shader.get_shader_module() != nullptr);
}