#include <chrono>
#include <thread>

#include <generated/deferredFragment.h>
#include <generated/deferredVertex.h>
#include <generated/mrtFragment.h>
#include <generated/mrtVertex.h>
#include <uppexo.hpp>

#include <core/gui.hpp>

#define IMAGE_AVAILABLE_SEMAPHORE 2
#define RENDER_FINISH_SEMAPHORE 0

struct LightObj {
  glm::vec3 pos;
  glm::vec4 radius;
  glm::vec4 color;
};

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
      uppexo::presetBufferCellBlueprint::UBO_at_host(sizeof(LightObj)));
  buffer.create();

  auto offscreenRenderPass = uppexoEngine.addRenderPass(device);
  auto albedoAttachment = offscreenRenderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::OffscreenAttachment(
          VK_FORMAT_R32G32B32A32_SFLOAT));
  auto normalAttachment = offscreenRenderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::OffscreenAttachment(
          VK_FORMAT_R32G32B32A32_SFLOAT));
  auto specularAttachment = offscreenRenderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::OffscreenAttachment(
          VK_FORMAT_R32G32B32A32_SFLOAT));
  auto depthAttachment = offscreenRenderPass.addAttachment(
      uppexo::presetAttachmentBlueprint::DepthAttachment());
  offscreenRenderPass.addSubpass(
      {albedoAttachment, normalAttachment, specularAttachment},
      depthAttachment);
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
  int textureImage = image.addImageCell(
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

  image.addImageUsage(specularImg, VK_IMAGE_USAGE_SAMPLED_BIT);
  image.addImageUsage(albedoImg, VK_IMAGE_USAGE_SAMPLED_BIT);
  image.addImageUsage(normalImg, VK_IMAGE_USAGE_SAMPLED_BIT);
  image.addImageUsage(depthImage, VK_IMAGE_USAGE_SAMPLED_BIT);

  image.create();

  auto displayFrameBuffer =
      uppexoEngine.addFrameBuffer(device, deferredRenderPass);
  displayFrameBuffer.addImageView(image, depthImage, 0);
  displayFrameBuffer.create();

  auto offscreenFrameBuffer =
      uppexoEngine.addFrameBuffer(device, offscreenRenderPass);
  offscreenFrameBuffer.imageView.clear();
  offscreenFrameBuffer.imageView.resize(1);
  offscreenFrameBuffer.addImageView(image, specularImg, 0);
  offscreenFrameBuffer.addImageView(image, normalImg, 0);
  offscreenFrameBuffer.addImageView(image, albedoImg, 0);
  offscreenFrameBuffer.addImageView(image, depthImage, 0);
  offscreenFrameBuffer.create();

  auto gui = uppexoEngine.addGui(device, commandBuffer);
  gui.create();

  auto offscreenDescriptorSet = uppexoEngine.addDescriptorSet(device);
  for (int i = 0; i < 2; i++) {
    offscreenDescriptorSet.addBinding(
        i, uppexo::presetDescriptorSetBindingBlueprint::UBO_at_vertex_shader(
               buffer, uniformBuffer0, sizeof(uppexo::MVP)));
    int offscreenAlbedoBinding = offscreenDescriptorSet.addBinding(
        i,
        uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
            sampler, image, 0, textureImage, 0));
  }
  offscreenDescriptorSet.create();

  auto displayDescriptorSet = uppexoEngine.addDescriptorSet(device);
  for (int i = 0; i < 2; i++) {
    displayDescriptorSet.addBinding(
        i,
        uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
            sampler, image, 0, specularImg, 0));
    displayDescriptorSet.addBinding(
        i,
        uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
            sampler, image, 0, normalImg, 0));
    displayDescriptorSet.addBinding(
        i,
        uppexo::presetDescriptorSetBindingBlueprint::Sampler_at_fragment_shader(
            sampler, image, 0, albedoImg, 0));
    displayDescriptorSet.addBinding(
        i, uppexo::presetDescriptorSetBindingBlueprint::UBO_at_fragment_shader(
               buffer, sceneBuffer, sizeof(LightObj)));
  }
  displayDescriptorSet.create();

  auto offscreenGraphicPipeline = uppexoEngine.addGraphicPipeline(
      device, offscreenRenderPass, offscreenDescriptorSet);
  offscreenGraphicPipeline.addVertexShaderFromCode((char *)mrtVertex,
                                                   mrtVertex_size);
  offscreenGraphicPipeline.addFragmentShaderFromCode((char *)mrtFragment,
                                                     mrtFragment_size);
  offscreenGraphicPipeline.isDepthEnable = true;
  offscreenGraphicPipeline.create();

  auto displayGraphicPipeline = uppexoEngine.addGraphicPipeline(
      device, deferredRenderPass, displayDescriptorSet);
  displayGraphicPipeline.setVertexType<uppexo::NullVertex>();
  displayGraphicPipeline.addVertexShaderFromCode((char *)deferredVertex,
                                                 deferredVertex_size);
  displayGraphicPipeline.addFragmentShaderFromCode((char *)deferredFragment,
                                                   deferredFragment_size);
  displayGraphicPipeline.isDepthEnable = true;
  displayGraphicPipeline.create();

  auto synchronizer = uppexoEngine.addSynchronizer(device);
  synchronizer.addFence();
  synchronizer.addFence();
  synchronizer.addSemaphore();
  synchronizer.addSemaphore();
  synchronizer.addSemaphore();
  synchronizer.addSemaphore();
  synchronizer.create();

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

  int imageIndex = 0;
  int frame = 0;

  auto sequence = uppexoEngine.addSequence();
  sequence.add(uppexo::command::BeginRecorder());
  sequence.add(uppexo::command::BeginRenderPassWithCustomClearColour(
      offscreenRenderPass, offscreenFrameBuffer, device, imageIndex,
      {{0.0f, 0.0f, 0.0f, 1.0f},
       {0.0f, 0.0f, 0.0f, 1.0f},
       {0.0f, 0.0f, 0.0f, 1.0f},
       {1.0f, 0.0f}}));

  sequence.add(uppexo::command::BindGraphicPipeline(offscreenGraphicPipeline));
  sequence.add(uppexo::command::BindVertexBuffer(buffer, vertexBuffer));
  sequence.add(uppexo::command::SetViewport(device));
  sequence.add(uppexo::command::SetScissor(device));
  sequence.add(uppexo::command::BindGraphicDescriptorSet(
      offscreenDescriptorSet, offscreenGraphicPipeline, frame));
  sequence.add(
      uppexo::command::IndexedDraw(buffer, indexBuffer, mesh.getIndexCount()));
  sequence.add(uppexo::command::EndRenderPass());

  sequence.add(uppexo::command::TransitionImageLayout(
      image, albedoImg, 0, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_TRANSFER_READ_BIT,
      VK_ACCESS_SHADER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT));
  sequence.add(uppexo::command::TransitionImageLayout(
      image, specularImg, 0, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_TRANSFER_READ_BIT,
      VK_ACCESS_SHADER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT));
  sequence.add(uppexo::command::TransitionImageLayout(
      image, normalImg, 0, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_TRANSFER_READ_BIT,
      VK_ACCESS_SHADER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT));

  sequence.add(uppexo::command::BeginRenderPass(
      deferredRenderPass, displayFrameBuffer, device, imageIndex));

  sequence.add(uppexo::command::BindGraphicPipeline(displayGraphicPipeline));
  sequence.add(uppexo::command::BindVertexBuffer(buffer, vertexBuffer));
  sequence.add(uppexo::command::SetViewport(device));
  sequence.add(uppexo::command::SetScissor(device));
  sequence.add(uppexo::command::BindGraphicDescriptorSet(
      displayDescriptorSet, displayGraphicPipeline, frame));
  sequence.add(
      uppexo::command::IndexedDraw(buffer, indexBuffer, mesh.getIndexCount()));
  sequence.add(uppexo::command::RenderGUI());
  sequence.add(uppexo::command::EndRenderPass());

  sequence.add(uppexo::command::EndRecorder());

  auto startTime = std::chrono::high_resolution_clock::now();
  auto endTime = std::chrono::high_resolution_clock::now();

  // uppexo::RenderdocCapturer renderdocCapturer;
  //
  uppexo::MVP ubo{};
  ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f),
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

  ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  float radius = 50.0f;

  gui.getComponent().onRender = [&]() {
    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");
    ImGui::ColorEdit4("Color Picker", (float *)&color);
    ImGui::SliderFloat("Light Radius", &radius, 0.0f, 1000.0f);
    ImGui::End();
  };

  while (uppexoEngine.isRunning()) {
    // renderdocCapturer.startCapture();
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

    glm::vec3 originalVector(1.0f, 0.0f, 1.0f);
    glm::mat4 rotationMatrix =
        glm::scale(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f) * time,
                               glm::vec3(0.0f, 0.0f, 1.0f)),
                   glm::vec3(10, 10, 10));

    LightObj light;
    light.pos = glm::vec3(rotationMatrix * glm::vec4(originalVector, 1.0f));
    light.radius = glm::vec4(radius);
    light.color = glm::vec4(color.x, color.y, color.z, color.w);

    buffer.getComponent().copyByMapping(
        2 + frame, mesh.getMVPList(), mesh.getMVPCount() * sizeof(uppexo::MVP));
    buffer.getComponent().copyByMapping(sceneBuffer, &light, sizeof(LightObj));

    sequence.record(commandBuffer, frame);
    sequence.execute(commandBuffer, frame, device, graphicQueue, synchronizer,
                     {{IMAGE_AVAILABLE_SEMAPHORE + frame,
                       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}},
                     {RENDER_FINISH_SEMAPHORE + frame}, frame);

    uppexo::Present::presentImage(device, synchronizer,
                                  RENDER_FINISH_SEMAPHORE + frame, imageIndex);

    // renderdocCapturer.stopCapture();

    frame++;
    frame %= 2;
    // return 0;
  }
  return 0;
}
