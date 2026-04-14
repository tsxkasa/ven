#pragma once

#include "component_manager.h"
#include "entity_manager.h"
#include "pch.h"
#include "system_manager.h"

namespace ecs {
class Coordinator {
public:
  void init() {
    entManager = std::make_unique<ecs::EntityManager>();
    compManager = std::make_unique<ecs::ComponentManager>();
    sysManager = std::make_unique<ecs::SystemManager>();
  }

  inline Entity createEntity() {
    return entManager->createEntity();
  }

  inline void destroyEntity(Entity ent) {
    entManager->destroyEntity(ent);
    compManager->entityDestroyed(ent);
    sysManager->entityDestroyed(ent);
  }

  template <typename T>
  inline void registerComponent() {
    compManager->registerComponent<T>();
  }

  template <typename T>
  void addComponent(Entity ent, T comp) {
    compManager->addComponent(ent, comp);

    auto sig = entManager->getSignature(ent);
    sig.set(compManager->getComponentType<T>(), true);
    entManager->setSignature(ent, sig);

    sysManager->entitySignatureChanged(ent, sig);
  }

  template <typename T>
  void removeComponent(Entity ent) {
    compManager->removeComponent<T>(ent);
    auto sig = entManager->getSignature(ent);
    sig.set(compManager->getComponentType<T>(), false);
    entManager->setSignature(ent, sig);
    sysManager->entitySignatureChanged(ent, sig);
  }

  template <typename T>
  inline T& getComponent(Entity ent) {
    return compManager->getComponent<T>(ent);
  }

  template <typename T>
  inline ComponentType getComponentType() {
    return compManager->getComponentType<T>();
  }

  template <typename T, typename... Args>
  inline std::shared_ptr<T> registerSystem(Args&&... args) {
    return sysManager->registerSystem<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  inline void setSystemSignature(Signature sig) {
    sysManager->setSignature<T>(sig);
  }

private:
  std::unique_ptr<ecs::ComponentManager> compManager;
  std::unique_ptr<ecs::EntityManager> entManager;
  std::unique_ptr<ecs::SystemManager> sysManager;
};
} // namespace ecs