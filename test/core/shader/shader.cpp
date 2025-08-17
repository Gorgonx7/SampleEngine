#include <catch2/catch_test_macros.hpp>
#include <shader.hpp>
#include <vk_fake.hpp>
TEST_CASE("shader of 1 and higher are computed (pass)", "[shader-file]")
{
    FakeVulkan *vk = new FakeVulkan();
    // Shader shader = Shader("./test_files/shader.vert");
}