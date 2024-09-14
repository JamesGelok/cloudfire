#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../components/Acceleration.h"
#include "../components/Position.h"
#include "../components/Rotation.h" // Add Rotation component
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include "../systems/InputSystem.h"
#include <algorithm> // For std::clamp
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp> // Required for quaternion multiplication
#include <iostream>

class PhysicsSystem {
public:
  void update(float deltaTime, EntityManager &entityManager,
              ComponentManager &componentManager);
};

void PhysicsSystem::update(float deltaTime, EntityManager &entityManager,
                           ComponentManager &componentManager) {
  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {
    const ComponentMask &mask = entityManager.getComponentMask(entity);

    // Check if the entity has Position, Velocity, Acceleration, and Rotation
    if (mask.test(ComponentType<Position>::ID()) &&
        mask.test(ComponentType<Velocity>::ID()) &&
        mask.test(ComponentType<Acceleration>::ID()) &&
        mask.test(ComponentType<Rotation>::ID())) { // Rotation added here

      auto *position = componentManager.getComponent<Position>(entity);
      auto *velocity = componentManager.getComponent<Velocity>(entity);
      auto *acceleration = componentManager.getComponent<Acceleration>(entity);
      auto *rotation = componentManager.getComponent<Rotation>(entity);

      if (position && velocity && acceleration && rotation) {
        // Update velocity based on acceleration
        velocity->dx += acceleration->ax * deltaTime;
        velocity->dy += acceleration->ay * deltaTime;
        velocity->dz += acceleration->az * deltaTime;

        // Update position based on velocity
        position->x += velocity->dx * deltaTime;
        position->y += velocity->dy * deltaTime;
        position->z += velocity->dz * deltaTime;

        // Update angular velocity (rotational speed) based on angular
        // acceleration
        rotation->angularVelocity += rotation->angularAcceleration * deltaTime;

        // Convert angular velocity to a quaternion (for rotation update)
        glm::quat deltaRotation =
            glm::quat(glm::vec3(rotation->angularVelocity * deltaTime));

        // Update the quaternion in the rotation component
        rotation->quaternion = deltaRotation * rotation->quaternion;

        // Normalize the quaternion to prevent numerical drift
        rotation->quaternion = glm::normalize(rotation->quaternion);
      }
    }
  }
}

#endif // PHYSICS_SYSTEM_H
