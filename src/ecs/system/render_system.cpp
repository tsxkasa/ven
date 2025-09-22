#include "render_system.h"
#include "colors.h"
#include "model.h"
#include "transform.h"
#include "ven_app.h"

void ecs::sys::Render::update(ecs::Coordinator& coordinator,
                              std::unique_ptr<ven::Pipeline>& pipeline,
                              VkPipelineLayout& layout,
                              VkCommandBuffer& cmdBuffer) {
  pipeline->bind(cmdBuffer);
  for (const auto& ent : entities) {
    auto& transform = coordinator.getComponent<ecs::comp::Transform2D>(ent);
    auto& model = coordinator.getComponent<ecs::comp::Model>(ent);
    auto& color = coordinator.getComponent<ecs::comp::Color>(ent);

    ven::TempPushConstantData push{};
    push.offset = transform.translation;
    push.color = color.color;
    push.transform = transform.mat2();

    vkCmdPushConstants(cmdBuffer, layout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(ven::TempPushConstantData), &push);

    model.model->bind(cmdBuffer);
    model.model->draw(cmdBuffer);
  }
}