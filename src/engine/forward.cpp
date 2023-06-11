#include <algorithm>
#include <any>
#include <base/buffer.hpp>
#include <base/commandbuffer.hpp>
#include <base/descriptor.hpp>
#include <base/device.hpp>
#include <base/image.hpp>
#include <base/renderpass.hpp>
#include <base/sampler.hpp>
#include <chrono>
#include <component.hpp>
#include <core/commandBufferRecorder.hpp>
#include <core/datatype.hpp>
#include <core/mesh.hpp>
#include <cstdint>
#include <engine/forward.hpp>
#include <generated/forwardFragmentShader.h>
#include <generated/forwardVertexShader.h>
#include <memory>
#include <thread>
#include <utility>
#include <utils/log.hpp>
#include <variant>
#include <vector>

#define FRAME_IN_FLIGHT 2
#define IMAGE_AVAILABLE_SEMAPHORE 0
#define RENDER_FINISH_SEMAPHORE 2

#define UBO_1 2
#define UBO_2 3

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
  instanceID = addComponent<uppexo::Instance>(instanceBlueprint);

  uppexo::DeviceBlueprint deviceBlueprint(
      getComponent<uppexo::Instance>(instanceID));
  deviceBlueprint.isSwapchainRequire = true;
  deviceID = addComponent<uppexo::Device>(deviceBlueprint);

  uppexo::RenderpassBlueprint renderpassBlueprint(
      getComponent<uppexo::Device>(deviceID));
  uppexo::presetAttachmentBlueprint::SwapchainAttachment colorAttachment;
  uppexo::presetAttachmentBlueprint::DepthAttachment depthAttachment(
      getComponent<uppexo::Device>(deviceID).getPhysicalDevice());
  renderpassBlueprint.attachment.push_back(colorAttachment);
  renderpassBlueprint.attachment.push_back(depthAttachment);
  uppexo::SubpassBlueprint subpassBlueprint;
  subpassBlueprint.colourAttachment = {0};
  subpassBlueprint.depthAttachment = 1;
  renderpassBlueprint.subpass.push_back(subpassBlueprint);
  renderPassID = addComponent<uppexo::Renderpass>(renderpassBlueprint);

  uppexo::BufferBlueprint bufferBlueprint(
      getComponent<uppexo::Device>(deviceID));
  bufferBlueprint.cellList = {
      uppexo::presetBufferCellBlueprint::VBO_at_device(
          1024 * 1024 * sizeof(uppexo::FullVertex)),
      uppexo::presetBufferCellBlueprint::IBO_at_device(1024 * 1024),
      uppexo::presetBufferCellBlueprint::UBO_at_device(sizeof(uppexo::MVP)),
      uppexo::presetBufferCellBlueprint::UBO_at_device(sizeof(uppexo::MVP))};
  bufferID = addComponent<uppexo::Buffer>(bufferBlueprint);

  uppexo::SamplerBlueprint samplerBlueprint(
      getComponent<uppexo::Device>(deviceID));
  samplerBlueprint.cellList = {uppexo::SamplerCellBlueprint()};
  samplerID = addComponent<uppexo::Sampler>(samplerBlueprint);

  uppexo::CommandBufferBlueprint commandBufferBlueprint(
      getComponent<uppexo::Device>(deviceID));
  commandBufferBlueprint.bufferNum = FRAME_IN_FLIGHT;
  commandBufferID = addComponent<uppexo::CommandBuffer>(commandBufferBlueprint);

  uppexo::ImageBlueprint imageBlueprint(
      getComponent<uppexo::Device>(deviceID),
      getComponent<uppexo::Buffer>(bufferID),
      getComponent<uppexo::CommandBuffer>(commandBufferID));
  imageBlueprint.imageCellBlueprint = {
      uppexo::presetImageCellBlueprint::DepthImageCellBlueprint(
          getComponent<uppexo::Device>(deviceID).getPhysicalDevice()),
      uppexo::presetImageCellBlueprint::TextureImageCellBlueprint(
          "viking_room.png")};
  imageID = addComponent<uppexo::Image>(imageBlueprint);

  uppexo::FramebufferBlueprint framebufferBlueprint(
      getComponent<uppexo::Device>(deviceID),
      getComponent<uppexo::Renderpass>(renderPassID));
  framebufferBlueprint.addImageView(
      getComponent<uppexo::Image>(imageID).getImageView(0)[0]);
  framebufferID = addComponent<uppexo::Framebuffer>(framebufferBlueprint);

  uppexo::DescriptorSetBlueprint descriptorSetBlueprint(
      getComponent<uppexo::Device>(deviceID));
  descriptorSetBlueprint.binding = {
      {uppexo::presetDescriptorSetBindingBlueprint::UBO_at_vertex_shader(
           getComponent<uppexo::Buffer>(bufferID), UBO_1, sizeof(uppexo::MVP)),
       uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
           getComponent<uppexo::Sampler>(samplerID),
           getComponent<uppexo::Image>(imageID), 0, 1, 0)},
      {uppexo::presetDescriptorSetBindingBlueprint::UBO_at_vertex_shader(
           getComponent<uppexo::Buffer>(bufferID), UBO_2, sizeof(uppexo::MVP)),
       uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
           getComponent<uppexo::Sampler>(samplerID),
           getComponent<uppexo::Image>(imageID), 0, 1, 0)}};
  descriptorID = addComponent<uppexo::DescriptorSet>(descriptorSetBlueprint);

  uppexo::GraphicPipelineBlueprint graphicPipelineBlueprint(
      getComponent<uppexo::Device>(deviceID),
      getComponent<uppexo::Renderpass>(renderPassID),
      getComponent<uppexo::DescriptorSet>(descriptorID));
  graphicPipelineBlueprint.isDepthEnable = true;
  graphicPipelineBlueprint.directRead = true;

  graphicPipelineBlueprint.VertexShaderCode = (char *)forwardVertexShader;
  graphicPipelineBlueprint.VertexShaderLen = forwardVertexShader_size;
  graphicPipelineBlueprint.FragmentShaderCode = (char *)forwardFragmentShader;
  graphicPipelineBlueprint.FragmentShaderLen = forwardFragmentShader_size;

  pipelineID = addComponent<uppexo::GraphicPipeline>(graphicPipelineBlueprint);

  uppexo::SynchronizerBlueprint synchronizerBlueprint(
      getComponent<uppexo::Device>(deviceID));
  synchronizerBlueprint.semaphore = 2 * FRAME_IN_FLIGHT;
  synchronizerBlueprint.fence = 1 * FRAME_IN_FLIGHT;
  synchronizerBlueprint.event = 0;
  synchronizerID = addComponent<uppexo::Synchronizer>(synchronizerBlueprint);
}

