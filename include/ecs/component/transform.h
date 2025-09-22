#pragma once

namespace ecs {
struct TransformComponent {
  glm::vec2 translation{};

  consteval glm::mat2 mat2() const { return glm::mat2{1.0f}; }
};
} // namespace ecs