#include "mouse.h"
#include "pch.h"
#include "ven_app.h"
#include "ven_ui.h"

int main() {
  ven::App ven_app;
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  fmt::println("{:d} extensions supported.", extensionCount);
  ven_app.run();
  return 0;
}