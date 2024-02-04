#ifndef RENDERDOCCAPTURE_HPP_
#define RENDERDOCCAPTURE_HPP_

#include <utils/log.hpp>

#include <dlfcn.h>
#include <renderdoc_app.h>

namespace uppexo {
class RenderdocCapturer {
public:
  RenderdocCapturer() {
    uppexo::Log::GetInstance().logInfo(
        "Initializing Renderdoc frame capturer\n");

    if (void *mod = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD)) {
      pRENDERDOC_GetAPI RENDERDOC_GetAPI =
          (pRENDERDOC_GetAPI)dlsym(mod, "RENDERDOC_GetAPI");
      int ret =
          RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&rdoc_api);
      if (ret != 1) {
        uppexo::Log::GetInstance().logError(
            "Failed to initialize Renderdoc frame capturer\n");
      }
    }
  };

  void startCapture() {
    if (rdoc_api)
      rdoc_api->StartFrameCapture(nullptr, nullptr);
  }

  void stopCapture() {
    if (rdoc_api)
      rdoc_api->EndFrameCapture(nullptr, nullptr);
  }

private:
  RENDERDOC_API_1_1_2 *rdoc_api = nullptr;
};
} // namespace uppexo

#endif // !SEQUENCE_HPP_
