#include "base/buffer.hpp"
#include "base/commandbuffer.hpp"
#include "base/device.hpp"
#include "base/framebuffer.hpp"
#include "base/graphicPipeline.hpp"
#include "base/instance.hpp"
#include "base/renderpass.hpp"
#include "base/synchronizer.hpp"
#include "core/mesh.hpp"
#include "engine/compute.hpp"
#include "utils/log.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <uppexo.hpp>

int main(int argc, char *argv[]) {
  std::cout << "Uppexo Demo Test" << std::endl;

  uppexo::ComputeEngineBlueprint engineBlueprint;
  engineBlueprint.title = "Uppexo Demo";

  std::unique_ptr<uppexo::ComputeEngine> engine =
      uppexo::ComputeEngine::create(engineBlueprint);
  engine->run();
  engine.reset();

  return 0;
}
