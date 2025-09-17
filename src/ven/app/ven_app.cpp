#include "ven_app.h"

void ven::App::run() {
  init();
  loop();
  cleanup();
}

void ven::App::init() {}

void ven::App::loop() {
  while (!venWindow.shouldClose()) {
    glfwPollEvents();
  }
}

void ven::App::cleanup() {}