#pragma once

#include "pch.h"
#include "ven_window.h"

namespace ven {

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
  uint32_t graphicsFamily;
  uint32_t presentFamily;
  bool graphicsFamilyHasValue = false;
  bool presentFamilyHasValue = false;
  bool isComplete() {
    return graphicsFamilyHasValue && presentFamilyHasValue;
  }
};

class Device {
public:
#ifdef NDEBUG
  const bool enableValidationLayers = false;
#else
  const bool enableValidationLayers = true;
#endif

  Device(ven::Window& window);
  ~Device();

  // Not copyable or movable
  Device(const Device&) = delete;
  auto operator=(const Device&) -> Device = delete;
  Device(Device&&) = delete;
  auto operator=(Device&&) -> Device& = delete;

  auto getCommandPool() -> VkCommandPool {
    return commandPool;
  }
  auto device() -> VkDevice {
    return device_;
  }
  auto surface() -> VkSurfaceKHR {
    return surface_;
  }
  auto graphicsQueue() -> VkQueue {
    return graphicsQueue_;
  }
  auto presentQueue() -> VkQueue {
    return presentQueue_;
  }

  auto getSwapChainSupport() -> SwapChainSupportDetails {
    return querySwapChainSupport(physicalDevice);
  }
  auto findPhysicalQueueFamilies() -> QueueFamilyIndices {
    return findQueueFamilies(physicalDevice);
  }
  auto findSupportedFormat(const std::vector<VkFormat>& candidates,
                           VkImageTiling tiling, VkFormatFeatureFlags features)
      -> VkFormat;

  // Buffer Helper Functions
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags properties, VkBuffer& buffer,
                    VmaAllocation& allocation);
  auto beginSingleTimeCommands() -> VkCommandBuffer;
  void endSingleTimeCommands(VkCommandBuffer commandBuffer);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                         uint32_t height, uint32_t layerCount);

  void createImageWithInfo(const VkImageCreateInfo& imageInfo,
                           VkMemoryPropertyFlags properties, VkImage& image,
                           VmaAllocation& allocation);

  auto allocator() -> VmaAllocator& {
    return allocator_;
  }

  VkPhysicalDeviceProperties properties;

private:
  void createInstance();
  void setupDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createCommandPool();

  // helper functions
  auto isDeviceSuitable(VkPhysicalDevice device) -> bool;
  auto getRequiredExtensions() -> std::vector<const char*>;
  auto checkValidationLayerSupport() -> bool;
  auto findQueueFamilies(VkPhysicalDevice device) -> QueueFamilyIndices;
  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT& createInfo);
  void hasGflwRequiredInstanceExtensions();
  auto checkDeviceExtensionSupport(VkPhysicalDevice device) -> bool;
  auto querySwapChainSupport(VkPhysicalDevice device)
      -> SwapChainSupportDetails;

  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  ven::Window& window;
  VkCommandPool commandPool;

  VkDevice device_;
  VkSurfaceKHR surface_;
  VkQueue graphicsQueue_;
  VkQueue presentQueue_;

  const std::vector<const char*> validationLayers = {
      "VK_LAYER_KHRONOS_validation"};
  const std::vector<const char*> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  VmaAllocator allocator_{};
};

} // namespace ven
