#include <base/buffer.hpp>
#include <base/commandbuffer.hpp>
#include <base/device.hpp>
#include <base/framebuffer.hpp>
#include <base/graphicPipeline.hpp>
#include <base/instance.hpp>
#include <base/renderpass.hpp>
#include <base/synchronizer.hpp>
#include <chrono>
#include <core/mesh.hpp>
#include <engine/forward.hpp>
#include <iostream>
#include <memory>
#include <uppexo.hpp>
#include <utils/log.hpp>

#define IMAGE_AVAILABLE_SEMAPHORE 0
#define RENDER_FINISH_SEMAPHORE 1

void loopFunc(uppexo::ForwardRenderingEngine &context) {
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                   currentTime - startTime)
                   .count();

  uppexo::MVP ubo{};
  ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
                          glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.view =
      glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj = glm::perspective(
      glm::radians(45.0f),
      (float)context.getComponent<uppexo::Device>(context.deviceID)
              .getSwapChainExtend()
              .width /
          (float)context.getComponent<uppexo::Device>(context.deviceID)
              .getSwapChainExtend()
              .height,
      0.1f, 100.0f);
  ubo.proj[1][1] *= -1;

  context.mesh.getMVPList()[0] = ubo;
}

int main(int argc, char *argv[]) {
  try {
    uppexo::Log::GetInstance().logInfo("Uppexo forward rendering test\n");

    uppexo::ForwardRenderingEngineBlueprint engineBlueprint;
    engineBlueprint.width = 800;
    engineBlueprint.height = 800;
    engineBlueprint.title = "Uppexo Demo";

    uppexo::MeshInfo room;
    room.path = "./viking_room.obj";

    std::unique_ptr<uppexo::ForwardRenderingEngine> engine =
        uppexo::ForwardRenderingEngine::create(engineBlueprint);
    engine->earlyLoopFunction = loopFunc;
    engine->mesh.addMesh(room);
    engine->update();
    engine->run();
    engine.reset();

    return 0;
  } catch (const std::exception &) {
    return -1;
  }
}
