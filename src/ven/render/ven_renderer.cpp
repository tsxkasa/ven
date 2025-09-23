#include "ven_renderer.h"

ven::Renderer::Renderer(ven::Window& window, ven::Device& device)
    : venWindow{window}
    , venDevice{device} {
  recreateSwapChain();
  createCmdBuffers();
}

ven::Renderer::~Renderer() {
  freeCmdBuffers();
}

void ven::Renderer::createCmdBuffers() {
  cmdBuffers.resize(ven::SwapChain::MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = venDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());

  if (vkAllocateCommandBuffers(venDevice.device(), &allocInfo,
                               cmdBuffers.data()) != VK_SUCCESS)
    throw std::runtime_error("Failed to create command buffers");
}

void ven::Renderer::freeCmdBuffers() {
  vkFreeCommandBuffers(venDevice.device(), venDevice.getCommandPool(),
                       static_cast<float>(cmdBuffers.size()),
                       cmdBuffers.data());
  cmdBuffers.clear();
}

void ven::Renderer::recreateSwapChain() {
  auto extent = venWindow.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = venWindow.getExtent();
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(venDevice.device());
  if (!venSwapChain)
    venSwapChain = std::make_unique<ven::SwapChain>(venDevice, extent);
  else {
    std::shared_ptr<ven::SwapChain> oldSwapChain = std::move(venSwapChain);
    venSwapChain =
        std::make_unique<ven::SwapChain>(venDevice, extent, oldSwapChain);

    if (!oldSwapChain->compareSwapFormats(*venSwapChain.get()))
      throw std::runtime_error("Swap chain image(or depth) format has changed");
  }
  // createPipeline();
}

VkCommandBuffer ven::Renderer::beginFrame() {
  assert(!isFrameStarted && "Can't call begin frame while frame in progress");
  VkResult result = venSwapChain->acquireNextImage(&currentImageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return nullptr;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    throw std::runtime_error("Failed to acquire swap chain image");

  isFrameStarted = true;

  auto cmdBuffer = getCurrentCmdBuffer();

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(cmdBuffer, &beginInfo) != VK_SUCCESS)
    throw std::runtime_error("Failed to begin recording command buffer");

  return cmdBuffer;
}

void ven::Renderer::endFrame() {
  assert(isFrameStarted && "Can't call end frame while frame not in progress");

  auto cmdBuffer = getCurrentCmdBuffer();

  if (vkEndCommandBuffer(cmdBuffer) != VK_SUCCESS)
    throw std::runtime_error("Failed to record command buffer");

  auto result =
      venSwapChain->submitCommandBuffers(&cmdBuffer, &currentImageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      venWindow.wasResized()) {
    venWindow.resetWindowResizedFlag();
    recreateSwapChain();
  } else if (result != VK_SUCCESS)
    throw std::runtime_error("Failed to present swap chain image");

  isFrameStarted = false;
  currentFrameIndex =
      (currentFrameIndex + 1) % ven::SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void ven::Renderer::beginSwapChainRenderPass(VkCommandBuffer cmdBuffer) {
  assert(isFrameStarted &&
         "Can't begin render pass while frame not in progress");
  assert(cmdBuffer == getCurrentCmdBuffer() &&
         "Can't begin render pass from a different frame");

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = venSwapChain->getRenderPass();
  renderPassInfo.framebuffer = venSwapChain->getFrameBuffer(currentImageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = venSwapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  // BACKGROUND: #D7E6BC
  clearValues[0].color = {{0.4f, 0.309f, 0.549f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(venSwapChain->getSwapChainExtent().width);
  viewport.height =
      static_cast<float>(venSwapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, venSwapChain->getSwapChainExtent()};
  vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);
  vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);
}
void ven::Renderer::endSwapChainRenderPass(VkCommandBuffer cmdBuffer) {
  assert(isFrameStarted && "Can't end render pass while frame not in progress");
  assert(cmdBuffer == getCurrentCmdBuffer() &&
         "Can't end render pass from a different frame");
  vkCmdEndRenderPass(cmdBuffer);
}