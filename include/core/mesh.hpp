#ifndef MESH_H_
#define MESH_H_

#include <array>
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace uppexo {
struct FullVertex {
  float pos[3];
  float normal[3];
  float uv[2];
  float color[4];

  static std::vector<VkVertexInputBindingDescription> getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription;
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(FullVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return std::vector<VkVertexInputBindingDescription>({bindingDescription});
  }

  static std::vector<VkVertexInputAttributeDescription>
  getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(FullVertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(FullVertex, normal);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(FullVertex, uv);

    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[3].offset = offsetof(FullVertex, color);
    return std::vector<VkVertexInputAttributeDescription>(
        {attributeDescriptions[0], attributeDescriptions[1],
         attributeDescriptions[2], attributeDescriptions[3]});
  }
};

template <typename T> class Mesh {
public:
  Mesh(){};
  ~Mesh(){};
  void pushVertex(T vertex) { vertexList.push_back(vertex); }
  T *data() { return vertexList.data(); };

private:
  std::vector<T> vertexList;
};
} // namespace uppexo

#endif // !MESH_H_
