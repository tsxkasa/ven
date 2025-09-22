#pragma once

namespace ecs {
class System {
public:
  std::set<Entity> entities;
};
} // namespace ecs