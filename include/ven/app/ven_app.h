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
    return ven_window;
  }

private:
  ven::Window ven_window{"App"};
  ven::Device ven_device{ven_window};
  ven::Pipeline pipeline{
      ven_device, "shaders/shader.vert.spv", "shaders/shader.frag.spv",
      Pipeline::defaultConfigInfo(ven_window.getVidMode()->width,
                                  ven_window.getVidMode()->height)};

  void init();
  void loop();
  void cleanup();
};
} // namespace ven