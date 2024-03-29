#include <base/buffer.hpp>
#include <core/commandBufferRecorder.hpp>
#include <cstring>
#include <iostream>
#include <map>
#include <utils/log.hpp>
#include <utils/vulkan_util.hpp>
#include <vulkan/vulkan_core.h>

uppexo::Buffer::Buffer(uppexo::BufferBlueprint bufferBlueprint) {
  uppexo::Log::GetInstance().logInfo("Allocating buffer\n");
  deviceHandle = bufferBlueprint.device;
  physicalDeviceHandle = bufferBlueprint.physicalDevice;
  bufferBlueprint.cellList.push_back(uppexo::BufferCellBlueprint{
      .size = 50 * MB,
      .location = ON_HOST_VISIBLE_TO_DEVICE,
      .usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                                       VK_BUFFER_USAGE_TRANSFER_DST_BIT)});

  int i = 0;
  for (uppexo::BufferCellBlueprint cell : bufferBlueprint.cellList) {
    VkBufferCreateInfo bufferInfo{};
    uppexo::Log::GetInstance().logVerbose(
        "Creating buffer #%i, size(byte):%i\n", i, cell.size);
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = cell.size;
    bufferInfo.usage = cell.usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkBuffer buffer;
    if (vkCreateBuffer(deviceHandle, &bufferInfo, nullptr, &buffer) !=
        VK_SUCCESS) {
      uppexo::Log::GetInstance().logError("Failed to create buffer!\n");
    }
    bufferList.push_back(buffer);
    bufferSizeList.push_back(cell.size);
    i++;
  }

  bufferMemoryPairList.clear();
  uppexo::Log::GetInstance().logVerbose("Optimizing memory allocation\n");
  std::map<int, int> memoryMap;
  i = 0;
  for (VkBuffer buffer : bufferList) {
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(deviceHandle, buffer, &memRequirements);

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(bufferBlueprint.physicalDevice,
                                        &memProperties);
    VkMemoryPropertyFlags properties;
    switch (bufferBlueprint.cellList[i].location) {
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

    int selectedIndex = -1;
    for (uint32_t j = 0; j < memProperties.memoryTypeCount; j++) {
      if ((memRequirements.memoryTypeBits & (1 << j)) &&
          (memProperties.memoryTypes[j].propertyFlags & properties) ==
              properties) {
        if (selectedIndex == -1) {
          selectedIndex = j;
        }
      }
    }

    if (selectedIndex == -1) {
      uppexo::Log::GetInstance().logError(
          "No suitable memory is available for buffer!\n");
    }
    memoryMap[selectedIndex] += memRequirements.size;
    bufferMemoryPairList.push_back(selectedIndex);
    i++;
  }

  uppexo::Log::GetInstance().logVerbose("Memory allocation map:\n");
  uppexo::Log::GetInstance().logVerbose("ID - size(byte) - paired buffer\n");
  for (auto &pair : memoryMap) {
    uppexo::Log::GetInstance().logVerbose("%i - %i - ", pair.first,
                                          pair.second);
    int i = 0;
    for (auto buffer : bufferMemoryPairList) {
      if (buffer == pair.first) {
        std::cout << i << " ";
      }
      i++;
    }
    std::cout << std::endl;
  }

  uppexo::Log::GetInstance().logVerbose("Allocating device memory\n");
  for (auto &pair : memoryMap) {
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = pair.second;
    allocInfo.memoryTypeIndex = pair.first;
    VkDeviceMemory memory;
    if (vkAllocateMemory(deviceHandle, &allocInfo, nullptr, &memory) !=
        VK_SUCCESS) {
      uppexo::Log::GetInstance().logError(
          "Failed to allocate buffer memory!\n");
    }
    memoryList[pair.first] = memory;
  }

  uppexo::Log::GetInstance().logVerbose("Binding buffer to memory\n");
  i = 0;
  std::map<int, int> offsetList;
  for (VkBuffer buffer : bufferList) {
    vkBindBufferMemory(deviceHandle, buffer,
                       memoryList[bufferMemoryPairList[i]],
                       offsetList[bufferMemoryPairList[i]]);
    bufferOffsetList.push_back(offsetList[bufferMemoryPairList[i]]);
    offsetList[bufferMemoryPairList[i]] += bufferBlueprint.cellList[i].size;
    i++;
  }
  cellList = bufferBlueprint.cellList;
  stagingBufferID = bufferList.size() - 1;
}

uppexo::Buffer::~Buffer() {
  uppexo::Log::GetInstance().logInfo("Deallocating buffer\n");
  for (VkBuffer buffer : bufferList) {
    vkDestroyBuffer(deviceHandle, buffer, nullptr);
  }
  for (auto &memory : memoryList) {
    vkFreeMemory(deviceHandle, memory.second, nullptr);
  }
}

