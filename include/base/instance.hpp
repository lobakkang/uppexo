#ifndef INSTANCE_H
#define INSTANCE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <string>

namespace uppexo {
struct InstanceBlueprint {
  bool isValidationLayerEnable = true;
  std::string appName = "Uppexo client";
  bool isGraphicEnable = true;
  int windowWidth = 100;
  int windowHeight = 100;

  InstanceBlueprint() = default;
};

class Instance {
public:
  Instance(InstanceBlueprint instanceBlueprint);
  ~Instance();
  VkInstance &getInstance();
  VkSurfaceKHR &getSurface();
  GLFWwindow* getWindow();
  bool IsValidationLayerEnable();

private:
  VkInstance instance;
  VkSurfaceKHR surface;
  GLFWwindow *window;
  bool isValidationLayerEnable;
  bool isGraphicEnable;
  VkDebugUtilsMessengerEXT debugMessenger;
};
} // namespace uppexo

#endif
