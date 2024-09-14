#ifndef ENTITY_H
#define ENTITY_H

#include <bitset>
#include <vector>

constexpr size_t MAX_COMPONENTS = 64; // Limit for different component types

using EntityID = size_t; // Entity is represented by an ID
using ComponentMask =
    std::bitset<MAX_COMPONENTS>; // Each entity has a bitmask for its components

class EntityManager {
public:
  EntityID createEntity();
  void destroyEntity(EntityID entity);

  // Return a non-const reference so it can be modified
  ComponentMask &getComponentMask(EntityID entity);

  // New method to return the count of entities
  size_t entityCount() const;

private:
  std::vector<ComponentMask> componentMasks; // Stores bitmask for each entity
  std::vector<EntityID> availableEntities;   // Reusable entity IDs
};

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

#endif // ENTITY_H
