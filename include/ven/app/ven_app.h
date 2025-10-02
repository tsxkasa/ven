#pragma once

#include "coordinator.h"
#include "keyboard_movement_controller_system.h"
#include "pch.h"
#include "point_light_system.h"
#include "render_system.h"
#include "ven_buffer.h"
#include "ven_descriptors.h"
#include "ven_device.h"
#include "ven_renderer.h"
#include "ven_window.h"

extern std::unique_ptr<ecs::Coordinator> gCoordinator;

namespace ven {
class App {
public:
  App();
  ~App();

  App(const App&) = delete;
  auto operator=(const App&) -> App& = delete;

  void run();

  [[nodiscard]] auto getAppWindow() const -> const ven::Window& {
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
  std::shared_ptr<ecs::sys::Render> renderSystem;
  std::shared_ptr<ecs::sys::PointLight> pointLightSystem;
  std::shared_ptr<ecs::sys::KeyboardMovementControllerSystem>
      keyboardMovementSystem;
  std::unique_ptr<ven::DescriptorPool> globalPool;
  std::unique_ptr<ven::DescriptorSetLayout> globalSetLayout;

  Entity viewerObj;
};
} // namespace ven