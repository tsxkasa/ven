#pragma once

#include "component.h"
#include "entity.h"
#include "pch.h"
#include "system.h"

namespace ecs {
namespace management {
class ResourceControl {
private:
  std::vector<Entity> entities;
  std::unordered_map<Entity, ecs::comp::Transform> transforms;
  std::unordered_map<Entity, ecs::comp::Physics> physics;
  std::unordered_map<Entity, ecs::comp::Render> renderables;

  ecs::sys::Physics physics_system;
  ecs::sys::Render render_system;

public:
  ResourceControl(unsigned int shader, const glm::mat4 &proj,
                  const glm::mat4 &v);
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

  inline void add_entity(Entity e) { entities.push_back(e); }
  inline std::vector<Entity> &get_entity() { return entities; }
  inline std::size_t entity_amount() { return entities.size(); }
  inline std::unordered_map<Entity, ecs::comp::Transform> &get_transforms() {
    return transforms;
  }
  inline std::unordered_map<Entity, ecs::comp::Physics> &get_physics() {
    return physics;
  }
  inline std::unordered_map<Entity, ecs::comp::Render> &get_renderables() {
    return renderables;
  }
};
} // namespace management
} // namespace ecs