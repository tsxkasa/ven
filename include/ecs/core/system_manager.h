#pragma once

#include "pch.h"
#include "system.h"

namespace ecs {
class SystemManager {
public:
  template <typename T, typename... Args>
  std::shared_ptr<T> registerSystem(Args&&... args) {
    const char* typeName = typeid(T).name();
    assert(systems.find(typeName) == systems.end() &&
           "Registering system more than once");
    auto system = std::make_shared<T>(std::forward<Args>(args)...);
    systems.insert({typeName, system});
    return system;
  }

  template <typename T>
  void setSignature(Signature sig) {
    const char* typeName = typeid(T).name();
    assert(systems.find(typeName) != systems.end() &&
           "System used before registered");
    signatures.insert({typeName, sig});
  }

  void entityDestroyed(Entity ent) {
    for (const auto& pair : systems) {
      const auto& system = pair.second;
      system->m_entities.erase(ent);
    }
  }

  void entitySignatureChanged(Entity ent, Signature sig) {
    for (const auto& pair : systems) {
      const auto& type = pair.first;
      const auto& system = pair.second;
      const auto& systemSig = signatures[type];
      if ((sig & systemSig) == systemSig)
        system->m_entities.insert(ent);
      else
        system->m_entities.erase(ent);
    }
  }

private:
  std::unordered_map<const char*, Signature> signatures{};
  std::unordered_map<const char*, std::shared_ptr<System>> systems{};
};
} // namespace ecs