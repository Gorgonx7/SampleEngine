#include "texture.hpp"
#include <vulkan/vulkan.hpp>
#include <state.hpp>
#include <fs/fs.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(vk_state *state, const std::string &texturePath)
{
    this->state = state;
    textureImage = nullptr;
    textureSampler = VK_NULL_HANDLE;
    createTextureImage(state, texturePath);
    createTextureSampler(state, mipLevels);
}
Texture::~Texture()
{
    vkDestroySampler(state->vk_logical_device->get_device(), textureSampler, nullptr);
    delete textureImage;
}
void Texture::createTextureImage(vk_state *state, const std::string &TEXTURE_PATH)
{
    fs::exists_test0(TEXTURE_PATH.c_str());
    int texWidth, texHeight, texChannels;
    stbi_uc *pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;
    mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

    if (!pixels)
    {
        throw std::runtime_error("failed to load texture image!");
    }

    Buffer *buffer = new Buffer(state->vk_logical_device->get_device(), state->vk_physical_device->get_device(), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    buffer->copy_to_buffer(state->commandPool, state->graphicsQueue, pixels, imageSize);

    stbi_image_free(pixels);
    textureImage = new Image(state->vk_logical_device->get_device(), state->vk_physical_device->get_device(), texWidth, texHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
    textureImage->transitionImageLayout(state->commandPool, state->graphicsQueue, state->vk_logical_device->get_device(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
    textureImage->copyBufferToImage(state->commandPool, state->graphicsQueue, state->vk_logical_device->get_device(), buffer, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    // transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps

    textureImage->generateMipmaps(state->commandPool, state->graphicsQueue, state->vk_logical_device->get_device(), state->vk_physical_device->get_device(), VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
    delete buffer;
}

void Texture::createTextureSampler(vk_state *state, uint32_t mipLevels)
{
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(state->vk_physical_device->get_device(), &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = static_cast<float>(mipLevels);
    samplerInfo.mipLodBias = 0.0f;

    if (vkCreateSampler(state->vk_logical_device->get_device(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }
}