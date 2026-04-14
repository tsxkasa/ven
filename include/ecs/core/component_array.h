#pragma once

#include "pch.h"

namespace ecs {
class IComponentArray {
public:
  virtual ~IComponentArray() = default;
  virtual void entityDestroyed(Entity ent) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
public:
  void insertData(Entity ent, T comp) {
    assert(entityToIndexMap.find(ent) == entityToIndexMap.end() &&
           "Component added to same entity more than once");

    size_t newIndex = size;
    entityToIndexMap[ent] = newIndex;
    indexToEntityMap[newIndex] = ent;
    componentArray[newIndex] = comp;
    size++;
  }

  void removeData(Entity ent) {
    assert(entityToIndexMap.find(ent) != entityToIndexMap.end() &&
           "Removing non-existent component");

    size_t indexOfRemovedEnt = entityToIndexMap[ent];
    size_t indexOfLastElem = size - 1;
    componentArray[indexOfRemovedEnt] = componentArray[indexOfLastElem];

    Entity entOfLastElem = indexToEntityMap[indexOfLastElem];
    entityToIndexMap[entOfLastElem] = indexOfRemovedEnt;
    indexToEntityMap[indexOfRemovedEnt] = entOfLastElem;

    entityToIndexMap.erase(ent);
    indexToEntityMap.erase(indexOfLastElem);

    size--;
  }

  inline T& getData(Entity ent) {
    assert(entityToIndexMap.find(ent) != entityToIndexMap.end() &&
           "Retrieving non-existent component");

    return componentArray[entityToIndexMap[ent]];
  }

  void entityDestroyed(Entity ent) override {
    if (entityToIndexMap.find(ent) != entityToIndexMap.end()) {
      removeData(ent);
    }
  }

private:
  std::array<T, MAX_ENTITIES> componentArray;
  std::unordered_map<Entity, size_t> entityToIndexMap;
  std::unordered_map<size_t, Entity> indexToEntityMap;

  size_t size;
};
} // namespace ecs