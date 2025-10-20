#include "pch.h"
#include "ven_app.h"
#include "ven_ui.h"
#include <GLFW/glfw3.h>
#include <print>

std::unique_ptr<ecs::Coordinator> gCoordinator;

auto main() -> int {
  gCoordinator = std::make_unique<ecs::Coordinator>();
  ven::App app{};
  try {
    app.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  gCoordinator.reset();
  return EXIT_SUCCESS;
}
