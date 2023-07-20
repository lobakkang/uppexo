#ifndef RENDER_COMMAND_HPP_
#define RENDER_COMMAND_HPP_

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <base/renderpass.hpp>
#include <component.hpp>
#include <core/command/general.hpp>
#include <functional>
#include <utils/log.hpp>

#include <base/descriptor.hpp>
#include <base/device.hpp>
#include <base/framebuffer.hpp>
#include <base/graphicPipeline.hpp>

namespace uppexo {
namespace command {
class BeginRenderPass : public Command {
public:
  BeginRenderPass(TrackedBlueprint<RenderpassBlueprint> &renderPass,
                  TrackedBlueprint<FramebufferBlueprint> &framebuffer,
                  TrackedBlueprint<DeviceBlueprint> &device, int &framebufferID)
      : Command() {
    this->renderpass = renderPass.getComponent().getRenderPass();
    this->frameBuffer = &framebuffer.getComponent();
    this->extend = device.getComponent().getSwapChainExtend();
    this->framebufferID = &framebufferID;
  }

  BeginRenderPass(TrackedBlueprint<RenderpassBlueprint> &renderPass,
                  TrackedBlueprint<FramebufferBlueprint> &framebuffer,
                  VkExtent2D extend, int &framebufferID)
      : Command() {
    this->renderpass = renderPass.getComponent().getRenderPass();
    this->frameBuffer = &framebuffer.getComponent();
    this->extend = extend;
    this->framebufferID = &framebufferID;
  }

  void execute(VkCommandBuffer commandbuffer) override {
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderpass;
    renderPassInfo.framebuffer = frameBuffer->getFramebuffer()[*framebufferID];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extend;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandbuffer, &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);
  }

private:
  VkRenderPass renderpass;
  VkExtent2D extend;
  Framebuffer *frameBuffer;
  int *framebufferID;
};

class EndRenderPass : public Command {
public:
  void execute(VkCommandBuffer commandbuffer) override {
    vkCmdEndRenderPass(commandbuffer);
  }
};

class BindGraphicPipeline : public Command {
public:
  BindGraphicPipeline(
      TrackedBlueprint<GraphicPipelineBlueprint> &graphicPipeline)
      : Command() {
    pipeline = graphicPipeline.getComponent().getPipeline();
  }

  void execute(VkCommandBuffer commandbuffer) override {
    vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  }

private:
  VkPipeline pipeline;
};

class SetScissor : public Command {
public:
  SetScissor(VkExtent2D extend) : Command() { this->extend = extend; }
  SetScissor(TrackedBlueprint<DeviceBlueprint> &device) : Command() {
    this->extend = device.getComponent().getSwapChainExtend();
  }
  void execute(VkCommandBuffer commandbuffer) override {
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = extend;
    vkCmdSetScissor(commandbuffer, 0, 1, &scissor);
  }

private:
  VkExtent2D extend;
};

class SetViewport : public Command {
public:
  SetViewport(VkExtent2D extend) : Command() { this->extend = extend; }
  SetViewport(TrackedBlueprint<DeviceBlueprint> &device) : Command() {
    this->extend = device.getComponent().getSwapChainExtend();
  }
  void execute(VkCommandBuffer commandbuffer) override {
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extend.width;
    viewport.height = (float)extend.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandbuffer, 0, 1, &viewport);
  }

private:
  VkExtent2D extend;
};

class BindVertexBuffer : public Command {
public:
  BindVertexBuffer(TrackedBlueprint<BufferBlueprint> &vertexBuffer,
                   int bufferID)
      : Command() {
    buffer = vertexBuffer.getComponent().getBuffer(bufferID);
  }

  void execute(VkCommandBuffer commandbuffer) override {
    VkBuffer vertexBuffers[] = {buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertexBuffers, offsets);
  }

private:
  VkBuffer buffer;
};

class BindGraphicDescriptorSet : public Command {
public:
  BindGraphicDescriptorSet(TrackedBlueprint<DescriptorSetBlueprint> &descriptorSet,
                    TrackedBlueprint<GraphicPipelineBlueprint> &graphicPipeline,
                    int &setID)
      : Command() {
    this->descriptorSet = &descriptorSet.getComponent();
    this->setID = &setID;
    this->layout = graphicPipeline.getComponent().getLayout();
  }

  void execute(VkCommandBuffer commandbuffer) override {
    vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            layout, 0, 1, &descriptorSet->getSet()[*setID], 0,
                            nullptr);
  }

private:
  DescriptorSet *descriptorSet;
  int *setID;
  VkPipelineLayout layout;
};

class IndexedDraw : public Command {
public:
  IndexedDraw(TrackedBlueprint<BufferBlueprint> &buffer, int indexBufferID,
              int vertexCount)
      : Command() {
    this->vertexCount = vertexCount;
    indexBuffer = buffer.getComponent().getBuffer(indexBufferID);
  }

  void execute(VkCommandBuffer commandbuffer) override {
    vkCmdBindIndexBuffer(commandbuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandbuffer, vertexCount, 1, 0, 0, 0);
  }

private:
  int vertexCount;
  VkBuffer indexBuffer;
};

} // namespace command
} // namespace uppexo
//
#endif // !RENDER_COMMAND_HPP_
