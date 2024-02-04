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
#include <core/renderdocCapture.hpp>

typedef int UppexoHandle;

namespace uppexo {
typedef TrackedBlueprint<DeviceBlueprint> TrkDeviceBlueprint;
typedef TrackedBlueprint<BufferBlueprint> TrkBufferBlueprint;
typedef TrackedBlueprint<SamplerBlueprint> TrkSamplerBlueprint;
typedef TrackedBlueprint<CommandBufferBlueprint> TrkCommandBufferBlueprint;
typedef TrackedBlueprint<RenderpassBlueprint> TrkRenderpassBlueprint;
typedef TrackedBlueprint<GraphicPipelineBlueprint> TrkGraphicPipelineBlueprint;
typedef TrackedBlueprint<DescriptorSetBlueprint> TrkDescriptorSetBlueprint;
typedef TrackedBlueprint<GuiBlueprint> TrkGuiBlueprint;
typedef TrackedBlueprint<SynchronizerBlueprint> TrkSynchronizerBlueprint;
typedef TrackedBlueprint<ComputePipelineBlueprint> TrkComputePipelineBlueprint;
typedef TrackedBlueprint<ImageBlueprint> TrkImageBlueprint;
typedef TrackedBlueprint<FramebufferBlueprint> TrkFramebufferBlueprint;

struct UppexoExecutionInfo {
  int guiID = -1;
};

class Uppexo {
public:
  Uppexo(VkExtent2D windowSize = {0, 0}, std::string title = "UPPEXO",
         bool validationLayer = false);
  ~Uppexo();

  std::function<void(long)> preRender;
  std::function<void(long, int)> onRender = {};
  std::function<void(long)> postRender;

  void setOnRenderFunc(std::function<void(long, int)> onRenderFunc);

  TrkDeviceBlueprint addDevice();
  TrkBufferBlueprint addBuffer(TrkDeviceBlueprint &device);
  TrkSamplerBlueprint addSampler(TrkDeviceBlueprint &device);
  TrkCommandBufferBlueprint
  addCommandBuffer(TrkDeviceBlueprint &device,
                   std::tuple<QueueType, int> queue = {QueueType::graphic, 0});
  TrkRenderpassBlueprint addRenderPass(TrkDeviceBlueprint &device);
  TrkImageBlueprint addImage(TrkDeviceBlueprint &device,
                             TrkCommandBufferBlueprint &commandBuffer,
                             TrkBufferBlueprint &buffer);
  TrkFramebufferBlueprint addFrameBuffer(TrkDeviceBlueprint &device,
                                         TrkRenderpassBlueprint &renderPass,
                                         VkExtent2D size = {0, 0});
  TrkDescriptorSetBlueprint addDescriptorSet(TrkDeviceBlueprint &device);
  TrkGraphicPipelineBlueprint
  addGraphicPipeline(TrkDeviceBlueprint &device,
                     TrkRenderpassBlueprint &renderPass,
                     TrkDescriptorSetBlueprint &descriptorSet);
  TrkComputePipelineBlueprint
  addComputePipeline(TrkDeviceBlueprint &device,
                     TrkDescriptorSetBlueprint &descriptorSet);
  TrkSynchronizerBlueprint addSynchronizer(TrkDeviceBlueprint &device);
  TrkGuiBlueprint addGui(TrkDeviceBlueprint &device,
                         TrkCommandBufferBlueprint &commandBuffer);

  Sequence &addSequence();
  bool isRunning();
  void run(TrkGuiBlueprint &gui, TrkSynchronizerBlueprint &synchronizer,
           TrkCommandBufferBlueprint &commandBuffer, TrkDeviceBlueprint &device,
           Sequence &sequence, std::tuple<QueueType, int> queue);

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

  Sequence &getSequence(int id) { return sequenceList[id]; }

  int &getFrame() { return frame; }
  int &getImageIndex() { return imageIndex; }

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

  int frame = 0;
  int imageIndex = 0;
};
} // namespace uppexo

#endif // !ENGINE_HPP_
