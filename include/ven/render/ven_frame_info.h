#pragma once

#include "pch.h"
#include "ven_camera.h"

namespace ven {
struct FrameInfo {
  int frameIndex;
  float dt;
  VkCommandBuffer cmdBuffer;
  ven::Camera& camera;
  VkDescriptorSet globalDescriptorSet;
};
} // namespace ven