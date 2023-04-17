#ifndef VULKAN_UTIL_H
#define VULKAN_UTIL_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
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
VkShaderModule createShaderModule(VkDevice device,
                                  const std::vector<char> &code);
std::vector<char> readFile(std::string filename);
std::string MemoryPropertyFlagsToString(VkMemoryPropertyFlags flags);
}; // namespace uppexo

#endif /* end of include guard: VULKAN_UTIL_H */
