#include <generated/forwardFragmentShader.h>
#include <generated/forwardVertexShader.h>
#include <iostream>
#include <thread>
#include <uppexo.hpp>

#define IMAGE_AVAILABLE_SEMAPHORE 2
#define RENDER_FINISH_SEMAPHORE 0

int main(void) {
  uppexo::Uppexo uppexoEngine({0, 0}, "offscreen rendering demo", true);

  auto device = uppexoEngine.addDevice();
  auto graphicQueue = device.addQueue(uppexo::graphic);
  device.create();

  auto buffer = uppexoEngine.addBuffer(device);
  int vertexBuffer =
      buffer.addCell(uppexo::presetBufferCellBlueprint::VBO_at_device(
          sizeof(uppexo::FullVertex) * 1024 * 1024));
  int indexBuffer = buffer.addCell(
      uppexo::presetBufferCellBlueprint::IBO_at_device(1024 * 1024));
  int uniformBuffer0 = buffer.addCell(
      uppexo::presetBufferCellBlueprint::UBO_at_host(sizeof(uppexo::MVP)));
  int uniformBuffer1 = buffer.addCell(
      uppexo::presetBufferCellBlueprint::UBO_at_host(sizeof(uppexo::MVP)));
  buffer.create();

  /*auto renderPass = uppexoEngine.addRenderPass(device);
  auto depthAttachment = renderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::DepthAttachment());
  renderPass.addSubpass({}, depthAttachment);
  renderPass.create();*/

  auto sampler = uppexoEngine.addSampler(device);
  sampler.addSampler();
  sampler.create();

  auto commandBuffer = uppexoEngine.addCommandBuffer(device);
  commandBuffer.setBufferNumber(2);
  commandBuffer.create();

  auto image = uppexoEngine.addImage(device, commandBuffer, buffer);
  image.addImageCell(
      uppexo::presetImageCellBlueprint::TextureImageCellBlueprint(
          "./demo/viking_room.png"));
  image.create();

  image.getComponent().exportImageToFile(
      "./lol.png", commandBuffer.getComponent(), buffer.getComponent(), 0, 0);
}
