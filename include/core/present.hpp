#ifndef PRESENT_HPP_
#define PRESENT_HPP_

#include <component.hpp>

namespace uppexo {
class Present {
public:
  static uint32_t getImage(TrackedBlueprint<DeviceBlueprint> &device, TrackedBlueprint<SynchronizerBlueprint> &synchronizer, int semaphoreID = -1);
  static void presentImage(TrackedBlueprint<DeviceBlueprint> &device, TrackedBlueprint<SynchronizerBlueprint> &synchronizer, int semaphoreID = -1, unsigned int imageID = 0);
};
}

#endif
