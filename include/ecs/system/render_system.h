#pragma once

#include "coordinator.h"
#include "pch.h"
#include "system.h"
#include "ven_pipeline.h"

namespace ecs {
namespace sys {
class Render : public ecs::System {
public:
  void update(ecs::Coordinator& coordinator,
              std::unique_ptr<ven::Pipeline>& pipeline,
              VkPipelineLayout& layout, VkCommandBuffer& cmdBuffer);
};
} // namespace sys
} // namespace ecs