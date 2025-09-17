#include "mouse.h"
#include "pch.h"
#include "ven_app.h"
#include "ven_ui.h"

int main() {
  ven::App venApp;
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  fmt::println("{:d} extensions supported.", extensionCount);
  venApp.run();
  return 0;
}