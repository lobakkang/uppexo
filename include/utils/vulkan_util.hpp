#ifndef VULKAN_UTIL_H
#define VULKAN_UTIL_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <base/buffer.hpp>
#include <string>
#include <vector>

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

namespace uppexo {
bool checkValidationLayerSupport();
std::vector<const char *> getRequiredExtensions(bool isValidationLayerEnable);
void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo);
bool checkDeviceExtensionSupport(VkPhysicalDevice device);
VkShaderModule createShaderModule(VkDevice device, char *code, int len);
std::vector<char> readFile(std::string filename);
std::string MemoryPropertyFlagsToString(VkMemoryPropertyFlags flags);
std::string BufferLocationToString(BufferLocation location);
VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                             VkImageTiling tiling,
                             VkFormatFeatureFlags features,
                             VkPhysicalDevice physicalDevice);
VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties,
                        VkPhysicalDevice physicalDevice);
}; // namespace uppexo

#endif /* end of include guard: VULKAN_UTIL_H */
