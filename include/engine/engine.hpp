#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include <any>
#include <core/present.hpp>
#include <base/descriptor.hpp>
#include <base/framebuffer.hpp>
#include <base/graphicPipeline.hpp>
#include <component.hpp>
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
  addCommandBuffer(TrackedBlueprint<DeviceBlueprint> &device);
  TrackedBlueprint<RenderpassBlueprint>
  addRenderPass(TrackedBlueprint<DeviceBlueprint> &device);
  TrackedBlueprint<ImageBlueprint>
  addImage(TrackedBlueprint<DeviceBlueprint> &device,
           TrackedBlueprint<CommandBufferBlueprint> &commandBuffer,
           TrackedBlueprint<BufferBlueprint> &buffer);
  TrackedBlueprint<FramebufferBlueprint>
  addFrameBuffer(TrackedBlueprint<DeviceBlueprint> &device,
                 TrackedBlueprint<RenderpassBlueprint> &renderPass);
  TrackedBlueprint<DescriptorSetBlueprint>
  addDescriptorSet(TrackedBlueprint<DeviceBlueprint> &device);
  TrackedBlueprint<GraphicPipelineBlueprint>
  addGraphicPipeline(TrackedBlueprint<DeviceBlueprint> &device,
                     TrackedBlueprint<RenderpassBlueprint> &renderPass,
                     TrackedBlueprint<DescriptorSetBlueprint> &descriptorSet);
  TrackedBlueprint<SynchronizerBlueprint>
  addSynchronizer(TrackedBlueprint<DeviceBlueprint> &device);

  Sequence &addSequence();
  bool isRunning();

  template <typename T> inline T &getComponent(int id) {
    return *static_cast<T *>(componentList[id].get());
  };

private:
  // blueprint, component, dependency, completed?
  std::vector<std::unique_ptr<void, void (*)(void *)>> componentList;
  std::vector<Sequence> sequenceList;

  template <typename Ta, typename Tb> inline int addComponent(Tb &blueprint) {
    componentList.emplace_back(new Ta(blueprint),
                               [](void *p) { delete static_cast<Ta *>(p); });
    return componentList.size() - 1;
  }
};
} // namespace uppexo

#endif // !ENGINE_HPP_
