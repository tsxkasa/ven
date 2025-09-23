#include "pch.h"
#include "ven_app.h"
#include "ven_ui.h"

int main() {
  ven::App app{};
  try {
    app.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}