#include "point_light_system.h"

ecs::sys::PointLight::PointLight(ven::Device& device, VkRenderPass renderPass,
                                 VkDescriptorSetLayout globalSetLayout)
    : venDevice{device} {
  createPipelineLayout(globalSetLayout);
  createPipeline(renderPass);
}
ecs::sys::PointLight::~PointLight() {
  vkDestroyPipelineLayout(venDevice.device(), pipelineLayout, nullptr);
}

void ecs::sys::PointLight::createPipelineLayout(
    VkDescriptorSetLayout globalSetLayout) {
  // VkPushConstantRange pushConstRange{};
  // pushConstRange.stageFlags =
  //     VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  // pushConstRange.offset = 0;
  // pushConstRange.size = sizeof(ven::TempPushConstantData);

  std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount =
      static_cast<uint32_t>(descriptorSetLayouts.size());
  pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(venDevice.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS)
    throw std::runtime_error("Failed to create pipeline layout.");
}

void ecs::sys::PointLight::createPipeline(VkRenderPass renderPass) {
  assert(pipelineLayout && "Cannot create pipeline before layout");

  ven::PipelineConfigInfo pipelineConfig{};
  ven::Pipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.attributeDescription.clear();
  pipelineConfig.bindingDescription.clear();
  pipelineConfig.renderPass = renderPass;
  pipelineConfig.pipelineLayout = pipelineLayout;
  venPipeline = std::make_unique<ven::Pipeline>(
      venDevice, "shaders/point_light.vert.spv", "shaders/point_light.frag.spv",
      pipelineConfig);
}

void ecs::sys::PointLight::update(ven::FrameInfo& frameInfo) {
  venPipeline->bind(frameInfo.cmdBuffer);

  vkCmdBindDescriptorSets(frameInfo.cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet,
                          0, nullptr);

  vkCmdDraw(frameInfo.cmdBuffer, 6, 1, 0, 0);
}