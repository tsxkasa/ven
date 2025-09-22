#pragma once

#include "pch.h"

namespace ecs {
namespace comp {
struct Transform2D {
  glm::vec2 translation{};
  glm::vec2 scale{1.0f, 1.0f};
  float rotation;

  glm::mat2 mat2() const {
    const float s = glm::sin(rotation);
    const float c = glm::cos(rotation);
    glm::mat2 rotMat{{c, s}, {-s, c}};
    glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
    return rotMat * scaleMat;
  }
};
} // namespace comp
} // namespace ecs