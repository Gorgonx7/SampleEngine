#include <vulkan/vulkan.hpp>
#include <instance.hpp>
#include <surface.hpp>
#include <device.hpp>

struct FakeVulkan
{
    instance *i;
    surface *s;
    physical_device *p;
};
VkDevice get_fake_device()
{
    instance *i = new instance();
    surface *s = new surface(i->get_instance(), nullptr);

    return nullptr;
}