#ifndef ENGINE_H_
#define ENGINE_H_

#include "base/instance.hpp"
#include <any>
#include <component.hpp>
#include <core/commandBufferRecorder.hpp>
#include <functional>
#include <memory>
#include <utils/log.hpp>
#include <vector>

namespace uppexo {
template <typename Derived, typename DerivedBlueprint> class Engine {
public:
  Engine() {
    uppexo::Log::GetInstance().logInfo("Initialize uppexo engine\n");
    componentList.clear();
  };

  ~Engine() {
    uppexo::Log::GetInstance().logInfo("Shutting down engine\n");
    std::for_each(
        componentList.rbegin(), componentList.rend(),
        [](std::unique_ptr<void, void (*)(void *)> &i) { i.reset(); });
  };

  virtual void run() {
    uppexo::Log::GetInstance().logError(
        "Invalid engine: empty main loop (DEV FAULT)\n");
  };

  static std::unique_ptr<Derived> create(DerivedBlueprint blueprint) {
    std::unique_ptr<Derived> obj = std::make_unique<Derived>(blueprint);
    obj->buildComponent();
    obj->prerecordCommandBuffer();
    return obj;
  }

protected:
  virtual void buildComponent() {
    uppexo::Log::GetInstance().logError(
        "Invalid engine: empty component build function (DEV FAULT)\n");
  };

  virtual void prerecordCommandBuffer() {
    uppexo::Log::GetInstance().logError(
        "Invalid engine: empty prerecord command buffer function (DEV "
        "FAULT)\n");
  }

  template <typename T> inline T &getComponent(int id) {
    return *static_cast<T *>(componentList[id].get());
  }

  template <typename Ta, typename Tb> inline void addComponent(Tb blueprint) {
    componentList.emplace_back(new Ta(blueprint),
                               [](void *p) { delete static_cast<Ta *>(p); });
  }

  std::vector<std::unique_ptr<void, void (*)(void *)>> componentList;
  std::vector<std::function<void(std::any)>> recorderList;
};
} // namespace uppexo

#endif // !ENGINE_H_
