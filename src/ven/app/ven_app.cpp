#include "ven_app.h"
#include "colors.h"
#include "model.h"
#include "transform.h"

glm::vec3 rgb(float h, float s, float v) {
  float c = v * s; // Chroma
  float x = c * (1.0f - glm::abs(glm::mod(h / 60.0f, 2.0f) - 1.0f));
  float m = v - c;

  float r, g, b;
  if (h >= 0.0f && h < 60.0f) {
    r = c;
    g = x;
    b = 0.0f;
  } else if (h >= 60.0f && h < 120.0f) {
    r = x;
    g = c;
    b = 0.0f;
  } else if (h >= 120.0f && h < 180.0f) {
    r = 0.0f;
    g = c;
    b = x;
  } else if (h >= 180.0f && h < 240.0f) {
    r = 0.0f;
    g = x;
    b = c;
  } else if (h >= 240.0f && h < 300.0f) {
    r = x;
    g = 0.0f;
    b = c;
  } else {
    r = c;
    g = 0.0f;
    b = x;
  }

  return glm::vec3(r + m, g + m, b + m);
}

ven::App::App() {
  init();
  loadObjects();
}

void ven::App::init() {
  coordinator.init();

  coordinator.registerComponent<ecs::comp::Transform2D>();
  coordinator.registerComponent<ecs::comp::Color>();
  coordinator.registerComponent<ecs::comp::Model>();

  renderSystem = coordinator.registerSystem<ecs::sys::Render>(
      venDevice, venRenderer.getSwapChainRenderPass());
  Signature renderSig;
  renderSig.set(coordinator.getComponentType<ecs::comp::Transform2D>());
  renderSig.set(coordinator.getComponentType<ecs::comp::Model>());
  coordinator.setSystemSignature<ecs::sys::Render>(renderSig);
}

ven::App::~App() {}

void ven::App::run() {
  while (!venWindow.shouldClose()) {
    glfwPollEvents();
    if (auto cmdBuffer = venRenderer.beginFrame()) {
      venRenderer.beginSwapChainRenderPass(cmdBuffer);
      renderSystem->update(cmdBuffer);
      venRenderer.endSwapChainRenderPass(cmdBuffer);
      venRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(venDevice.device());
}

void ven::App::loadObjects() {
  int screenWidth = venWindow.getVidMode()->width;
  int screenHeight = venWindow.getVidMode()->height;
  float radius = 100.0f;
  float normalizedX = (2.0f * radius) / screenWidth;
  float normalizedY = (2.0f * radius) / screenHeight;
  int vert = 52;
  float step = (2.0f * glm::pi<float>()) / (vert - 2);
  std::vector<ven::Model::Vertex> vertices{};

  vertices.push_back(ven::Model::Vertex{{0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});

  for (int i = 0; i < vert; i++) {
    float angle = step * i;

    glm::vec2 position = {normalizedX * cos(angle), normalizedY * sin(angle)};

    float hue = glm::mod((angle / (2.0f * glm::pi<float>())) * 360.0f, 360.0f);

    float saturation = 1.0f;
    float value = 1.0f;

    glm::vec3 color = rgb(hue, saturation, value);

    vertices.push_back(ven::Model::Vertex{position, color});
  }

  auto venModel = std::make_shared<ven::Model>(venDevice, vertices);

  auto circle = coordinator.createEntity();
  coordinator.addComponent(circle, ecs::comp::Color{{0.4f, 0.309f, 0.549f}});
  auto transform2d = ecs::comp::Transform2D{};
  transform2d.translation.x = 0.2f;
  transform2d.scale = {0.2f, 0.5f};
  transform2d.rotation = 0.25 * glm::two_pi<float>();
  coordinator.addComponent(circle, transform2d);
  coordinator.addComponent(circle, ecs::comp::Model{{venModel}});
}