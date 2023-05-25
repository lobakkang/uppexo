#include <algorithm>
#include <base/device.hpp>
#include <base/instance.hpp>
#include <limits>
#include <set>
#include <string>
#include <utils/log.hpp>
#include <utils/vulkan_util.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

uppexo::Device::Device(uppexo::DeviceBlueprint deviceBlueprint) {
  uppexo::Log::GetInstance().logInfo("Picking physical device\n");
  uppexo::Log::GetInstance().logVerbose(
      "Selecting GPU for optimum performance\n");
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(deviceBlueprint.instance, &deviceCount, nullptr);

  if (deviceCount == 0) {
    uppexo::Log::GetInstance().logError(
        "Failed to find GPUs with Vulkan support!\n");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  uppexo::Log::GetInstance().logInfo("Found %s suitable device\n",
                                     std::to_string(deviceCount).c_str());
  vkEnumeratePhysicalDevices(deviceBlueprint.instance, &deviceCount,
                             devices.data());

  physicalDevice = VK_NULL_HANDLE;
  uppexo::Log::GetInstance().logInfo("#N - %-10s - %-30s\n", "Vendor ID",
                                     "Device Name");
  int index = 1;
  if (deviceBlueprint.graphicQueue > 0) {
    queueData[graphic] =
        (QueueData){.queue = std::vector<VkQueue>(deviceBlueprint.graphicQueue),
                    .queueCount = deviceBlueprint.graphicQueue,
                    .type = graphic,
                    .queueFamilyID = 0};
  }
  if (deviceBlueprint.presentQueue > 0) {
    queueData[present] =
        (QueueData){.queue = std::vector<VkQueue>(deviceBlueprint.presentQueue),
                    .queueCount = deviceBlueprint.presentQueue,
                    .type = present,
                    .queueFamilyID = 0};
  }
  if (deviceBlueprint.sparseQueue > 0) {
    queueData[sparse] =
        (QueueData){.queue = std::vector<VkQueue>(deviceBlueprint.sparseQueue),
                    .queueCount = deviceBlueprint.sparseQueue,
                    .type = sparse,
                    .queueFamilyID = 0};
  }
  if (deviceBlueprint.transferQueue > 0) {
    queueData[transfer] = (QueueData){
        .queue = std::vector<VkQueue>(deviceBlueprint.transferQueue),
        .queueCount = deviceBlueprint.transferQueue,
        .type = transfer,
        .queueFamilyID = 0};
  }
  if (deviceBlueprint.computeQueue > 0) {
    queueData[compute] =
        (QueueData){.queue = std::vector<VkQueue>(deviceBlueprint.computeQueue),
                    .queueCount = deviceBlueprint.computeQueue,
                    .type = compute,
                    .queueFamilyID = 0};
  }

  for (const auto &device : devices) {
    for (auto it = queueData.begin(); it != queueData.end(); ++it) {
      it->second.queueFamilyID.reset();
    }

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    uppexo::Log::GetInstance().logInfo(
        "#%d - %-10d - %-30s\n", index, properties.vendorID,
        std::string(properties.deviceName).c_str());

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             queueFamilies.data());

    int i = 0;
    for (auto &queueFamily : queueFamilies) {
      if ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
          (queueData.count(graphic) > 0)) {
        if (queueFamily.queueCount >= queueData[graphic].queueCount) {
          queueData[graphic].queueFamilyID = i;
          queueFamily.queueCount -= queueData[graphic].queueCount;
        }
      }
      if ((queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) &&
          (queueData.count(transfer) > 0)) {
        if (queueFamily.queueCount >= queueData[transfer].queueCount) {
          queueData[transfer].queueFamilyID = i;
          queueFamily.queueCount -= queueData[transfer].queueCount;
        }
      }
      if ((queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
          (queueData.count(compute) > 0)) {
        if (queueFamily.queueCount >= queueData[compute].queueCount) {
          queueData[compute].queueFamilyID = i;
          queueFamily.queueCount -= queueData[compute].queueCount;
        }
      }
      if ((queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) &&
          (queueData.count(sparse) > 0)) {
        if (queueFamily.queueCount >= queueData[sparse].queueCount) {
          queueData[sparse].queueFamilyID = i;
          queueFamily.queueCount -= queueData[sparse].queueCount;
        }
      }

      if (deviceBlueprint.IsGraphicEnable) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, deviceBlueprint.surface,
                                             &presentSupport);
        if (presentSupport) {
          if (queueFamily.queueCount >= queueData[present].queueCount) {
            queueData[present].queueFamilyID = i;
            queueFamily.queueCount -= queueData[present].queueCount;
          }
        }
      }

      i++;
    }
    bool isComplete = true;
    for (auto it = queueData.begin(); it != queueData.end(); ++it) {
      isComplete &= it->second.queueFamilyID.has_value();
    }
    if (isComplete) {
      physicalDevice = device;
      break;
    }

    index++;
  }

  if (physicalDevice == VK_NULL_HANDLE) {
    uppexo::Log::GetInstance().logError("No suitable GPU is selected!\n");
  } else {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);
    uppexo::Log::GetInstance().logInfo("Selected GPU: %s\n",
                                       properties.deviceName);
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    uppexo::Log::GetInstance().logInfo("Listing system memory\n");
    uppexo::Log::GetInstance().logInfo("#N - size - flags\n");
    for (int memIdx = 0; memIdx < memProperties.memoryTypeCount; memIdx++) {
      uppexo::Log::GetInstance().logInfo(
          "%i - %iMB - %s\n", memIdx,
          memProperties.memoryHeaps[memProperties.memoryTypes[memIdx].heapIndex]
                  .size /
              (1024 * 1024),
          uppexo::MemoryPropertyFlagsToString(
              memProperties.memoryTypes[memIdx].propertyFlags)
              .c_str());
    }
  }

  uppexo::Log::GetInstance().logVerbose("Creating logical device\n");

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

  float queuePriority = 1.0f;
  for (auto it = queueData.begin(); it != queueData.end(); ++it) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = it->second.queueFamilyID.value();
    queueCreateInfo.queueCount = it->second.queueCount;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};
  VkDeviceCreateInfo deviceCreateInfo{};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.queueCreateInfoCount =
      static_cast<uint32_t>(queueCreateInfos.size());
  deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
  deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
  if (deviceBlueprint.isSwapchainRequire) {
    deviceCreateInfo.enabledExtensionCount =
        static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
  } else {
    deviceCreateInfo.enabledExtensionCount = 0;
  }
  if (deviceBlueprint.IsValidationLayerEnable) {
    deviceCreateInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    deviceCreateInfo.enabledLayerCount = 0;
  }
  if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr,
                     &logicalDevice) != VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to create logical device\n");
  }

  uppexo::Log::GetInstance().logVerbose("Fetching Vulkan queue\n");

  for (auto it = queueData.begin(); it != queueData.end(); ++it) {
    for (int i = 0; i < it->second.queueCount; i++) {
      vkGetDeviceQueue(logicalDevice, it->second.queueFamilyID.value(), i,
                       &it->second.queue[i]);
    }
  }

  if (deviceBlueprint.isSwapchainRequire) {
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        physicalDevice, deviceBlueprint.surface, &SwapChainCapabilities);
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        physicalDevice, deviceBlueprint.surface, &formatCount, nullptr);
    if (formatCount != 0) {
      SwapChainFormats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(
          physicalDevice, deviceBlueprint.surface, &formatCount,
          SwapChainFormats.data());
    }
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        physicalDevice, deviceBlueprint.surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
      SwapChainPresentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(
          physicalDevice, deviceBlueprint.surface, &presentModeCount,
          SwapChainPresentModes.data());
    }

    VkSurfaceFormatKHR surfaceFormat = SwapChainFormats[0];
    for (const auto &availableFormat : SwapChainFormats) {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
          availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        surfaceFormat = availableFormat;
      }
    }

    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (const auto &availablePresentMode : SwapChainPresentModes) {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        presentMode = availablePresentMode;
      }
    }

    VkExtent2D extent;
    if (SwapChainCapabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max()) {
      extent = SwapChainCapabilities.currentExtent;
    } else {
      int width, height;
      glfwGetFramebufferSize(deviceBlueprint.window, &width, &height);

      VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                                 static_cast<uint32_t>(height)};

      actualExtent.width = std::clamp(
          actualExtent.width, SwapChainCapabilities.minImageExtent.width,
          SwapChainCapabilities.maxImageExtent.width);
      actualExtent.height = std::clamp(
          actualExtent.height, SwapChainCapabilities.minImageExtent.height,
          SwapChainCapabilities.maxImageExtent.height);

      extent = actualExtent;
    }

    uint32_t imageCount = SwapChainCapabilities.minImageCount + 1;
    if (SwapChainCapabilities.maxImageCount > 0 &&
        imageCount > SwapChainCapabilities.maxImageCount) {
      imageCount = SwapChainCapabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo{};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = deviceBlueprint.surface;

    swapChainCreateInfo.minImageCount = imageCount;
    swapChainCreateInfo.imageFormat = surfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent = extent;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[2] = {
        (uint32_t)queueData[graphic].queueFamilyID.value(),
        (uint32_t)queueData[present].queueFamilyID.value()};

    if (queueFamilyIndices[1] != queueFamilyIndices[0]) {
      swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      swapChainCreateInfo.queueFamilyIndexCount = 2;
      swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
      swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    swapChainCreateInfo.preTransform = SwapChainCapabilities.currentTransform;
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.clipped = VK_TRUE;

    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(logicalDevice, &swapChainCreateInfo, nullptr,
                             &swapChain) != VK_SUCCESS) {
      uppexo::Log::GetInstance().logError("Failed to create swap chain\n");
    }

    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount,
                            swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
      VkImageViewCreateInfo imageViewCreateInfo{};
      imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      imageViewCreateInfo.image = swapChainImages[i];
      imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      imageViewCreateInfo.format = swapChainImageFormat;
      imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewCreateInfo.subresourceRange.aspectMask =
          VK_IMAGE_ASPECT_COLOR_BIT;
      imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
      imageViewCreateInfo.subresourceRange.levelCount = 1;
      imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
      imageViewCreateInfo.subresourceRange.layerCount = 1;

      if (vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr,
                            &swapChainImageViews[i]) != VK_SUCCESS) {
        uppexo::Log::GetInstance().logError("Failed to create image view\n");
      }
    }
  }
  isSwapChainEnable = deviceBlueprint.isSwapchainRequire;
}

