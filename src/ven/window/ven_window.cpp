#include "ven_window.h"

ven::Window::Window(std::string name) : window_title(name) {
  glfwInit();
  monitor = glfwGetPrimaryMonitor();
  vmode = glfwGetVideoMode(monitor);
  initWindow();
}

ven::Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void ven::Window::initWindow() {
  // Set window hints to create a borderless window
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_RED_BITS, vmode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, vmode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, vmode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, vmode->refreshRate);

  window =
      glfwCreateWindow(vmode->width, vmode->height, "Hello World", NULL, NULL);

  glfwSetWindowPos(window, 0, 0);
}

int ven::Window::shouldClose() const { return glfwWindowShouldClose(window); }

void ven::Window::createWindowSurface(VkInstance instance,
                                      VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
    throw std::runtime_error("Failed to create window surface.");
}