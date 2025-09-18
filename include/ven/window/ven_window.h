#pragma once

#include "pch.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <vulkan/vulkan_core.h>

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
  int width;
  int height;
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