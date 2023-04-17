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

  VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandbuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
};

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

inline void bindVertexBuffer(VkCommandBuffer commandbuffer, VkBuffer vertexBuffer) {
  VkBuffer vertexBuffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertexBuffers, offsets);
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
