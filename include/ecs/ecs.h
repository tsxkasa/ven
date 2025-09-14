#pragma once

#include "component.h"
#include "entity.h"
#include "pch.h"
#include "system.h"

namespace ecs {
namespace management {
class ResourceControl {
public:
  std::vector<Entity> entities;
  std::unordered_map<Entity, ecs::comp::Transform> transforms;
  std::unordered_map<Entity, ecs::comp::Physics> physics;
  std::unordered_map<Entity, ecs::comp::Render> renderables;

  ecs::sys::Physics physics_system;
  ecs::sys::Render render_system;

  ResourceControl(unsigned int shader, const glm::mat4 &proj, const glm::mat4 &v);
  inline std::vector<Entity> update(float dt, const GLFWvidmode &vmode) {
    render_system.draw(transforms, renderables);
    return physics_system.update(dt, transforms, physics, vmode.height);
  }

  void clear_scene() {
    transforms.clear();
    physics.clear();
    renderables.clear();
    entities.clear();
  }
};
} // namespace management
} // namespace ecs