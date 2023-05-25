#include <dlfcn.h>
#include <engine/compute.hpp>
#include <glm/fwd.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
  try {
    uppexo::Log::GetInstance().logInfo("Uppexo compute array test\n");
    uppexo::ComputeEngineBlueprint engineBlueprint;

    std::vector<float> data;
    for (int i = 0; i < 10; i++) {
      data.push_back(i);
    }
    std::cout << std::endl;
    engineBlueprint.memorySize = 1024;
    engineBlueprint.title = "Compute array test";

    std::unique_ptr<uppexo::ComputeEngine> engine =
        uppexo::ComputeEngine::create(engineBlueprint);
    engine->pushMemory(data.data(), data.size() * sizeof(float));
    uppexo::Log::GetInstance().logInfo("Before:\n");
    for (int i = 0; i < 10; i++) {
      std::cout << data[i] << " ";
    }
    std::cout << std::endl;
    engine->run();

    engine->pullMemory(data.data(), data.size() * sizeof(float));
    uppexo::Log::GetInstance().logInfo("After:\n");
    for (int i = 0; i < 10; i++) {
      std::cout << data[i] << " ";
    }
    std::cout << std::endl;
    engine.reset();

    return 0;
  } catch (const std::exception &exp) {
    uppexo::Log::GetInstance().logInfo("%s\n", exp.what());
    return -1;
  }
}
