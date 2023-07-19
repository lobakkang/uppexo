#include <engine/engine.hpp>
#include <vulkan/vulkan_core.h>

uppexo::Uppexo::Uppexo(VkExtent2D windowSize, std::string title,
                       bool validationLayer) {
  InstanceBlueprint instanceBlueprint;

  instanceBlueprint.appName = title;
  instanceBlueprint.isValidationLayerEnable = validationLayer;
  if (windowSize.height != 0 && windowSize.width != 0) {
    uppexo::Log::GetInstance().logVerbose("Graphic enabled\n");
    instanceBlueprint.windowWidth = windowSize.width;
    instanceBlueprint.windowHeight = windowSize.height;
    instanceBlueprint.isGraphicEnable = true;
  } else {
    uppexo::Log::GetInstance().logVerbose("Graphic disabled\n");
    instanceBlueprint.isGraphicEnable = false;
  }

  addComponent<Instance>(instanceBlueprint);
}

uppexo::Uppexo::~Uppexo() {
  uppexo::Log::GetInstance().logInfo("Shuting down uppexo engine\n");
  vkDeviceWaitIdle(getComponent<Device>(1).getLogicalDevice());
  std::for_each(componentList.rbegin(), componentList.rend(),
                [](std::unique_ptr<void, void (*)(void *)> &i) { i.reset(); });
}

TrackedBlueprint<uppexo::BufferBlueprint>
uppexo::Uppexo::addBuffer(TrackedBlueprint<DeviceBlueprint> &device) {
  TrackedBlueprint<uppexo::BufferBlueprint> blueprint(
      getComponent<Device>(device.componentID));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<Buffer>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> Buffer & {
    return this->getComponent<Buffer>(blueprint.componentID);
  };
  return blueprint;
}

TrackedBlueprint<uppexo::SamplerBlueprint>
uppexo::Uppexo::addSampler(TrackedBlueprint<DeviceBlueprint> &device) {
  TrackedBlueprint<uppexo::SamplerBlueprint> blueprint(
      getComponent<Device>(device.componentID));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<Sampler>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> Sampler & {
    return this->getComponent<Sampler>(blueprint.componentID);
  };
  return blueprint;
}

TrackedBlueprint<uppexo::SynchronizerBlueprint>
uppexo::Uppexo::addSynchronizer(TrackedBlueprint<DeviceBlueprint> &device) {
  TrackedBlueprint<uppexo::SynchronizerBlueprint> blueprint(
      getComponent<Device>(device.componentID));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<Synchronizer>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> Synchronizer & {
    return this->getComponent<Synchronizer>(blueprint.componentID);
  };
  return blueprint;
}

TrackedBlueprint<uppexo::CommandBufferBlueprint>
uppexo::Uppexo::addCommandBuffer(TrackedBlueprint<DeviceBlueprint> &device) {
  TrackedBlueprint<uppexo::CommandBufferBlueprint> blueprint(
      getComponent<Device>(device.componentID));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<CommandBuffer>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> CommandBuffer & {
    return this->getComponent<CommandBuffer>(blueprint.componentID);
  };
  return blueprint;
}

TrackedBlueprint<uppexo::RenderpassBlueprint>
uppexo::Uppexo::addRenderPass(TrackedBlueprint<DeviceBlueprint> &device) {
  TrackedBlueprint<uppexo::RenderpassBlueprint> blueprint(
      getComponent<Device>(device.componentID));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<Renderpass>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> Renderpass & {
    return this->getComponent<Renderpass>(blueprint.componentID);
  };
  return blueprint;
}

TrackedBlueprint<uppexo::ImageBlueprint> uppexo::Uppexo::addImage(
    TrackedBlueprint<DeviceBlueprint> &device,
    TrackedBlueprint<CommandBufferBlueprint> &commandBuffer,
    TrackedBlueprint<BufferBlueprint> &buffer) {
  TrackedBlueprint<uppexo::ImageBlueprint> blueprint(
      getComponent<Device>(device.componentID),
      getComponent<Buffer>(buffer.componentID),
      getComponent<CommandBuffer>(commandBuffer.componentID));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<Image>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> Image & {
    return this->getComponent<Image>(blueprint.componentID);
  };
  return blueprint;
}

TrackedBlueprint<uppexo::DescriptorSetBlueprint>
uppexo::Uppexo::addDescriptorSet(TrackedBlueprint<DeviceBlueprint> &device) {
  TrackedBlueprint<uppexo::DescriptorSetBlueprint> blueprint(
      getComponent<Device>(device.componentID));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<DescriptorSet>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> DescriptorSet & {
    return this->getComponent<DescriptorSet>(blueprint.componentID);
  };
  return blueprint;
}

TrackedBlueprint<uppexo::FramebufferBlueprint> uppexo::Uppexo::addFrameBuffer(
    TrackedBlueprint<DeviceBlueprint> &device,
    TrackedBlueprint<RenderpassBlueprint> &renderPass) {
  TrackedBlueprint<uppexo::FramebufferBlueprint> blueprint(
      getComponent<Device>(device.componentID),
      getComponent<Renderpass>(renderPass.componentID));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<Framebuffer>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> Framebuffer & {
    return this->getComponent<Framebuffer>(blueprint.componentID);
  };
  return blueprint;
}

TrackedBlueprint<uppexo::GraphicPipelineBlueprint>
uppexo::Uppexo::addGraphicPipeline(
    TrackedBlueprint<DeviceBlueprint> &device,
    TrackedBlueprint<RenderpassBlueprint> &renderPass,
    TrackedBlueprint<DescriptorSetBlueprint> &descriptorSet) {
  TrackedBlueprint<uppexo::GraphicPipelineBlueprint> blueprint(
      getComponent<Device>(device.componentID),
      getComponent<Renderpass>(renderPass.componentID),
      getComponent<DescriptorSet>(descriptorSet.componentID));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<GraphicPipeline>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> GraphicPipeline & {
    return this->getComponent<GraphicPipeline>(blueprint.componentID);
  };
  return blueprint;
}

TrackedBlueprint<uppexo::DeviceBlueprint> uppexo::Uppexo::addDevice() {
  TrackedBlueprint<uppexo::DeviceBlueprint> blueprint(
      getComponent<Instance>(0));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<Device>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> Device & {
    return this->getComponent<Device>(blueprint.componentID);
  };
  return blueprint;
}

uppexo::Sequence &uppexo::Uppexo::addSequence() {
  Sequence sequence;
  sequenceList.push_back(sequence);
  return sequenceList[sequenceList.size() - 1];
}

bool uppexo::Uppexo::isRunning() {
  glfwPollEvents();
  return !glfwWindowShouldClose(getComponent<Instance>(0).getWindow());
}
