#ifndef COMMANDBUFFER_H_
#define COMMANDBUFFER_H_

#include <base/device.hpp>
#include <tuple>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace uppexo {
struct CommandBufferBlueprint {
  int queueID = 0;
  int bufferNum = 1;
  VkDevice device = 0;

  CommandBufferBlueprint() = default;
  CommandBufferBlueprint(Device &device) {
    bufferNum = 1;
    this->device = device.getLogicalDevice();
    this->queueID = device.getQueue(QueueType::graphic).queueFamilyID.value();
  }
};

class CommandBuffer {
public:
  CommandBuffer(CommandBufferBlueprint commandBufferBlueprint);
  ~CommandBuffer();
  VkCommandBuffer &getBuffer(int id);

private:
  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffer;
  VkDevice deviceHandle;
};
} // namespace uppexo

#endif // !COMMANDBUFFER_H_
