#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <base/buffer.hpp>
#include <base/commandbuffer.hpp>
#include <base/device.hpp>
#include <base/instance.hpp>
#include <base/renderpass.hpp>
#include <base/shader.hpp>
#include <base/synchronizer.hpp>

#include <functional>

namespace uppexo {}

template <typename tp> struct TrackedBlueprint : public tp {
  template <typename... Args> TrackedBlueprint(Args &...args) : tp(args...) {}
  using Component = typename tp::Component;

  std::function<void(void)> create;
  std::function<typename tp::Component&(void)> getComponent;

  int componentID = 0;
};

#endif // !COMPONENT_H_
