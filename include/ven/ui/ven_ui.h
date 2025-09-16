#pragma once

#include "pch.h"

namespace ven {
class UI {
public:
  UI(VkDevice device, VkRenderPass renderPass, GLFWwindow *window);
  ~UI();

  void beginFrame();
  void endFrame();

private:
  VkDescriptorPool dpool;
};
} // namespace ven