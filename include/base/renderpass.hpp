#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <base/device.hpp>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace uppexo {
struct AttachmentBlueprint {
  std::optional<VkFormat> imageFormat;

  AttachmentBlueprint() = default;
};

struct RenderpassBlueprint {
  VkDevice device;
  VkFormat swapChainImageViewFormat;
  std::vector<AttachmentBlueprint> attachment;

  RenderpassBlueprint() = default;
  RenderpassBlueprint(Device &device) {
    this->device = device.getLogicalDevice();
    this->swapChainImageViewFormat = device.getSwapChainImageFormat();
  }
};

class Renderpass {
public:
  Renderpass(RenderpassBlueprint renderpassBlueprint);
  ~Renderpass();

  VkRenderPass getRenderPass();

private:
  VkDevice deviceHandle;
  VkRenderPass renderpass;

  std::vector<VkAttachmentDescription> attachmentDescription;
  std::vector<VkAttachmentReference> attachmentReference;
};
} // namespace uppexo

#endif // !RENDERPASS_HPP_
