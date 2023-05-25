#include <base/instance.hpp>
#include <utils/log.hpp>
#include <utils/vulkan_util.hpp>

uppexo::Instance::Instance(uppexo::InstanceBlueprint instanceBlueprint) {
  uppexo::Log::GetInstance().logInfo("Creating Instance\n");
  if (instanceBlueprint.isGraphicEnable) {
    uppexo::Log::GetInstance().logVerbose("Creating GLFW window\n");
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(
        instanceBlueprint.windowWidth, instanceBlueprint.windowHeight,
        instanceBlueprint.appName.c_str(), nullptr, nullptr);
  }

  if (instanceBlueprint.isValidationLayerEnable &&
      !checkValidationLayerSupport()) {
    uppexo::Log::GetInstance().logError(
        "Validation layer requested but not available\n");
  }

  uppexo::Log::GetInstance().logVerbose("Creating Vulkan instance\n");
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = instanceBlueprint.appName.c_str();
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Uppexo Render Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  auto extensions =
      getRequiredExtensions(instanceBlueprint.isValidationLayerEnable);
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (instanceBlueprint.isValidationLayerEnable) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;

    createInfo.pNext = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    uppexo::Log::GetInstance().logError("Failed to create Vulkan instance\n");
  }

  this->isValidationLayerEnable = instanceBlueprint.isValidationLayerEnable;
  this->isGraphicEnable = instanceBlueprint.isGraphicEnable;

  uppexo::Log::GetInstance().logVerbose("Creating debug messenger\n");
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);
    func(instance, &createInfo, nullptr, &debugMessenger);
  } else {
    uppexo::Log::GetInstance().logError("Failed to setup debug messenger\n");
  }

  if (instanceBlueprint.isGraphicEnable) {
    uppexo::Log::GetInstance().logVerbose("Creating Vulkan surface\n");
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) !=
        VK_SUCCESS) {
      uppexo::Log::GetInstance().logError("failed to create window surface!\n");
    }
  }
}

uppexo::Instance::~Instance() {
  uppexo::Log::GetInstance().logInfo("Deallocating Instance\n");
  if (isGraphicEnable) {
    uppexo::Log::GetInstance().logVerbose("Destroying Vulkan surface\n");
    vkDestroySurfaceKHR(instance, surface, nullptr);
  }

  uppexo::Log::GetInstance().logVerbose("Destroying debug messenger\n");
  if (isValidationLayerEnable) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
      func(instance, debugMessenger, nullptr);
    }
  }
  uppexo::Log::GetInstance().logVerbose("Destroying Vulkan instance\n");
  vkDestroyInstance(instance, nullptr);
  if (isGraphicEnable) {
    uppexo::Log::GetInstance().logVerbose("Destroying GLFW window\n");
    glfwDestroyWindow(window);
    glfwTerminate();
  }
}

VkInstance &uppexo::Instance::getInstance() { return instance; }

VkSurfaceKHR &uppexo::Instance::getSurface() { return surface; }

bool uppexo::Instance::IsValidationLayerEnable() {
  return isValidationLayerEnable;
}

bool uppexo::Instance::IsGraphicEnable() { return isGraphicEnable; }

GLFWwindow *uppexo::Instance::getWindow() { return window; }
