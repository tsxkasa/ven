#include "ven_app.h"
#include "ven_pipeline.h"
#include "ven_swap_chain.h"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

ven::App::App() {
  createPipelineLayout();
  createPipeline();
  createCmdBuffers();
}

ven::App::~App() {
  vkDestroyPipelineLayout(venDevice.device(), pipelineLayout, nullptr);
}

void ven::App::run() {
  while (!venWindow.shouldClose()) {
    glfwPollEvents();
    draw();
  }

  vkDeviceWaitIdle(venDevice.device());
}

void ven::App::createPipelineLayout() {
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(venDevice.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS)
    throw std::runtime_error("Failed to create pipeline layout.");
}

void ven::App::createPipeline() {
  PipelineConfigInfo pipelineConfig{};
  ven::Pipeline::defaultPipelineConfigInfo(pipelineConfig, venSwapChain.width(),
                                           venSwapChain.height());
  pipelineConfig.renderPass = venSwapChain.getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  venPipeline = std::make_unique<ven::Pipeline>(
      venDevice, "shaders/shader.vert.spv", "shaders/shader.frag.spv",
      pipelineConfig);
}

void ven::App::createCmdBuffers() {
  cmdBuffers.resize(venSwapChain.imageCount());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = venDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());

  if (vkAllocateCommandBuffers(venDevice.device(), &allocInfo,
                               cmdBuffers.data()) != VK_SUCCESS)
    throw std::runtime_error("Failed to create command buffers");

  for (uint32_t i = 0; i < cmdBuffers.size(); i++) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(cmdBuffers[i], &beginInfo) != VK_SUCCESS)
      throw std::runtime_error("Failed to begin recording command buffer");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = venSwapChain.getRenderPass();
    renderPassInfo.framebuffer = venSwapChain.getFrameBuffer(i);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = venSwapChain.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    // BACKGROUND: #D7E6BC
    clearValues[0].color = {{0.843f, 0.901f, 0.737f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(cmdBuffers[i], &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);

    venPipeline->bind(cmdBuffers[i]);
    vkCmdDraw(cmdBuffers[i], 3, 1, 0, 0);

    vkCmdEndRenderPass(cmdBuffers[i]);
    if (vkEndCommandBuffer(cmdBuffers[i]) != VK_SUCCESS)
      throw std::runtime_error("Failed to record command buffer");
  }
}

void ven::App::draw() {
  uint32_t imageIndex = 0;
  VkResult result = venSwapChain.acquireNextImage(&imageIndex);

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    throw std::runtime_error("Failed to acquire swap chain image");

  result =
      venSwapChain.submitCommandBuffers(&cmdBuffers[imageIndex], &imageIndex);

  if (result != VK_SUCCESS)
    throw std::runtime_error("Failed to present swap chain image");
}