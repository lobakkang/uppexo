#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include <any>
#include <base/computePipeline.hpp>
#include <base/descriptor.hpp>
#include <base/framebuffer.hpp>
#include <base/graphicPipeline.hpp>
#include <component.hpp>
#include <core/present.hpp>
#include <memory>
#include <tuple>
#include <vector>
#include <vulkan/vulkan_core.h>

#include <core/sequence.hpp>

typedef int UppexoHandle;

namespace uppexo {
class Uppexo {
public:
  Uppexo(VkExtent2D windowSize = {0, 0}, std::string title = "UPPEXO",
         bool validationLayer = false);
  ~Uppexo();
  TrackedBlueprint<DeviceBlueprint> addDevice();
  TrackedBlueprint<BufferBlueprint>
  addBuffer(TrackedBlueprint<DeviceBlueprint> &device);
  TrackedBlueprint<SamplerBlueprint>
  addSampler(TrackedBlueprint<DeviceBlueprint> &device);
  TrackedBlueprint<CommandBufferBlueprint>
  addCommandBuffer(TrackedBlueprint<DeviceBlueprint> &device,
                   std::tuple<QueueType, int> queue = {QueueType::graphic, 0});
  TrackedBlueprint<RenderpassBlueprint>
  addRenderPass(TrackedBlueprint<DeviceBlueprint> &device);
  TrackedBlueprint<ImageBlueprint>
  addImage(TrackedBlueprint<DeviceBlueprint> &device,
           TrackedBlueprint<CommandBufferBlueprint> &commandBuffer,
           TrackedBlueprint<BufferBlueprint> &buffer);
  TrackedBlueprint<FramebufferBlueprint>
  addFrameBuffer(TrackedBlueprint<DeviceBlueprint> &device,
                 TrackedBlueprint<RenderpassBlueprint> &renderPass,
                 VkExtent2D size = {0, 0});
  TrackedBlueprint<DescriptorSetBlueprint>
  addDescriptorSet(TrackedBlueprint<DeviceBlueprint> &device);
  TrackedBlueprint<GraphicPipelineBlueprint>
  addGraphicPipeline(TrackedBlueprint<DeviceBlueprint> &device,
                     TrackedBlueprint<RenderpassBlueprint> &renderPass,
                     TrackedBlueprint<DescriptorSetBlueprint> &descriptorSet);
  TrackedBlueprint<ComputePipelineBlueprint>
  addComputePipeline(TrackedBlueprint<DeviceBlueprint> &device,
                     TrackedBlueprint<DescriptorSetBlueprint> &descriptorSet);
  TrackedBlueprint<SynchronizerBlueprint>
  addSynchronizer(TrackedBlueprint<DeviceBlueprint> &device);
  TrackedBlueprint<GuiBlueprint>
  addGui(TrackedBlueprint<DeviceBlueprint> &device,
         TrackedBlueprint<CommandBufferBlueprint> &commandBuffer);

  Sequence &addSequence();
  bool isRunning();

  template <typename T> inline T &getComponent(int id) {
    return *static_cast<T *>(componentList[id].get());
  };

  template <typename T> void sequenceAddCommand(int id, T cmd) {
    sequenceList[id].add(cmd);
  }

  void deleteComponentByID(int id) {
    uppexo::Log::GetInstance().logVerbose("Explicitly deleting component %d\n",
                                          id);
    componentList[id].reset();
    isComponentDeleted[id] = true;
  }

  template <typename T> void deleteComponent(TrackedBlueprint<T> &t) {
    deleteComponentByID(t.componentID);
  };

private:
  // blueprint, component, dependency, completed?
  std::vector<std::unique_ptr<void, void (*)(void *)>> componentList;
  std::vector<bool> isComponentDeleted;
  std::vector<Sequence> sequenceList;

  template <typename Ta, typename Tb> inline int addComponent(Tb &blueprint) {
    componentList.emplace_back(new Ta(blueprint),
                               [](void *p) { delete static_cast<Ta *>(p); });
    isComponentDeleted.push_back(false);
    return componentList.size() - 1;
  }
};
} // namespace uppexo

#endif // !ENGINE_HPP_
