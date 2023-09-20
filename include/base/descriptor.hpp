#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <base/device.hpp>
#include <base/image.hpp>
#include <base/sampler.hpp>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <component.hpp>

#include <base/buffer.hpp>

#include <optional>
#include <string>
#include <vector>

namespace uppexo {
struct DescriptorSetBindingBlueprint {
  VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  VkShaderStageFlags stage = VK_SHADER_STAGE_VERTEX_BIT;
  VkBuffer buffer;
  VkSampler sampler;
  VkImageView texture;
  int size;

  DescriptorSetBindingBlueprint() = default;
};

namespace presetDescriptorSetBindingBlueprint {
struct UBO_at_vertex_shader : DescriptorSetBindingBlueprint {
  UBO_at_vertex_shader(VkBuffer buffer, int size) {
    this->buffer = buffer;
    this->size = size;
    this->type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    this->stage = VK_SHADER_STAGE_VERTEX_BIT;
    this->sampler = VK_NULL_HANDLE;
  }
  UBO_at_vertex_shader(Buffer &buffer, int id, int size) {
    this->buffer = buffer.getBuffer(id);
    this->size = size;
    this->type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    this->stage = VK_SHADER_STAGE_VERTEX_BIT;
    this->sampler = VK_NULL_HANDLE;
  }
  UBO_at_vertex_shader(TrackedBlueprint<BufferBlueprint> &buffer, int id,
                       int size) {
    this->buffer = buffer.getComponent().getBuffer(id);
    this->size = size;
    this->type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    this->stage = VK_SHADER_STAGE_VERTEX_BIT;
    this->sampler = VK_NULL_HANDLE;
  }
};

struct UBO_at_fragment_shader : DescriptorSetBindingBlueprint {
  UBO_at_fragment_shader(VkBuffer buffer, int size) {
    this->buffer = buffer;
    this->size = size;
    this->type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    this->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    this->sampler = VK_NULL_HANDLE;
  }
  UBO_at_fragment_shader(Buffer &buffer, int id, int size) {
    this->buffer = buffer.getBuffer(id);
    this->size = size;
    this->type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    this->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    this->sampler = VK_NULL_HANDLE;
  }
  UBO_at_fragment_shader(TrackedBlueprint<BufferBlueprint> &buffer, int id,
                       int size) {
    this->buffer = buffer.getComponent().getBuffer(id);
    this->size = size;
    this->type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    this->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    this->sampler = VK_NULL_HANDLE;
  }
};

struct Sampler_at_fragment_shader : DescriptorSetBindingBlueprint {
  Sampler_at_fragment_shader(VkSampler sampler, VkImageView texture) {
    this->sampler = sampler;
    this->texture = texture;
    this->type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    this->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    this->buffer = VK_NULL_HANDLE;
  }

  Sampler_at_fragment_shader(Sampler &sampler, Image &texture, int samplerID,
                             int textureListID, int textureID) {
    this->sampler = sampler.getSampler(samplerID);
    this->texture = texture.getImageView(textureListID)[textureID];
    this->type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    this->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    this->buffer = VK_NULL_HANDLE;
  }

  Sampler_at_fragment_shader(TrackedBlueprint<SamplerBlueprint> &sampler,
                             TrackedBlueprint<ImageBlueprint> &texture,
                             int samplerID, int textureListID, int textureID) {
    this->sampler = sampler.getComponent().getSampler(samplerID);
    this->texture =
        texture.getComponent().getImageView(textureListID)[textureID];
    this->type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    this->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    this->buffer = VK_NULL_HANDLE;
  }
};

struct SSBO_at_compute_shader : DescriptorSetBindingBlueprint {
  SSBO_at_compute_shader(VkBuffer buffer, int size) {
    this->buffer = buffer;
    this->size = size;
    this->type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    this->stage = VK_SHADER_STAGE_COMPUTE_BIT;
    this->sampler = VK_NULL_HANDLE;
  }
  SSBO_at_compute_shader(Buffer &buffer, int id, int size) {
    this->buffer = buffer.getBuffer(id);
    this->size = size;
    this->type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    this->stage = VK_SHADER_STAGE_COMPUTE_BIT;
    this->sampler = VK_NULL_HANDLE;
  }
  SSBO_at_compute_shader(TrackedBlueprint<BufferBlueprint> &buffer, int id,
                         int size) {
    this->buffer = buffer.getComponent().getBuffer(id);
    this->size = size;
    this->type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    this->stage = VK_SHADER_STAGE_COMPUTE_BIT;
    this->sampler = VK_NULL_HANDLE;
  }
};

struct SSBO_at_fragment_shader : DescriptorSetBindingBlueprint {
  SSBO_at_fragment_shader(VkBuffer buffer, int size) {
    this->buffer = buffer;
    this->size = size;
    this->type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    this->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    this->sampler = VK_NULL_HANDLE;
  }
  SSBO_at_fragment_shader(Buffer &buffer, int id, int size) {
    this->buffer = buffer.getBuffer(id);
    this->size = size;
    this->type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    this->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    this->sampler = VK_NULL_HANDLE;
  }
  SSBO_at_fragment_shader(TrackedBlueprint<BufferBlueprint> &buffer, int id,
                         int size) {
    this->buffer = buffer.getComponent().getBuffer(id);
    this->size = size;
    this->type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    this->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    this->sampler = VK_NULL_HANDLE;
  }
};
}; // namespace presetDescriptorSetBindingBlueprint

class DescriptorSet;

struct DescriptorSetBlueprint {
  using Component = DescriptorSet;

  VkDevice device;
  std::vector<std::vector<DescriptorSetBindingBlueprint>> binding;

  DescriptorSetBlueprint() = default;
  DescriptorSetBlueprint(uppexo::Device &device) {
    this->device = device.getLogicalDevice();
  };

  void addBinding(int set, DescriptorSetBindingBlueprint binding) {
    this->binding.resize(std::max(set + 1, (int)this->binding.size()));
    this->binding[set].push_back(binding);
  }
};

class DescriptorSet {
public:
  DescriptorSet(DescriptorSetBlueprint descriptorBlueprint);
  ~DescriptorSet();

  void update();
  std::vector<VkDescriptorSetLayout> &getLayout();
  std::vector<VkDescriptorSet> &getSet();
  VkDescriptorPool getPool() { return descriptorPool; }

private:
  VkDevice device;
  std::vector<std::vector<DescriptorSetBindingBlueprint>> binding;
  std::vector<VkDescriptorSetLayout> descriptorSetLayoutList;
  VkDescriptorPool descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;
};
} // namespace uppexo

#endif
