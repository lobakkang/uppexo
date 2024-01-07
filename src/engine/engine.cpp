#include <chrono>
#include <thread>

#include <engine/engine.hpp>
#include <vulkan/vulkan_core.h>

#define IMAGE_AVAILABLE_SEMAPHORE 2
#define RENDER_FINISH_SEMAPHORE 0

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
  int x = componentList.size() - 1;
  std::for_each(componentList.rbegin(), componentList.rend(),
                [&](std::unique_ptr<void, void (*)(void *)> &i) {
                  if (!isComponentDeleted[x]) {
                    i.reset();
                  }
                  x--;
                });
}

uppexo::TrkBufferBlueprint
uppexo::Uppexo::addBuffer(TrkDeviceBlueprint &device) {
  uppexo::TrkBufferBlueprint blueprint(
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

uppexo::TrkSamplerBlueprint
uppexo::Uppexo::addSampler(TrkDeviceBlueprint &device) {
  uppexo::TrkSamplerBlueprint blueprint(
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

uppexo::TrkSynchronizerBlueprint
uppexo::Uppexo::addSynchronizer(TrkDeviceBlueprint &device) {
  uppexo::TrkSynchronizerBlueprint blueprint(
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

uppexo::TrkCommandBufferBlueprint
uppexo::Uppexo::addCommandBuffer(TrkDeviceBlueprint &device,
                                 std::tuple<QueueType, int> queue) {
  uppexo::TrkCommandBufferBlueprint blueprint(
      getComponent<Device>(device.componentID), std::get<0>(queue));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<CommandBuffer>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> CommandBuffer & {
    return this->getComponent<CommandBuffer>(blueprint.componentID);
  };
  return blueprint;
}

uppexo::TrkRenderpassBlueprint
uppexo::Uppexo::addRenderPass(TrkDeviceBlueprint &device) {
  uppexo::TrkRenderpassBlueprint blueprint(
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

uppexo::TrkImageBlueprint
uppexo::Uppexo::addImage(TrkDeviceBlueprint &device,
                         TrkCommandBufferBlueprint &commandBuffer,
                         TrkBufferBlueprint &buffer) {
  uppexo::TrkImageBlueprint blueprint(
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

uppexo::TrkDescriptorSetBlueprint
uppexo::Uppexo::addDescriptorSet(TrkDeviceBlueprint &device) {
  uppexo::TrkDescriptorSetBlueprint blueprint(
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

uppexo::TrkFramebufferBlueprint
uppexo::Uppexo::addFrameBuffer(TrkDeviceBlueprint &device,
                               TrkRenderpassBlueprint &renderPass,
                               VkExtent2D extend) {
  uppexo::TrkFramebufferBlueprint blueprint(
      getComponent<Device>(device.componentID),
      getComponent<Renderpass>(renderPass.componentID), extend);
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<Framebuffer>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> Framebuffer & {
    return this->getComponent<Framebuffer>(blueprint.componentID);
  };
  return blueprint;
}

uppexo::TrkGraphicPipelineBlueprint
uppexo::Uppexo::addGraphicPipeline(TrkDeviceBlueprint &device,
                                   TrkRenderpassBlueprint &renderPass,
                                   TrkDescriptorSetBlueprint &descriptorSet) {
  uppexo::TrkGraphicPipelineBlueprint blueprint(
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

uppexo::TrkComputePipelineBlueprint
uppexo::Uppexo::addComputePipeline(TrkDeviceBlueprint &device,
                                   TrkDescriptorSetBlueprint &descriptorSet) {
  uppexo::TrkComputePipelineBlueprint blueprint(
      getComponent<Device>(device.componentID),
      getComponent<DescriptorSet>(descriptorSet.componentID));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<ComputePipeline>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> ComputePipeline & {
    return this->getComponent<ComputePipeline>(blueprint.componentID);
  };
  return blueprint;
}

uppexo::TrkDeviceBlueprint uppexo::Uppexo::addDevice() {
  uppexo::TrkDeviceBlueprint blueprint(getComponent<Instance>(0));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<Device>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> Device & {
    return this->getComponent<Device>(blueprint.componentID);
  };
  return blueprint;
}

uppexo::TrkGuiBlueprint
uppexo::Uppexo::addGui(TrkDeviceBlueprint &device,
                       TrkCommandBufferBlueprint &commandBuffer) {
  uppexo::TrkGuiBlueprint blueprint(
      getComponent<Instance>(0), getComponent<Device>(device.componentID),
      getComponent<CommandBuffer>(commandBuffer.componentID));
  blueprint.create = [this, &blueprint]() {
    blueprint.componentID = this->componentList.size();
    this->addComponent<Gui>(blueprint);
  };
  blueprint.getComponent = [this, &blueprint]() -> Gui & {
    return this->getComponent<Gui>(blueprint.componentID);
  };
  return blueprint;
}

uppexo::Sequence &uppexo::Uppexo::addSequence() {
  Sequence sequence(sequenceList.size());
  sequenceList.push_back(sequence);
  return sequenceList[sequenceList.size() - 1];
}

bool uppexo::Uppexo::isRunning() {
  glfwPollEvents();
  return !glfwWindowShouldClose(getComponent<Instance>(0).getWindow());
}

void uppexo::Uppexo::setOnRenderFunc(
    std::function<void(long, int)> onRenderFunc) {
  onRender = onRenderFunc;
}

void uppexo::Uppexo::run(TrkGuiBlueprint &gui,
                         TrkSynchronizerBlueprint &synchronizer,
                         TrkCommandBufferBlueprint &commandBuffer,
                         TrkDeviceBlueprint &device, Sequence &sequence,
                         std::tuple<QueueType, int> queue) {
  auto startTime = std::chrono::high_resolution_clock::now();
  auto endTime = std::chrono::high_resolution_clock::now();
  static auto initTime = std::chrono::high_resolution_clock::now();

  int frame = 0;
  int imageIndex = 0;

  uppexo::Log::GetInstance().logInfo("Rendering loop stared\n");
  while (isRunning()) {
    endTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       endTime - startTime)
                       .count();
    long tick = std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                                      initTime)
                    .count();
    startTime = endTime;

    const int MAX_FRAME_RATE = 60;
    const int MIN_FRAME_TIME = 1000 / MAX_FRAME_RATE;
    if (elapsed < MIN_FRAME_TIME) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(MIN_FRAME_TIME - elapsed));
    }

    preRender(tick);
    gui.getComponent().render();

    synchronizer.getComponent().waitForFence({frame}, true);
    imageIndex = uppexo::Present::getImage(device, synchronizer,
                                           IMAGE_AVAILABLE_SEMAPHORE + frame);
    onRender(tick, frame);

    sequence.record(commandBuffer, frame);
    sequence.execute(commandBuffer, frame, device, queue, synchronizer,
                     {{IMAGE_AVAILABLE_SEMAPHORE + frame,
                       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}},
                     {RENDER_FINISH_SEMAPHORE + frame}, frame);

    uppexo::Present::presentImage(device, synchronizer,
                                  RENDER_FINISH_SEMAPHORE + frame, imageIndex);

    postRender(tick);

    frame++;
    frame %= 2;
  }
}
