#include "base/buffer.hpp"
#include "base/commandbuffer.hpp"
#include "base/synchronizer.hpp"
#include "core/commandBufferRecorder.hpp"
#include <any>
#include <base/computePipeline.hpp>
#include <base/device.hpp>
#include <base/instance.hpp>
#include <engine/compute.hpp>
//#include <generated/computeShader.h>
#include <vulkan/vulkan_core.h>

int deviceID;
int instanceID;
int bufferID;
int pipelineID;
int descriptorID;
int commandBufferID;
int synchronizerID;

uppexo::ComputeEngine::ComputeEngine(
    uppexo::ComputeEngineBlueprint computeEngineBlueprint)
    : Engine<ComputeEngine, ComputeEngineBlueprint>() {
  this->title = computeEngineBlueprint.title;
  this->memorySize = computeEngineBlueprint.memorySize;
}

void uppexo::ComputeEngine::buildComponent() {
  uppexo::Log::GetInstance().logInfo("Building component\n");

  uppexo::InstanceBlueprint instanceBlueprint;
  instanceBlueprint.isValidationLayerEnable = true;
  instanceBlueprint.appName = title;
  instanceBlueprint.isGraphicEnable = false;
  instanceBlueprint.windowWidth = 100;
  instanceBlueprint.windowHeight = 100;
  instanceID = addComponent<uppexo::Instance>(instanceBlueprint);

  uppexo::DeviceBlueprint deviceBlueprint(
      getComponent<uppexo::Instance>(instanceID));
  deviceBlueprint.graphicQueue = 0;
  deviceBlueprint.presentQueue = 0;
  deviceBlueprint.sparseQueue = 0;
  deviceBlueprint.computeQueue = 1;
  deviceBlueprint.isSwapchainRequire = false;
  deviceID = addComponent<uppexo::Device>(deviceBlueprint);

  uppexo::BufferBlueprint bufferBlueprint(
      getComponent<uppexo::Device>(deviceID));
  bufferBlueprint.cellList = {
      uppexo::presetBufferCellBlueprint::SSBO_at_device(memorySize),
      uppexo::presetBufferCellBlueprint::SSBO_at_device(memorySize)};
  bufferID = addComponent<uppexo::Buffer>(bufferBlueprint);

  uppexo::DescriptorSetBlueprint descriptorSetBlueprint(
      getComponent<uppexo::Device>(deviceID));
  descriptorSetBlueprint.binding = {
      {uppexo::presetDescriptorSetBindingBlueprint::SSBO_at_compute_shader(
           getComponent<uppexo::Buffer>(bufferID), 0, memorySize),
       uppexo::presetDescriptorSetBindingBlueprint::SSBO_at_compute_shader(
           getComponent<uppexo::Buffer>(bufferID), 1, memorySize)}};
  descriptorID = addComponent<uppexo::DescriptorSet>(descriptorSetBlueprint);

  uppexo::ComputePipelineBlueprint pipelineBlueprint(
      getComponent<uppexo::Device>(deviceID),
      getComponent<uppexo::DescriptorSet>(descriptorID));
  pipelineBlueprint.directRead = false;
  pipelineBlueprint.ComputeShader = "shaders/computeShader.spv";
  //pipelineBlueprint.ComputeShaderCode = (char *)computeShader;
  //pipelineBlueprint.ComputeShaderLen = computeShader_size;
  pipelineID = addComponent<uppexo::ComputePipeline>(pipelineBlueprint);

  uppexo::SynchronizerBlueprint synchronizerBlueprint(
      getComponent<uppexo::Device>(deviceID));
  synchronizerBlueprint.semaphore = 0;
  synchronizerBlueprint.fence = 1;
  synchronizerBlueprint.event = 0;
  synchronizerID = addComponent<uppexo::Synchronizer>(synchronizerBlueprint);

  uppexo::CommandBufferBlueprint commandBufferBlueprint(
      getComponent<uppexo::Device>(deviceID), QueueType::compute);
  commandBufferBlueprint.bufferNum = 1;
  commandBufferID = addComponent<uppexo::CommandBuffer>(commandBufferBlueprint);
}

void uppexo::ComputeEngine::prerecordCommandBuffer() {
  uppexo::Log::GetInstance().logInfo("Prerecording command buffer\n");
  uppexo::CommandBuffer &commandBuffer =
      getComponent<uppexo::CommandBuffer>(commandBufferID);
  uppexo::Device &device = getComponent<uppexo::Device>(deviceID);
  uppexo::ComputePipeline &pipeline =
      getComponent<uppexo::ComputePipeline>(pipelineID);
  uppexo::Buffer &buffer = getComponent<uppexo::Buffer>(bufferID);
  uppexo::DescriptorSet &DescriptorSet =
      getComponent<uppexo::DescriptorSet>(descriptorID);

  recorderList.push_back([&](std::any params) {
    uppexo::command::initializeComputeRecorder(commandBuffer.getBuffer(
        std::get<1>(std::any_cast<std::tuple<int, int>>(params))));
    uppexo::command::bindComputePipeline(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        pipeline.getPipeline());
    uppexo::command::bindComputeDescriptorSet(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        DescriptorSet
            .getSet()[std::get<1>(std::any_cast<std::tuple<int, int>>(params))],
        pipeline.getLayout());
    uppexo::command::dispatchCompute(
        commandBuffer.getBuffer(
            std::get<1>(std::any_cast<std::tuple<int, int>>(params))),
        1, 1, 1);
    uppexo::command::deinitializeComputeRecorder(commandBuffer.getBuffer(
        std::get<1>(std::any_cast<std::tuple<int, int>>(params))));
  });
}

void uppexo::ComputeEngine::run() {
  uppexo::Log::GetInstance().logInfo("Run\n");
  auto &device = getComponent<uppexo::Device>(deviceID);
  auto &commandbuffer = getComponent<uppexo::CommandBuffer>(commandBufferID);
  auto &synchronizer = getComponent<uppexo::Synchronizer>(synchronizerID);
  recorderList[0](std::tuple<int, int>(0, 0));

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandbuffer.getBuffer(0);

  vkResetFences(device.getLogicalDevice(), 1, &synchronizer.getFence(0));
  if (vkQueueSubmit(device.getQueue(uppexo::QueueType::compute).queue[0], 1,
                    &submitInfo, synchronizer.getFence(0)) != VK_SUCCESS) {
    uppexo::Log::GetInstance().logError(
        "Failed to submit draw command buffer!\n");
  }
  synchronizer.waitForFence({0}, true);

  vkDeviceWaitIdle(device.getLogicalDevice());
}

uppexo::ComputeEngine::~ComputeEngine() {}

void uppexo::ComputeEngine::pushMemory(void *addr, size_t len) {
  getComponent<uppexo::Buffer>(bufferID).copyByMapping(0, addr, len);
}

void uppexo::ComputeEngine::pullMemory(void *addr, size_t len) {
  getComponent<uppexo::Buffer>(bufferID).copyOutByMapping(1, addr, len);
}
