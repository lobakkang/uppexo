#ifndef COMPUTE_COMMAND_HPP_
#define COMPUTE_COMMAND_HPP_

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <base/computePipeline.hpp>
#include <component.hpp>
#include <core/command/general.hpp>
#include <functional>
#include <memory>
#include <utils/log.hpp>

namespace uppexo {
namespace command {
class BindComputePipeline : public Command {
public:
  BindComputePipeline(
      TrackedBlueprint<ComputePipelineBlueprint> &computePipeline)
      : Command() {
    pipeline = computePipeline.getComponent().getPipeline();
  }

  BindComputePipeline(ComputePipeline &computePipeline) : Command() {
    pipeline = computePipeline.getPipeline();
  }

  void execute(VkCommandBuffer commandbuffer) override {
    vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
  }

private:
  VkPipeline pipeline;
};

class BindComputeDescriptorSet : public Command {
public:
  BindComputeDescriptorSet(
      TrackedBlueprint<DescriptorSetBlueprint> &descriptorSet,
      TrackedBlueprint<ComputePipelineBlueprint> &computePipeline, int &setID)
      : Command(), setID(&setID) {
    this->descriptorSet = &descriptorSet.getComponent();
    this->layout = computePipeline.getComponent().getLayout();
  }

  BindComputeDescriptorSet(
      TrackedBlueprint<DescriptorSetBlueprint> &descriptorSet,
      TrackedBlueprint<ComputePipelineBlueprint> &computePipeline, int setID)
      : Command(), setID(new int(setID)) {
    this->descriptorSet = &descriptorSet.getComponent();
    this->layout = computePipeline.getComponent().getLayout();
  }

  BindComputeDescriptorSet(DescriptorSet &descriptorSet,
                           ComputePipeline &computePipeline, int &setID)
      : Command(), setID(&setID) {
    this->descriptorSet = &descriptorSet;
    this->layout = computePipeline.getLayout();
  }

  BindComputeDescriptorSet(DescriptorSet &descriptorSet,
                           ComputePipeline &computePipeline, int setID)
      : Command(), setID(new int(setID)) {
    this->descriptorSet = &descriptorSet;
    this->layout = computePipeline.getLayout();
  }

  void execute(VkCommandBuffer commandbuffer) override {
    vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                            layout, 0, 1,
                            &descriptorSet->getSet()[*setID.get()], 0, nullptr);
    // uppexo::Log::GetInstance().logInfo("ID %d\n", *setID.get());
  }

private:
  DescriptorSet *descriptorSet;
  std::shared_ptr<int> setID;
  VkPipelineLayout layout;
};

class DispatchCompute : public Command {
public:
  DispatchCompute(int x, int y, int z) : Command() {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  void execute(VkCommandBuffer commandbuffer) override {
    vkCmdDispatch(commandbuffer, x, y, z);
  }

private:
  int x, y, z;
};
} // namespace command
} // namespace uppexo

#endif
