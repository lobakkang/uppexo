#include <base/computePipeline.hpp>
#include <utils/log.hpp>

uppexo::ComputePipeline::ComputePipeline(
    uppexo::ComputePipelineBlueprint pipelineBlueprint) {
  uppexo::Log::GetInstance().logInfo("Creating compute pipeline\n");
  this->deviceHandle = pipelineBlueprint.device;
  VkShaderModule computeShaderModule;
  if (!pipelineBlueprint.directRead) {
    std::vector<char> computeShader =
        uppexo::readFile(pipelineBlueprint.ComputeShader);
    computeShaderModule = uppexo::createShaderModule(
        deviceHandle, computeShader.data(), computeShader.size());
  } else {
    computeShaderModule = uppexo::createShaderModule(
        deviceHandle, pipelineBlueprint.ComputeShaderCode,
        pipelineBlueprint.ComputeShaderLen);
  }

  VkPipelineShaderStageCreateInfo computeShaderStageInfo{};
  computeShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  computeShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
  computeShaderStageInfo.module = computeShaderModule;
  computeShaderStageInfo.pName = "main";

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

  uppexo::Log::GetInstance().logVerbose("Creating compute pipeline layout\n");
  if (vkCreatePipelineLayout(deviceHandle, &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    uppexo::Log::GetInstance().logError(
        "Failed to create compute pipeline layout!");
  }

  VkComputePipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
  pipelineInfo.layout = pipelineLayout;
  pipelineInfo.stage = computeShaderStageInfo;

  uppexo::Log::GetInstance().logVerbose("Constructing compute pipeline\n");
  if (vkCreateComputePipelines(deviceHandle, VK_NULL_HANDLE, 1, &pipelineInfo,
                               nullptr, &pipeline) != VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to create compute pipeline!");
  }

  uppexo::Log::GetInstance().logVerbose("Destroying shader modules\n");
  vkDestroyShaderModule(deviceHandle, computeShaderModule, nullptr);
  if (pipelineBlueprint.descriptorSet != nullptr) {
    pipelineBlueprint.descriptorSet->update();
  }
}

uppexo::ComputePipeline::~ComputePipeline() {
  uppexo::Log::GetInstance().logInfo("Deallocating compute pipeline\n");
  vkDestroyPipeline(deviceHandle, pipeline, nullptr);
  vkDestroyPipelineLayout(deviceHandle, pipelineLayout, nullptr);
}

VkPipeline uppexo::ComputePipeline::getPipeline() { return pipeline; }
VkPipelineLayout uppexo::ComputePipeline::getLayout() { return pipelineLayout; }
