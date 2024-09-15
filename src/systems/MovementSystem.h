#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../components/Acceleration.h"
#include "../components/PlayerControlled.h" // Include the PlayerControlled component
#include "../components/Position.h"
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include "../systems/InputSystem.h"
#include <algorithm>
#include <iostream>

const float MAX_SPEED = 500.0f;
const float ACCELERATION = 1500.0f;
const float FRICTION = 8.0f;

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
        mask.test(ComponentType<PlayerControlled>::ID())) {

      auto *velocity = componentManager.getComponent<Velocity>(entity);
      if (velocity) {

        // Adjust velocity directly based on input
        if (inputSystem->isKeyPressed(GLFW_KEY_UP)) {
          velocity->dy += ACCELERATION * deltaTime;
        }
        if (inputSystem->isKeyPressed(GLFW_KEY_DOWN)) {
          velocity->dy -= ACCELERATION * deltaTime;
        }
        if (inputSystem->isKeyPressed(GLFW_KEY_LEFT)) {
          velocity->dx -= ACCELERATION * deltaTime;
        }
        if (inputSystem->isKeyPressed(GLFW_KEY_RIGHT)) {
          velocity->dx += ACCELERATION * deltaTime;
        }

        // Apply friction when no input is pressed
        if (!inputSystem->isKeyPressed(GLFW_KEY_UP) &&
            !inputSystem->isKeyPressed(GLFW_KEY_DOWN)) {
          velocity->dy *= (1.0f - FRICTION * deltaTime);
        }
        if (!inputSystem->isKeyPressed(GLFW_KEY_LEFT) &&
            !inputSystem->isKeyPressed(GLFW_KEY_RIGHT)) {
          velocity->dx *= (1.0f - FRICTION * deltaTime);
        }

        // Clamp velocity to max speed
        velocity->dx = std::clamp(velocity->dx, -MAX_SPEED, MAX_SPEED);
        velocity->dy = std::clamp(velocity->dy, -MAX_SPEED, MAX_SPEED);

        // Optional: Debugging output
        std::cout << "Player Entity: " << entity
                  << " | Velocity X: " << velocity->dx
                  << " | Velocity Y: " << velocity->dy << std::endl;
      }
    }
  }
}

#endif
