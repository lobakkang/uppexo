#include <engine/compute.hpp>

int deviceID;
int instanceID;

uppexo::ComputeEngine::ComputeEngine(
    uppexo::ComputeEngineBlueprint computeEngineBlueprint)
    : Engine<ComputeEngine, ComputeEngineBlueprint>() {
  this->title = computeEngineBlueprint.title;
}

void uppexo::ComputeEngine::buildComponent() {
  uppexo::Log::GetInstance().logInfo("Building component\n");

  uppexo::InstanceBlueprint instanceBlueprint;
  instanceBlueprint.isValidationLayerEnable = true;
  instanceBlueprint.appName = title;
  instanceBlueprint.isGraphicEnable = false;
  instanceBlueprint.windowWidth = 100;
  instanceBlueprint.windowHeight = 100;
  instanceID = addComponent<uppexo::Instance>(instanceBlueprint);

  uppexo::DeviceBlueprint deviceBlueprint(
      getComponent<uppexo::Instance>(instanceID));
  deviceBlueprint.graphicQueue = 0;
  deviceBlueprint.presentQueue = 0;
  deviceBlueprint.sparseQueue = 0;
  deviceBlueprint.computeQueue = 1;
  deviceID = addComponent<uppexo::Device>(deviceBlueprint);
}

void uppexo::ComputeEngine::prerecordCommandBuffer() {
  uppexo::Log::GetInstance().logInfo("Prerecording command buffer\n");
}

void uppexo::ComputeEngine::run() {
  uppexo::Log::GetInstance().logInfo("Run\n");
  auto &device = getComponent<uppexo::Device>(deviceID);
  vkDeviceWaitIdle(device.getLogicalDevice());
}

uppexo::ComputeEngine::~ComputeEngine() {}
