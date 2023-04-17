#include <algorithm>
#include <base/synchronizer.hpp>
#include <utils/log.hpp>

uppexo::Synchronizer::Synchronizer(
    uppexo::SynchronizerBlueprint synchronizerBlueprint) {
  uppexo::Log::GetInstance().logInfo("Creating synchronization object\n");

  deviceHandle = synchronizerBlueprint.device;

  fenceList.resize(synchronizerBlueprint.fence);
  semaphoreList.resize(synchronizerBlueprint.semaphore);
  eventList.resize(synchronizerBlueprint.event);

  for (int i = 0; i < synchronizerBlueprint.fence; i++) {
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (vkCreateFence(synchronizerBlueprint.device, &fenceInfo, nullptr,
                      &fenceList[i]) != VK_SUCCESS) {
      uppexo::Log::GetInstance().logError(
          "Failed to create synchronization object: Fence!\n");
    }
  }
  for (int i = 0; i < synchronizerBlueprint.semaphore; i++) {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(synchronizerBlueprint.device, &semaphoreInfo, nullptr,
                          &semaphoreList[i]) != VK_SUCCESS) {
      uppexo::Log::GetInstance().logError(
          "Failed to create synchronization object: Semaphore!\n");
    }
  }
}

uppexo::Synchronizer::~Synchronizer() {
  uppexo::Log::GetInstance().logInfo("Destroying synchronization object\n");
  for (int i = 0; i < fenceList.size(); i++) {
    vkDestroyFence(deviceHandle, fenceList[i], nullptr);
  }
  for (int i = 0; i < semaphoreList.size(); i++) {
    vkDestroySemaphore(deviceHandle, semaphoreList[i], nullptr);
  }
}

void uppexo::Synchronizer::waitForFence(std::vector<int> fenceList,
                                        bool waitAll) {
  std::vector<VkFence> fenceHandleList(fenceList.size());
  std::transform(fenceList.begin(), fenceList.end(), fenceHandleList.begin(),
                 [&](int index) { return this->fenceList[index]; });
  vkWaitForFences(deviceHandle, fenceList.size(), &fenceHandleList[0], waitAll,
                  UINT64_MAX);
  vkResetFences(deviceHandle, fenceList.size(), &fenceHandleList[0]);
}

VkFence &uppexo::Synchronizer::getFence(int fenceID) {
  return fenceList[fenceID];
}
VkSemaphore &uppexo::Synchronizer::getSemaphore(int semaphoreID) {
  return semaphoreList[semaphoreID];
}
