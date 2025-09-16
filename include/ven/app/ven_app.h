#pragma once

#include "pch.h"
#include "ven_pipeline.h"
#include "ven_ui.h"
#include "ven_window.h"

namespace ven {
class App {
public:
  void run();

  inline const ven::Window &getAppWindow() const { return ven_window; }

private:
  ven::Window ven_window{"App"};
  ven::Pipeline pipeline{"shaders/shader.vert.spv", "shaders/shader.frag.spv"};

  void init(){};
  void loop(){};
  void cleanup(){};
};
} // namespace ven