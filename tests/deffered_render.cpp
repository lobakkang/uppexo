#include <chrono>
#include <thread>

#include <generated/mrtVertex.h>
#include <generated/mrtFragment.h>
#include <generated/deferredVertex.h>
#include <generated/deferredFragment.h>
#include <uppexo.hpp>

#include <core/gui.hpp>

#define IMAGE_AVAILABLE_SEMAPHORE 2
#define RENDER_FINISH_SEMAPHORE 0

int main(void) {
  uppexo::Uppexo uppexoEngine({640, 480}, "Deffered rendering demo", true);

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
  int sceneBuffer = buffer.addCell(
      uppexo::presetBufferCellBlueprint::UBO_at_host(sizeof(uppexo::MVP)));
  buffer.create();

  auto offscreenRenderPass = uppexoEngine.addRenderPass(device);
  auto albedoAttachment = offscreenRenderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::OffscreenAttachment(
          VK_FORMAT_R8G8B8A8_SRGB));
  auto normalAttachment = offscreenRenderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::OffscreenAttachment(
          VK_FORMAT_R8G8B8A8_SRGB));
  auto specularAttachment = offscreenRenderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::OffscreenAttachment(
          VK_FORMAT_R8G8B8A8_SRGB));
  auto depthAttachment = offscreenRenderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::DepthAttachment());
  offscreenRenderPass.addSubpass({albedoAttachment, normalAttachment, specularAttachment}, depthAttachment);
  offscreenRenderPass.create();

  auto deferredRenderPass = uppexoEngine.addRenderPass(device);
  auto swapchainAttachment = deferredRenderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::SwapchainAttachment());
  auto deferredDepthAttachment = deferredRenderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::DepthAttachment());
  deferredRenderPass.addSubpass({swapchainAttachment}, deferredDepthAttachment);
  deferredRenderPass.create();

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
  int depthImage = image.addImageCell(
      uppexo::presetImageCellBlueprint::DepthImageCellBlueprint());

  int albedoImg = image.addImageCell(
      uppexo::presetImageCellBlueprint::OffscreenRenderingImageCellBlueprint(
          {640, 480}));
  int normalImg = image.addImageCell(
      uppexo::presetImageCellBlueprint::OffscreenRenderingImageCellBlueprint(
          {640, 480}));
  int specularImg = image.addImageCell(
      uppexo::presetImageCellBlueprint::OffscreenRenderingImageCellBlueprint(
          {640, 480}));
  image.addImageCell(
      uppexo::presetImageCellBlueprint::OffscreenDepthImageCellBlueprint(
          {640, 480}));
  image.create();

  auto displayFrameBuffer = uppexoEngine.addFrameBuffer(device, deferredRenderPass);
  displayFrameBuffer.addImageView(image, depthImage, 0);
  displayFrameBuffer.create();

  auto offscreenFrameBuffer = uppexoEngine.addFrameBuffer(device, offscreenRenderPass);
  offscreenFrameBuffer.imageView.resize(0);
  offscreenFrameBuffer.addImageView(image, albedoImg, 0);
  offscreenFrameBuffer.addImageView(image, normalImg, 0);
  offscreenFrameBuffer.addImageView(image, specularImg, 0);
  offscreenFrameBuffer.create();

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
      0,
      uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
          sampler, image, 0, 0, 0));
  descriptorSet.addBinding(
      0,
      uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
          sampler, image, 0, 0, 0));
  descriptorSet.addBinding(
      0, uppexo::presetDescriptorSetBindingBlueprint::UBO_at_fragment_shader(
             buffer, sceneBuffer, sizeof(uppexo::MVP)));
  descriptorSet.addBinding(
      1, uppexo::presetDescriptorSetBindingBlueprint::UBO_at_vertex_shader(
             buffer, uniformBuffer1, sizeof(uppexo::MVP)));
  descriptorSet.addBinding(
      1,
      uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
          sampler, image, 0, 0, 0));
  descriptorSet.addBinding(
      1,
      uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
          sampler, image, 0, 0, 0));
  descriptorSet.addBinding(
      1,
      uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
          sampler, image, 0, 0, 0));
  descriptorSet.addBinding(
      1, uppexo::presetDescriptorSetBindingBlueprint::UBO_at_fragment_shader(
             buffer, sceneBuffer, sizeof(uppexo::MVP)));
  descriptorSet.create();

  auto offscreenGraphicPipeline =
      uppexoEngine.addGraphicPipeline(device, offscreenRenderPass, descriptorSet);
  offscreenGraphicPipeline.addVertexShaderFromCode((char *)mrtVertex,
                                          mrtVertex_size);
  offscreenGraphicPipeline.addFragmentShaderFromCode((char *)mrtFragment,
                                            mrtFragment_size);
  offscreenGraphicPipeline.isDepthEnable = true;
  offscreenGraphicPipeline.create();

  auto deferredGraphicPipeline =
      uppexoEngine.addGraphicPipeline(device, deferredRenderPass, descriptorSet);
  deferredGraphicPipeline.setVertexType<uppexo::NullVertex>();
  deferredGraphicPipeline.addVertexShaderFromCode((char *)deferredVertex,
                                          deferredVertex_size);
  deferredGraphicPipeline.addFragmentShaderFromCode((char *)deferredFragment,
                                            deferredFragment_size);
  deferredGraphicPipeline.isDepthEnable = true;
  deferredGraphicPipeline.create();

  auto synchronizer = uppexoEngine.addSynchronizer(device);
  synchronizer.addFence();
  synchronizer.addFence();
  synchronizer.addSemaphore();
  synchronizer.addSemaphore();
  synchronizer.addSemaphore();
  synchronizer.addSemaphore();
  synchronizer.create();

  int imageIndex = 0;
  int frame = 0;

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
  sequence.add(uppexo::command::BeginRenderPass(offscreenRenderPass, offscreenFrameBuffer, device,
                                                imageIndex));
  sequence.add(uppexo::command::BindGraphicPipeline(offscreenGraphicPipeline));
  sequence.add(uppexo::command::BindVertexBuffer(buffer, vertexBuffer));
  sequence.add(uppexo::command::SetViewport(device));
  sequence.add(uppexo::command::SetScissor(device));
  sequence.add(uppexo::command::BindGraphicDescriptorSet(
      descriptorSet, offscreenGraphicPipeline, frame));
  sequence.add(
      uppexo::command::IndexedDraw(buffer, indexBuffer, mesh.getIndexCount()));
  sequence.add(uppexo::command::RenderGUI());
  sequence.add(uppexo::command::EndRenderPass());
  sequence.add(uppexo::command::EndRecorder());

  auto startTime = std::chrono::high_resolution_clock::now();
  auto endTime = std::chrono::high_resolution_clock::now();

  return 1;

  while (uppexoEngine.isRunning()) {
    endTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       endTime - startTime)
                       .count();
    startTime = endTime;

    const int MAX_FRAME_RATE = 60;
    const int MIN_FRAME_TIME = 1000 / MAX_FRAME_RATE;
    if (elapsed < MIN_FRAME_TIME) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(MIN_FRAME_TIME - elapsed));
    }

    gui.getComponent().render();

    synchronizer.getComponent().waitForFence({frame}, true);
    imageIndex = uppexo::Present::getImage(device, synchronizer,
                                           IMAGE_AVAILABLE_SEMAPHORE + frame);

    static auto initTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(
                     currentTime - initTime)
                     .count();

    uppexo::MVP ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
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

    sequence.record(commandBuffer, frame);
    sequence.execute(commandBuffer, frame, device, graphicQueue, synchronizer,
                     {{IMAGE_AVAILABLE_SEMAPHORE + frame,
                       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}},
                     {RENDER_FINISH_SEMAPHORE + frame}, frame);

    uppexo::Present::presentImage(device, synchronizer,
                                  RENDER_FINISH_SEMAPHORE + frame, imageIndex);

    frame++;
    frame %= 2;
  }
}
