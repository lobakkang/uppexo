#include <algorithm>
#include <component.hpp>
#include <engine/engine.hpp>
#include <utils/log.hpp>
#include <variant>

/*template <typename Derived> uppexo::Engine<Derived>::Engine() {
  uppexo::Log::GetInstance().logInfo("Initialize uppexo engine\n");
}

template <typename Derived> uppexo::Engine<Derived>::~Engine() {
  uppexo::Log::GetInstance().logInfo("Shutting down engine\n");
  std::for_each(componentList.rbegin(), componentList.rend(),
                [](std::unique_ptr<void, void (*)(void *)> &i) { i.reset(); });
}

template <typename Derived> void uppexo::Engine<Derived>::buildComponent() {
  uppexo::Log::GetInstance().logError(
      "Invalid engine: empty component build function (DEV FAULT)\n");
}
template <typename Derived> void uppexo::Engine<Derived>::run() {
  uppexo::Log::GetInstance().logError(
      "Invalid engine: empty main loop (DEV FAULT)\n");
}*/
