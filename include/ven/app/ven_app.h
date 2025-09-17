#pragma once

#include "pch.h"
#include "ven_device.h"
#include "ven_pipeline.h"
#include "ven_ui.h"
#include "ven_window.h"

namespace ven {
class App {
public:
  void run();

  inline const ven::Window& getAppWindow() const {
    return venWindow;
  }

private:
  ven::Window venWindow{"App"};
  ven::Device venDevice{venWindow};
  ven::Pipeline pipeline{
      venDevice, "shaders/shader.vert.spv", "shaders/shader.frag.spv",
      Pipeline::defaultConfigInfo(venWindow.getVidMode()->width,
                                  venWindow.getVidMode()->height)};

  void init();
  void loop();
  void cleanup();
};
} // namespace ven