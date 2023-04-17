#include <engine/deferred.hpp>

uppexo::DeferredRenderingEngine::DeferredRenderingEngine(
    uppexo::DeferredRenderingEngineBlueprint forwardRenderingEngineBlueprint)
    : Engine<DeferredRenderingEngine, DeferredRenderingEngineBlueprint>() {
  this->width = forwardRenderingEngineBlueprint.width;
  this->height = forwardRenderingEngineBlueprint.height;
  this->title = forwardRenderingEngineBlueprint.title;
}

void uppexo::DeferredRenderingEngine::buildComponent() {}
void uppexo::DeferredRenderingEngine::prerecordCommandBuffer() {}
void uppexo::DeferredRenderingEngine::run() {}
uppexo::DeferredRenderingEngine::~DeferredRenderingEngine() {}
