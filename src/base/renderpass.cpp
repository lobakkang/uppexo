#include <base/renderpass.hpp>
#include <utils/log.hpp>

uppexo::Renderpass::Renderpass(RenderpassBlueprint renderpassBlueprint) {
  uppexo::Log::GetInstance().logInfo("Creating renderpass\n");
  deviceHandle = renderpassBlueprint.device;

  attachmentReference.clear();
  attachmentDescription.clear();

  for (int i = 0; i < renderpassBlueprint.attachment.size(); i++) {
    VkAttachmentDescription attachment{};
    if (renderpassBlueprint.attachment[i].imageFormat.has_value()) {
      attachment.format = renderpassBlueprint.attachment[i].imageFormat.value();
    } else {
      attachment.format = renderpassBlueprint.swapChainImageViewFormat;
    }
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    attachmentDescription.push_back(attachment);

    VkAttachmentReference attachmentRef{};
    attachmentRef.attachment = i;
    attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    attachmentReference.push_back(attachmentRef);
  }

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = attachmentReference.size();
  subpass.pColorAttachments = &attachmentReference[0];

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = attachmentDescription.size();
  renderPassInfo.pAttachments = &attachmentDescription[0];
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;

  if (vkCreateRenderPass(deviceHandle, &renderPassInfo, nullptr, &renderpass) !=
      VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to create render pass!");
  }
}

uppexo::Renderpass::~Renderpass() {
  uppexo::Log::GetInstance().logInfo("Destroying renderpass\n");
  vkDestroyRenderPass(deviceHandle, renderpass, nullptr);
}

VkRenderPass uppexo::Renderpass::getRenderPass() { return renderpass; }