void uppexo::ForwardRenderingEngine::prerecordCommandBuffer() {
  uppexo::Log::GetInstance().logInfo("Prerecording command buffer\n");
  uppexo::CommandBuffer &commandBuffer =
      getComponent<uppexo::CommandBuffer>(commandBufferID);
  uppexo::Device &device = getComponent<uppexo::Device>(deviceID);
  uppexo::Renderpass &renderpass =
      getComponent<uppexo::Renderpass>(renderPassID);
  uppexo::Framebuffer &framebuffer =
      getComponent<uppexo::Framebuffer>(framebufferID);
  uppexo::GraphicPipeline &pipeline =
      getComponent<uppexo::GraphicPipeline>(pipelineID);
  uppexo::Buffer &buffer = getComponent<uppexo::Buffer>(bufferID);
  uppexo::DescriptorSet &DescriptorSet =
      getComponent<uppexo::DescriptorSet>(descriptorID);

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
        buffer.getBuffer(0));
    uppexo::command::setViewport(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        device.getSwapChainExtend().width, device.getSwapChainExtend().height);
    uppexo::command::setScissor(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        device.getSwapChainExtend());
    uppexo::command::bindDescriptorSet(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        DescriptorSet
            .getSet()[std::get<1>(std::any_cast<std::tuple<int, int>>(params))],
        pipeline.getLayout());
    /*uppexo::command::normalDraw(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        vertexCount * 3, 1, 0, 0);*/
    uppexo::command::indexedDraw(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        vertexCount, buffer.getBuffer(1));
    uppexo::command::deinitializeRecorder(commandBuffer.getBuffer(
        std::get<1>(std::any_cast<std::tuple<int, int>>(params))));
  });
}

auto startTime = std::chrono::high_resolution_clock::now();
auto endTime = std::chrono::high_resolution_clock::now();

void uppexo::ForwardRenderingEngine::run() {
  uppexo::Log::GetInstance().logInfo("Run\n");
  auto &instance = getComponent<uppexo::Instance>(instanceID);
  auto &synchronizer = getComponent<uppexo::Synchronizer>(synchronizerID);
  auto &device = getComponent<uppexo::Device>(deviceID);
  auto &commandbuffer = getComponent<uppexo::CommandBuffer>(commandBufferID);
  while (!glfwWindowShouldClose(instance.getWindow())) {
    glfwPollEvents();
    earlyLoopFunction(*this);
    endTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       endTime - startTime)
                       .count();
    startTime = endTime;

    // limit the frame rate to 60 FPS
    const int MAX_FRAME_RATE = 60;
    const int MIN_FRAME_TIME = 1000 / MAX_FRAME_RATE;
    if (elapsed < MIN_FRAME_TIME) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(MIN_FRAME_TIME - elapsed));
    }
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

    getComponent<uppexo::Buffer>(bufferID).copyByMapping(
        2 + current_frame, mesh.getMVPList(),
        mesh.getMVPCount() * sizeof(uppexo::MVP));

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
    // uppexo::Log::GetInstance().logError("END\n");
  }
  vkDeviceWaitIdle(device.getLogicalDevice());
}

uppexo::ForwardRenderingEngine::~ForwardRenderingEngine() {}

void uppexo::ForwardRenderingEngine::update() {
  uppexo::Buffer &buffer = getComponent<uppexo::Buffer>(bufferID);
  buffer.copyByMapping(0, mesh.getVertexList(),
                       mesh.getVertexCount() * sizeof(uppexo::FullVertex));
  buffer.copyByMapping(1, mesh.getIndexList(),
                       mesh.getIndexCount() * sizeof(uint32_t));
  vertexCount = mesh.getIndexCount();
}
