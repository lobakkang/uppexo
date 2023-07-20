#ifndef GENERAL_COMMAND_HPP_
#define GENERAL_COMMAND_HPP_

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <functional>
#include <utils/log.hpp>

namespace uppexo {
namespace command {
class Command {
public:
  Command() = default;
  virtual void execute(VkCommandBuffer commandBuffer) {
    uppexo::Log::GetInstance().logError("ERROR empty command is executed\n");
  };

  Command& operator=(const Command& other) = delete;
};

class BeginRecorder : public Command {
public:
  void execute(VkCommandBuffer commandbuffer) override {
    vkResetCommandBuffer(commandbuffer, 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandbuffer, &beginInfo) != VK_SUCCESS) {
      uppexo::Log::GetInstance().logError(
          "Failed to begin recording command buffer!\n");
    }
  }
};

class EndRecorder : public Command {
public:
  void execute(VkCommandBuffer commandbuffer) override {
    if (vkEndCommandBuffer(commandbuffer) != VK_SUCCESS) {
      uppexo::Log::GetInstance().logError("Failed to record command buffer!\n");
    }
  }
};

} // namespace command
} // namespace uppexo

#endif // !GENERAL_COMMAND_HPP_
