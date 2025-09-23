#pragma once

#include "pch.h"
#include "ven_device.h"
#include "ven_swap_chain.h"
#include "ven_window.h"

namespace ven {
class Renderer {
public:
  Renderer(ven::Window& window, ven::Device& device);
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  inline VkRenderPass getSwapChainRenderPass() const {
    return venSwapChain->getRenderPass();
  }

  inline bool isFrameInProgress() const {
    return isFrameStarted;
  }

  inline VkCommandBuffer getCurrentCmdBuffer() const {
    assert(isFrameStarted &&
           "Cannot get command buffer when frame not in progress");
    return cmdBuffers[currentImageIndex];
  }

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer cmdBuffer);
  void endSwapChainRenderPass(VkCommandBuffer cmdBuffer);

private:
  void createCmdBuffers();
  void freeCmdBuffers();
  void recreateSwapChain();
  ven::Window& venWindow;
  ven::Device& venDevice;
  std::unique_ptr<ven::SwapChain> venSwapChain;
  std::vector<VkCommandBuffer> cmdBuffers;

  uint32_t currentImageIndex;
  bool isFrameStarted = false;
};
} // namespace ven