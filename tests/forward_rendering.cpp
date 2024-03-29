#include <chrono>
#include <generated/forwardFragmentShader.h>
#include <generated/forwardVertexShader.h>
#include <thread>
#include <uppexo.hpp>

#include <core/gui.hpp>

#define IMAGE_AVAILABLE_SEMAPHORE 2
#define RENDER_FINISH_SEMAPHORE 0

int main(void) {
  uppexo::Uppexo uppexoEngine({640, 480}, "forward rendering demo", true);

  auto device = uppexoEngine.addDevice();
  auto graphicQueue = device.addQueue(uppexo::graphic);
  auto presentQueue = device.addQueue(uppexo::present);
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

  auto renderPass = uppexoEngine.addRenderPass(device);
  auto swapchainAttachment = renderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::SwapchainAttachment());
  auto depthAttachment = renderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::DepthAttachment());
  renderPass.addSubpass({swapchainAttachment}, depthAttachment);
  renderPass.create();

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
  image.addImageCell(
      uppexo::presetImageCellBlueprint::DepthImageCellBlueprint());
  image.create();

  auto frameBuffer = uppexoEngine.addFrameBuffer(device, renderPass);
  frameBuffer.addImageView(image, 1, 0);
  frameBuffer.create();

  auto gui = uppexoEngine.addGui(device, commandBuffer);
  gui.create();

  auto descriptorSet = uppexoEngine.addDescriptorSet(device);
  descriptorSet.addBinding(
      0, uppexo::presetDescriptorSetBindingBlueprint::UBO_at_vertex_shader(
             buffer, uniformBuffer0, sizeof(uppexo::MVP)));
  descriptorSet.addBinding(
      0,
      uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
          sampler, image, 0, 0, 0));
  descriptorSet.addBinding(
      1, uppexo::presetDescriptorSetBindingBlueprint::UBO_at_vertex_shader(
             buffer, uniformBuffer1, sizeof(uppexo::MVP)));
  descriptorSet.addBinding(
      1,
      uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
          sampler, image, 0, 0, 0));
  descriptorSet.create();

  auto graphicPipeline =
      uppexoEngine.addGraphicPipeline(device, renderPass, descriptorSet);
  graphicPipeline.addVertexShaderFromCode((char *)forwardVertexShader,
                                          forwardVertexShader_size);
  graphicPipeline.addFragmentShaderFromCode((char *)forwardFragmentShader,
                                            forwardFragmentShader_size);
  graphicPipeline.isDepthEnable = true;
  graphicPipeline.create();

  auto synchronizer = uppexoEngine.addSynchronizer(device);
  synchronizer.addFence();
  synchronizer.addFence();
  synchronizer.addSemaphore();
  synchronizer.addSemaphore();
  synchronizer.addSemaphore();
  synchronizer.addSemaphore();
  synchronizer.create();

  // int imageIndex = 0;
  //  int frame = 0;

  uppexo::MeshInfo room;
  room.path = "./demo/viking_room.obj";
  uppexo::Mesh<uppexo::FullVertex, uppexo::MVP> mesh;
  mesh.addMesh(room);

  buffer.getComponent().copyByStaging(
      0, commandBuffer.getComponent(), mesh.getVertexList(),
      mesh.getVertexCount() * sizeof(uppexo::FullVertex));
  buffer.getComponent().copyByStaging(1, commandBuffer.getComponent(),
                                      mesh.getIndexList(),
                                      mesh.getIndexCount() * sizeof(uint32_t));

  auto sequence = uppexoEngine.addSequence();
  sequence.add(uppexo::command::BeginRecorder());
  sequence.add(uppexo::command::BeginRenderPass(renderPass, frameBuffer, device,
                                                uppexoEngine.getImageIndex()));
  sequence.add(uppexo::command::BindGraphicPipeline(graphicPipeline));
  sequence.add(uppexo::command::BindVertexBuffer(buffer, vertexBuffer));
  sequence.add(uppexo::command::SetViewport(device));
  sequence.add(uppexo::command::SetScissor(device));
  sequence.add(uppexo::command::BindGraphicDescriptorSet(
      descriptorSet, graphicPipeline, uppexoEngine.getFrame()));
  sequence.add(
      uppexo::command::IndexedDraw(buffer, indexBuffer, mesh.getIndexCount()));
  sequence.add(uppexo::command::RenderGUI());
  sequence.add(uppexo::command::EndRenderPass());
  sequence.add(uppexo::command::EndRecorder());

  uppexoEngine.preRender = [&](int tick) {};
  uppexoEngine.onRender = [&](int tick, int frame) {
    uppexo::MVP ubo{};
    ubo.model =
        glm::rotate(glm::mat4(1.0f), (float)tick / 1000 * glm::radians(90.0f),
                    glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view =
        glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(
        glm::radians(45.0f),
        (float)device.getComponent().getSwapChainExtend().width /
            (float)device.getComponent().getSwapChainExtend().height,
        0.1f, 100.0f);
    ubo.proj[1][1] *= -1;
    mesh.getMVPList()[0] = ubo;

    buffer.getComponent().copyByMapping(
        2 + frame, mesh.getMVPList(), mesh.getMVPCount() * sizeof(uppexo::MVP));
  };
  uppexoEngine.postRender = [&](int tick) {};

  uppexoEngine.run(gui, synchronizer, commandBuffer, device, sequence,
                   graphicQueue);
  return 0;
}
