#include "ven_app.h"
#include "camera.h"
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
  try {
    init();
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
}

void ven::App::init() {
  gCoordinator->init();

  gCoordinator->registerComponent<ecs::comp::CameraTransform3D>();
  gCoordinator->registerComponent<ecs::comp::Transform3D>();
  gCoordinator->registerComponent<ecs::comp::Color>();
  gCoordinator->registerComponent<ecs::comp::Model>();
  gCoordinator->registerComponent<ecs::comp::Camera>();

  renderSystem = gCoordinator->registerSystem<ecs::sys::Render>(
      venDevice, venRenderer.getSwapChainRenderPass());
  Signature renderSig;
  renderSig.set(gCoordinator->getComponentType<ecs::comp::Transform3D>());
  renderSig.set(gCoordinator->getComponentType<ecs::comp::Model>());
  gCoordinator->setSystemSignature<ecs::sys::Render>(renderSig);

  loadObjects();
}

ven::App::~App() {}

void ven::App::run() {
  ven::Camera camera{};
  camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

  viewerObj = gCoordinator->createEntity();
  gCoordinator->addComponent(viewerObj, ecs::comp::Camera{});
  auto camTransform = ecs::comp::CameraTransform3D{};
  gCoordinator->addComponent(viewerObj, camTransform);

  keyboardMovementSystem =
      gCoordinator
          ->registerSystem<ecs::sys::KeyboardMovementControllerSystem>();
  Signature kbmSig;
  kbmSig.set(gCoordinator->getComponentType<ecs::comp::CameraTransform3D>());
  kbmSig.set(gCoordinator->getComponentType<ecs::comp::Camera>());
  gCoordinator->setSystemSignature<ecs::sys::KeyboardMovementControllerSystem>(
      kbmSig);

  auto currentTime = std::chrono::high_resolution_clock::now();

  while (!venWindow.shouldClose()) {
    glfwPollEvents();

    auto newTime = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float, std::chrono::seconds::period>(
                   newTime - currentTime)
                   .count();
    currentTime = newTime;

    keyboardMovementSystem->moveInPlaneXZ(venWindow.getWindow(), dt);
    auto& camTransform =
        gCoordinator->getComponent<ecs::comp::CameraTransform3D>(viewerObj);
    camera.setViewYXZ(camTransform.translation, camTransform.rotation);

    float aspect = venRenderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);

    if (auto cmdBuffer = venRenderer.beginFrame()) {
      venRenderer.beginSwapChainRenderPass(cmdBuffer);
      renderSystem->update(cmdBuffer, camera);
      venRenderer.endSwapChainRenderPass(cmdBuffer);
      venRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(venDevice.device());
}

// Takes in file and reads position in format of {float, float, float} in bytes
std::vector<ven::Model::Vertex>
LoadVertices(const char* filepath,
             glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f)) {
  std::vector<ven::Model::Vertex> vertices;
  std::ifstream file(filepath, std::ios::binary | std::ios::ate);
  if (!file) {
    std::cerr << "Failed to open file: " << filepath << "\n";
    return vertices;
  }

  std::streamsize size = file.tellg();
  if (size % (3 * sizeof(float)) != 0) {
    std::cerr
        << "File size is not multiple of vertex position size (3 floats)\n";
    return vertices;
  }

  file.seekg(0, std::ios::beg);
  size_t vertexCount = size / (3 * sizeof(float));
  vertices.resize(vertexCount);

  std::vector<float> positions(vertexCount * 3);

  if (!file.read(reinterpret_cast<char*>(positions.data()), size)) {
    std::cerr << "Failed to read vertex data\n";
    vertices.clear();
    return vertices;
  }

  for (size_t i = 0; i < vertexCount; ++i) {
    vertices[i].position = glm::vec3(positions[i * 3 + 0], positions[i * 3 + 1],
                                     positions[i * 3 + 2]);

    // Assign solid red color
    vertices[i].color = color;
  }

  return vertices;
}

std::unique_ptr<ven::Model> createCubeModel(ven::Device& device,
                                            glm::vec3 offset) {
  // std::vector<ven::Model::Vertex> vertices{

  //     // left face (white)
  //     {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
  //     {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
  //     {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
  //     {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
  //     {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
  //     {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

  //     // right face (yellow)
  //     {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
  //     {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
  //     {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
  //     {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
  //     {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
  //     {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

  //     // top face (orange, remember y axis points down)
  //     {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
  //     {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
  //     {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
  //     {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
  //     {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
  //     {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

  //     // bottom face (red)
  //     {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
  //     {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
  //     {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
  //     {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
  //     {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
  //     {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

  //     // nose face (blue)
  //     {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
  //     {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
  //     {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
  //     {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
  //     {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
  //     {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

  //     // tail face (green)
  //     {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
  //     {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
  //     {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
  //     {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
  //     {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
  //     {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

  // };
  auto vertices = LoadVertices("vertices");
  for (auto& v : vertices) {
    v.position += offset;
  }
  return std::make_unique<ven::Model>(device, vertices);
}

void ven::App::loadObjects() {
  std::shared_ptr<ven::Model> venModel =
      createCubeModel(venDevice, {0.0f, 0.0f, 0.0f});
  cube = gCoordinator->createEntity();
  gCoordinator->addComponent(cube, ecs::comp::Model{venModel});
  auto transform3d = ecs::comp::Transform3D{};
  transform3d.translation = {0.0f, 0.0f, 2.5f};
  transform3d.scale = {0.5f, 0.5f, 0.5f};
  gCoordinator->addComponent(cube, transform3d);
}