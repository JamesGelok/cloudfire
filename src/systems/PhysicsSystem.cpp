#include "PhysicsSystem.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

const float GRAVITY = -9.81f * 5.0f;

void PhysicsSystem::update(float deltaTime, EntityManager &entityManager,
                           ComponentManager &componentManager) {
  // Update physics for each entity
  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {
    const ComponentMask &mask = entityManager.getComponentMask(entity);

    // Update position and velocity
    if (mask.test(ComponentType<Position>::ID()) &&
        mask.test(ComponentType<Velocity>::ID()) &&
        mask.test(ComponentType<Acceleration>::ID())) {

      auto *position = componentManager.getComponent<Position>(entity);
      auto *velocity = componentManager.getComponent<Velocity>(entity);
      auto *acceleration = componentManager.getComponent<Acceleration>(entity);

      // Apply gravity to entities with GravityAffected component
      if (mask.test(ComponentType<GravityAffected>::ID())) {
        acceleration->ay += GRAVITY;
      }

      // Update velocity based on acceleration
      velocity->dx += acceleration->ax * deltaTime;
      velocity->dy += acceleration->ay * deltaTime;
      velocity->dz += acceleration->az * deltaTime;

      // Update position based on velocity
      position->x += velocity->dx * deltaTime;
      position->y += velocity->dy * deltaTime;
      position->z += velocity->dz * deltaTime;

      // Reset acceleration for the next frame
      acceleration->ax = 0.0f;
      acceleration->ay = 0.0f;
      acceleration->az = 0.0f;
    }

    // Update rotation for entities with Rotation component
    if (mask.test(ComponentType<Rotation>::ID())) {
      auto *rotation = componentManager.getComponent<Rotation>(entity);

      // Update angular velocity based on angular acceleration
      rotation->angularVelocity += rotation->angularAcceleration * deltaTime;

      // Convert angular velocity to a quaternion
      glm::quat deltaRotation =
          glm::quat(glm::vec3(rotation->angularVelocity * deltaTime));

      // Update the quaternion in the rotation component
      rotation->quaternion = deltaRotation * rotation->quaternion;

      // Normalize the quaternion to prevent numerical drift
      rotation->quaternion = glm::normalize(rotation->quaternion);

      // Reset angular acceleration
      rotation->angularAcceleration = glm::vec3(0.0f);
    }
  }

  // Handle collisions and grounded state
  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {
    const ComponentMask &mask = entityManager.getComponentMask(entity);

    if (mask.test(ComponentType<Position>::ID()) &&
        mask.test(ComponentType<Velocity>::ID()) &&
        mask.test(ComponentType<Collidable>::ID())) {

      auto *position = componentManager.getComponent<Position>(entity);
      auto *velocity = componentManager.getComponent<Velocity>(entity);

      // Only check collisions for entities affected by gravity
      if (mask.test(ComponentType<GravityAffected>::ID())) {
        bool isGrounded = false;

        // Check collision with other collidable entities
        for (EntityID otherEntity = 0;
             otherEntity < entityManager.entityCount(); ++otherEntity) {
          if (otherEntity == entity) {
            continue;
          }

          const ComponentMask &otherMask =
              entityManager.getComponentMask(otherEntity);

          if (otherMask.test(ComponentType<Position>::ID()) &&
              otherMask.test(ComponentType<Collidable>::ID())) {

            auto *otherPosition =
                componentManager.getComponent<Position>(otherEntity);

            // Get scales for both entities
            float entityHalfSizeX = 0.5f;
            float entityHalfSizeY = 0.5f;
            float entityHalfSizeZ = 0.5f;
            if (mask.test(ComponentType<Scale>::ID())) {
              auto *scale = componentManager.getComponent<Scale>(entity);
              entityHalfSizeX *= scale->scale.x;
              entityHalfSizeY *= scale->scale.y;
              entityHalfSizeZ *= scale->scale.z;
            }

            float otherHalfSizeX = 0.5f;
            float otherHalfSizeY = 0.5f;
            float otherHalfSizeZ = 0.5f;
            if (otherMask.test(ComponentType<Scale>::ID())) {
              auto *otherScale =
                  componentManager.getComponent<Scale>(otherEntity);
              otherHalfSizeX *= otherScale->scale.x;
              otherHalfSizeY *= otherScale->scale.y;
              otherHalfSizeZ *= otherScale->scale.z;
            }

            // Check for collision along each axis (X, Y, Z)
            bool collisionX = position->x + entityHalfSizeX >=
                                  otherPosition->x - otherHalfSizeX &&
                              otherPosition->x + otherHalfSizeX >=
                                  position->x - entityHalfSizeX;
            bool collisionY = position->y + entityHalfSizeY >=
                                  otherPosition->y - otherHalfSizeY &&
                              otherPosition->y + otherHalfSizeY >=
                                  position->y - entityHalfSizeY;
            bool collisionZ = position->z + entityHalfSizeZ >=
                                  otherPosition->z - otherHalfSizeZ &&
                              otherPosition->z + otherHalfSizeZ >=
                                  position->z - entityHalfSizeZ;

            if (collisionX && collisionY && collisionZ) {
              // Collision detected

              // Determine penetration depth in Y-axis
              float penetrationY =
                  std::min(position->y + entityHalfSizeY -
                               (otherPosition->y - otherHalfSizeY),
                           otherPosition->y + otherHalfSizeY -
                               (position->y - entityHalfSizeY));

              // Correct the entity's position to resolve collision
              if (penetrationY > 0.0f) {
                if (velocity->dy < 0.0f) {
                  // Landing on top of the other entity
                  position->y += penetrationY;
                  velocity->dy = 0.0f;
                  isGrounded = true; // Entity is grounded
                } else if (velocity->dy > 0.0f) {
                  // Hitting the underside of an object
                  position->y -= penetrationY;
                  velocity->dy = 0.0f;
                }
              }
            }
          }
        }

        // Update the OnGround component based on the isGrounded flag
        if (isGrounded) {
          // Add the OnGround component if not already present
          if (!mask.test(ComponentType<OnGround>::ID())) {
            componentManager.addComponent(entity, OnGround(), entityManager);
          }
        } else {
          // Remove the OnGround component if present
          if (mask.test(ComponentType<OnGround>::ID())) {
            componentManager.removeComponent<OnGround>(entity);
            entityManager.getComponentMask(entity).reset(
                ComponentType<OnGround>::ID());
          }
        }
      }
    }
  }
}
