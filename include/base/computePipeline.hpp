#ifndef COMPUTE_PIPELINE_H
#define COMPUTE_PIPELINE_H

#include "core/mesh.hpp"
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <base/descriptor.hpp>
#include <base/device.hpp>
#include <base/renderpass.hpp>

namespace uppexo {
class ComputePipeline;

struct ComputePipelineBlueprint {
  using Component = ComputePipeline;

  std::string ComputeShader = "./main.vs";
  std::vector<VkVertexInputBindingDescription> bindingDescription;
  std::vector<VkVertexInputAttributeDescription> attributeDescription;
  bool isDepthEnable = false;
  bool directRead = false;
  VkDevice device;

  char *ComputeShaderCode;
  int ComputeShaderLen;

  DescriptorSet *descriptorSet;

  ComputePipelineBlueprint() = default;
  ComputePipelineBlueprint(Device &device) {
    bindingDescription = FullVertex::getBindingDescription();
    attributeDescription = FullVertex::getAttributeDescriptions();
    this->device = device.getLogicalDevice();
    this->descriptorSet = nullptr;
  }

  ComputePipelineBlueprint(Device &device, DescriptorSet &descriptor) {
    bindingDescription = FullVertex::getBindingDescription();
    attributeDescription = FullVertex::getAttributeDescriptions();
    this->device = device.getLogicalDevice();
    this->descriptorSet = &descriptor;
  }

  void addShaderFromFile(std::string path) {
    directRead = false;
    ComputeShader = path;
  }

  void addShaderFromCode(char* code, int len) {
    directRead = true;
    ComputeShaderCode = code;
    ComputeShaderLen = len;
  }
};

class ComputePipeline {
public:
  ComputePipeline(ComputePipelineBlueprint pipelineBlueprint);
  ~ComputePipeline();
  VkPipeline getPipeline();
  VkPipelineLayout getLayout();

private:
  VkDevice deviceHandle;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;
};
} // namespace uppexo

#endif
