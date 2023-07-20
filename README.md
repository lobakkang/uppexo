<div style="display: flex; align-items: center;">
  <img src="./demo/image/potato.webp" alt="Image" width="100" height="100" style="margin-right: 10px;">
  <h1 style="margin: 0;">Uppexo</h1>
</div>

## Unified Performant Potato Execution Engine without Optimization
![GitHub](https://img.shields.io/badge/Version-0.0.1-purple.svg)
![GitHub](https://img.shields.io/badge/C++-17-blue.svg)
![GitHub](https://img.shields.io/badge/Build-cmake-green.svg)
![GitHub](https://img.shields.io/badge/Made_with-LOVE-red.svg)
![GitHub](https://img.shields.io/badge/targeted_user-nerd-yellow.svg)

**A general purpose Vulkan engine for rendering and computing**

It has high flexibility and modular design. It fits almost every scenario, from 3D rendering to machine learning. In other word, users can customized the engine layout and suits their needs. 

### This engine may suits you if you want:
- Minimal to almost no boilerplate code 
- To harness power of Vulkan without seeing a single line of vulkan code

### Features
- Easy to use
- High performance
- The design is very human

### Showcase
| Forward rendering                 | Adding operation using compute shader                 |
|-------------------------|-------------------------|
| ![Forward](demo/image/forward.png) | ![Compute](demo/image/compute.png) |

### Demo Code

```cpp 
#include <uppexo.hpp>

int main(void) {
  // create uppexo engine
  uppexo::Uppexo uppexoEngine({0, 0}, "uppexo demo", true);

  // create logical device and queue
  auto device = uppexoEngine.addDevice();
  auto computeQueue = device.addQueue(uppexo::compute);
  device.create();

  // create two SSBO buffer
  auto buffer = uppexoEngine.addBuffer(device);
  int inputBuffer = buffer.addCell(
      uppexo::presetBufferCellBlueprint::SSBO_at_device(sizeof(int) * 100));
  int outputBuffer = buffer.addCell(
      uppexo::presetBufferCellBlueprint::SSBO_at_device(sizeof(int) * 100));
  buffer.create();
}
```

### Documentation
WIP

### Caveats

This Vulkan engine is primarily developed for personal use and has been designed to match my chaotic taste. However, I do not recommend using it in critical projects. For more professional and robust Vulkan libraries, consider exploring alternatives like [Liblava](https://github.com/liblava/liblava).
