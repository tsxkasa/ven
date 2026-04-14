#include "ven_window.h"
#include <GLFW/glfw3.h>

#include <fmt/base.h>
#include <stdexcept>
#include <utility>
#include <vulkan/vulkan_core.h>

ven::Window::Window(std::string name, int w, int h)
    : width(w), height(h), windowTitle(std::move(name)) {
#ifdef VEN_USE_WAYLAND
  if (glfwPlatformSupported(GLFW_PLATFORM_WAYLAND)) {
    printf("Hinting GLFW to use Wayland\n");
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
  }
#endif
  if (!glfwInit()) {
    fprintf(stderr, "glfwInit failed\n");
    throw std::runtime_error("glfwInit failed\n");
  }
  printf("GLFW Init successful. Version: %s\n", glfwGetVersionString());
  if (!glfwVulkanSupported()) {
    printf("GLFW reports Vulkan is not supported!\n");
  } else {
    printf("GLFW reports Vulkan IS supported.\n");
  }
  monitor = glfwGetPrimaryMonitor();
  vidMode = glfwGetVideoMode(monitor);
  initWindow();
}

ven::Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void ven::Window::initWindow() {
  /*
     // Set window hints to create a borderless window
     glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
     glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
     glfwWindowHint(GLFW_RED_BITS, vidMode->redBits);
     glfwWindowHint(GLFW_GREEN_BITS, vidMode->greenBits);
     glfwWindowHint(GLFW_BLUE_BITS, vidMode->blueBits);
     glfwWindowHint(GLFW_REFRESH_RATE, vidMode->refreshRate);

     window = glfwCreateWindow(vidMode->width, vidMode->height, "Hello World",
                               NULL, NULL);

     glfwSetWindowPos(window, 0, 0);
   */
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  window =
      glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizedCallback);
}

void ven::Window::createWindowSurface(VkInstance instance,
                                      VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
    throw std::runtime_error("Failed to create window surface.");
}

void ven::Window::framebufferResizedCallback(GLFWwindow *window, int width,
                                             int height) {
  auto venWindow =
      reinterpret_cast<ven::Window *>(glfwGetWindowUserPointer(window));

  venWindow->framebufferResized = true;
  venWindow->width = width;
  venWindow->height = height;
}
