#ifndef COMMANDBUFFER_H_
#define COMMANDBUFFER_H_

#include <base/device.hpp>
#include <tuple>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace uppexo {
struct CommandBufferBlueprint {
  int queueID = 0;
  VkQueue commandQueue;
  int bufferNum = 1;
  VkDevice device = 0;

  CommandBufferBlueprint() = default;
  CommandBufferBlueprint(Device &device) {
    bufferNum = 1;
    this->device = device.getLogicalDevice();
    this->queueID = device.getQueue(QueueType::graphic).queueFamilyID.value();
    this->commandQueue = device.getQueue(QueueType::graphic).queue[0];
  }

  CommandBufferBlueprint(Device &device, QueueType type) {
    bufferNum = 1;
    this->device = device.getLogicalDevice();
    this->queueID = device.getQueue(type).queueFamilyID.value();
    this->commandQueue = device.getQueue(type).queue[0];
  }
};

class CommandBuffer {
public:
  CommandBuffer(CommandBufferBlueprint commandBufferBlueprint);
  ~CommandBuffer();
  VkCommandBuffer &getBuffer(int id);
  VkCommandBuffer createSingleUseCommandBuffer();
  void submitSingleUseCommandBuffer(VkCommandBuffer singleUseCommandBuffer);

private:
  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffer;
  std::vector<VkCommandBuffer> singleUseCommandBuffer;
  VkDevice deviceHandle;
  VkQueue commandQueue;
};
} // namespace uppexo

#endif // !COMMANDBUFFER_H_
