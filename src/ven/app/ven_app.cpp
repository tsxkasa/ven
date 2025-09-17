#include "ven_app.h"

void ven::App::run() {
  init();
  loop();
  cleanup();
}

void ven::App::init() {}

void ven::App::loop() {
  while (!ven_window.shouldClose()) {
    glfwPollEvents();
  }
}

void ven::App::cleanup() {}