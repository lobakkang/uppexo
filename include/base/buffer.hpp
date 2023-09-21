#ifndef BUFFER_H_
#define BUFFER_H_

#include <base/commandbuffer.hpp>
#include <base/device.hpp>

#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define KB 1024
#define MB KB * 1024

namespace uppexo {
enum BufferLocation {
  ON_HOST_VISIBLE_TO_DEVICE,
  ON_DEVICE_VISIBLE_TO_HOST,
  ON_DEVICE_INVISIBLE_TO_HOST
};

struct BufferCellBlueprint {
  unsigned int size = 1 * KB;
  BufferLocation location = ON_HOST_VISIBLE_TO_DEVICE;
  VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

  BufferCellBlueprint() = default;
};

namespace presetBufferCellBlueprint {
struct VBO_at_host : BufferCellBlueprint {
  VBO_at_host(int size) {
    this->size = size;
    this->location = ON_HOST_VISIBLE_TO_DEVICE;
    this->usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  }
};

struct VBO_at_device : BufferCellBlueprint {
  VBO_at_device(int size) {
    this->size = size;
    this->location = ON_DEVICE_INVISIBLE_TO_HOST;
    this->usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                                          VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  }
};

struct UBO_at_host : BufferCellBlueprint {
  UBO_at_host(int size) {
    this->size = size;
    this->location = ON_HOST_VISIBLE_TO_DEVICE;
    this->usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  }
};

struct UBO_at_device : BufferCellBlueprint {
  UBO_at_device(int size) {
    this->size = size;
    this->location = ON_DEVICE_INVISIBLE_TO_HOST;
    this->usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
                                          VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  }
};

struct IBO_at_device : BufferCellBlueprint {
  IBO_at_device(int size) {
    this->size = size;
    this->location = ON_DEVICE_INVISIBLE_TO_HOST;
    this->usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
                                          VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  }
};

struct SSBO_at_device : BufferCellBlueprint {
  SSBO_at_device(int size) {
    this->size = size;
    this->location = ON_DEVICE_INVISIBLE_TO_HOST;
    this->usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
                                          VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  }
};

struct SSBO_at_host : BufferCellBlueprint {
  SSBO_at_host(int size) {
    this->size = size;
    this->location = ON_HOST_VISIBLE_TO_DEVICE;
    this->usage = (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
                                          VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  }
};
}; // namespace presetBufferCellBlueprint

class Buffer;

struct BufferBlueprint {
  using Component = Buffer;

  std::vector<BufferCellBlueprint> cellList;

  VkDevice device;
  VkPhysicalDevice physicalDevice;

  BufferBlueprint() = default;
  BufferBlueprint(Device &device) {
    this->device = device.getLogicalDevice();
    this->physicalDevice = device.getPhysicalDevice();
  }

  int addCell(BufferCellBlueprint bufferCell) {
    cellList.push_back(bufferCell);
    return cellList.size() - 1;
  }
};

class Buffer {
public:
  Buffer(BufferBlueprint bufferBlueprint);
  ~Buffer();

  int addBuffer(BufferCellBlueprint cell);
  void deleteBuffer(int id);

  void copyByMapping(int id, void *data, unsigned int size,
                     unsigned int offset = 0);
  void copyOutByMapping(int id, void *data, unsigned int size,
                        unsigned int offset = 0);
  void copyByStaging(int id, CommandBuffer &commandBuffer, void *data,
                     unsigned int size, unsigned int offset = 0);
  void copyOutByStaging(int id, CommandBuffer &commandBuffer, void *data,
                        unsigned int size, unsigned int offset = 0);
  // void* mapBuffer(int id);
  // void demapBuffer(int id);
  VkBuffer &getBuffer(int id) { return bufferList[id]; }
  int getBufferSize(int id) { return bufferSizeList[id]; };
  int getStagingBufferID() { return stagingBufferID; }

private:
  int stagingBufferID = 0;

  std::vector<BufferCellBlueprint> cellList;
  std::vector<int> bufferOffsetList;
  std::vector<int> bufferMemoryPairList;
  std::vector<VkBuffer> bufferList;
  std::vector<int> bufferSizeList;
  std::map<int, VkDeviceMemory> memoryList;

  VkDevice deviceHandle;
  VkPhysicalDevice physicalDeviceHandle;
};
} // namespace uppexo

#endif // !BUFFER_H_
