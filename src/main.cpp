#include "pch.h"
#include "ven_app.h"
#include "ven_ui.h"

std::unique_ptr<ecs::Coordinator> gCoordinator;

int main() {
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