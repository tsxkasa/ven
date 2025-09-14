#include "system.h"

std::vector<Entity> ecs::sys::Physics::update(
    float delta, std::unordered_map<Entity, ecs::comp::Transform> &transforms,
    std::unordered_map<Entity, ecs::comp::Physics> &physics, float floorY) {

  std::vector<Entity> toRemove;
  toRemove.reserve(8);

  for (auto &kv : physics) {
    Entity entity = kv.first;
    ecs::comp::Physics &phys = kv.second;

    auto itTransform = transforms.find(entity);
    if (itTransform == transforms.end())
      continue;

    ecs::comp::Transform &transform = itTransform->second;

    // v = v + a * dt
    phys.velocity += phys.gravity * delta;

    // x = x + v * dt
    transform.position += phys.velocity * delta;

    float bottom = transform.position.y + (2.0f * transform.scale);
    if (bottom > floorY) {
      toRemove.push_back(entity);
    }
  }

  return toRemove;
}