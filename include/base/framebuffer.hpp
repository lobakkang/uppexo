#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <vector>

#include <base/device.hpp>
#include <base/renderpass.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace uppexo {
struct FramebufferBlueprint {
  VkRenderPass renderpass;
  std::vector<VkImageView> imageView;
  VkExtent2D extend;
  VkDevice device;

  FramebufferBlueprint() = default;
  FramebufferBlueprint(Device &device, Renderpass &renderpass) {
    this->device = device.getLogicalDevice();
    this->extend = device.getSwapChainExtend();
    this->imageView = device.getSwapChainImageView();
    this->renderpass = renderpass.getRenderPass();
  }
};

class Framebuffer {
public:
  Framebuffer(FramebufferBlueprint framebufferBlueprint);
  ~Framebuffer();

  std::vector<VkFramebuffer> getFramebuffer();
  int test();

private:
  std::vector<VkFramebuffer> framebuffer;
  VkDevice deviceHandle;
};
} // namespace uppexo

#endif // !FRAMEBUFFER_H_
