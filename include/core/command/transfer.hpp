#ifndef TRANSFER_COMMAND_HPP_
#define TRANSFER_COMMAND_HPP_

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <core/command/general.hpp>
#include <functional>
#include <utils/log.hpp>

#include <base/device.hpp>
#include <base/image.hpp>
#include <component.hpp>

namespace uppexo {
namespace command {
class CopyImageToBuffer : public Command {
public:
  CopyImageToBuffer(TrackedBlueprint<BufferBlueprint> &buffer,
                    TrackedBlueprint<ImageBlueprint> &image, int imageCellID,
                    int imageID, VkImageSubresourceLayers subresource,
                    uint32_t &imageWidth, uint32_t &imageHeight) {
    this->image = image.getComponent().getImage(imageCellID)[imageID];
    this->buffer = buffer.getComponent().getBuffer(
        buffer.getComponent().getStagingBufferID());
    this->subresource = subresource;
    this->imageWidth = &imageWidth;
    this->imageHeight = &imageHeight;
  }

  CopyImageToBuffer(Buffer &buffer, Image &image, int imageCellID, int imageID,
                    VkImageSubresourceLayers subresource, uint32_t &imageWidth,
                    uint32_t &imageHeight) {
    this->image = image.getImage(imageCellID)[imageID];
    this->buffer = buffer.getBuffer(buffer.getStagingBufferID());
    this->subresource = subresource;
    this->imageWidth = &imageWidth;
    this->imageHeight = &imageHeight;
  }

  void execute(VkCommandBuffer commandbuffer) override {
    VkBufferImageCopy copyRegion = {};
    copyRegion.bufferOffset = 0;
    copyRegion.bufferRowLength = 0;
    copyRegion.bufferImageHeight = 0;
    copyRegion.imageSubresource = subresource;
    copyRegion.imageOffset = {0, 0, 0};
    copyRegion.imageExtent = {*imageWidth, *imageHeight, 1};

    vkCmdCopyImageToBuffer(commandbuffer, image,
                           VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buffer, 1,
                           &copyRegion);
  }

private:
  VkImageSubresourceLayers subresource;
  VkImage image;
  VkBuffer buffer;

  uint32_t *imageWidth = 0;
  uint32_t *imageHeight = 0;
};

class TransitionImageLayout : public Command {
public:
  TransitionImageLayout(TrackedBlueprint<ImageBlueprint> &image,
                        int imageCellID, int imageID, VkImageLayout oldLayout,
                        VkImageLayout newLayout) {
    this->image = image.getComponent().getImage(imageCellID)[imageID];
    this->format = image.getComponent().getFormat(imageCellID);
    this->oldLayout = oldLayout;
    this->newLayout = newLayout;
  }

  TransitionImageLayout(Image &image, int imageCellID, int imageID,
                        VkImageLayout oldLayout, VkImageLayout newLayout) {
    this->image = image.getImage(imageCellID)[imageID];
    this->format = image.getFormat(imageCellID);
    this->oldLayout = oldLayout;
    this->newLayout = newLayout;
  }

  void execute(VkCommandBuffer commandbuffer) override {
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
        newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
               newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL &&
               newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
      uppexo::Log::GetInstance().logError("Unsupported layout transition!\n");
    }

    vkCmdPipelineBarrier(commandbuffer, sourceStage, destinationStage, 0, 0,
                         nullptr, 0, nullptr, 1, &barrier);
  }

private:
  VkImage image;
  VkFormat format;
  VkImageLayout oldLayout;
  VkImageLayout newLayout;

  uint32_t *imageWidth = 0;
  uint32_t *imageHeight = 0;
};
} // namespace command
} // namespace uppexo

#endif // !TRANSFER_COMMAND_HPP_
