#pragma once

#include "pch.h"
#include "ven_device.h"

namespace ven {

class Buffer {
public:
  Buffer(ven::Device& device, VkDeviceSize instanceSize, uint32_t instanceCount,
         VkBufferUsageFlags usageFlags,
         VkMemoryPropertyFlags memoryPropertyFlags,
         VkDeviceSize minOffsetAlignment = 1);
  ~Buffer();

  Buffer(const ven::Buffer&) = delete;
  auto operator=(const ven::Buffer&) -> Buffer& = delete;

  auto map() -> VkResult;
  void unmap();
  void write(const void* data, VkDeviceSize size, VkDeviceSize offset);

  void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE,
                     VkDeviceSize offset = 0);
  auto flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0)
      -> VkResult;
  auto descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE,
                      VkDeviceSize offset = 0) -> VkDescriptorBufferInfo;
  auto invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0)
      -> VkResult;

  void writeToIndex(void* data, int index);
  auto flushIndex(int index) -> VkResult;
  auto descriptorInfoForIndex(int index) -> VkDescriptorBufferInfo;
  auto invalidateIndex(int index) -> VkResult;

  [[nodiscard]] auto getBuffer() const -> VkBuffer {
    return buffer;
  }
  [[nodiscard]] auto getMappedMemory() const -> void* {
    return mapped;
  }
  [[nodiscard]] auto getInstanceCount() const -> uint32_t {
    return instanceCount;
  }
  [[nodiscard]] auto getInstanceSize() const -> VkDeviceSize {
    return instanceSize;
  }
  [[nodiscard]] auto getAlignmentSize() const -> VkDeviceSize {
    return instanceSize;
  }
  [[nodiscard]] auto getUsageFlags() const -> VkBufferUsageFlags {
    return usageFlags;
  }
  [[nodiscard]] auto getMemoryPropertyFlags() const -> VkMemoryPropertyFlags {
    return memoryPropertyFlags;
  }
  [[nodiscard]] auto getBufferSize() const -> VkDeviceSize {
    return bufferSize;
  }

private:
  static auto getAlignment(VkDeviceSize instanceSize,
                           VkDeviceSize minOffsetAlignment) -> VkDeviceSize;

  ven::Device& venDevice;
  void* mapped = nullptr;
  VkBuffer buffer = VK_NULL_HANDLE;
  VmaAllocation allocation;

  VkDeviceSize bufferSize;
  uint32_t instanceCount;
  VkDeviceSize instanceSize;
  VkDeviceSize alignmentSize;
  VkBufferUsageFlags usageFlags;
  VkMemoryPropertyFlags memoryPropertyFlags;
};

} // namespace ven
