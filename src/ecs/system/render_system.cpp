#include "render_system.h"
#include "colors.h"
#include "model.h"
#include "transform.h"

ecs::sys::Render::Render(ven::Device& device, VkRenderPass renderPass)
    : venDevice{device} {
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

void ecs::sys::Render::update(VkCommandBuffer cmdBuffer,
                              const ven::Camera& camera) {
  venPipeline->bind(cmdBuffer);

  auto projectionView = camera.getProjection() * camera.getView();
  for (const auto& ent : m_entities) {
    auto& transform3d = gCoordinator->getComponent<ecs::comp::Transform3D>(ent);
    auto& model = gCoordinator->getComponent<ecs::comp::Model>(ent);
    // auto& color = gCoordinator->getComponent<ecs::comp::Color>(ent);

    ven::TempPushConstantData push{};
    // push.color = color.color;
    push.transform = projectionView * transform3d.mat4();

    vkCmdPushConstants(cmdBuffer, pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(ven::TempPushConstantData), &push);

    model.model->bind(cmdBuffer);
    model.model->draw(cmdBuffer);
  }
}