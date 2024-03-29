#ifndef DEVICE_H
#define DEVICE_H

#include <cstdint>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <base/instance.hpp>

#include <map>
#include <optional>
#include <utils/log.hpp>
#include <vector>

namespace uppexo {
enum QueueType { graphic, present, sparse, transfer, compute };

class Device;

struct DeviceBlueprint {
  using Component = Device;

  int graphicQueue = 0;
  int presentQueue = 0;
  int transferQueue = 0;
  int sparseQueue = 0;
  int computeQueue = 0;

  bool isSwapchainRequire = true;

  VkInstance instance;
  bool IsValidationLayerEnable;
  bool IsGraphicEnable;
  VkSurfaceKHR surface;
  GLFWwindow *window;

  DeviceBlueprint() = default;
  DeviceBlueprint(Instance &instance) {
    this->instance = instance.getInstance();
    this->window = instance.getWindow();
    this->IsValidationLayerEnable = instance.IsValidationLayerEnable();
    this->surface = instance.getSurface();
    this->IsGraphicEnable = instance.IsGraphicEnable();
    this->isSwapchainRequire = instance.IsGraphicEnable();
  }

  std::tuple<QueueType, int> addQueue(QueueType type) {
    int id = 0;
    switch (type) {
    case QueueType::sparse:
      id = sparseQueue;
      sparseQueue++;
      break;
    case QueueType::graphic:
      id = graphicQueue;
      graphicQueue++;
      break;
    case QueueType::transfer:
      id = transferQueue;
      transferQueue++;
      break;
    case QueueType::present:
      id = presentQueue;
      presentQueue++;
      break;
    case QueueType::compute:
      id = computeQueue;
      computeQueue++;
      break;
    default:
      uppexo::Log::GetInstance().logError("ERROR UNKNOWN QUEUE TYPE\n");
    }
    return {type, id};
  }
};

struct QueueData {
  std::vector<VkQueue> queue;
  int queueCount;
  QueueType type;
  std::optional<int> queueFamilyID;
};

class Device {
public:
  Device(uppexo::DeviceBlueprint blueprint);
  ~Device();
  VkPhysicalDevice getPhysicalDevice();
  VkDevice getLogicalDevice();
  VkSwapchainKHR getSwapChain();
  VkFormat getSwapChainImageFormat();
  std::vector<VkImageView> getSwapChainImageView();
  VkExtent2D getSwapChainExtend();
  QueueData getQueue(QueueType type);

private:
  std::map<QueueType, QueueData> queueData;
  VkPhysicalDevice physicalDevice;
  VkDevice logicalDevice;

  VkSurfaceCapabilitiesKHR SwapChainCapabilities;
  std::vector<VkSurfaceFormatKHR> SwapChainFormats;
  std::vector<VkPresentModeKHR> SwapChainPresentModes;
  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  std::vector<VkImageView> swapChainImageViews;

  bool isSwapChainEnable;
};
}; // namespace uppexo

#endif
