#ifndef MESH_H_
#define MESH_H_

#include <array>
#include <obj/tiny_obj_loader.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan_core.h>

#include <utils/log.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

namespace uppexo {
struct MVP {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

struct MVP_with_normalized_matrix {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
  alignas(16) glm::mat4 norm_model;
};

struct Material {
  alignas(16) glm::vec3 ambient;
  alignas(16) glm::vec3 diffuse;
  alignas(16) glm::vec3 specular;
  alignas(16) glm::vec3 shininess;
};

struct NullVertex {
  static std::vector<VkVertexInputBindingDescription> getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription;
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(NullVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return std::vector<VkVertexInputBindingDescription>({bindingDescription});
  }

  static std::vector<VkVertexInputAttributeDescription>
  getAttributeDescriptions() {
    return std::vector<VkVertexInputAttributeDescription>({});
  }

  bool operator==(const NullVertex &other) const { return true; }
};

struct PhongVertex {
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 uv;
  glm::uint32 mat;

  static std::vector<VkVertexInputBindingDescription> getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription;
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(PhongVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return std::vector<VkVertexInputBindingDescription>({bindingDescription});
  }

  static std::vector<VkVertexInputAttributeDescription>
  getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(PhongVertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(PhongVertex, normal);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(PhongVertex, uv);

    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32_UINT;
    attributeDescriptions[3].offset = offsetof(PhongVertex, mat);
    return std::vector<VkVertexInputAttributeDescription>(
        {attributeDescriptions[0], attributeDescriptions[1],
         attributeDescriptions[2], attributeDescriptions[3]});
  }

  bool operator==(const PhongVertex &other) const {
    return pos == other.pos && mat == other.mat && uv == other.uv &&
           normal == other.normal;
  }
};

struct FullVertex {
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 uv;
  glm::vec4 color;

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

  bool operator==(const FullVertex &other) const {
    return pos == other.pos && color == other.color && uv == other.uv &&
           normal == other.normal;
  }
};

} // namespace uppexo

namespace std {
template <> struct hash<uppexo::FullVertex> {
  size_t operator()(uppexo::FullVertex const &vertex) const {
    return ((hash<glm::vec3>()(vertex.pos) ^
             (hash<glm::vec3>()(vertex.color) << 1)) >>
            1) ^
           (hash<glm::vec2>()(vertex.uv) << 1);
  }
};

template <> struct hash<uppexo::PhongVertex> {
  size_t operator()(uppexo::PhongVertex const &vertex) const {
    return ((hash<glm::vec3>()(vertex.pos) ^
             (hash<glm::uint32_t>()(vertex.mat) << 1)) >>
            1) ^
           (hash<glm::vec2>()(vertex.uv) << 1);
  }
};
} // namespace std

#endif // !MESH_H_
