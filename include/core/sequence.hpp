#ifndef SEQUENCE_HPP_
#define SEQUENCE_HPP_

#include <base/commandbuffer.hpp>
#include <component.hpp>
#include <core/command/compute.hpp>
#include <core/command/general.hpp>
#include <core/command/render.hpp>
#include <functional>
#include <memory>

namespace uppexo {
class Sequence {
public:
  Sequence();
  void record(TrackedBlueprint<CommandBufferBlueprint> &commandBuffer, int id);
  void
  execute(CommandBuffer &commandBuffer, int commandBufferID, Device &device,
          std::tuple<QueueType, int> queue, Synchronizer &synchronizer,
          std::vector<std::tuple<int, VkPipelineStageFlags>> waitSemaphoresID,
          std::vector<int> signalSemaphoresID, int fenceID);
  void
  execute(TrackedBlueprint<CommandBufferBlueprint> &commandBuffer,
          int commandBufferID, TrackedBlueprint<DeviceBlueprint> &device,
          std::tuple<QueueType, int> queue,
          TrackedBlueprint<SynchronizerBlueprint> &synchronizer,
          std::vector<std::tuple<int, VkPipelineStageFlags>> waitSemaphoresID,
          std::vector<int> signalSemaphoresID, int fenceID) {
    execute(commandBuffer.getComponent(), commandBufferID,
            device.getComponent(), queue, synchronizer.getComponent(),
            waitSemaphoresID, signalSemaphoresID, fenceID);
  }

  template <typename Ta> int add(Ta cmd);

private:
  std::vector<std::shared_ptr<command::Command>> sequence;
};
} // namespace uppexo

#endif // !SEQUENCE_HPP_
