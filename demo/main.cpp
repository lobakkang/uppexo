#include "base/buffer.hpp"
#include "base/commandbuffer.hpp"
#include "base/device.hpp"
#include "base/framebuffer.hpp"
#include "base/graphicPipeline.hpp"
#include "base/instance.hpp"
#include "base/renderpass.hpp"
#include "base/synchronizer.hpp"
#include "core/mesh.hpp"
#include "engine/forward.hpp"
#include <iostream>
#include <memory>
#include <uppexo.hpp>

#define IMAGE_AVAILABLE_SEMAPHORE 0
#define RENDER_FINISH_SEMAPHORE 1

int main(int argc, char *argv[]) {
  std::cout << "Uppexo Demo Test" << std::endl;

  uppexo::ForwardRenderingEngineBlueprint engineBlueprint;
  engineBlueprint.width = 800;
  engineBlueprint.height = 800;
  engineBlueprint.title = "Uppexo Demo";

  uppexo::Mesh<uppexo::FullVertex> mesh;
  mesh.pushVertex({.pos = {0.0f, -0.5f, 0.0f},
                   .normal = {0.0f, 0.0f, 0.0f},
                   .uv = {0.0f, 0.0f},
                   .color = {1.0f, 0.0f, 0.0f, 1.0f}});
  mesh.pushVertex({.pos = {0.5f, 0.5f, 0.0f},
                   .normal = {0.0f, 0.0f, 0.0f},
                   .uv = {0.0f, 0.0f},
                   .color = {0.0f, 1.0f, 0.0f, 1.0f}});
  mesh.pushVertex({.pos = {-0.5f, 0.5f, 0.0f},
                   .normal = {0.0f, 0.0f, 0.0f},
                   .uv = {0.0f, 0.0f},
                   .color = {0.0f, 0.0f, 1.0f, 1.0f}});

  std::unique_ptr<uppexo::ForwardRenderingEngine> engine =
      uppexo::ForwardRenderingEngine::create(engineBlueprint);
  engine->pushVertexBuffer(mesh.data());
  engine->run();
  engine.reset();

  return 0;
}
