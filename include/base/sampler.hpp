#ifndef SAMPLER_HPP_
#define SAMPLER_HPP_

#include <base/device.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace uppexo {
struct SamplerCellBlueprint {
  SamplerCellBlueprint() = default;
  VkBool32 anisotropyEnable = false;
};

struct SamplerBlueprint {
  VkDevice device;
  VkPhysicalDevice physicalDevice;
  std::vector<SamplerCellBlueprint> cellList;

  SamplerBlueprint() = default;
  SamplerBlueprint(Device &device) {
    this->device = device.getLogicalDevice();
    this->physicalDevice = device.getPhysicalDevice();
  }
};

class Sampler {
public:
  Sampler(SamplerBlueprint samplerBlueprint);
  ~Sampler();
  VkSampler getSampler(int id);

private:
  VkDevice device;

  std::vector<VkSampler> samplerList;
};
} // namespace uppexo

#endif
