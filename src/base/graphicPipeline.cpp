#include <base/graphicPipeline.hpp>
#include <utils/log.hpp>
#include <utils/vulkan_util.hpp>

template <>
void uppexo::GraphicPipelineBlueprint::setVertexType<uppexo::FullVertex>() {
  bindingDescription = FullVertex::getBindingDescription();
  attributeDescription = FullVertex::getAttributeDescriptions();
};

template <>
void uppexo::GraphicPipelineBlueprint::setVertexType<uppexo::PhongVertex>() {
  bindingDescription = PhongVertex::getBindingDescription();
  attributeDescription = PhongVertex::getAttributeDescriptions();
};

template <>
void uppexo::GraphicPipelineBlueprint::setVertexType<uppexo::NullVertex>() {
  bindingDescription = NullVertex::getBindingDescription();
  attributeDescription = NullVertex::getAttributeDescriptions();
};

uppexo::GraphicPipeline::GraphicPipeline(
    uppexo::GraphicPipelineBlueprint pipelineBlueprint) {
  uppexo::Log::GetInstance().logInfo("Creating graphic pipeline\n");
  this->deviceHandle = pipelineBlueprint.device;

  // uppexo::Log::GetInstance().logVerbose("\n");
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
  if (!pipelineBlueprint.vertexDirectRead) {
    uppexo::Log::GetInstance().logVerbose(
        "Loading vertex shader binaries from file\n");
    std::vector<char> vertexShader =
        uppexo::readFile(pipelineBlueprint.VertexShader);
    vertShaderModule = uppexo::createShaderModule(
        deviceHandle, vertexShader.data(), vertexShader.size());
  } else {
    uppexo::Log::GetInstance().logVerbose(
        "Loading vertex shader binaries from source code\n");
    vertShaderModule = uppexo::createShaderModule(
        deviceHandle, pipelineBlueprint.VertexShaderCode,
        pipelineBlueprint.VertexShaderLen);
  }
  if (!pipelineBlueprint.fragmentDirectRead) {
    uppexo::Log::GetInstance().logVerbose(
        "Loading fragment shader binaries from file\n");
    std::vector<char> fragmentShader =
        uppexo::readFile(pipelineBlueprint.FragmentShader);
    fragShaderModule = uppexo::createShaderModule(
        deviceHandle, fragmentShader.data(), fragmentShader.size());
  } else {
    uppexo::Log::GetInstance().logVerbose(
        "Loading fragment shader binaries from source code\n");
    fragShaderModule = uppexo::createShaderModule(
        deviceHandle, pipelineBlueprint.FragmentShaderCode,
        pipelineBlueprint.FragmentShaderLen);
  }
  uppexo::Log::GetInstance().logVerbose("Creating shader modules\n");
  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShaderModule;
  vertShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragShaderModule;
  fragShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                    fragShaderStageInfo};

  uppexo::Log::GetInstance().logVerbose("Setting fixed function\n");
  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = 1;
  vertexInputInfo.vertexAttributeDescriptionCount =
      static_cast<uint32_t>(pipelineBlueprint.attributeDescription.size());
  vertexInputInfo.pVertexBindingDescriptions =
      &pipelineBlueprint.bindingDescription[0];
  vertexInputInfo.pVertexAttributeDescriptions =
      &pipelineBlueprint.attributeDescription[0];

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = pipelineBlueprint.topology;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.scissorCount = 1;

  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = pipelineBlueprint.polygonMode;
  rasterizer.lineWidth = pipelineBlueprint.lineWidth;
  rasterizer.cullMode = VK_CULL_MODE_NONE;
  rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;

  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  VkPipelineDepthStencilStateCreateInfo depthStencil{};
  depthStencil.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencil.depthTestEnable = VK_TRUE;
  depthStencil.depthWriteEnable = VK_TRUE;
  depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
  depthStencil.depthBoundsTestEnable = VK_FALSE;
  depthStencil.stencilTestEnable = VK_FALSE;

  int colorAttachmentCount = pipelineBlueprint.colourAttachmentCount;
  std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachment;
  for (int i = 0; i < colorAttachmentCount; i++) {
    colorBlendAttachment.push_back(
        {.blendEnable = VK_FALSE,
         .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                           VK_COLOR_COMPONENT_B_BIT |
                           VK_COLOR_COMPONENT_A_BIT});
  }

  VkPipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = colorAttachmentCount;
  colorBlending.pAttachments = colorBlendAttachment.data();
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
                                               VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamicState{};
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
  dynamicState.pDynamicStates = dynamicStates.data();

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  if (pipelineBlueprint.descriptorSet != nullptr) {
    pipelineLayoutInfo.setLayoutCount =
        pipelineBlueprint.descriptorSet->getLayout().size();
    pipelineLayoutInfo.pSetLayouts =
        pipelineBlueprint.descriptorSet->getLayout().data();
  } else {
    pipelineLayoutInfo.setLayoutCount = 0;
  }

  pipelineLayoutInfo.pushConstantRangeCount = 0;

  uppexo::Log::GetInstance().logVerbose("Creating graphic pipeline layout\n");
  if (vkCreatePipelineLayout(deviceHandle, &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to create pipeline layout\n");
  }

  uppexo::Log::GetInstance().logVerbose("Constructing graphic pipeline\n");
  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  if (pipelineBlueprint.isDepthEnable) {
    pipelineInfo.pDepthStencilState = &depthStencil;
  }
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = &dynamicState;
  pipelineInfo.layout = pipelineLayout;
  pipelineInfo.renderPass = pipelineBlueprint.renderpass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

  if (vkCreateGraphicsPipelines(deviceHandle, VK_NULL_HANDLE, 1, &pipelineInfo,
                                nullptr, &pipeline) != VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to create graphics pipeline!");
  }

  uppexo::Log::GetInstance().logVerbose("Destroying shader modules\n");
  vkDestroyShaderModule(deviceHandle, fragShaderModule, nullptr);
  vkDestroyShaderModule(deviceHandle, vertShaderModule, nullptr);

  if (pipelineBlueprint.descriptorSet != nullptr) {
    pipelineBlueprint.descriptorSet->update();
  }
}

uppexo::GraphicPipeline::~GraphicPipeline() {
  uppexo::Log::GetInstance().logInfo("Deallocating graphic pipeline\n");
  vkDestroyPipeline(deviceHandle, pipeline, nullptr);
  vkDestroyPipelineLayout(deviceHandle, pipelineLayout, nullptr);
}

VkPipeline uppexo::GraphicPipeline::getPipeline() { return pipeline; }
VkPipelineLayout uppexo::GraphicPipeline::getLayout() { return pipelineLayout; }
