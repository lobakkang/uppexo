#include <base/buffer.hpp>
#include <cstring>
#include <utils/log.hpp>
#include <vulkan/vulkan_core.h>

uppexo::Buffer::Buffer(uppexo::BufferBlueprint bufferBlueprint) {
  uppexo::Log::GetInstance().logInfo("Allocating buffer\n");
  deviceHandle = bufferBlueprint.device;

  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = bufferBlueprint.size;
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(deviceHandle, &bufferInfo, nullptr, &buffer) !=
      VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to create vertex buffer!\n");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(deviceHandle, buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;

  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(bufferBlueprint.physicalDevice,
                                      &memProperties);
  VkMemoryPropertyFlags properties;
  switch (bufferBlueprint.location) {
  case BufferLocation::ON_HOST_VISIBLE_TO_DEVICE:
    properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    break;
  case BufferLocation::ON_DEVICE_INVISIBLE_TO_HOST:
    properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    break;
  case BufferLocation::ON_DEVICE_VISIBLE_TO_HOST:
    properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT |
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    break;
  default:
    uppexo::Log::GetInstance().logError("Unknown buffer location options\n");
    break;
  }

  allocInfo.memoryTypeIndex = 6942;
  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((memRequirements.memoryTypeBits & (1 << i)) &&
        (memProperties.memoryTypes[i].propertyFlags & properties) ==
            properties) {
      allocInfo.memoryTypeIndex = i;
    }
  }

  if (allocInfo.memoryTypeIndex == 6942) {
    uppexo::Log::GetInstance().logError(
        "No suitable memory is available for buffer!\n");
  }
  if (vkAllocateMemory(deviceHandle, &allocInfo, nullptr, &memory) !=
      VK_SUCCESS) {
    uppexo::Log::GetInstance().logError(
        "Failed to allocate vertex buffer memory!\n");
  }
  vkBindBufferMemory(deviceHandle, buffer, memory, 0);
}

uppexo::Buffer::~Buffer() {
  uppexo::Log::GetInstance().logInfo("Deallocating buffer\n");
  vkDestroyBuffer(deviceHandle, buffer, nullptr);
  vkFreeMemory(deviceHandle, memory, nullptr);
}

void uppexo::Buffer::copyByMapping(void *data, unsigned int size) {
  void *mappedMemory;
  vkMapMemory(deviceHandle, memory, 0, size, 0, &mappedMemory);
  memcpy(mappedMemory, data, size);
  vkUnmapMemory(deviceHandle, memory);
}

VkBuffer &uppexo::Buffer::getBuffer() { return buffer; }
