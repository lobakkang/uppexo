#include <core/command/general.hpp>
#include <core/command/render.hpp>
#include <core/sequence.hpp>
#include <utils/log.hpp>

uppexo::Sequence::Sequence() {
  uppexo::Log::GetInstance().logInfo("Creating sequence\n");
}

void uppexo::Sequence::record(
    TrackedBlueprint<CommandBufferBlueprint> &commandBuffer, int id) {
  for (std::shared_ptr<command::Command> seq : sequence) {
    seq->execute(commandBuffer.getComponent().getBuffer(id));
  }
}

void uppexo::Sequence::execute(
    TrackedBlueprint<CommandBufferBlueprint> &commandBuffer,
    int commandBufferID, TrackedBlueprint<DeviceBlueprint> &device,
    std::tuple<QueueType, int> queue,
    TrackedBlueprint<SynchronizerBlueprint> &synchronizer,
    std::vector<std::tuple<int, VkPipelineStageFlags>> waitSemaphoresID, std::vector<int> signalSemaphoresID,
    int fenceID) {
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  std::vector<VkSemaphore> waitSemaphores;
  std::vector<VkSemaphore> signalSemaphores;
  std::vector<VkPipelineStageFlags> waitStages;
  for (auto semaphore : waitSemaphoresID) {
    waitSemaphores.push_back(
        synchronizer.getComponent().getSemaphore(std::get<0>(semaphore)));
    waitStages.push_back(std::get<1>(semaphore));
  }
  for (int semaphore : signalSemaphoresID) {
    signalSemaphores.push_back(
        synchronizer.getComponent().getSemaphore(semaphore));
  }
  submitInfo.waitSemaphoreCount = waitSemaphores.size();
  submitInfo.pWaitSemaphores = waitSemaphores.data();
  submitInfo.pWaitDstStageMask = waitStages.data();

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers =
      &commandBuffer.getComponent().getBuffer(commandBufferID);

  submitInfo.signalSemaphoreCount = signalSemaphores.size();
  submitInfo.pSignalSemaphores = signalSemaphores.data();

  if (vkQueueSubmit(device.getComponent()
                        .getQueue(std::get<0>(queue))
                        .queue[std::get<1>(queue)],
                    1, &submitInfo,
                    synchronizer.getComponent().getFence(fenceID)) !=
      VK_SUCCESS) {
    uppexo::Log::GetInstance().logError(
        "Failed to submit draw command buffer!\n");
  }
}

template <typename Ta> int uppexo::Sequence::add(Ta cmd) {
  sequence.push_back(std::make_shared<Ta>(cmd));
  return 0;
}

template int uppexo::Sequence::add<uppexo::command::BeginRecorder>(
    uppexo::command::BeginRecorder cmd);
template int uppexo::Sequence::add<uppexo::command::BeginRenderPass>(
    uppexo::command::BeginRenderPass cmd);
template int uppexo::Sequence::add<uppexo::command::EndRecorder>(
    uppexo::command::EndRecorder cmd);
template int uppexo::Sequence::add<uppexo::command::EndRenderPass>(
    uppexo::command::EndRenderPass cmd);
template int uppexo::Sequence::add<uppexo::command::BindComputePipeline>(
    uppexo::command::BindComputePipeline cmd);
template int uppexo::Sequence::add<uppexo::command::BindGraphicPipeline>(
    uppexo::command::BindGraphicPipeline cmd);
template int uppexo::Sequence::add<uppexo::command::BindGraphicDescriptorSet>(
    uppexo::command::BindGraphicDescriptorSet cmd);
template int uppexo::Sequence::add<uppexo::command::BindComputeDescriptorSet>(
    uppexo::command::BindComputeDescriptorSet cmd);
template int uppexo::Sequence::add<uppexo::command::BindVertexBuffer>(
    uppexo::command::BindVertexBuffer cmd);
template int uppexo::Sequence::add<uppexo::command::SetScissor>(
    uppexo::command::SetScissor cmd);
template int uppexo::Sequence::add<uppexo::command::SetViewport>(
    uppexo::command::SetViewport cmd);
template int uppexo::Sequence::add<uppexo::command::IndexedDraw>(
    uppexo::command::IndexedDraw cmd);
template int uppexo::Sequence::add<uppexo::command::DispatchCompute>(
    uppexo::command::DispatchCompute cmd);
