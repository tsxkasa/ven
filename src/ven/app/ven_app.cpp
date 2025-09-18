#include "ven_app.h"
#include "ven_pipeline.h"
#include "ven_swap_chain.h"
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
  }
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

void ven::App::createCmdBuffers() {}

void ven::App::draw() {}