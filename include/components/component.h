#pragma once

#include "color.h"
#include "pch.h"

namespace ecs {
namespace comp {
struct Transform {
  glm::vec2 position{0.0f, 0.0f};
  glm::vec2 rotation{0.0f, 0.0f};
  float scale{1.0f};
};

struct Physics {
  float mass{1.0f};
  glm::vec2 velocity{0.0f, 0.0f};
  glm::vec2 gravity{0.0f, 9.80665f};
};

struct Render {
  unsigned int VAO{0}, VBO{0};
  int vertex_count{0};
  Color color{1, 1, 1, 1};
};
} // namespace components
} // namespace ecs