void uppexo::Buffer::copyByMapping(int id, void *data, unsigned int size,
                                   unsigned int offset) {
  // uppexo::Log::GetInstance().logVerbose(
  //     "Copying memory, size: %i, offset: %i\n", size, bufferOffsetList[id]);
  void *mappedMemory;
  vkMapMemory(deviceHandle, memoryList[bufferMemoryPairList[id]],
              bufferOffsetList[id], size, 0, &mappedMemory);
  memcpy(static_cast<char *>(mappedMemory) + offset, data, size);
  vkUnmapMemory(deviceHandle, memoryList[bufferMemoryPairList[id]]);
}

void uppexo::Buffer::copyOutByMapping(int id, void *data, unsigned int size,
                                      unsigned int offset) {
  // uppexo::Log::GetInstance().logVerbose(
  //     "Copying memory, size: %i, offset: %i\n", size, bufferOffsetList[id]);
  void *mappedMemory;
  vkMapMemory(deviceHandle, memoryList[bufferMemoryPairList[id]],
              bufferOffsetList[id], size, 0, &mappedMemory);
  memcpy(data, static_cast<char *>(mappedMemory) + offset, size);
  vkUnmapMemory(deviceHandle, memoryList[bufferMemoryPairList[id]]);
}

void uppexo::Buffer::copyByStaging(int id, CommandBuffer &commandBuffer,
                                   void *data, unsigned int size,
                                   unsigned int offset) {
  void *mappedMemory;
  vkMapMemory(deviceHandle, memoryList[bufferMemoryPairList[stagingBufferID]],
              bufferOffsetList[stagingBufferID], size, 0, &mappedMemory);
  memcpy(static_cast<char *>(mappedMemory) + offset, data, size);
  vkUnmapMemory(deviceHandle,
                memoryList[bufferMemoryPairList[stagingBufferID]]);

  VkCommandBuffer singleUseCommandBuffer =
      commandBuffer.createSingleUseCommandBuffer();
  uppexo::command::copyBufferToBuffer(singleUseCommandBuffer,
                                      bufferList[stagingBufferID],
                                      bufferList[id], size);
  commandBuffer.submitSingleUseCommandBuffer(singleUseCommandBuffer);
}

void uppexo::Buffer::copyOutByStaging(int id, CommandBuffer &commandBuffer,
                                      void *data, unsigned int size,
                                      unsigned int offset) {
  // uppexo::Log::GetInstance().logVerbose(
  //     "Copying memory, size: %i, offset: %i\n", size, bufferOffsetList[id]);
  void *mappedMemory;
  vkMapMemory(deviceHandle, memoryList[cellList[id].location],
              bufferOffsetList[id], size, 0, &mappedMemory);
  memcpy(data, static_cast<char *>(mappedMemory) + offset, size);
  vkUnmapMemory(deviceHandle, memoryList[cellList[id].location]);
}

int uppexo::Buffer::addBuffer(BufferCellBlueprint cell) {
  VkBufferCreateInfo bufferInfo{};
  uppexo::Log::GetInstance().logVerbose("Creating buffer with size(byte):%i\n",
                                        cell.size);
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = cell.size;
  bufferInfo.usage = cell.usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VkBuffer buffer;
  if (vkCreateBuffer(deviceHandle, &bufferInfo, nullptr, &buffer) !=
      VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to create buffer!\n");
  }
  bufferList.push_back(buffer);
  bufferSizeList.push_back(cell.size);

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(deviceHandle, buffer, &memRequirements);

  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDeviceHandle, &memProperties);
  VkMemoryPropertyFlags properties;
  switch (cell.location) {
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

  int selectedIndex = -1;
  for (uint32_t j = 0; j < memProperties.memoryTypeCount; j++) {
    if ((memRequirements.memoryTypeBits & (1 << j)) &&
        (memProperties.memoryTypes[j].propertyFlags & properties) ==
            properties) {
      selectedIndex = j;
    }
  }

  if (selectedIndex == -1) {
    uppexo::Log::GetInstance().logError(
        "No suitable memory is available for buffer!\n");
  }
  bufferMemoryPairList.push_back(selectedIndex);

  uppexo::Log::GetInstance().logVerbose("Allocating device memory\n");
  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = selectedIndex;
  VkDeviceMemory memory;
  if (vkAllocateMemory(deviceHandle, &allocInfo, nullptr, &memory) !=
      VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to allocate buffer memory!\n");
  }
  memoryList[bufferList.size() + 100] = memory;

  uppexo::Log::GetInstance().logVerbose("Binding buffer to memory\n");
  std::map<int, int> offsetList;
  vkBindBufferMemory(deviceHandle, buffer, memoryList[bufferList.size() + 100],
                     0);
  bufferOffsetList.push_back(0);
  cellList.push_back(cell);

  return bufferList.size() - 1;
}

void uppexo::Buffer::deleteBuffer(int id) {
  vkDestroyBuffer(deviceHandle, bufferList[id], nullptr);
}
