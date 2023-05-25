#ifndef FORWARD_H_
#define FORWARD_H_

#include "core/mesh.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <engine/engine.hpp>

namespace uppexo {
struct ComputeEngineBlueprint {
  std::string title;
  int memorySize;
};

class ComputeEngine : public Engine<ComputeEngine, ComputeEngineBlueprint> {
public:
  ComputeEngine(ComputeEngineBlueprint computeEngineBlueprint);
  ~ComputeEngine();
  void run() override;
  void pushMemory(void* addr, size_t len);
  void pullMemory(void *addr, size_t len);

protected:
  std::string title;
  int memorySize;

  friend class Engine<ComputeEngine, ComputeEngineBlueprint>;
  virtual void buildComponent() override;
  virtual void prerecordCommandBuffer() override;
};
} // namespace uppexo

#endif // !FORWARD_H_
