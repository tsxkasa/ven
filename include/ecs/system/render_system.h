#pragma once

#include "coordinator.h"
#include "pch.h"
#include "system.h"
#include "ven_device.h"
#include "ven_pipeline.h"

namespace ven {
struct TempPushConstantData {
  glm::mat4 transform{1.0f};
  alignas(16) glm::vec3 color; // vec3 requires 16 bytes alignment
};
} // namespace ven

namespace ecs {
namespace sys {
class Render : public ecs::System {
public:
  Render(ven::Device& device, VkRenderPass renderPass, ecs::Coordinator& coord);
  ~Render();

  Render(const Render&) = delete;
  Render& operator=(const Render&) = delete;

  void update(VkCommandBuffer cmdBuffer);

private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);
  ven::Device& venDevice;
  std::unique_ptr<ven::Pipeline> venPipeline;
  VkPipelineLayout pipelineLayout;
  ecs::Coordinator& coordinator;
};
} // namespace sys
} // namespace ecs