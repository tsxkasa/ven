#pragma once

#include "coordinator.h"
#include "pch.h"
#include "system.h"
#include "ven_camera.h"
#include "ven_device.h"
#include "ven_pipeline.h"

extern std::unique_ptr<ecs::Coordinator> gCoordinator;

namespace ven {
struct TempPushConstantData {
  glm::mat4 transform{1.0f};
  glm::mat4 modelMatrix{1.0f};
};
} // namespace ven

namespace ecs {
namespace sys {
class Render : public ecs::System {
public:
  Render(ven::Device& device, VkRenderPass renderPass);
  ~Render();

  Render(const Render&) = delete;
  Render& operator=(const Render&) = delete;

  void update(VkCommandBuffer cmdBuffer, const ven::Camera& camera);

private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);
  ven::Device& venDevice;
  std::unique_ptr<ven::Pipeline> venPipeline;
  VkPipelineLayout pipelineLayout;
};
} // namespace sys
} // namespace ecs