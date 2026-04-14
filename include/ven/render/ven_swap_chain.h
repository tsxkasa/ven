#pragma once

#include "pch.h"
#include "ven_device.h"

namespace ven {

class SwapChain {
public:
  static constexpr int MAX_FRAMES_IN_FLIGHT = 3;

  SwapChain(ven::Device& deviceRef, VkExtent2D windowExtent);
  SwapChain(ven::Device& deviceRef, VkExtent2D windowExtent,
            std::shared_ptr<SwapChain> previous);
  ~SwapChain();

  SwapChain(const SwapChain&) = delete;
  auto operator=(const SwapChain&) -> SwapChain = delete;

  auto getFrameBuffer(int index) -> VkFramebuffer {
    return swapChainFramebuffers[index];
  }
  auto getRenderPass() -> VkRenderPass {
    return renderPass;
  }
  auto getImageView(int index) -> VkImageView {
    return swapChainImageViews[index];
  }
  auto imageCount() -> size_t {
    return swapChainImages.size();
  }
  auto getSwapChainImageFormat() -> VkFormat {
    return swapChainImageFormat;
  }
  auto getSwapChainExtent() -> VkExtent2D {
    return swapChainExtent;
  }
  auto width() -> uint32_t {
    return swapChainExtent.width;
  }
  auto height() -> uint32_t {
    return swapChainExtent.height;
  }

  auto extentAspectRatio() -> float {
    return static_cast<float>(swapChainExtent.width) /
           static_cast<float>(swapChainExtent.height);
  }
  auto findDepthFormat() -> VkFormat;

  auto acquireNextImage(uint32_t* imageIndex) -> VkResult;
  auto submitCommandBuffers(const VkCommandBuffer* buffers,
                            uint32_t* imageIndex) -> VkResult;

  [[nodiscard]] inline auto
  compareSwapFormats(const ven::SwapChain& swapchain) const -> bool {
    return swapchain.swapchainDepthFormat == swapchainDepthFormat &&
           swapchain.swapChainImageFormat == swapChainImageFormat;
  }

private:
  void init();
  void createSwapChain();
  void createImageViews();
  void createDepthResources();
  void createRenderPass();
  void createFramebuffers();
  void createSyncObjects();

  // Helper functions
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

  VkFormat swapChainImageFormat;
  VkFormat swapchainDepthFormat;
  VkExtent2D swapChainExtent;

  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkRenderPass renderPass;

  std::vector<VkImage> depthImages;
  std::vector<VmaAllocation> depthImageMemorys;
  std::vector<VkImageView> depthImageViews;
  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> swapChainImageViews;

  Device& device;
  VkExtent2D windowExtent;

  VkSwapchainKHR swapChain;
  std::shared_ptr<SwapChain> oldSwapChain;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;
  size_t currentFrame = 0;
};

} // namespace ven