#ifndef PIPELINE_H
#define PIPELINE_H

#include "core/mesh.hpp"
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <base/device.hpp>
#include <base/renderpass.hpp>

namespace uppexo {
struct GraphicPipelineBlueprint {
  std::string VertexShader = "./main.vs";
  std::string FragmentShader = "./main.fs";
  std::vector<VkVertexInputBindingDescription> bindingDescription;
  std::vector<VkVertexInputAttributeDescription> attributeDescription;
  VkDevice device;
  VkRenderPass renderpass;

  GraphicPipelineBlueprint() = default;
  GraphicPipelineBlueprint(Device &device, Renderpass &renderpass) {
    bindingDescription = FullVertex::getBindingDescription();
    attributeDescription = FullVertex::getAttributeDescriptions();
    this->device = device.getLogicalDevice();
    this->renderpass = renderpass.getRenderPass();
  }
};

class GraphicPipeline {
public:
  GraphicPipeline(GraphicPipelineBlueprint pipelineBlueprint);
  ~GraphicPipeline();
  VkPipeline getPipeline();

private:
  VkDevice deviceHandle;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;
};
} // namespace uppexo

#endif // !RENDERPASS_HPP_
