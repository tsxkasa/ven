#pragma once

#include "pch.h"

namespace ecs {
namespace comp {
struct Transform2D {
  glm::vec2 translation{};

  consteval glm::mat2 mat2() const {
    return glm::mat2{1.0f};
  }
};
} // namespace comp
} // namespace ecs