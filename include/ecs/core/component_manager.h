#pragma once

#include "component_array.h"
#include "pch.h"

namespace ecs {
class ComponentManager {
public:
  template <typename T>
  void registerComponent() {
    const char* typeName = typeid(T).name();

    assert(componentTypes.find(typeName) == componentTypes.end() &&
           "Registering component type more than once");

    componentTypes.insert({typeName, nextComponentType});
    componentArrays.insert(
        {typeName, std::make_shared<ecs::ComponentArray<T>>()});

    nextComponentType++;
  }

  template <typename T>
  ComponentType getComponentType() {
    const char* typeName = typeid(T).name();
    assert(componentTypes.find(typeName) != componentTypes.end() &&
           "Component not registered before use");

    return componentTypes[typeName];
  }

  template <typename T>
  inline void addComponent(Entity ent, T comp) {
    getComponentArray<T>()->insertData(ent, comp);
  }

  template <typename T>
  inline void removeComponent(Entity ent) {
    getComponentArray<T>()->removeData(ent);
  }

  template <typename T>
  inline T& getComponent(Entity ent) {
    return getComponentArray<T>()->getData(ent);
  }

  void entityDestroyed(Entity ent) {
    for (const auto& pair : componentArrays) {

      const auto& comp = pair.second;
      comp->entityDestroyed(ent);
    }
  }

private:
  std::unordered_map<const char*, ComponentType> componentTypes{};
  std::unordered_map<const char*, std::shared_ptr<ecs::IComponentArray>>
      componentArrays{};
  ComponentType nextComponentType{};

  template <typename T>
  std::shared_ptr<ecs::ComponentArray<T>> getComponentArray() {
    const char* typeName = typeid(T).name();

    assert(componentTypes.find(typeName) != componentTypes.end() &&
           "Component not registered before use");
    return std::static_pointer_cast<ecs::ComponentArray<T>>(
        componentArrays[typeName]);
  }
};
} // namespace ecs