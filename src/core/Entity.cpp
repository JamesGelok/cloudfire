#include "Entity.h"

EntityID EntityManager::createEntity() {
  EntityID newEntity;
  if (!availableEntities.empty()) {
    // Reuse an old entity ID if available
    newEntity = availableEntities.back();
    availableEntities.pop_back();
  } else {
    // Create a new entity ID
    newEntity = componentMasks.size();
    componentMasks.push_back(
        ComponentMask()); // Default mask is empty (no components)
  }
  return newEntity;
}

void EntityManager::destroyEntity(EntityID entity) {
  // Mark the entity as reusable by adding it to availableEntities
  componentMasks[entity].reset();
  availableEntities.push_back(entity);
}

// Return a non-const reference to allow modification
ComponentMask &EntityManager::getComponentMask(EntityID entity) {
  return componentMasks[entity];
}

// This method returns the total number of entities that have been created
size_t EntityManager::entityCount() const { return componentMasks.size(); }
