#include <core/present.hpp>

uint32_t uppexo::Present::getImage(
    TrackedBlueprint<DeviceBlueprint> &device,
    TrackedBlueprint<SynchronizerBlueprint> &synchronizer, int semaphoreID) {
  uint32_t imageIndex;
  VkSemaphore semaphore = VK_NULL_HANDLE;
  if (semaphoreID != -1) {
    semaphore = synchronizer.getComponent().getSemaphore(semaphoreID);
  }
  vkAcquireNextImageKHR(device.getComponent().getLogicalDevice(),
                        device.getComponent().getSwapChain(), UINT64_MAX,
                        semaphore, VK_NULL_HANDLE, &imageIndex);
  return imageIndex;
}

void uppexo::Present::presentImage(
    TrackedBlueprint<DeviceBlueprint> &device,
    TrackedBlueprint<SynchronizerBlueprint> &synchronizer, int semaphoreID,
    unsigned int imageID) {
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores =
      &synchronizer.getComponent().getSemaphore(semaphoreID);
  VkSwapchainKHR swapChains[] = {device.getComponent().getSwapChain()};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageID;

  vkQueuePresentKHR(
      device.getComponent().getQueue(uppexo::QueueType::present).queue[0],
      &presentInfo);
}
