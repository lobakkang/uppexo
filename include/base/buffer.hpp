#ifndef BUFFER_H_
#define BUFFER_H_

#include <base/device.hpp>

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

struct BufferBlueprint {
  unsigned int size = 1 * KB;
  BufferLocation location = ON_HOST_VISIBLE_TO_DEVICE;

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

  void copyByMapping(void *data, unsigned int size);
  VkBuffer &getBuffer();

private:
  VkBuffer buffer;
  VkDeviceMemory memory;
  VkDevice deviceHandle;
};
} // namespace uppexo

#endif // !BUFFER_H_
