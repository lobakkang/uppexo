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
class GraphicPipeline;

struct GraphicPipelineBlueprint {
  using Component = GraphicPipeline;

  std::string VertexShader = "./main.vs";
  std::string FragmentShader = "./main.fs";
  std::vector<VkVertexInputBindingDescription> bindingDescription;
  std::vector<VkVertexInputAttributeDescription> attributeDescription;
  bool isDepthEnable = false;
  bool vertexDirectRead = false;
  bool fragmentDirectRead = false;
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

  void addVertexShaderFromFile(std::string path) {
    vertexDirectRead = false;
    VertexShader = path;
  }

  void addFragmentShaderFromFile(std::string path) {
    fragmentDirectRead = false;
    FragmentShader = path;
  }

  void addVertexShaderFromCode(char* code, int len) {
    vertexDirectRead = true;
    VertexShaderCode = code;
    VertexShaderLen = len;
  }

  void addFragmentShaderFromCode(char* code, int len) {
    fragmentDirectRead = true;
    FragmentShaderCode = code;
    FragmentShaderLen = len;
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
