#include <base/buffer.hpp>
#include <cstring>
#include <map>
#include <utils/log.hpp>
#include <utils/vulkan_util.hpp>
#include <vulkan/vulkan_core.h>

struct memoryInfo {
  int size;
  uppexo::BufferLocation location;
};

uppexo::Buffer::Buffer(uppexo::BufferBlueprint bufferBlueprint) {
  uppexo::Log::GetInstance().logInfo("Allocating buffer\n");
  deviceHandle = bufferBlueprint.device;
  bufferBlueprint.cellList.push_back(
      uppexo::BufferCellBlueprint{.size = 50 * MB,
                                  .location = ON_HOST_VISIBLE_TO_DEVICE,
                                  .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT});

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
    i++;
  }

  // uppexo::Log::GetInstance().logVerbose("\n");
  uppexo::Log::GetInstance().logVerbose("Optimizing memory allocation\n");
  std::map<int, memoryInfo> memoryMap;
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
        selectedIndex = j;
      }
    }

    if (selectedIndex == -1) {
      uppexo::Log::GetInstance().logError(
          "No suitable memory is available for buffer!\n");
    }
    memoryMap[selectedIndex].size += memRequirements.size;
    memoryMap[selectedIndex].location = bufferBlueprint.cellList[i].location;
    i++;
  }

  uppexo::Log::GetInstance().logVerbose("Memory allocation map:\n");
  uppexo::Log::GetInstance().logVerbose("ID - size(byte) - location\n");
  for (auto &pair : memoryMap) {
    uppexo::Log::GetInstance().logVerbose(
        "%i - %i - %s\n", pair.first, pair.second.size,
        uppexo::BufferLocationToString(pair.second.location).c_str());
  }

  uppexo::Log::GetInstance().logVerbose("Allocating device memory\n");
  for (auto &pair : memoryMap) {
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = pair.second.size;
    allocInfo.memoryTypeIndex = pair.first;
    VkDeviceMemory memory;
    if (vkAllocateMemory(deviceHandle, &allocInfo, nullptr, &memory) !=
        VK_SUCCESS) {
      uppexo::Log::GetInstance().logError(
          "Failed to allocate buffer memory!\n");
    }
    memoryList[pair.second.location] = memory;
  }

  uppexo::Log::GetInstance().logVerbose("Binding buffer to memory\n");
  i = 0;
  std::map<uppexo::BufferLocation, int> offsetList;
  for (VkBuffer buffer : bufferList) {
    vkBindBufferMemory(deviceHandle, buffer,
                       memoryList[bufferBlueprint.cellList[i].location],
                       offsetList[bufferBlueprint.cellList[i].location]);
    bufferOffsetList.push_back(
        offsetList[bufferBlueprint.cellList[i].location]);
    offsetList[bufferBlueprint.cellList[i].location] +=
        bufferBlueprint.cellList[i].size;
    i++;
  }
  cellList = bufferBlueprint.cellList;
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

void uppexo::Buffer::copyByMapping(int id, void *data, unsigned int size) {
  // uppexo::Log::GetInstance().logVerbose(
  //     "Copying memory, size: %i, offset: %i\n", size, bufferOffsetList[id]);
  void *mappedMemory;
  vkMapMemory(deviceHandle, memoryList[cellList[id].location],
              bufferOffsetList[id], size, 0, &mappedMemory);
  memcpy(mappedMemory, data, size);
  vkUnmapMemory(deviceHandle, memoryList[cellList[id].location]);
}

VkBuffer &uppexo::Buffer::getBuffer(int id) { return bufferList[id]; }

int uppexo::Buffer::getStagingBufferID() {
  return bufferList.size() - 1;
}
