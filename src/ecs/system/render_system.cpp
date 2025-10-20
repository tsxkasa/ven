#include "render_system.h"
#include "colors.h"
#include "model.h"
#include "transform.h"

ecs::sys::Render::Render(ven::Device& device, VkRenderPass renderPass,
                         VkDescriptorSetLayout globalSetLayout)
    : venDevice{device} {
  createPipelineLayout(globalSetLayout);
  createPipeline(renderPass);
}
ecs::sys::Render::~Render() {
  vkDestroyPipelineLayout(venDevice.device(), pipelineLayout, nullptr);
}

void ecs::sys::Render::createPipelineLayout(
    VkDescriptorSetLayout globalSetLayout) {
  VkPushConstantRange pushConstRange{};
  pushConstRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstRange.offset = 0;
  pushConstRange.size = sizeof(ven::TempPushConstantData);

  std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount =
      static_cast<uint32_t>(descriptorSetLayouts.size());
  pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
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

void ecs::sys::Render::update(ven::FrameInfo& frameInfo) {
  venPipeline->bind(frameInfo.cmdBuffer);

  vkCmdBindDescriptorSets(frameInfo.cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet,
                          0, nullptr);

  auto projectionView =
      frameInfo.camera.getProjection() * frameInfo.camera.getView();
  for (const auto& ent : m_entities) {
    auto& transform3d = gCoordinator->getComponent<ecs::comp::Transform3D>(ent);
    auto& model = gCoordinator->getComponent<ecs::comp::Model>(ent);
    // auto& color = gCoordinator->getComponent<ecs::comp::Color>(ent);

    ven::TempPushConstantData push{};
    // push.color = color.color;
    push.modelMatrix = transform3d.mat4();
    push.normalMatrix = transform3d.normalMatrix();

    vkCmdPushConstants(frameInfo.cmdBuffer, pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(ven::TempPushConstantData), &push);

    model.model->bind(frameInfo.cmdBuffer);
    model.model->draw(frameInfo.cmdBuffer);
  }
}