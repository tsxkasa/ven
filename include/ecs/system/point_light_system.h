#pragma once

#include "coordinator.h"
#include "pch.h"
#include "system.h"
#include "ven_camera.h"
#include "ven_device.h"
#include "ven_frame_info.h"
#include "ven_pipeline.h"

extern std::unique_ptr<ecs::Coordinator> gCoordinator;

namespace ecs {
namespace sys {
class PointLight : public ecs::System {
public:
  PointLight(ven::Device& device, VkRenderPass renderPass,
             VkDescriptorSetLayout globalSetLayout);
  ~PointLight();

  PointLight(const PointLight&) = delete;
  PointLight& operator=(const PointLight&) = delete;

  void update(ven::FrameInfo& frameInfo, ven::GlobalUBO& ubo);

  void draw(ven::FrameInfo& frameInfo);

private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);
  ven::Device& venDevice;
  std::unique_ptr<ven::Pipeline> venPipeline;
  VkPipelineLayout pipelineLayout;
};
} // namespace sys
} // namespace ecs