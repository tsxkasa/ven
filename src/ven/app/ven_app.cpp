#include "ven_app.h"
#include "camera.h"
#include "colors.h"
#include "model.h"
#include "point_light.h"
#include "transform.h"

auto rgb(float h, float s, float v) -> glm::vec3 {
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

  return {r + m, g + m, b + m};
}

ven::App::App() {
  try {
    init();
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
}

void ven::App::init() {
  globalSetLayout = ven::DescriptorSetLayout::Builder(venDevice)
                        .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                    VK_SHADER_STAGE_ALL_GRAPHICS)
                        .build();

  gCoordinator->init();

  gCoordinator->registerComponent<ecs::comp::Transform3D>();
  gCoordinator->registerComponent<ecs::comp::Color>();
  gCoordinator->registerComponent<ecs::comp::Model>();
  gCoordinator->registerComponent<ecs::comp::Camera>();
  gCoordinator->registerComponent<ecs::comp::PointLight>();

  renderSystem = gCoordinator->registerSystem<ecs::sys::Render>(
      venDevice, venRenderer.getSwapChainRenderPass(),
      globalSetLayout->getDescriptorSetLayout());
  Signature renderSig;
  renderSig.set(gCoordinator->getComponentType<ecs::comp::Transform3D>());
  renderSig.set(gCoordinator->getComponentType<ecs::comp::Model>());
  gCoordinator->setSystemSignature<ecs::sys::Render>(renderSig);

  pointLightSystem = gCoordinator->registerSystem<ecs::sys::PointLight>(
      venDevice, venRenderer.getSwapChainRenderPass(),
      globalSetLayout->getDescriptorSetLayout());
  Signature pointLightSig;
  pointLightSig.set(gCoordinator->getComponentType<ecs::comp::Transform3D>());
  pointLightSig.set(gCoordinator->getComponentType<ecs::comp::PointLight>());
  pointLightSig.set(gCoordinator->getComponentType<ecs::comp::Color>());
  gCoordinator->setSystemSignature<ecs::sys::PointLight>(pointLightSig);

  keyboardMovementSystem =
      gCoordinator
          ->registerSystem<ecs::sys::KeyboardMovementControllerSystem>();
  Signature kbmSig;
  kbmSig.set(gCoordinator->getComponentType<ecs::comp::Transform3D>());
  kbmSig.set(gCoordinator->getComponentType<ecs::comp::Camera>());
  gCoordinator->setSystemSignature<ecs::sys::KeyboardMovementControllerSystem>(
      kbmSig);

  globalPool = ven::DescriptorPool::Builder(venDevice)
                   .setMaxSets(ven::SwapChain::MAX_FRAMES_IN_FLIGHT)
                   .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                ven::SwapChain::MAX_FRAMES_IN_FLIGHT)
                   .build();

  loadObjects();
}

ven::App::~App() = default;

void ven::App::run() {
  std::vector<std::unique_ptr<ven::Buffer>> uboBuffers(
      ven::SwapChain::MAX_FRAMES_IN_FLIGHT);
  for (auto& uboBuffer : uboBuffers) {
    uboBuffer = std::make_unique<ven::Buffer>(
        venDevice, sizeof(GlobalUBO), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    uboBuffer->map();
  }

  std::vector<VkDescriptorSet> globalDescriptorSets(
      ven::SwapChain::MAX_FRAMES_IN_FLIGHT);
  for (size_t i = 0; i < globalDescriptorSets.size(); i++) {
    auto bufferInfo = uboBuffers[i]->descriptorInfo();
    ven::DescriptorWriter(*globalSetLayout, *globalPool)
        .writeBuffer(0, &bufferInfo)
        .build(globalDescriptorSets[i]);
  }

  ven::Camera camera{};
  camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

  viewerObj = gCoordinator->createEntity();
  gCoordinator->addComponent(viewerObj, ecs::comp::Camera{});
  auto camTransform = ecs::comp::Transform3D{};
  gCoordinator->addComponent(viewerObj, camTransform);

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
        gCoordinator->getComponent<ecs::comp::Transform3D>(viewerObj);
    camera.setViewYXZ(camTransform.translation, camTransform.rotation);

    float aspect = venRenderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 1000.0f);

    if (auto cmdBuffer = venRenderer.beginFrame()) {
      int frameIndex = venRenderer.getFrameIndex();
      ven::FrameInfo frameInfo{.frameIndex = frameIndex,
                               .dt = dt,
                               .cmdBuffer = cmdBuffer,
                               .camera = camera,
                               .globalDescriptorSet =
                                   globalDescriptorSets[frameIndex]};
      ven::GlobalUBO ubo{};
      ubo.projection = camera.getProjection();
      ubo.view = camera.getView();
      ubo.inverseView = camera.getInverseView();
      pointLightSystem->update(frameInfo, ubo);
      uboBuffers[frameIndex]->writeToBuffer(&ubo);
      uboBuffers[frameIndex]->flush();

      venRenderer.beginSwapChainRenderPass(cmdBuffer);
      renderSystem->update(frameInfo);
      pointLightSystem->draw(frameInfo);
      venRenderer.endSwapChainRenderPass(cmdBuffer);
      venRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(venDevice.device());
}

void ven::App::loadObjects() {
  std::shared_ptr<ven::Model> venModel =
      ven::Model::createModel(venDevice, "assets/models/kanade.obj");
  auto object = gCoordinator->createEntity();
  gCoordinator->addComponent(object, ecs::comp::Model{venModel});
  auto transform3d = ecs::comp::Transform3D{};
  transform3d.translation = {0.0f, 0.0f, 2.5f};
  transform3d.scale = {0.5f, 0.5f, 0.5f};
  gCoordinator->addComponent(object, transform3d);

  std::vector<glm::vec3> lightColors{{1.f, .1f, .1f}, {.1f, .1f, 1.f},
                                     {.1f, 1.f, .1f}, {1.f, 1.f, .1f},
                                     {.1f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

  for (size_t i = 0; i < lightColors.size(); i++) {
    auto pointLight = gCoordinator->createEntity();
    gCoordinator->addComponent(pointLight, ecs::comp::Color{lightColors[i]});
    auto rotateLight =
        glm::rotate(glm::mat4(1.0f),
                    (static_cast<float>(i) * glm::two_pi<float>() /
                     static_cast<float>(lightColors.size())),
                    {0.0f, -1.0f, 0.0f});
    auto pointLightTransform3d = ecs::comp::Transform3D{};
    pointLightTransform3d.translation =
        glm::vec3(rotateLight * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
    pointLightTransform3d.scale = {0.5f, 0.5f, 0.5f};
    gCoordinator->addComponent(pointLight, pointLightTransform3d);
    gCoordinator->addComponent(pointLight, ecs::comp::PointLight{0.2f});
  }
}
