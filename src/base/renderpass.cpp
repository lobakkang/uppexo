#include <base/renderpass.hpp>
#include <utils/log.hpp>
#include <vulkan/vulkan_core.h>

uppexo::Renderpass::Renderpass(RenderpassBlueprint renderpassBlueprint) {
  uppexo::Log::GetInstance().logInfo("Creating renderpass\n");
  deviceHandle = renderpassBlueprint.device;

  attachmentReference.clear();
  attachmentDescription.clear();
  colourAttachmentCount = 0;

  uppexo::Log::GetInstance().logVerbose("Creating attachment\n");
  for (int i = 0; i < renderpassBlueprint.attachment.size(); i++) {
    VkAttachmentDescription attachment{};
    if (renderpassBlueprint.attachment[i].imageFormat != VK_FORMAT_UNDEFINED) {
      attachment.format = renderpassBlueprint.attachment[i].imageFormat;
    } else {
      switch (renderpassBlueprint.attachment[i].purpose) {
      case AttachmentPurpose::SWAPCHAIN_COLOUR:
        attachment.format = renderpassBlueprint.swapChainImageViewFormat;
        break;
      case AttachmentPurpose::DEPTH:
        attachment.format = renderpassBlueprint.depthImageViewFormat;
        break;
      default:
        attachment.format = renderpassBlueprint.swapChainImageViewFormat;
        break;
      }
    }
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp = renderpassBlueprint.attachment[i].imageLoad;
    attachment.storeOp = renderpassBlueprint.attachment[i].imageStore;
    attachment.stencilLoadOp = renderpassBlueprint.attachment[i].stencilLoad;
    attachment.stencilStoreOp = renderpassBlueprint.attachment[i].stencilStore;
    attachment.initialLayout = renderpassBlueprint.attachment[i].initial;
    attachment.finalLayout = renderpassBlueprint.attachment[i].final;

    attachmentDescription.push_back(attachment);

    VkAttachmentReference attachmentRef{};
    attachmentRef.attachment = i;
    attachmentRef.layout = renderpassBlueprint.attachment[i].optimize;

    attachmentReference.push_back(attachmentRef);
  }

  uppexo::Log::GetInstance().logVerbose("Creating subpass\n");
  std::vector<VkSubpassDescription> subpassList;
  for (uppexo::SubpassBlueprint subpassBlueprint :
       renderpassBlueprint.subpass) {
    VkSubpassDescription subpassDesc{};
    subpassDesc.pipelineBindPoint = subpassBlueprint.pipelineType;
    subpassDesc.colorAttachmentCount = subpassBlueprint.colourAttachment.size();

    std::vector<VkAttachmentReference *> colourAttachment;
    for (int id : subpassBlueprint.colourAttachment) {
      colourAttachment.push_back(&attachmentReference[id]);
      colourAttachmentCount++;
    }
    subpassDesc.pColorAttachments = colourAttachment[0];

    if (subpassBlueprint.depthAttachment != -1) {
      subpassDesc.pDepthStencilAttachment =
          &attachmentReference[subpassBlueprint.depthAttachment];
    }
    subpassList.push_back(subpassDesc);
  }

  uppexo::Log::GetInstance().logVerbose("Creating subpass dependency\n");

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                             VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  uppexo::Log::GetInstance().logVerbose(
      "Creating renderpass with %i attachment and %i subpass\n",
      attachmentDescription.size(), subpassList.size());
  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount =
      static_cast<uint32_t>(attachmentDescription.size());
  renderPassInfo.pAttachments = attachmentDescription.data();
  renderPassInfo.subpassCount = static_cast<uint32_t>(subpassList.size());
  renderPassInfo.pSubpasses = subpassList.data();
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

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
int uppexo::Renderpass::getAttachmentCount() {
  return attachmentDescription.size();
};
