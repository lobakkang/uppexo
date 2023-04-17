#include <base/framebuffer.hpp>
#include <utils/log.hpp>

uppexo::Framebuffer::Framebuffer(
    uppexo::FramebufferBlueprint framebufferBlueprint) {
  uppexo::Log::GetInstance().logInfo("Creating Framebuffer\n");

  deviceHandle = framebufferBlueprint.device;
  framebuffer.resize(framebufferBlueprint.imageView.size());

  for (size_t i = 0; i < framebufferBlueprint.imageView.size(); i++) {
    VkImageView attachments[] = {framebufferBlueprint.imageView[i]};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = framebufferBlueprint.renderpass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = framebufferBlueprint.extend.width;
    framebufferInfo.height = framebufferBlueprint.extend.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(framebufferBlueprint.device, &framebufferInfo,
                            nullptr, &framebuffer[i]) != VK_SUCCESS) {
      uppexo::Log::GetInstance().logError("Failed to create framebuffer!\n");
    }
  }
}

uppexo::Framebuffer::~Framebuffer() {
  uppexo::Log::GetInstance().logInfo("Deallocating Framebuffer\n");
  for (auto framebuffer : framebuffer) {
    vkDestroyFramebuffer(deviceHandle, framebuffer, nullptr);
  }
}

int uppexo::Framebuffer::test() { return framebuffer.size(); }

std::vector<VkFramebuffer> uppexo::Framebuffer::getFramebuffer() {
  return framebuffer;
}
