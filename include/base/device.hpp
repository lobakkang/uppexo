#ifndef DEVICE_H
#define DEVICE_H

#include <cstdint>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <base/instance.hpp>

#include <map>
#include <optional>
#include <vector>

namespace uppexo {
struct DeviceBlueprint {
  int graphicQueue = 1;
  int presentQueue = 1;
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
  }
};

enum QueueType { graphic, present, sparse, transfer, compute };

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
