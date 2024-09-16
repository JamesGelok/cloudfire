#pragma once

#include <bitset>
#include <vector>

// Maximum number of components an entity can have
constexpr size_t MAX_COMPONENTS = 64;

// Entity is represented by an ID
using EntityID = size_t;
// Each entity has a bitmask for its components
using ComponentMask = std::bitset<MAX_COMPONENTS>;

class EntityManager {
public:
  EntityID createEntity();
  void destroyEntity(EntityID entity);

  // Return a non-const reference so it can be modified
  ComponentMask &getComponentMask(EntityID entity);

  // Method to return the count of entities
  size_t entityCount() const;

private:
  // Stores bitmask for each entity
  std::vector<ComponentMask> componentMasks;
  // Reusable entity IDs
  std::vector<EntityID> availableEntities;
};
