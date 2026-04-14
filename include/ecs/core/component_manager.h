#pragma once

#include "component_array.h"
#include "pch.h"

namespace ecs {
class ComponentManager {
public:
  template <typename T>
  void registerComponent() {
    std::type_index typeIndex(typeid(T));

    assert(componentTypes.find(typeIndex) == componentTypes.end() &&
           "Registering component type more than once");

    componentTypes.insert({typeIndex, nextComponentType});
    componentArrays.insert(
        {typeIndex, std::make_shared<ecs::ComponentArray<T>>()});

    nextComponentType++;
  }

  template <typename T>
  ComponentType getComponentType() {
    std::type_index typeIndex(typeid(T));
    assert(componentTypes.find(typeIndex) != componentTypes.end() &&
           "Component not registered before use");

    return componentTypes[typeIndex];
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
  std::unordered_map<std::type_index, ComponentType> componentTypes{};
  std::unordered_map<std::type_index, std::shared_ptr<ecs::IComponentArray>>
      componentArrays{};
  ComponentType nextComponentType{};

  template <typename T>
  std::shared_ptr<ecs::ComponentArray<T>> getComponentArray() {
    std::type_index typeIndex(typeid(T));

    assert(componentTypes.find(typeIndex) != componentTypes.end() &&
           "Component not registered before use");
    return std::static_pointer_cast<ecs::ComponentArray<T>>(
        componentArrays[typeIndex]);
  }
};
} // namespace ecs