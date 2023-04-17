#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <base/buffer.hpp>
#include <base/commandbuffer.hpp>
#include <base/device.hpp>
#include <base/framebuffer.hpp>
#include <base/graphicPipeline.hpp>
#include <base/instance.hpp>
#include <base/renderpass.hpp>
#include <base/shader.hpp>
#include <base/synchronizer.hpp>

#include <memory>
#include <variant>

namespace uppexo {
/*typedef std::variant<std::unique_ptr<Buffer>, std::unique_ptr<CommandBuffer>,
                     std::unique_ptr<Device>, std::unique_ptr<Framebuffer>,
                     std::unique_ptr<GraphicPipeline>,
                     std::unique_ptr<Instance>, std::unique_ptr<Renderpass>,
                     std::unique_ptr<Synchronizer>, Instance>
    Component;

typedef std::variant<BufferBlueprint, CommandBufferBlueprint, DeviceBlueprint,
                     FramebufferBlueprint, GraphicPipelineBlueprint,
                     InstanceBlueprint, RenderpassBlueprint,
                     SynchronizerBlueprint>
    ComponentBlueprint;*/
} // namespace uppexo

#endif // !COMPONENT_H_
