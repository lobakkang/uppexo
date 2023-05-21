#include <base/commandbuffer.hpp>
#include <utils/log.hpp>
#include <vulkan/vulkan_core.h>

uppexo::CommandBuffer::CommandBuffer(
    uppexo::CommandBufferBlueprint commandBufferBlueprint) {
  uppexo::Log::GetInstance().logInfo("Creating command buffer\n");
  deviceHandle = commandBufferBlueprint.device;
  commandQueue = commandBufferBlueprint.commandQueue;

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

VkCommandBuffer uppexo::CommandBuffer::createSingleUseCommandBuffer() {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = commandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(deviceHandle, &allocInfo, &commandBuffer);
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vkBeginCommandBuffer(commandBuffer, &beginInfo);
  // singleUseCommandBuffer.push_back(commandBuffer);

  return commandBuffer;
}

void uppexo::CommandBuffer::submitSingleUseCommandBuffer(
    VkCommandBuffer singleUseCommandBuffer) {
  vkEndCommandBuffer(singleUseCommandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &singleUseCommandBuffer;

  vkQueueSubmit(commandQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(commandQueue);

  vkFreeCommandBuffers(deviceHandle, commandPool, 1, &singleUseCommandBuffer);
}
