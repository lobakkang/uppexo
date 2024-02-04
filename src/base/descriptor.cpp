#include <base/descriptor.hpp>
#include <utils/log.hpp>

#include <array>
#include <vulkan/vulkan_core.h>

uppexo::DescriptorSet::DescriptorSet(uppexo::DescriptorSetBlueprint blueprint) {
  uppexo::Log::GetInstance().logInfo("Creating descriptor set\n");
  uppexo::Log::GetInstance().logVerbose("Creating descriptor set blueprint\n");
  this->binding = blueprint.binding;
  this->device = blueprint.device;
  for (std::vector<uppexo::DescriptorSetBindingBlueprint> bindingBlueprintList :
       blueprint.binding) {
    int i = 0;
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    for (uppexo::DescriptorSetBindingBlueprint bindingBlueprint :
         bindingBlueprintList) {
      VkDescriptorSetLayoutBinding binding{};
      binding.binding = i;
      binding.descriptorCount = 1;
      binding.descriptorType = bindingBlueprint.type;
      binding.pImmutableSamplers = nullptr;
      binding.stageFlags = bindingBlueprint.stage;
      i++;
      bindings.push_back(binding);
    }
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    VkDescriptorSetLayout descriptorSetLayout;
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr,
                                    &descriptorSetLayout) != VK_SUCCESS) {
      uppexo::Log::GetInstance().logError(
          "Failed to create descriptor set layout!\n");
    }
    descriptorSetLayoutList.push_back(descriptorSetLayout);
  }

  uppexo::Log::GetInstance().logVerbose("Allocating descriptor pool\n");
  std::vector<VkDescriptorPoolSize> poolSizes;
  for (std::vector<uppexo::DescriptorSetBindingBlueprint> bindingBlueprintList :
       blueprint.binding) {
    for (uppexo::DescriptorSetBindingBlueprint bindingBlueprint :
         bindingBlueprintList) {
      VkDescriptorPoolSize poolSize;
      poolSize.type = bindingBlueprint.type;
      poolSize.descriptorCount = 1;
      poolSizes.push_back(poolSize);
    }
  }

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  poolInfo.pPoolSizes = poolSizes.data();
  poolInfo.maxSets = blueprint.binding.size();

  if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) !=
      VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to create descriptor pool!\n");
  }

  uppexo::Log::GetInstance().logVerbose("Allocating descriptor set\n");
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = descriptorPool;
  allocInfo.descriptorSetCount =
      static_cast<uint32_t>(blueprint.binding.size());
  allocInfo.pSetLayouts = descriptorSetLayoutList.data();

  descriptorSets.resize(blueprint.binding.size());
  if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) !=
      VK_SUCCESS) {
    uppexo::Log::GetInstance().logError(
        "Failed to allocate descriptor sets!\n");
  }
}

void uppexo::DescriptorSet::update() {
  uppexo::Log::GetInstance().logInfo("Updating descriptor set binding\n");
  int i = 0;
  for (std::vector<uppexo::DescriptorSetBindingBlueprint> bindingBlueprintList :
       binding) {
    std::vector<VkWriteDescriptorSet> descriptorWrites{};
    descriptorWrites.clear();
    int j = 0;
    for (uppexo::DescriptorSetBindingBlueprint bindingBlueprint :
         bindingBlueprintList) {
      VkWriteDescriptorSet writeDescriptorSet;
      writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      writeDescriptorSet.dstSet = descriptorSets[i];
      writeDescriptorSet.dstBinding = j;
      writeDescriptorSet.dstArrayElement = 0;
      writeDescriptorSet.descriptorType = bindingBlueprint.type;
      writeDescriptorSet.descriptorCount = 1;
      j++;

      if (bindingBlueprint.buffer != VK_NULL_HANDLE) {
        uppexo::Log::GetInstance().logVerbose("Updating buffer binding\n");
        VkDescriptorBufferInfo *bufferInfo = new VkDescriptorBufferInfo{};
        bufferInfo->buffer = bindingBlueprint.buffer;
        bufferInfo->offset = 0;
        bufferInfo->range = bindingBlueprint.size;
        writeDescriptorSet.pBufferInfo = bufferInfo;
      }

      if (bindingBlueprint.sampler != VK_NULL_HANDLE) {
        uppexo::Log::GetInstance().logVerbose("Updating texture binding\n");
        VkDescriptorImageInfo *imageInfo = new VkDescriptorImageInfo{};
        imageInfo->imageLayout = bindingBlueprint.textureLayout;
        imageInfo->imageView = bindingBlueprint.texture;
        imageInfo->sampler = bindingBlueprint.sampler;
        writeDescriptorSet.pImageInfo = imageInfo;
      }

      descriptorWrites.push_back(writeDescriptorSet);
    }
    vkUpdateDescriptorSets(device,
                           static_cast<uint32_t>(descriptorWrites.size()),
                           descriptorWrites.data(), 0, nullptr);
    i++;
  }
}

uppexo::DescriptorSet::~DescriptorSet() {
  uppexo::Log::GetInstance().logInfo("Destroying descriptor set\n");
  vkDestroyDescriptorPool(device, descriptorPool, nullptr);
  for (VkDescriptorSetLayout descriptorSetLayout : descriptorSetLayoutList) {
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
  }
}

std::vector<VkDescriptorSetLayout> &uppexo::DescriptorSet::getLayout() {
  return descriptorSetLayoutList;
}

std::vector<VkDescriptorSet> &uppexo::DescriptorSet::getSet() {
  return descriptorSets;
}
