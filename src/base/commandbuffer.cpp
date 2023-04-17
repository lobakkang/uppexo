#include <base/commandbuffer.hpp>
#include <utils/log.hpp>
#include <vulkan/vulkan_core.h>

uppexo::CommandBuffer::CommandBuffer(
    uppexo::CommandBufferBlueprint commandBufferBlueprint) {
  uppexo::Log::GetInstance().logInfo("Creating command buffer\n");
  deviceHandle = commandBufferBlueprint.device;

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = commandBufferBlueprint.queueID;

  if (vkCreateCommandPool(deviceHandle, &poolInfo, nullptr, &commandPool) !=
      VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to create command pool!\n");
  }

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = commandBufferBlueprint.bufferNum;

  commandBuffer.clear();
  commandBuffer.resize(commandBufferBlueprint.bufferNum);

  if (vkAllocateCommandBuffers(deviceHandle, &allocInfo, &commandBuffer[0]) !=
      VK_SUCCESS) {
    uppexo::Log::GetInstance().logError(
        "Failed to allocate command buffers!\n");
  }
}

uppexo::CommandBuffer::~CommandBuffer() {
  uppexo::Log::GetInstance().logInfo("Destroying command buffer\n");
  vkDestroyCommandPool(deviceHandle, commandPool, nullptr);
}

VkCommandBuffer &uppexo::CommandBuffer::getBuffer(int id) {
  return commandBuffer[id];
}

/*void uppexo::CommandBuffer::submitBuffer(int id, VkSemaphore semaphore,
                                         VkPipelineStageFlags pipelineStage) {
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {semaphore};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer[id];

  VkSemaphore signalSemaphores[] = {
      synchronizer.getSemaphore(RENDER_FINISH_SEMAPHORE)};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(device.getQueue(uppexo::QueueType::graphic).queue[0], 1,
                    &submitInfo, synchronizer.getFence(0)) != VK_SUCCESS) {
    uppexo::Log::GetInstance().logError(
        "Failed to submit draw command buffer!\n");
  }
}*/
