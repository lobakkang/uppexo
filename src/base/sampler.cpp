#include <base/sampler.hpp>

#include <utils/log.hpp>

uppexo::Sampler::Sampler(uppexo::SamplerBlueprint samplerBlueprint) {
  uppexo::Log::GetInstance().logInfo("Creating %i texture sampler\n",
                                     samplerBlueprint.cellList.size());
  this->device = samplerBlueprint.device;
  VkPhysicalDeviceProperties properties{};
  vkGetPhysicalDeviceProperties(samplerBlueprint.physicalDevice, &properties);

  for (uppexo::SamplerCellBlueprint cellBlueprint : samplerBlueprint.cellList) {
    VkSampler sampler;
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = cellBlueprint.anisotropyEnable;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    if (vkCreateSampler(samplerBlueprint.device, &samplerInfo, nullptr,
                        &sampler) != VK_SUCCESS) {
      uppexo::Log::GetInstance().logError("Failed to create texture sampler\n");
    }
    samplerList.push_back(sampler);
  }
}

uppexo::Sampler::~Sampler() {
  uppexo::Log::GetInstance().logInfo("Destroying texture sampler\n");
  for (VkSampler sampler : samplerList) {
    vkDestroySampler(device, sampler, nullptr);
  }
}

VkSampler uppexo::Sampler::getSampler(int id) { return samplerList[id]; }
