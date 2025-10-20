#pragma once

namespace ecs {
class System {
public:
  std::set<Entity> m_entities;
};
} // namespace ecs