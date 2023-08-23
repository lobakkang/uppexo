#include <iostream>
#include <thread>
#include <uppexo.hpp>

#include <generated/offscreenFragmentShader.h>
#include <generated/offscreenVertexShader.h>

#define IMAGE_AVAILABLE_SEMAPHORE 2
#define RENDER_FINISH_SEMAPHORE 0

int main(void) {
  uppexo::Uppexo uppexoEngine({0, 0}, "offscreen rendering demo", true);

  //uppexo::Log::GetInstance().setLevel(uppexo::LOG_LEVEL_INFO);

  auto device = uppexoEngine.addDevice();
  auto graphicQueue = device.addQueue(uppexo::graphic);
  device.create();

  auto buffer = uppexoEngine.addBuffer(device);
  int vertexBuffer =
      buffer.addCell(uppexo::presetBufferCellBlueprint::VBO_at_device(
          sizeof(uppexo::PhongVertex) * 1024 * 1024));
  int indexBuffer = buffer.addCell(
      uppexo::presetBufferCellBlueprint::IBO_at_device(1024 * 1024));
  int uniformBuffer =
      buffer.addCell(uppexo::presetBufferCellBlueprint::UBO_at_host(
          sizeof(uppexo::MVP_with_normalized_matrix)));
  int materialBuffer =
      buffer.addCell(uppexo::presetBufferCellBlueprint::UBO_at_host(
          sizeof(uppexo::Material) * 10));
  buffer.create();

  uppexo::MeshInfo gun;
  gun.path = "./demo/ak-47.obj";
  uppexo::Mesh<uppexo::PhongVertex, uppexo::MVP_with_normalized_matrix> mesh;
  mesh.addMesh(gun);

  buffer.getComponent().copyByMapping(0, mesh.getVertexList(),
                                      mesh.getVertexCount() *
                                          sizeof(uppexo::PhongVertex));
  buffer.getComponent().copyByMapping(1, mesh.getIndexList(),
                                      mesh.getIndexCount() * sizeof(uint32_t));
  buffer.getComponent().copyByMapping(3, mesh.getMaterialList(),
                                      mesh.getMaterialCount() *
                                          sizeof(uppexo::Material));

  auto renderPass = uppexoEngine.addRenderPass(device);
  auto offscreenAttachment = renderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::OffscreenAttachment(
          VK_FORMAT_R8G8B8A8_SRGB));
  auto depthAttachment = renderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::DepthAttachment(VK_FORMAT_D32_SFLOAT));
  renderPass.addSubpass({offscreenAttachment}, depthAttachment);
  renderPass.create();

  auto commandBuffer = uppexoEngine.addCommandBuffer(device);
  commandBuffer.setBufferNumber(1);
  commandBuffer.create();

  auto descriptorSet = uppexoEngine.addDescriptorSet(device);
  descriptorSet.addBinding(
      0,
      uppexo::presetDescriptorSetBindingBlueprint::UBO_at_vertex_shader(
          buffer, uniformBuffer, sizeof(uppexo::MVP_with_normalized_matrix)));
  descriptorSet.addBinding(
      0, uppexo::presetDescriptorSetBindingBlueprint::UBO_at_fragment_shader(
             buffer, materialBuffer,
             mesh.getMaterialCount() * sizeof(uppexo::Material)));
  descriptorSet.create();

  auto graphicPipeline =
      uppexoEngine.addGraphicPipeline(device, renderPass, descriptorSet);
  graphicPipeline.setVertexType<uppexo::PhongVertex>();
  graphicPipeline.addVertexShaderFromCode((char *)offscreenVertexShader,
                                          offscreenVertexShader_size);
  graphicPipeline.addFragmentShaderFromCode((char *)offscreenFragmentShader,
                                            offscreenFragmentShader_size);
  graphicPipeline.isDepthEnable = true;
  graphicPipeline.create();

  auto image = uppexoEngine.addImage(device, commandBuffer, buffer);
  image.addImageCell(
      uppexo::presetImageCellBlueprint::OffscreenRenderingImageCellBlueprint(
          {640, 480}));
  image.addImageCell(
      uppexo::presetImageCellBlueprint::OffscreenDepthImageCellBlueprint(
          {640, 480}));
  image.create();

  auto frameBuffer =
      uppexoEngine.addFrameBuffer(device, renderPass, {640, 480});
  frameBuffer.addImageView(image, 0, 0);
  frameBuffer.addImageView(image, 1, 0);
  frameBuffer.create();

  int imageIndex = 0;
  int frame = 0;

  auto sequence = uppexoEngine.addSequence();
  sequence.add(uppexo::command::BeginRecorder());
  sequence.add(uppexo::command::BeginRenderPass(renderPass, frameBuffer,
                                                {640, 480}, imageIndex));
  sequence.add(uppexo::command::BindGraphicPipeline(graphicPipeline));
  sequence.add(uppexo::command::BindVertexBuffer(buffer, vertexBuffer));
  sequence.add(uppexo::command::SetViewport({640, 480}));
  sequence.add(uppexo::command::SetScissor({640, 480}));
  sequence.add(uppexo::command::BindGraphicDescriptorSet(
      descriptorSet, graphicPipeline, frame));
  sequence.add(
      uppexo::command::IndexedDraw(buffer, indexBuffer, mesh.getIndexCount()));
  sequence.add(uppexo::command::EndRenderPass());
  sequence.add(uppexo::command::EndRecorder());

  auto synchronizer = uppexoEngine.addSynchronizer(device);
  synchronizer.addFence();
  synchronizer.create();

  synchronizer.getComponent().waitForFence({frame}, true);

  uppexo::MVP_with_normalized_matrix ubo{};
  ubo.model = glm::rotate(glm::mat4(1.0f), 1 * glm::radians(90.0f),
                          glm::vec3(1.0f, 0.0f, 0.0f));
  ubo.view =
      glm::lookAt(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj = glm::perspective(glm::radians(45.0f), (float)(640.0f / 480.0f),
                              0.1f, 100.0f);
  ubo.proj[1][1] *= -1;
  ubo.norm_model = (glm::transpose(glm::inverse(ubo.model)));
  mesh.getMVPList()[0] = ubo;

  buffer.getComponent().copyByMapping(
      2, mesh.getMVPList(), sizeof(uppexo::MVP_with_normalized_matrix));

  sequence.record(commandBuffer, frame);
  sequence.execute(commandBuffer, frame, device, graphicQueue, synchronizer, {},
                   {}, frame);

  synchronizer.getComponent().waitForFence({frame}, true);
  image.getComponent().exportImageToFile(
      "lol.png", commandBuffer.getComponent(), buffer.getComponent(), 0, 0);

  uppexoEngine.deleteComponent(image);
}
