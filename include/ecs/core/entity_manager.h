#pragma once

#include "pch.h"

namespace ecs {
class EntityManager {
public:
  EntityManager() {
    for (Entity ent = 0; ent < MAX_ENTITIES; ent++) {
      availableEntities.push(ent);
    }
  }

  Entity createEntity() {
    assert(livingEntityCount < MAX_ENTITIES && "Too many entities alive");

    Entity id = availableEntities.front();
    availableEntities.pop();
    livingEntityCount++;
    return id;
  }

  void destroyEntity(Entity ent) {
    assert(ent < MAX_ENTITIES && "Entity out of range");

    signatures[ent].reset();

    availableEntities.push(ent);
    livingEntityCount--;
  }

  inline void setSignature(Entity ent, Signature sig) {
    assert(ent < MAX_ENTITIES && "Entity out of range");

    signatures[ent] = sig;
  }

  Signature getSignature(Entity ent) {
    assert(ent < MAX_ENTITIES && "Entity out of range");

    return signatures[ent];
  }

private:
  std::queue<Entity> availableEntities{};
  std::array<Signature, MAX_ENTITIES> signatures{};
  uint32_t livingEntityCount{};
};
} // namespace ecs