uppexo::Device::~Device() {
  uppexo::Log::GetInstance().logInfo("Deallocating device\n");
  if (isSwapChainEnable) {
    uppexo::Log::GetInstance().logVerbose("Destroying image view\n");
    for (auto imageView : swapChainImageViews) {
      vkDestroyImageView(logicalDevice, imageView, nullptr);
    }
    uppexo::Log::GetInstance().logVerbose("Destroying swap chain\n");
    vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
  }
  uppexo::Log::GetInstance().logVerbose("Destroying logical device\n");
  vkDestroyDevice(logicalDevice, nullptr);
}

VkDevice uppexo::Device::getLogicalDevice() { return logicalDevice; }
VkPhysicalDevice uppexo::Device::getPhysicalDevice() { return physicalDevice; }

VkFormat uppexo::Device::getSwapChainImageFormat() {
  return swapChainImageFormat;
}

std::vector<VkImageView> uppexo::Device::getSwapChainImageView() {
  return swapChainImageViews;
}

VkExtent2D uppexo::Device::getSwapChainExtend() { return swapChainExtent; }

uppexo::QueueData uppexo::Device::getQueue(uppexo::QueueType type) {
  return queueData[type];
}

VkSwapchainKHR uppexo::Device::getSwapChain() { return swapChain; }
