#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../components/Acceleration.h"
#include "../components/Position.h" // Include individual component headers
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include "../systems/InputSystem.h"
#include <algorithm> // For std::clamp

class PhysicsSystem {
public:
  void update(float deltaTime, EntityManager &entityManager,
              ComponentManager &componentManager);
};

void PhysicsSystem::update(float deltaTime, EntityManager &entityManager,
                           ComponentManager &componentManager) {
  for (EntityID entity = 0; entity < entityManager.entityCount();
       ++entity) { // Corrected loop
    const ComponentMask &mask = entityManager.getComponentMask(entity);

    // Check if the entity has Position, Velocity, and Acceleration components
    if (mask.test(ComponentType<Position>::ID()) && // Fixed ID() function call
        mask.test(ComponentType<Velocity>::ID()) && // Fixed ID() function call
        mask.test(
            ComponentType<Acceleration>::ID())) { // Fixed ID() function call

      auto *position = componentManager.getComponent<Position>(entity);
      auto *velocity = componentManager.getComponent<Velocity>(entity);
      auto *acceleration = componentManager.getComponent<Acceleration>(entity);

      if (position && velocity && acceleration) {
        // Update velocity based on acceleration
        velocity->dx += acceleration->ax * deltaTime;
        velocity->dy += acceleration->ay * deltaTime;

        // Update position based on velocity
        position->x += velocity->dx * deltaTime;
        position->y += velocity->dy * deltaTime;
      }
    }
  }
}

#endif // PHYSICS_SYSTEM_H
