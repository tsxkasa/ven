#include "point_light_system.h"
#include "colors.h"
#include "point_light.h"
#include "transform.h"
#include "ven_frame_info.h"

ecs::sys::PointLight::PointLight(ven::Device &device, VkRenderPass renderPass,
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
  VkPushConstantRange pushConstRange{};
  pushConstRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstRange.offset = 0;
  pushConstRange.size = sizeof(ven::PointLightPushConstants);

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

void ecs::sys::PointLight::update(ven::FrameInfo &frameInfo,
                                  ven::GlobalUBO &ubo) {
  auto rotateLight =
      glm::rotate(glm::mat4(1.0f), frameInfo.dt, {0.0f, -1.0f, 0.0f});
  int lightIndex = 0;
  for (const auto &ent : m_entities) {
    auto &transform3d = gCoordinator->getComponent<ecs::comp::Transform3D>(ent);
    auto &color = gCoordinator->getComponent<ecs::comp::Color>(ent);
    auto &pointLight = gCoordinator->getComponent<ecs::comp::PointLight>(ent);

    assert(lightIndex < MAX_LIGHTS && "Point lights exceeded limits");

    transform3d.translation =
        glm::vec3(rotateLight * glm::vec4(transform3d.translation, 1.0f));

    ubo.pointLights[lightIndex].position =
        glm::vec4(transform3d.translation, 1.0f);
    ubo.pointLights[lightIndex].color =
        glm::vec4(color.color, pointLight.lightIntensity);

    ++lightIndex;
  }
  ubo.numLights = lightIndex;
}

void ecs::sys::PointLight::draw(ven::FrameInfo &frameInfo) {
  venPipeline->bind(frameInfo.cmdBuffer);

  vkCmdBindDescriptorSets(frameInfo.cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet,
                          0, nullptr);

  for (const auto &ent : m_entities) {
    auto &transform3d = gCoordinator->getComponent<ecs::comp::Transform3D>(ent);
    auto &color = gCoordinator->getComponent<ecs::comp::Color>(ent);
    auto &pointLight = gCoordinator->getComponent<ecs::comp::PointLight>(ent);
    ven::PointLightPushConstants push{};
    push.position = glm::vec4(transform3d.translation, 1.0f);
    push.color = glm::vec4(color.color, pointLight.lightIntensity);
    push.radius = transform3d.scale.x;

    vkCmdPushConstants(frameInfo.cmdBuffer, pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(ven::PointLightPushConstants), &push);

    vkCmdDraw(frameInfo.cmdBuffer, 6, 1, 0, 0);
  }
}