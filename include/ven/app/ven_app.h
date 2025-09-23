#pragma once

#include "coordinator.h"
#include "pch.h"
#include "render_system.h"
#include "ven_device.h"
#include "ven_renderer.h"
#include "ven_window.h"

namespace ven {
class App {
public:
  App();
  ~App();

  App(const App&) = delete;
  App& operator=(const App&) = delete;

  void run();

  inline const ven::Window& getAppWindow() const {
    return venWindow;
  }

private:
  void init();
  void loadObjects();
  int constexpr static WIDTH = 800;
  int constexpr static HEIGHT = 600;
  ven::Window venWindow{"App", WIDTH, HEIGHT};
  ven::Device venDevice{venWindow};
  ven::Renderer venRenderer{venWindow, venDevice};
  ecs::Coordinator coordinator;
  std::shared_ptr<ecs::sys::Render> renderSystem;
};
} // namespace ven