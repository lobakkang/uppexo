#ifndef DEFERRED_H_
#define DEFERRED_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <engine/engine.hpp>

namespace uppexo {
struct DeferredRenderingEngineBlueprint {
  int width;
  int height;
  std::string title;
};


class DeferredRenderingEngine : public Engine<DeferredRenderingEngine, DeferredRenderingEngineBlueprint> {
public:
  DeferredRenderingEngine(
      DeferredRenderingEngineBlueprint forwardRenderingEngineBlueprint);
  ~DeferredRenderingEngine();
  void run() override;
protected:
  friend class Engine<DeferredRenderingEngine, DeferredRenderingEngineBlueprint>;
  virtual void buildComponent() override;
  virtual void prerecordCommandBuffer() override;

  int width;
  int height;
  std::string title;
};
} // namespace uppexo
  //
#endif // !DEFERRED_H_
