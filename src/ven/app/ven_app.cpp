#include "ven_app.h"
#include "ven_model.h"
#include "ven_pipeline.h"
#include "ven_swap_chain.h"

namespace ven {
struct TempPushConstantData {
  glm::mat2 transform{1.0f};
  glm::vec2 offset;
  alignas(16) glm::vec3 color; // vec3 requires 16 bytes alignment
};
} // namespace ven

glm::vec3 rgb(float h, float s, float v) {
  float c = v * s; // Chroma
  float x = c * (1.0f - glm::abs(glm::mod(h / 60.0f, 2.0f) - 1.0f));
  float m = v - c;

  float r, g, b;
  if (h >= 0.0f && h < 60.0f) {
    r = c;
    g = x;
    b = 0.0f;
  } else if (h >= 60.0f && h < 120.0f) {
    r = x;
    g = c;
    b = 0.0f;
  } else if (h >= 120.0f && h < 180.0f) {
    r = 0.0f;
    g = c;
    b = x;
  } else if (h >= 180.0f && h < 240.0f) {
    r = 0.0f;
    g = x;
    b = c;
  } else if (h >= 240.0f && h < 300.0f) {
    r = x;
    g = 0.0f;
    b = c;
  } else {
    r = c;
    g = 0.0f;
    b = x;
  }

  return glm::vec3(r + m, g + m, b + m);
}

ven::App::App() {
  loadModels();
  createPipelineLayout();
  recreateSwapChain();
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

  VkPushConstantRange pushConstRange{};
  pushConstRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstRange.offset = 0;
  pushConstRange.size = sizeof(ven::TempPushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstRange;

  if (vkCreatePipelineLayout(venDevice.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS)
    throw std::runtime_error("Failed to create pipeline layout.");
}

void ven::App::createPipeline() {
  assert(venSwapChain && "Cannot create pipeline before swap chain");
  assert(pipelineLayout && "Cannot create pipeline before layout");

  ven::PipelineConfigInfo pipelineConfig{};
  ven::Pipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = venSwapChain->getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  venPipeline = std::make_unique<ven::Pipeline>(
      venDevice, "shaders/shader.vert.spv", "shaders/shader.frag.spv",
      pipelineConfig);
}

void ven::App::createCmdBuffers() {
  cmdBuffers.resize(venSwapChain->imageCount());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = venDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());

  if (vkAllocateCommandBuffers(venDevice.device(), &allocInfo,
                               cmdBuffers.data()) != VK_SUCCESS)
    throw std::runtime_error("Failed to create command buffers");
}

void ven::App::freeCmdBuffers() {
  vkFreeCommandBuffers(venDevice.device(), venDevice.getCommandPool(),
                       static_cast<float>(cmdBuffers.size()),
                       cmdBuffers.data());
  cmdBuffers.clear();
}

void ven::App::recordCmdBuffer(int imageIndex) {
  static int frame = 0;
  frame = (frame + 1) % 100;

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(cmdBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
    throw std::runtime_error("Failed to begin recording command buffer");

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = venSwapChain->getRenderPass();
  renderPassInfo.framebuffer = venSwapChain->getFrameBuffer(imageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = venSwapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  // BACKGROUND: #D7E6BC
  clearValues[0].color = {{0.843f, 0.901f, 0.737f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(cmdBuffers[imageIndex], &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(venSwapChain->getSwapChainExtent().width);
  viewport.height =
      static_cast<float>(venSwapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, venSwapChain->getSwapChainExtent()};
  vkCmdSetViewport(cmdBuffers[imageIndex], 0, 1, &viewport);
  vkCmdSetScissor(cmdBuffers[imageIndex], 0, 1, &scissor);

  venPipeline->bind(cmdBuffers[imageIndex]);
  venModel->bind(cmdBuffers[imageIndex]);

  for (int j = 0; j < 4; j++) {
    TempPushConstantData push{};
    push.offset = {-0.5f + frame * 0.02, -0.4f + j * 0.25f};
    float hue = glm::mod((frame * 5.0f + j * 60.0f), 360.0f);
    float saturation = 1.0f;
    float value = 1.0f;
    glm::vec3 color = rgb(hue, saturation, value);
    float brightnessMod = 0.6f + 0.4f * sin(frame * 0.2f + j);
    color *= brightnessMod;
    push.color = color;

    vkCmdPushConstants(cmdBuffers[imageIndex], pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(TempPushConstantData), &push);

    venModel->draw(cmdBuffers[imageIndex]);
  }

  vkCmdEndRenderPass(cmdBuffers[imageIndex]);
  if (vkEndCommandBuffer(cmdBuffers[imageIndex]) != VK_SUCCESS)
    throw std::runtime_error("Failed to record command buffer");
}

void ven::App::recreateSwapChain() {
  auto extent = venWindow.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = venWindow.getExtent();
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(venDevice.device());
  if (!venSwapChain)
    venSwapChain = std::make_unique<ven::SwapChain>(venDevice, extent);
  else {
    venSwapChain = std::make_unique<ven::SwapChain>(venDevice, extent,
                                                    std::move(venSwapChain));

    if (venSwapChain->imageCount() != cmdBuffers.size()) {
      freeCmdBuffers();
      createCmdBuffers();
    }
  }
  createPipeline();
}

void ven::App::draw() {
  uint32_t imageIndex = 0;
  VkResult result = venSwapChain->acquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    throw std::runtime_error("Failed to acquire swap chain image");

  recordCmdBuffer(imageIndex);
  result =
      venSwapChain->submitCommandBuffers(&cmdBuffers[imageIndex], &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      venWindow.wasResized()) {
    venWindow.resetWindowResizedFlag();
    recreateSwapChain();
    return;
  }
  if (result != VK_SUCCESS)
    throw std::runtime_error("Failed to present swap chain image");
}

void ven::App::loadModels() {
  int screenWidth = venWindow.getVidMode()->width;
  int screenHeight = venWindow.getVidMode()->height;
  float radius = 100.0f;
  float normalizedX = (2.0f * radius) / screenWidth;
  float normalizedY = (2.0f * radius) / screenHeight;
  int vert = 52;
  float step = (2.0f * glm::pi<float>()) / (vert - 2);
  std::vector<ven::Model::Vertex> vertices{};

  vertices.push_back(ven::Model::Vertex{{0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});

  for (int i = 0; i < vert; i++) {
    float angle = step * i;

    glm::vec2 position = {normalizedX * cos(angle), normalizedY * sin(angle)};

    float hue = glm::mod((angle / (2.0f * glm::pi<float>())) * 360.0f, 360.0f);

    float saturation = 1.0f;
    float value = 1.0f;

    glm::vec3 color = rgb(hue, saturation, value);

    vertices.push_back(ven::Model::Vertex{position, color});
  }

  venModel = std::make_unique<ven::Model>(venDevice, vertices);
}