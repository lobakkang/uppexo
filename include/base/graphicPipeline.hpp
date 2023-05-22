#ifndef PIPELINE_H
#define PIPELINE_H

#include "core/mesh.hpp"
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <base/descriptor.hpp>
#include <base/device.hpp>
#include <base/renderpass.hpp>

namespace uppexo {
struct GraphicPipelineBlueprint {
  std::string VertexShader = "./main.vs";
  std::string FragmentShader = "./main.fs";
  std::vector<VkVertexInputBindingDescription> bindingDescription;
  std::vector<VkVertexInputAttributeDescription> attributeDescription;
  bool isDepthEnable = false;
  bool directRead = false;
  VkDevice device;
  VkRenderPass renderpass;

  char* VertexShaderCode;
  char* FragmentShaderCode;
  int VertexShaderLen;
  int FragmentShaderLen;

  DescriptorSet *descriptorSet;

  GraphicPipelineBlueprint() = default;
  GraphicPipelineBlueprint(Device &device, Renderpass &renderpass) {
    bindingDescription = FullVertex::getBindingDescription();
    attributeDescription = FullVertex::getAttributeDescriptions();
    this->device = device.getLogicalDevice();
    this->renderpass = renderpass.getRenderPass();
    this->descriptorSet = nullptr;
  }

  GraphicPipelineBlueprint(Device &device, Renderpass &renderpass,
                           DescriptorSet &descriptor) {
    bindingDescription = FullVertex::getBindingDescription();
    attributeDescription = FullVertex::getAttributeDescriptions();
    this->device = device.getLogicalDevice();
    this->renderpass = renderpass.getRenderPass();
    this->descriptorSet = &descriptor;
  }
};

class GraphicPipeline {
public:
  GraphicPipeline(GraphicPipelineBlueprint pipelineBlueprint);
  ~GraphicPipeline();
  VkPipeline getPipeline();
  VkPipelineLayout getLayout();

private:
  VkDevice deviceHandle;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;
};
} // namespace uppexo

#endif // !RENDERPASS_HPP_
