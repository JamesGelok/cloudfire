// src/systems/MovementSystem.h
#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../components/Acceleration.h"
#include "../components/PlayerControlled.h"
#include "../components/Position.h"
#include "../components/Rotation.h"
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include "../systems/InputSystem.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

const float MAX_SPEED = 500.0f;
const float ACCELERATION = 1500.0f;
const float ROTATION_ACCELERATION =
    glm::radians(180.0f); // Degrees per second squared
const float MAX_ROTATION_SPEED = glm::radians(90.0f); // Degrees per second
const float FRICTION = 8.0f;
const float ROTATIONAL_FRICTION = 8.0f;
const float JUMP_FORCE = 600.0f; // Adjust as needed

class MovementSystem {
private:
  InputSystem *inputSystem;

public:
  MovementSystem(InputSystem *inputSys);

  void update(float deltaTime, EntityManager &entityManager,
              ComponentManager &componentManager);
};

MovementSystem::MovementSystem(InputSystem *inputSys) : inputSystem(inputSys) {}

void MovementSystem::update(float deltaTime, EntityManager &entityManager,
                            ComponentManager &componentManager) {

  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {

    const ComponentMask &mask = entityManager.getComponentMask(entity);

    // Only apply movement if the entity has the PlayerControlled component
    if (mask.test(ComponentType<Velocity>::ID()) &&
        mask.test(ComponentType<Position>::ID()) &&
        mask.test(ComponentType<PlayerControlled>::ID()) &&
        mask.test(ComponentType<Rotation>::ID())) {

      auto *velocity = componentManager.getComponent<Velocity>(entity);
      auto *rotation = componentManager.getComponent<Rotation>(entity);
      auto *acceleration = componentManager.getComponent<Acceleration>(entity);
      auto *position = componentManager.getComponent<Position>(entity);

      if (velocity && rotation && acceleration && position) {
        // Handle rotation input (left and right arrows)
        if (inputSystem->isKeyPressed(GLFW_KEY_LEFT)) {
          rotation->angularVelocity.y += ROTATION_ACCELERATION * deltaTime;
        }
        if (inputSystem->isKeyPressed(GLFW_KEY_RIGHT)) {
          rotation->angularVelocity.y -= ROTATION_ACCELERATION * deltaTime;
        }

        // Apply rotational friction when no input
        if (!inputSystem->isKeyPressed(GLFW_KEY_LEFT) &&
            !inputSystem->isKeyPressed(GLFW_KEY_RIGHT)) {
          rotation->angularVelocity.y *=
              (1.0f - ROTATIONAL_FRICTION * deltaTime);
        }

        // Clamp rotational velocity
        rotation->angularVelocity.y =
            std::clamp(rotation->angularVelocity.y, -MAX_ROTATION_SPEED,
                       MAX_ROTATION_SPEED);

        // Calculate forward direction from rotation
        glm::vec3 forward = rotation->quaternion * glm::vec3(0.0f, 0.0f, -1.0f);

        // Handle movement input (up and down arrows)
        if (inputSystem->isKeyPressed(GLFW_KEY_UP)) {
          velocity->dx += forward.x * ACCELERATION * deltaTime;
          velocity->dz += forward.z * ACCELERATION * deltaTime;
        }
        if (inputSystem->isKeyPressed(GLFW_KEY_DOWN)) {
          velocity->dx -= forward.x * ACCELERATION * deltaTime;
          velocity->dz -= forward.z * ACCELERATION * deltaTime;
        }

        // Apply friction when no input is pressed
        if (!inputSystem->isKeyPressed(GLFW_KEY_UP) &&
            !inputSystem->isKeyPressed(GLFW_KEY_DOWN)) {
          velocity->dx *= (1.0f - FRICTION * deltaTime);
          velocity->dz *= (1.0f - FRICTION * deltaTime);
        }

        // Clamp velocity to max speed
        float horizontalSpeed =
            sqrt(velocity->dx * velocity->dx + velocity->dz * velocity->dz);
        if (horizontalSpeed > MAX_SPEED) {
          float scalingFactor = MAX_SPEED / horizontalSpeed;
          velocity->dx *= scalingFactor;
          velocity->dz *= scalingFactor;
        }

        // Handle jump input
        if (inputSystem->isKeyPressed(GLFW_KEY_SPACE)) {
          // Check if the player is on the ground before allowing to jump
          if (velocity->dy == 0.0f) {
            velocity->dy += JUMP_FORCE;
          }
        }
      }
    }
  }
}

#endif // MOVEMENT_SYSTEM_H
