#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "../components/Acceleration.h"
#include "../components/ComponentType.h"
#include "../components/Position.h"
#include "../components/Renderable.h"
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include <memory>
#include <typeindex>
#include <unordered_map>

// Forward declare EntityID and EntityManager
using EntityID = size_t;

template <typename T> class ComponentPool {
public:
  T &addComponent(EntityID entity, T component);
  void removeComponent(EntityID entity);
  T *getComponent(EntityID entity);

private:
  std::unordered_map<EntityID, T> components;
};

class ComponentManager {
public:
  template <typename T>
  T &addComponent(EntityID entity, T component, EntityManager &entityManager);
  template <typename T> void removeComponent(EntityID entity);
  template <typename T> T *getComponent(EntityID entity);

private:
  std::unordered_map<std::type_index, std::shared_ptr<void>> componentPools;

  template <typename T> ComponentPool<T> &getComponentPool();
};

// Implementation of ComponentPool methods
template <typename T>
T &ComponentPool<T>::addComponent(EntityID entity, T component) {
  components[entity] = component;
  return components[entity];
}

template <typename T> void ComponentPool<T>::removeComponent(EntityID entity) {
  components.erase(entity);
}

template <typename T> T *ComponentPool<T>::getComponent(EntityID entity) {
  auto it = components.find(entity);
  return (it != components.end()) ? &it->second : nullptr;
}

// Implementation of ComponentManager methods
template <typename T>
T &ComponentManager::addComponent(EntityID entity, T component,
                                  EntityManager &entityManager) {
  auto &pool = getComponentPool<T>();
  auto &addedComponent = pool.addComponent(entity, component);
  entityManager.getComponentMask(entity).set(ComponentType<T>::ID());
  return addedComponent;
}

template <typename T> void ComponentManager::removeComponent(EntityID entity) {
  auto &pool = getComponentPool<T>();
  pool.removeComponent(entity);
}

template <typename T> T *ComponentManager::getComponent(EntityID entity) {
  auto &pool = getComponentPool<T>();
  return pool.getComponent(entity);
}

template <typename T> ComponentPool<T> &ComponentManager::getComponentPool() {
  auto typeIndex = std::type_index(typeid(T));
  if (componentPools.find(typeIndex) == componentPools.end()) {
    componentPools[typeIndex] = std::make_shared<ComponentPool<T>>();
  }
  return *static_cast<ComponentPool<T> *>(componentPools[typeIndex].get());
}

#endif // COMPONENT_MANAGER_H
