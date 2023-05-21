#ifndef BUFFER_H_
#define BUFFER_H_

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
    this->usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
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
    this->usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  }
};

struct IBO_at_device : BufferCellBlueprint {
  IBO_at_device(int size) {
    this->size = size;
    this->location = ON_DEVICE_INVISIBLE_TO_HOST;
    this->usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  }
};
}; // namespace presetBufferCellBlueprint

struct BufferBlueprint {
  std::vector<BufferCellBlueprint> cellList;

  VkDevice device;
  VkPhysicalDevice physicalDevice;

  BufferBlueprint() = default;
  BufferBlueprint(Device &device) {
    this->device = device.getLogicalDevice();
    this->physicalDevice = device.getPhysicalDevice();
  }
};

class Buffer {
public:
  Buffer(BufferBlueprint bufferBlueprint);
  ~Buffer();

  void copyByMapping(int id, void *data, unsigned int size);
  // void* mapBuffer(int id);
  // void demapBuffer(int id);
  VkBuffer &getBuffer(int id);
  int getStagingBufferID();

private:
  std::vector<BufferCellBlueprint> cellList;
  std::vector<int> bufferOffsetList;
  std::vector<VkBuffer> bufferList;
  std::map<BufferLocation, VkDeviceMemory> memoryList;
  VkDevice deviceHandle;
};
} // namespace uppexo

#endif // !BUFFER_H_
