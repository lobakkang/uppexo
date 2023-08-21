#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <vector>

#include <utils/log.hpp>

#include <base/device.hpp>
#include <base/image.hpp>
#include <base/renderpass.hpp>
#include <component.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace uppexo {
class Framebuffer;

struct FramebufferBlueprint {
  using Component = Framebuffer;

  VkRenderPass renderpass;
  std::vector<std::vector<VkImageView>> imageView;
  VkExtent2D extend;
  VkDevice device;

  FramebufferBlueprint() = default;
  FramebufferBlueprint(Device &device, Renderpass &renderpass) {
    this->imageView.resize(device.getSwapChainImageView().size());
    this->device = device.getLogicalDevice();
    this->extend = device.getSwapChainExtend();
    int i = 0;
    for (VkImageView &view : device.getSwapChainImageView()) {
      this->imageView[i] = {view};
      i++;
    }
    this->renderpass = renderpass.getRenderPass();
  }

  FramebufferBlueprint(Device &device, Renderpass &renderpass,
                       VkExtent2D extend) {
    this->imageView.resize(device.getSwapChainImageView().size());
    if (this->imageView.size() == 0) {
      this->imageView.resize(1);
    }
    this->device = device.getLogicalDevice();
    if (extend.width == 0 && extend.height == 0) {
      this->extend = device.getSwapChainExtend();
    } else {
      this->extend = extend;
    }
    int i = 0;
    for (VkImageView &view : device.getSwapChainImageView()) {
      this->imageView[i] = {view};
      i++;
    }
    this->renderpass = renderpass.getRenderPass();
  }

  void addImageView(TrackedBlueprint<ImageBlueprint> &image, int imageId,
                    int id) {
    for (int i = 0; i < this->imageView.size(); i++) {
      // this->imageView[i].push_back(image .getImageView(imageId)[id]);
      this->imageView[i].push_back(
          image.getComponent().getImageView(imageId)[id]);
    }
  }
};

class Framebuffer {
public:
  Framebuffer(FramebufferBlueprint framebufferBlueprint);
  ~Framebuffer();

  std::vector<VkFramebuffer> getFramebuffer();

private:
  std::vector<VkFramebuffer> framebuffer;
  VkDevice deviceHandle;
};
} // namespace uppexo

#endif // !FRAMEBUFFER_H_
