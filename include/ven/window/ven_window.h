#pragma once

#include "pch.h"

namespace ven {
class Window {
public:
  /**
   * @param {name} window title
   */
  Window(std::string name, int w, int h);
  ~Window();

  [[nodiscard]] inline auto shouldClose() const -> int {
    return glfwWindowShouldClose(window);
  }

  [[nodiscard]] inline auto getExtent() const -> VkExtent2D {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

  inline auto wasResized() -> bool {
    return framebufferResized;
  }

  void resetWindowResizedFlag() {
    framebufferResized = false;
  }

  [[nodiscard]] inline auto getWindow() const -> GLFWwindow* {
    return window;
  }
  [[nodiscard]] inline auto getMonitor() const -> GLFWmonitor* {
    return monitor;
  }
  [[nodiscard]] inline auto getVidMode() const -> const GLFWvidmode* {
    return vidMode;
  }

private:
  int width;
  int height;
  bool framebufferResized = false;
  std::string windowTitle;
  GLFWwindow* window;
  GLFWmonitor* monitor;
  const GLFWvidmode* vidMode;

  /**
   * @brief Initializes the borderless window.
   */
  void initWindow();

  static void framebufferResizedCallback(GLFWwindow* window, int width,
                                         int height);
};
} // namespace ven
