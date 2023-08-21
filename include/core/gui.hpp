#ifndef GUI_HPP_
#define GUI_HPP_

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/imgui.h>

#include <base/commandbuffer.hpp>
#include <base/descriptor.hpp>
#include <base/device.hpp>
#include <base/instance.hpp>
#include <base/renderpass.hpp>
#include <base/sampler.hpp>

#include <memory>

namespace uppexo {
class Gui;

struct GuiBlueprint {
  using Component = Gui;

  Device *device;
  Instance *instance;
  CommandBuffer *commandbuffer;

  GuiBlueprint() = default;
  GuiBlueprint(Instance &instance, Device &device,
               CommandBuffer &commandbuffer) {
    this->device = &device;
    this->instance = &instance;
    this->commandbuffer = &commandbuffer;
  }
};

class Gui {
public:
  Gui(GuiBlueprint guiBlueprint);
  ~Gui();

  void render();

private:
  VkDevice device;
  VkPipelineCache pipelineCache = VK_NULL_HANDLE;

  VkDescriptorPool descriptorPool;
  std::shared_ptr<Renderpass> renderpass;
};
} // namespace uppexo

#endif
