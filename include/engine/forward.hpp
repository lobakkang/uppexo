#ifndef FORWARD_H_
#define FORWARD_H_

#include <core/mesh.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <engine/engine.hpp>

namespace uppexo {
struct ForwardRenderingEngineBlueprint {
  int width;
  int height;
  std::string title;
};

class ForwardRenderingEngine
    : public Engine<ForwardRenderingEngine, ForwardRenderingEngineBlueprint> {
public:
  ForwardRenderingEngine(
      ForwardRenderingEngineBlueprint forwardRenderingEngineBlueprint);
  ~ForwardRenderingEngine();
  void run() override;
  void update();
  uppexo::Mesh<uppexo::FullVertex> mesh;

  int instanceID;
  int deviceID;
  int renderPassID;
  int pipelineID;
  int commandBufferID;
  int framebufferID;
  int synchronizerID;
  int bufferID;
  int descriptorID;
  int imageID;
  int samplerID;

protected:
  friend class Engine<ForwardRenderingEngine, ForwardRenderingEngineBlueprint>;
  virtual void buildComponent() override;
  virtual void prerecordCommandBuffer() override;

  int current_frame = 0;
  int vertexCount = 0;
  int width;
  int height;
  std::string title;
};
} // namespace uppexo

#endif // !FORWARD_H_
