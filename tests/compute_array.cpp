#include <generated/computeShader.h>
#include <iostream>
#include <thread>
#include <uppexo.hpp>

#define IMAGE_AVAILABLE_SEMAPHORE 2
#define RENDER_FINISH_SEMAPHORE 0

int main(void) {
  uppexo::Uppexo uppexoEngine({0, 0}, "forward rendering demo", true);

  auto device = uppexoEngine.addDevice();
  auto computeQueue = device.addQueue(uppexo::compute);
  device.create();

  auto buffer = uppexoEngine.addBuffer(device);
  int inputBuffer = buffer.addCell(
      uppexo::presetBufferCellBlueprint::SSBO_at_device(sizeof(int) * 100));
  int outputBuffer = buffer.addCell(
      uppexo::presetBufferCellBlueprint::SSBO_at_device(sizeof(int) * 100));
  buffer.create();

  auto commandBuffer = uppexoEngine.addCommandBuffer(device, computeQueue);
  commandBuffer.setBufferNumber(1);
  commandBuffer.create();

  auto descriptorSet = uppexoEngine.addDescriptorSet(device);
  descriptorSet.addBinding(
      0, uppexo::presetDescriptorSetBindingBlueprint::SSBO_at_compute_shader(
             buffer, inputBuffer, sizeof(int) * 100));
  descriptorSet.addBinding(
      0, uppexo::presetDescriptorSetBindingBlueprint::SSBO_at_compute_shader(
             buffer, outputBuffer, sizeof(int) * 100));
  descriptorSet.create();

  auto computePipeline = uppexoEngine.addComputePipeline(device, descriptorSet);
  computePipeline.addShaderFromCode((char *)computeShader, computeShader_size);
  computePipeline.create();

  auto synchronizer = uppexoEngine.addSynchronizer(device);
  synchronizer.addFence();
  synchronizer.create();

  auto sequence = uppexoEngine.addSequence();
  sequence.add(uppexo::command::BeginRecorder());
  sequence.add(uppexo::command::BindComputePipeline(computePipeline));
  sequence.add(uppexo::command::BindComputeDescriptorSet(descriptorSet,
                                                         computePipeline, 0));
  sequence.add(uppexo::command::DispatchCompute(1, 1, 1));
  sequence.add(uppexo::command::EndRecorder());

  std::vector<float> data;
  for (int i = 0; i < 100; i++) {
    data.push_back(i);
  }
  uppexo::Log::GetInstance().logInfo("Before:\n");
  for (int i = 0; i < 100; i++) {
    std::cout << data[i] << " ";
  }
  std::cout << std::endl;

  buffer.getComponent().copyByMapping(0, data.data(), sizeof(float) * 100);

  synchronizer.getComponent().resetFence({0});
  sequence.record(commandBuffer, 0);
  sequence.execute(commandBuffer, 0, device, computeQueue, synchronizer, {}, {},
                   0);
  synchronizer.getComponent().waitForFence({0}, true);
  buffer.getComponent().copyOutByMapping(1, data.data(), sizeof(float) * 100);

  uppexo::Log::GetInstance().logInfo("After:\n");
  for (int i = 0; i < 100; i++) {
    std::cout << data[i] << " ";
  }
  std::cout << std::endl;
}
