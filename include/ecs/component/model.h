#pragma once

#include "ven_model.h"

namespace ecs {
namespace comp {
struct Model {
  std::shared_ptr<ven::Model> model;
};
} // namespace comp
} // namespace ecs