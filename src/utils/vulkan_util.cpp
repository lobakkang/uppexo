#include <cstring>
#include <fstream>
#include <set>
#include <string>
#include <utils/log.hpp>
#include <utils/vulkan_util.hpp>

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData) {
  uppexo::Log::GetInstance().logWarning(
      "Validation Layer: %s \n", std::string(pCallbackData->pMessage).c_str());

  return VK_FALSE;
}

bool uppexo::checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

std::vector<const char *>
uppexo::getRequiredExtensions(bool isValidationLayerEnable) {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);

  if (isValidationLayerEnable) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

void uppexo::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
}

bool uppexo::checkDeviceExtensionSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);
  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());
  std::set<std::string> requiredExtensions(deviceExtensions.begin(),
                                           deviceExtensions.end());
  for (const auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }
  return requiredExtensions.empty();
}

VkShaderModule uppexo::createShaderModule(VkDevice device,
                                          const std::vector<char> &code) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) !=
      VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to create shader module\n");
  }

  return shaderModule;
}

std::vector<char> uppexo::readFile(std::string filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    uppexo::Log::GetInstance().logError("Failed to open file, %s\n",
                                        filename.c_str());
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

std::string uppexo::MemoryPropertyFlagsToString(VkMemoryPropertyFlags flags) {
  std::string result;

  if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
    result += "DEVICE_LOCAL ";
  }

  if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
    result += "HOST_VISIBLE ";
  }

  if (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
    result += "HOST_COHERENT ";
  }

  if (flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) {
    result += "HOST_CACHED ";
  }

  if (flags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) {
    result += "LAZILY_ALLOCATED ";
  }

  if (result.empty()) {
    result = "NONE";
  }

  return result;
}

std::string uppexo::BufferLocationToString(uppexo::BufferLocation location) {
  switch (location) {
  case uppexo::BufferLocation::ON_DEVICE_VISIBLE_TO_HOST:
    return "ON DEVICE VISIBLE TO HOST";
    break;
  case uppexo::BufferLocation::ON_DEVICE_INVISIBLE_TO_HOST:
    return "ON DEVICE INVISIBLE TO HOST";
    break;
  case uppexo::BufferLocation::ON_HOST_VISIBLE_TO_DEVICE:
    return "ON HOST VISIBLE TO DEVICE";
    break;
  default:
    return "UNKNOWN";
    break;
  }
}

VkFormat uppexo::findSupportedFormat(const std::vector<VkFormat> &candidates,
                                     VkImageTiling tiling,
                                     VkFormatFeatureFlags features,
                                     VkPhysicalDevice physicalDevice) {
  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
               (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  uppexo::Log::GetInstance().logError("Failed to find supported format\n");
  return candidates[0];
}

VkFormat uppexo::findDepthFormat(VkPhysicalDevice physicalDevice) {
  return uppexo::findSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
      physicalDevice);
}

uint32_t uppexo::findMemoryType(uint32_t typeFilter,
                                VkMemoryPropertyFlags properties,
                                VkPhysicalDevice physicalDevice) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    properties) == properties) {
      return i;
    }
  }

  uppexo::Log::GetInstance().logError("Failed to find suitable memory type\n");
  return 0;
}
