#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <stb/stb_image.h>

#include <base/buffer.hpp>
#include <base/commandbuffer.hpp>
#include <base/device.hpp>
#include <utils/log.hpp>
#include <utils/vulkan_util.hpp>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace uppexo {
struct ImageCellBlueprint {
  VkExtent2D size = {0, 0};
  VkFormat format = VK_FORMAT_UNDEFINED;
  VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
  VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT;
  BufferLocation location = ON_DEVICE_INVISIBLE_TO_HOST;
  VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
  std::string path = "";

  int num = 1;

  ImageCellBlueprint() = default;
};

namespace presetImageCellBlueprint {
struct OffscreenRenderingImageCellBlueprint : ImageCellBlueprint {
  OffscreenRenderingImageCellBlueprint(VkExtent2D size) {
    aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    format = VK_FORMAT_R8G8B8A8_SRGB;
    location = ON_DEVICE_INVISIBLE_TO_HOST;
    this->size = size;
    usage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    this->location = ON_DEVICE_INVISIBLE_TO_HOST;
  }
};

struct OffscreenDepthImageCellBlueprint : ImageCellBlueprint {
  OffscreenDepthImageCellBlueprint(VkExtent2D size) {
    aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
    format = VK_FORMAT_D32_SFLOAT;
    usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    location = ON_DEVICE_INVISIBLE_TO_HOST;
    this->size = size;
    this->location = ON_DEVICE_INVISIBLE_TO_HOST;
  }
};

struct DepthImageCellBlueprint : ImageCellBlueprint {
  DepthImageCellBlueprint() {
    aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
    format = VK_FORMAT_UNDEFINED;
    usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    location = ON_DEVICE_INVISIBLE_TO_HOST;
  }
};

struct TextureImageCellBlueprint : ImageCellBlueprint {
  TextureImageCellBlueprint(std::string path) {
    int texWidth, texHeight, texChannels;
    stbi_uc *pixels = stbi_load(path.c_str(), &texWidth, &texHeight,
                                &texChannels, STBI_rgb_alpha);
    if (!pixels) {
      uppexo::Log::GetInstance().logError("Failed to locate texture file, %s\n",
                                          path.c_str());
    }

    this->size.width = texWidth;
    this->size.height = texHeight;
    this->aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    this->format = VK_FORMAT_R8G8B8A8_SRGB;
    this->usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
                  VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    this->location = ON_DEVICE_INVISIBLE_TO_HOST;
    this->path = path;
  }
};
} // namespace presetImageCellBlueprint

class Image;

struct ImageBlueprint {
  using Component = Image;

  std::vector<ImageCellBlueprint> imageCellBlueprint;
  VkDevice device;
  VkPhysicalDevice physicalDevice;
  VkExtent2D extend;
  Buffer *buffer;
  CommandBuffer *commandBuffer;

  ImageBlueprint() = default;
  ImageBlueprint(Device &device) {
    this->device = device.getLogicalDevice();
    this->physicalDevice = device.getPhysicalDevice();
    this->extend = device.getSwapChainExtend();
  }
  ImageBlueprint(Device &device, Buffer &buffer, CommandBuffer &commandBuffer) {
    this->device = device.getLogicalDevice();
    this->physicalDevice = device.getPhysicalDevice();
    this->extend = device.getSwapChainExtend();
    this->buffer = &buffer;
    this->commandBuffer = &commandBuffer;
  }

  void addImageCell(ImageCellBlueprint image) {
    if (image.format == VK_FORMAT_UNDEFINED) {
      image.format = uppexo::findDepthFormat(physicalDevice);
    }
    imageCellBlueprint.push_back(image);
  }
};

struct ImageCell {
  VkExtent2D size;
  VkImageAspectFlags aspect;
  VkFormat format;
  VkImageLayout layout;

  VkDeviceMemory memory;
  std::vector<VkImage> image;
  std::vector<VkImageView> imageView;
};

class Image {
public:
  Image(ImageBlueprint blueprint);
  ~Image();
  std::vector<VkImageView> getImageView(int cellID);
  std::vector<VkImage> getImage(int cellID);
  VkFormat getFormat(int cellID) { return imageList[cellID].format; };
  VkImageAspectFlags getAspect(int cellID) { return imageList[cellID].aspect; };
  VkExtent2D getSize(int cellID) { return imageList[cellID].size; };
  void exportImageToFile(std::string path, CommandBuffer &commandBuffer,
                         Buffer &buffer, int imageCellID, int imageID);

private:
  std::vector<ImageCell> imageList;

  VkDevice device;
};
} // namespace uppexo

#endif
