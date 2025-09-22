#pragma once

#include "coordinator.h"
#include "pch.h"
#include "render_system.h"
#include "ven_device.h"
#include "ven_model.h"
#include "ven_pipeline.h"
#include "ven_swap_chain.h"
#include "ven_ui.h"
#include "ven_window.h"


namespace ven {
struct TempPushConstantData {
  glm::mat2 transform{1.0f};
  glm::vec2 offset;
  alignas(16) glm::vec3 color; // vec3 requires 16 bytes alignment
};

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
  void createPipelineLayout();
  void createPipeline();
  void createCmdBuffers();
  void freeCmdBuffers();
  void draw();
  void recreateSwapChain();
  void recordCmdBuffer(int imageIndex);
  int constexpr static WIDTH = 800;
  int constexpr static HEIGHT = 600;
  ven::Window venWindow{"App", WIDTH, HEIGHT};
  ven::Device venDevice{venWindow};
  std::unique_ptr<ven::SwapChain> venSwapChain;
  std::unique_ptr<ven::Pipeline> venPipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> cmdBuffers;
  ecs::Coordinator coordinator;
  std::shared_ptr<ecs::sys::Render> renderSystem;
};
} // namespace ven