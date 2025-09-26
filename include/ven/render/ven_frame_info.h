#pragma once

#include "pch.h"
#include "ven_camera.h"

namespace ven {
#define MAX_LIGHTS 10

struct PointLight {
  glm::vec4 position{};
  glm::vec4 color{};
};

struct PointLightPushConstants {
  glm::vec4 position{};
  glm::vec4 color{};
  float radius;
};

struct GlobalUBO {
  glm::mat4 projection{1.0f};
  glm::mat4 view{1.0f};
  glm::mat4 inverseView{1.0f};
  glm::vec4 ambientLightColor{1.0f, 1.0f, 1.0f, 0.02f}; // w is intensity
  PointLight pointLights[MAX_LIGHTS];
  int numLights;
};

struct FrameInfo {
  int frameIndex;
  float dt;
  VkCommandBuffer cmdBuffer;
  ven::Camera& camera;
  VkDescriptorSet globalDescriptorSet;
};
} // namespace ven