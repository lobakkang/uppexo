#ifndef COMMANDBUFFERRECORDER_H_
#define COMMANDBUFFERRECORDER_H_

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <utils/log.hpp>

#include <array>
#include <functional>
#include <type_traits>
#include <utility>

namespace uppexo {
namespace command {
inline void initializeRecorder(VkCommandBuffer commandbuffer,
                               VkRenderPass renderpass,
                               VkFramebuffer framebuffer, VkExtent2D extend) {
  vkResetCommandBuffer(commandbuffer, 0);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  if (vkBeginCommandBuffer(commandbuffer, &beginInfo) != VK_SUCCESS) {
    uppexo::Log::GetInstance().logError(
        "Failed to begin recording command buffer!\n");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = renderpass;
  renderPassInfo.framebuffer = framebuffer;
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = extend;

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = 2;
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandbuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
};

inline void transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image,
                                  VkFormat format, VkImageLayout oldLayout,
                                  VkImageLayout newLayout) {
  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  VkPipelineStageFlags sourceStage;
  VkPipelineStageFlags destinationStage;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
      newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else {
    uppexo::Log::GetInstance().logError("Unsupported layout transition!\n");
  }

  vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0,
                       nullptr, 0, nullptr, 1, &barrier);
}

inline void copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer,
                              VkImage image, uint32_t width, uint32_t height) {
  VkBufferImageCopy region{};
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;
  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;
  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};

  vkCmdCopyBufferToImage(commandBuffer, buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

inline void bindGraphicPipeline(VkCommandBuffer commandbuffer,
                                VkPipeline graphicPipeline) {
  vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphicPipeline);
}

inline void setScissor(VkCommandBuffer commandbuffer, VkExtent2D extend) {
  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = extend;
  vkCmdSetScissor(commandbuffer, 0, 1, &scissor);
}

inline void setViewport(VkCommandBuffer commandbuffer, int width, int height) {
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)width;
  viewport.height = (float)height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandbuffer, 0, 1, &viewport);
}

inline void normalDraw(VkCommandBuffer commandbuffer, int vertexCount,
                       int instanceCount = 1, int firstVertex = 0,
                       int firstInstance = 0) {
  vkCmdDraw(commandbuffer, vertexCount, instanceCount, firstVertex,
            firstInstance);
}

inline void indexedDraw(VkCommandBuffer commandbuffer, int vertexCount, VkBuffer indexBuffer) {
  vkCmdBindIndexBuffer(commandbuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
  vkCmdDrawIndexed(commandbuffer, vertexCount, 1, 0, 0, 0);
}

inline void bindVertexBuffer(VkCommandBuffer commandbuffer,
                             VkBuffer vertexBuffer) {
  VkBuffer vertexBuffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertexBuffers, offsets);
}

inline void bindDescriptorSet(VkCommandBuffer commandbuffer,
                              VkDescriptorSet set,
                              VkPipelineLayout pipelineLayout) {
  vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipelineLayout, 0, 1, &set, 0, nullptr);
}

inline void deinitializeRecorder(VkCommandBuffer commandbuffer) {
  vkCmdEndRenderPass(commandbuffer);
  if (vkEndCommandBuffer(commandbuffer) != VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to record command buffer!\n");
  }
}

} // namespace command
} // namespace uppexo

#endif
