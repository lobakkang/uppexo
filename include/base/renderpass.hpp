#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <base/device.hpp>
#include <utils/vulkan_util.hpp>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace uppexo {
enum AttachmentPurpose { SWAPCHAIN_COLOUR, DEPTH, UNKNOWN };

struct AttachmentBlueprint {
  VkFormat imageFormat;
  VkAttachmentLoadOp imageLoad = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  VkAttachmentLoadOp stencilLoad = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  VkAttachmentStoreOp imageStore = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  VkAttachmentStoreOp stencilStore = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  VkImageLayout initial;
  VkImageLayout final;
  VkImageLayout optimize;
  AttachmentPurpose purpose;

  AttachmentBlueprint() = default;
};

namespace presetAttachmentBlueprint {
struct SwapchainAttachment : AttachmentBlueprint {
  SwapchainAttachment(VkFormat format = VK_FORMAT_UNDEFINED) {
    imageFormat = format;
    imageLoad = VK_ATTACHMENT_LOAD_OP_CLEAR;
    imageStore = VK_ATTACHMENT_STORE_OP_STORE;
    stencilLoad = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    stencilStore = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    initial = VK_IMAGE_LAYOUT_UNDEFINED;
    final = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    optimize = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    purpose = AttachmentPurpose::SWAPCHAIN_COLOUR;
  };
};

struct DepthAttachment : AttachmentBlueprint {
  DepthAttachment(VkFormat format = VK_FORMAT_UNDEFINED) {
    imageFormat = format;
    imageLoad = VK_ATTACHMENT_LOAD_OP_CLEAR;
    imageStore = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    stencilLoad = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    stencilStore = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    initial = VK_IMAGE_LAYOUT_UNDEFINED;
    final = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    optimize = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    purpose = AttachmentPurpose::DEPTH;
  };
};
}; // namespace presetAttachmentBlueprint

struct SubpassBlueprint {
  VkPipelineBindPoint pipelineType = VK_PIPELINE_BIND_POINT_GRAPHICS;
  std::vector<int> colourAttachment;
  int depthAttachment = -1;

  SubpassBlueprint() = default;
};

class Renderpass;

struct RenderpassBlueprint {
  using Component = Renderpass;

  VkDevice device;
  VkFormat swapChainImageViewFormat;
  VkFormat depthImageViewFormat;
  std::vector<AttachmentBlueprint> attachment;
  std::vector<SubpassBlueprint> subpass;

  RenderpassBlueprint() = default;
  RenderpassBlueprint(Device &device) {
    this->device = device.getLogicalDevice();
    this->swapChainImageViewFormat = device.getSwapChainImageFormat();
    this->depthImageViewFormat =
        uppexo::findDepthFormat(device.getPhysicalDevice());
  }

  int addAttachment(AttachmentBlueprint attachment) {
    this->attachment.push_back(attachment);
    return this->attachment.size() - 1;
  }

  void addSubpass(std::vector<int> colourAttachment, int depthAttachment = -1) {
    uppexo::SubpassBlueprint subpassBlueprint;
    subpassBlueprint.colourAttachment = colourAttachment;
    subpassBlueprint.depthAttachment = depthAttachment;
    subpass.push_back(subpassBlueprint);
  }
};

class Renderpass {
public:
  Renderpass(RenderpassBlueprint renderpassBlueprint);
  ~Renderpass();

  VkRenderPass getRenderPass();
  int getAttachmentCount();

private:
  VkDevice deviceHandle;
  VkRenderPass renderpass;

  std::vector<VkAttachmentDescription> attachmentDescription;
  std::vector<VkAttachmentReference> attachmentReference;
};
} // namespace uppexo

#endif // !RENDERPASS_HPP_
