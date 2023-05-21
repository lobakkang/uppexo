#include <base/image.hpp>
#include <core/commandBufferRecorder.hpp>
#include <cstddef>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <utils/log.hpp>

uppexo::Image::Image(uppexo::ImageBlueprint blueprint) {
  uppexo::Log::GetInstance().logInfo("Creating image\n");
  this->device = blueprint.device;

  uppexo::Log::GetInstance().logVerbose("Allocating memory and image\n");

  for (uppexo::ImageCellBlueprint cellBlueprint :
       blueprint.imageCellBlueprint) {
    if (cellBlueprint.size.height == 0 && cellBlueprint.size.width == 0) {
      cellBlueprint.size = blueprint.extend;
    }

    uppexo::ImageCell imageCell;
    for (int i = 0; i < cellBlueprint.num; i++) {
      VkImage image;
      VkImageCreateInfo imageInfo{};
      imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
      imageInfo.imageType = VK_IMAGE_TYPE_2D;
      imageInfo.extent.width = cellBlueprint.size.width;
      imageInfo.extent.height = cellBlueprint.size.height;
      imageInfo.extent.depth = 1;
      imageInfo.mipLevels = 1;
      imageInfo.arrayLayers = 1;
      imageInfo.format = cellBlueprint.format;
      imageInfo.tiling = cellBlueprint.tiling;
      imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imageInfo.usage = cellBlueprint.usage;
      imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        uppexo::Log::GetInstance().logError("Failed to create image\n");
      }
      imageCell.image.push_back(image);
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, imageCell.image[0], &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size * cellBlueprint.num;
    VkMemoryPropertyFlags properties;
    switch (cellBlueprint.location) {
    case BufferLocation::ON_HOST_VISIBLE_TO_DEVICE:
      properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
      break;
    case BufferLocation::ON_DEVICE_INVISIBLE_TO_HOST:
      properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
      break;
    case BufferLocation::ON_DEVICE_VISIBLE_TO_HOST:
      properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT |
                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
      break;
    default:
      uppexo::Log::GetInstance().logError("Unknown buffer location options\n");
      break;
    }

    allocInfo.memoryTypeIndex = findMemoryType(
        memRequirements.memoryTypeBits, properties, blueprint.physicalDevice);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageCell.memory) !=
        VK_SUCCESS) {
      uppexo::Log::GetInstance().logError("Failed to allocate image memory\n");
    }

    for (int i = 0; i < imageCell.image.size(); i++) {
      vkBindImageMemory(device, imageCell.image[i], imageCell.memory,
                        memRequirements.size * i);
    }
    imageList.push_back(imageCell);

    for (int i = 0; i < cellBlueprint.num; i++) {
      if (cellBlueprint.path != "") {
        uppexo::Log::GetInstance().logVerbose("Loading texture %s\n",
                                              cellBlueprint.path.c_str());
        int texWidth, texHeight, texChannels;
        stbi_uc *pixels = stbi_load(cellBlueprint.path.c_str(), &texWidth,
                                    &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if (!pixels) {
          uppexo::Log::GetInstance().logError("Failed to load texture image!");
        }

        blueprint.buffer->copyByMapping(blueprint.buffer->getStagingBufferID(),
                                        pixels, static_cast<size_t>(imageSize));
        stbi_image_free(pixels);

        VkCommandBuffer singleUseCommandBuffer =
            blueprint.commandBuffer->createSingleUseCommandBuffer();
        uppexo::command::transitionImageLayout(
            singleUseCommandBuffer, imageCell.image[i], VK_FORMAT_R8G8B8A8_SRGB,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        uppexo::command::copyBufferToImage(
            singleUseCommandBuffer,
            blueprint.buffer->getBuffer(blueprint.buffer->getStagingBufferID()),
            imageCell.image[i], static_cast<uint32_t>(texWidth),
            static_cast<uint32_t>(texHeight));
        uppexo::command::transitionImageLayout(
            singleUseCommandBuffer, imageCell.image[i], VK_FORMAT_R8G8B8A8_SRGB,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        blueprint.commandBuffer->submitSingleUseCommandBuffer(
            singleUseCommandBuffer);
      }
    }
  }

  uppexo::Log::GetInstance().logVerbose("Creating image view\n");

  int j = 0;
  for (uppexo::ImageCellBlueprint cellBlueprint :
       blueprint.imageCellBlueprint) {
    for (int i = 0; i < cellBlueprint.num; i++) {
      VkImageViewCreateInfo viewInfo{};
      viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      viewInfo.image = imageList[j].image[i];
      viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      viewInfo.format = cellBlueprint.format;
      viewInfo.subresourceRange.aspectMask = cellBlueprint.aspect;
      viewInfo.subresourceRange.baseMipLevel = 0;
      viewInfo.subresourceRange.levelCount = 1;
      viewInfo.subresourceRange.baseArrayLayer = 0;
      viewInfo.subresourceRange.layerCount = 1;

      VkImageView imageView;
      if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) !=
          VK_SUCCESS) {
        uppexo::Log::GetInstance().logError(
            "Failed to create texture image view!");
      }
      imageList[j].imageView.push_back(imageView);
    }
    j++;
  }
}

uppexo::Image::~Image() {
  uppexo::Log::GetInstance().logInfo("Destroying image\n");
  uppexo::Log::GetInstance().logVerbose("Destroying image view\n");
  for (uppexo::ImageCell imageCell : imageList) {
    for (VkImageView imageView : imageCell.imageView) {
      vkDestroyImageView(device, imageView, nullptr);
    }
  }
  uppexo::Log::GetInstance().logVerbose("Destroying image and memory\n");
  for (uppexo::ImageCell imageCell : imageList) {
    for (VkImage image : imageCell.image) {
      vkDestroyImage(device, image, nullptr);
    }
    vkFreeMemory(device, imageCell.memory, nullptr);
  }
}

std::vector<VkImageView> uppexo::Image::getImageView(int cellID) {
  return imageList[cellID].imageView;
}
