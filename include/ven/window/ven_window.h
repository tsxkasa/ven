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

  inline int shouldClose() const {
    return glfwWindowShouldClose(window);
  }

  inline VkExtent2D getExtent() const {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

  inline bool wasResized() {
    return framebufferResized;
  }

  void resetWindowResizedFlag() {
    framebufferResized = false;
  }

  inline GLFWwindow* getWindow() const {
    return window;
  }
  inline GLFWmonitor* getMonitor() const {
    return monitor;
  }
  inline const GLFWvidmode* getVidMode() const {
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