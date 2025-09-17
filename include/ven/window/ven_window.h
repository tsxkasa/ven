#pragma once

#include "pch.h"

namespace ven {
class Window {
public:
  /**
   * @param {name} window title
   */
  Window(std::string name);
  ~Window();

  int shouldClose() const;

  void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

  inline const GLFWwindow* getWindow() const {
    return window;
  }
  inline const GLFWmonitor* getMonitor() const {
    return monitor;
  }
  inline const GLFWvidmode* getVidMode() const {
    return vidMode;
  }

private:
  std::string windowTitle;
  GLFWwindow* window;
  GLFWmonitor* monitor;
  const GLFWvidmode* vidMode;

  /**
   * @brief Initializes the borderless window.
   */
  void initWindow();
};
} // namespace ven