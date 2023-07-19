#ifndef SYNCHRONIZER_H_
#define SYNCHRONIZER_H_

#include <base/device.hpp>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace uppexo {
class Synchronizer;

struct SynchronizerBlueprint {
  using Component = Synchronizer;

  int fence = 0;
  int semaphore = 0;
  int event = 0;

  VkDevice device;

  SynchronizerBlueprint() = default;
  SynchronizerBlueprint(Device &device) {
    this->device = device.getLogicalDevice();
  }

  int addFence() {
    fence++;
    return fence - 1;
  }

  int addSemaphore() {
    semaphore++;
    return semaphore - 1;
  }

  int addEvent() {
    event++;
    return event - 1;
  }
};

class Synchronizer {
public:
  Synchronizer(SynchronizerBlueprint synchronizerBlueprint);
  ~Synchronizer();
  void waitForFence(std::vector<int> fenceList, bool waitAll);
  VkFence &getFence(int fenceID);
  VkSemaphore &getSemaphore(int semaphoreID);

private:
  std::vector<VkFence> fenceList;
  std::vector<VkSemaphore> semaphoreList;
  std::vector<VkEvent> eventList;

  VkDevice deviceHandle;
};
} // namespace uppexo

#endif // !SY&NCHRONIZER_H_
