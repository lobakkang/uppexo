#include "base/buffer.hpp"
#include "core/commandBufferRecorder.hpp"
#include "core/mesh.hpp"
#include <algorithm>
#include <any>
#include <component.hpp>
#include <cstdint>
#include <engine/forward.hpp>
#include <memory>
#include <utility>
#include <utils/log.hpp>
#include <variant>
#include <vector>

#define FRAME_IN_FLIGHT 2
#define IMAGE_AVAILABLE_SEMAPHORE 0
#define RENDER_FINISH_SEMAPHORE 2

uppexo::ForwardRenderingEngine::ForwardRenderingEngine(
    uppexo::ForwardRenderingEngineBlueprint forwardRenderingEngineBlueprint)
    : Engine<ForwardRenderingEngine, ForwardRenderingEngineBlueprint>() {
  this->width = forwardRenderingEngineBlueprint.width;
  this->height = forwardRenderingEngineBlueprint.height;
  this->title = forwardRenderingEngineBlueprint.title;
}

void uppexo::ForwardRenderingEngine::buildComponent() {
  uppexo::Log::GetInstance().logInfo("Building component\n");

  uppexo::InstanceBlueprint instanceBlueprint;
  instanceBlueprint.isValidationLayerEnable = true;
  instanceBlueprint.appName = title;
  instanceBlueprint.isGraphicEnable = true;
  instanceBlueprint.windowWidth = width;
  instanceBlueprint.windowHeight = height;
  addComponent<uppexo::Instance>(instanceBlueprint); // 0

  uppexo::DeviceBlueprint deviceBlueprint(getComponent<uppexo::Instance>(0));
  addComponent<uppexo::Device>(deviceBlueprint); // 1

  uppexo::RenderpassBlueprint renderpassBlueprint(
      getComponent<uppexo::Device>(1));
  uppexo::AttachmentBlueprint colorAttachment;
  renderpassBlueprint.attachment.push_back(colorAttachment);
  addComponent<uppexo::Renderpass>(renderpassBlueprint); // 2

  uppexo::GraphicPipelineBlueprint graphicPipelineBlueprint(
      getComponent<uppexo::Device>(1), getComponent<uppexo::Renderpass>(2));
  graphicPipelineBlueprint.VertexShader = "./demo/shader.vs";
  graphicPipelineBlueprint.FragmentShader = "./demo/shader.fs";
  addComponent<uppexo::GraphicPipeline>(graphicPipelineBlueprint); // 3

  uppexo::FramebufferBlueprint framebufferBlueprint(
      getComponent<uppexo::Device>(1), getComponent<uppexo::Renderpass>(2));
  addComponent<uppexo::Framebuffer>(framebufferBlueprint); // 4

  uppexo::CommandBufferBlueprint commandBufferBlueprint(
      getComponent<uppexo::Device>(1));
  commandBufferBlueprint.bufferNum = FRAME_IN_FLIGHT;
  addComponent<uppexo::CommandBuffer>(commandBufferBlueprint); // 5

  uppexo::SynchronizerBlueprint synchronizerBlueprint(
      getComponent<uppexo::Device>(1));
  synchronizerBlueprint.semaphore = 2 * FRAME_IN_FLIGHT;
  synchronizerBlueprint.fence = 1 * FRAME_IN_FLIGHT;
  synchronizerBlueprint.event = 0;
  addComponent<uppexo::Synchronizer>(synchronizerBlueprint); // 6

  uppexo::BufferBlueprint vertexBufferBlueprint(
      getComponent<uppexo::Device>(1));
  addComponent<uppexo::Buffer>(vertexBufferBlueprint); // 7
}

void uppexo::ForwardRenderingEngine::prerecordCommandBuffer() {
  uppexo::Log::GetInstance().logInfo("Prerecording command buffer\n");
  uppexo::CommandBuffer &commandBuffer = getComponent<uppexo::CommandBuffer>(5);
  uppexo::Device &device = getComponent<uppexo::Device>(1);
  uppexo::Renderpass &renderpass = getComponent<uppexo::Renderpass>(2);
  uppexo::Framebuffer &framebuffer = getComponent<uppexo::Framebuffer>(4);
  uppexo::GraphicPipeline &pipeline = getComponent<uppexo::GraphicPipeline>(3);
  uppexo::Buffer &vertexBuffer = getComponent<uppexo::Buffer>(7);

  recorderList.push_back([&](std::any params) {
    uppexo::command::initializeRecorder(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        renderpass.getRenderPass(),
        framebuffer.getFramebuffer()[std::get<0>(
            std::any_cast<std::tuple<int, int>>(params))],
        device.getSwapChainExtend());
    uppexo::command::bindGraphicPipeline(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        pipeline.getPipeline());
    uppexo::command::bindVertexBuffer(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        vertexBuffer.getBuffer());
    uppexo::command::setViewport(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        device.getSwapChainExtend().width, device.getSwapChainExtend().height);
    uppexo::command::setScissor(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        device.getSwapChainExtend());
    uppexo::command::normalDraw(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        3, 1, 0, 0);
    uppexo::command::deinitializeRecorder(commandBuffer.getBuffer(
        std::get<1>(std::any_cast<std::tuple<int, int>>(params))));
  });
}

void uppexo::ForwardRenderingEngine::run() {
  uppexo::Log::GetInstance().logInfo("Run\n");
  auto &instance = getComponent<uppexo::Instance>(0);
  auto &synchronizer = getComponent<uppexo::Synchronizer>(6);
  auto &device = getComponent<uppexo::Device>(1);
  auto &commandbuffer = getComponent<uppexo::CommandBuffer>(5);
  while (!glfwWindowShouldClose(instance.getWindow())) {
    glfwPollEvents();
    synchronizer.waitForFence({current_frame}, true);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(
        device.getLogicalDevice(), device.getSwapChain(), UINT64_MAX,
        synchronizer.getSemaphore(IMAGE_AVAILABLE_SEMAPHORE + current_frame),
        VK_NULL_HANDLE, &imageIndex);

    recorderList[0](std::tuple<int, int>(imageIndex, current_frame));
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {
        synchronizer.getSemaphore(IMAGE_AVAILABLE_SEMAPHORE + current_frame)};
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandbuffer.getBuffer(current_frame);

    VkSemaphore signalSemaphores[] = {
        synchronizer.getSemaphore(RENDER_FINISH_SEMAPHORE + current_frame)};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(device.getQueue(uppexo::QueueType::graphic).queue[0], 1,
                      &submitInfo,
                      synchronizer.getFence(current_frame)) != VK_SUCCESS) {
      uppexo::Log::GetInstance().logError(
          "Failed to submit draw command buffer!\n");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {device.getSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(device.getQueue(uppexo::QueueType::present).queue[0],
                      &presentInfo);

    current_frame = (current_frame + 1) % FRAME_IN_FLIGHT;
  }
  vkDeviceWaitIdle(device.getLogicalDevice());
}

uppexo::ForwardRenderingEngine::~ForwardRenderingEngine() {}

void uppexo::ForwardRenderingEngine::pushVertexBuffer(
    uppexo::FullVertex *vertex) {
  uppexo::Buffer &vertexBuffer = getComponent<uppexo::Buffer>(7);
  vertexBuffer.copyByMapping(vertex, 3 * sizeof(uppexo::FullVertex));
}
