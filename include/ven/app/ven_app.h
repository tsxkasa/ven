#pragma once

#include "pch.h"
#include "ven_device.h"
#include "ven_model.h"
#include "ven_pipeline.h"
#include "ven_swap_chain.h"
#include "ven_ui.h"
#include "ven_window.h"
#include <memory>

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
  void loadModels();
  void createPipelineLayout();
  void createPipeline();
  void createCmdBuffers();
  void draw();
  int constexpr static WIDTH = 800;
  int constexpr static HEIGHT = 600;
  ven::Window venWindow{"App", WIDTH, HEIGHT};
  ven::Device venDevice{venWindow};
  ven::SwapChain venSwapChain{venDevice, venWindow.getExtent()};
  std::unique_ptr<ven::Pipeline> venPipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> cmdBuffers;
  std::unique_ptr<ven::Model> venModel;
};
} // namespace ven