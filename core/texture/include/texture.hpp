#pragma once
#include <vulkan/vulkan.hpp>
#include <image.hpp>
#include <state.hpp>
class Texture
{
public:
    Texture(vk_state *state, const std::string &texturePath);
    ~Texture();
    void createTextureImage(vk_state *VkState, const std::string &texturePath);
    void createTextureSampler(vk_state *state, uint32_t mipLevels);
    Image *get_texture_image() { return textureImage; }
    VkSampler get_texture_sampler() { return textureSampler; }

private:
    VkSampler textureSampler;
    Image *textureImage;
    uint32_t mipLevels;
    vk_state *state;
};