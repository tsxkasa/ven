#include "render_system.h"
#include "colors.h"
#include "model.h"
#include "transform.h"

ecs::sys::Render::Render(ven::Device& device, VkRenderPass renderPass,
                         ecs::Coordinator& coord)
    : venDevice{device}
    , coordinator(coord) {
  createPipelineLayout();
  createPipeline(renderPass);
}
ecs::sys::Render::~Render() {
  vkDestroyPipelineLayout(venDevice.device(), pipelineLayout, nullptr);
}

void ecs::sys::Render::createPipelineLayout() {
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

void ecs::sys::Render::createPipeline(VkRenderPass renderPass) {
  assert(pipelineLayout && "Cannot create pipeline before layout");

  ven::PipelineConfigInfo pipelineConfig{};
  ven::Pipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = renderPass;
  pipelineConfig.pipelineLayout = pipelineLayout;
  venPipeline = std::make_unique<ven::Pipeline>(
      venDevice, "shaders/shader.vert.spv", "shaders/shader.frag.spv",
      pipelineConfig);
}

void ecs::sys::Render::update(VkCommandBuffer cmdBuffer) {
  venPipeline->bind(cmdBuffer);
  for (const auto& ent : m_entities) {
    auto& transform2d = coordinator.getComponent<ecs::comp::Transform2D>(ent);
    auto& model = coordinator.getComponent<ecs::comp::Model>(ent);
    auto& color = coordinator.getComponent<ecs::comp::Color>(ent);

    transform2d.rotation =
        glm::mod(transform2d.rotation + 0.01f, glm::two_pi<float>());

    ven::TempPushConstantData push{};
    push.offset = transform2d.translation;
    push.color = color.color;
    push.transform = transform2d.mat2();

    vkCmdPushConstants(cmdBuffer, pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(ven::TempPushConstantData), &push);

    model.model->bind(cmdBuffer);
    model.model->draw(cmdBuffer);
  }
}